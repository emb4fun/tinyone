/**************************************************************************
*  This file is part of the TAL project (Tiny Abstraction Layer)
*
*  Copyright (c) 2016 by Michael Fischer (www.emb4fun.de).
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*  
*  1. Redistributions of source code must retain the above copyright 
*     notice, this list of conditions and the following disclaimer.
*
*  2. Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the 
*     documentation and/or other materials provided with the distribution.
*
*  3. Neither the name of the author nor the names of its contributors may 
*     be used to endorse or promote products derived from this software 
*     without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
*  THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
*  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
*  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
*  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF 
*  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
*  SUCH DAMAGE.
*
***************************************************************************
*  History:
*
*  14.10.2016  mifi  First Version for the BeagleBone Black (B3).
**************************************************************************/
#define __TALCPU_C__

/*=======================================================================*/
/*  Include                                                              */
/*=======================================================================*/
#include <stdlib.h>
#include <intrinsics.h>
#include "tal.h"

#include "soc_AM335x.h"
#include "hw_cm_wkup.h"
#include "hw_cm_per.h"
#include "hw_cm_dpll.h"
#include "interrupt.h"
#include "dmtimer.h"

/*=======================================================================*/
/*  Extern                                                               */
/*=======================================================================*/

extern void (*fnRAMVectors[])(void);

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

/*
 * See http://e2e.ti.com/support/processors/f/791/t/355064#
 */
#define RNG_BASE  0x48310000

typedef struct _rng_ 
{
   uint64_t output;
   uint32_t status;
   uint32_t irq_en;
   uint32_t status_clr;
   uint32_t control;
   uint32_t config;
} rng_t;

#define RNG_STATUS_RDY  (1u <<  0)  /* output ready for reading */
#define RNG_STATUS_ERR  (1u <<  1)  /* FRO shutdown alarm */
#define RNG_STATUS_CLK  (1u << 31)  /* module functional clock active (no irq) */

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

static uint32_t dTimerReload = 0;
static uint32_t dHiResPeriod = 0;

static volatile rng_t *rng = (rng_t*)RNG_BASE;

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  SysTickTimer                                                         */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void SysTickTimer (void)
{
   TAL_CPU_IRQ_ENTER();

   OS_TimerCallback();
   
   /* Clear the status of the interrupt flags */
   DMTimerIntStatusClear(SOC_DMTIMER_2_REGS, DMTIMER_INT_OVF_IT_FLAG);
   
   TAL_CPU_IRQ_EXIT();
} /* SysTickTimer */

/*************************************************************************/
/*  Name  : SysTickInit                                                  */
/*                                                                       */
/*  Initialize the SysTick functionality, DMTimer2 is used.              */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: Period                                                       */
/*************************************************************************/
static uint32_t SysTickInit (void)
{
   /* 
    * Select the clock source for the Timer2 instance. 
    * (Based on the DMTimer2ModuleClkConfig function from the BeagleBone)
    */
   HWREG(SOC_CM_DPLL_REGS + CM_DPLL_CLKSEL_TIMER2_CLK) &=
         ~(CM_DPLL_CLKSEL_TIMER2_CLK_CLKSEL);

   HWREG(SOC_CM_DPLL_REGS + CM_DPLL_CLKSEL_TIMER2_CLK) |=
         CM_DPLL_CLKSEL_TIMER2_CLK_CLKSEL_CLK_M_OSC;

   while((HWREG(SOC_CM_DPLL_REGS + CM_DPLL_CLKSEL_TIMER2_CLK) &
          CM_DPLL_CLKSEL_TIMER2_CLK_CLKSEL) !=
          CM_DPLL_CLKSEL_TIMER2_CLK_CLKSEL_CLK_M_OSC);   /*lint !e722 */

   /* Writing to MODULEMODE field of CM_PER_TIMER2_CLKCTRL register. */
   HWREG(SOC_CM_PER_REGS + CM_PER_TIMER2_CLKCTRL) |=
         CM_PER_TIMER2_CLKCTRL_MODULEMODE_ENABLE;

   /* Waiting for MODULEMODE field to reflect the written value. */
   while((HWREG(SOC_CM_PER_REGS + CM_PER_TIMER2_CLKCTRL) &
          CM_PER_TIMER2_CLKCTRL_MODULEMODE) != 
          CM_PER_TIMER2_CLKCTRL_MODULEMODE_ENABLE);            /*lint !e722*/

   /*
    * Waiting for IDLEST field in CM_PER_TIMER2_CLKCTRL register 
    * for the module is fully functional.
    */
   while((HWREG(SOC_CM_PER_REGS + CM_PER_TIMER2_CLKCTRL) & 
          CM_PER_TIMER2_CLKCTRL_IDLEST) != 
          CM_PER_TIMER2_CLKCTRL_IDLEST_FUNC);                  /*lint !e722*/
    
   /* Waiting for the L4LS and TIMER2 clock */
   while(!(HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKSTCTRL) &
          (CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_L4LS_GCLK |
           CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_TIMER2_GCLK)));   /*lint !e722*/


   /* Setup SysTick interrupt */
   tal_CPUIrqSetPriority(SYSTICK_INT, SYSTICK_PRIO);
   tal_CPUIrqSetFunction(SYSTICK_INT, SysTickTimer);
   tal_CPUIrqEnable(SYSTICK_INT);

   /* Calculate reload value */
   dTimerReload = 0xffffffff - (OSC_CLOCK_FREQUENCY / OS_TICKS_PER_SECOND);
   
   /* Reset DMTimer */
   DMTimerReset(SOC_DMTIMER_2_REGS);
   
   /* Set DMTimer reload and counter value */
   DMTimerReloadSet(SOC_DMTIMER_2_REGS, dTimerReload);
   DMTimerCounterSet(SOC_DMTIMER_2_REGS, dTimerReload);
   
   /* Configure the DMTimer for auto reload mode */
   DMTimerModeConfigure(SOC_DMTIMER_2_REGS, DMTIMER_AUTORLD_NOCMP_ENABLE);
   
   /* Enable the DMTimer interrupt */
   DMTimerIntEnable(SOC_DMTIMER_2_REGS, DMTIMER_INT_OVF_EN_FLAG);
   
   return((0xffffffff - dTimerReload));
} /* SysTickInit */

/*************************************************************************/
/*  CPUGetRandomData                                                     */
/*                                                                       */
/*  Return 64 bits of random data.                                       */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 64 bits of random data                                       */
/*************************************************************************/
static uint64_t CPUGetRandomData (void)
{
   static int nInitDone = 0;
   uint64_t    Output;
   
   if (0 == nInitDone)
   {
      nInitDone = 1;
      
      /* 
       * Enable RNG module clock 
       */

      /* Writing to MODULEMODE field of CM_PER_RNG_CLKCTRL register. */
      HWREG(SOC_CM_PER_REGS + CM_PER_RNG_CLKCTRL) |=
            CM_PER_RNG_CLKCTRL_MODULEMODE_ENABLE;

      /* Waiting for MODULEMODE field to reflect the written value. */
      while((HWREG(SOC_CM_PER_REGS + CM_PER_RNG_CLKCTRL) &
             CM_PER_RNG_CLKCTRL_MODULEMODE) != 
             CM_PER_RNG_CLKCTRL_MODULEMODE_ENABLE);   /*lint !e722*/
      
      rng->irq_en = 0;
      
      rng->config = (34 << 16) |    /* Max refill 34 * 256 cycles */
                    (33 <<  0);     /* Min refill 33 * 64 cycles  */
                   
      rng->control = (33 << 16) |   /* Sartup 33 * 256 cycles */
                     (1 << 10);     /* Enable module          */
   }

   do {} while( ( rng->status & RNG_STATUS_RDY ) == 0 );
   Output = rng->output;
   rng->status_clr = RNG_STATUS_RDY;
   
   return(Output);
} /* CPUGetRandomData */

/*************************************************************************/
/*  GPMCClkConfig (based on platform\evmAM335x\nand.c)                   */
/*                                                                       */
/*  This function enables the system L3S and system L4_WKUP clocks.      */
/*  This also enables the clocks for UART0 instance.                     */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void GPMCClkConfig (void)
{
    HWREG(SOC_PRCM_REGS + CM_PER_L3S_CLKSTCTRL) |=
          CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_PRCM_REGS + CM_PER_L3S_CLKSTCTRL) &
           CM_PER_L3S_CLKSTCTRL_CLKTRCTRL) != 
           CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP);         /*lint !e722*/

    HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKSTCTRL) |=
          CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKSTCTRL) &
           CM_PER_L3_CLKSTCTRL_CLKTRCTRL) != 
           CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP);          /*lint !e722*/

    HWREG(SOC_PRCM_REGS + CM_PER_L3_INSTR_CLKCTRL) |=
          CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_PRCM_REGS + CM_PER_L3_INSTR_CLKCTRL) &
           CM_PER_L3_INSTR_CLKCTRL_MODULEMODE) != 
           CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE);      /*lint !e722*/

    HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKCTRL) |=
          CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKCTRL) &
           CM_PER_L3_CLKCTRL_MODULEMODE) != 
           CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE);            /*lint !e722*/

    HWREG(SOC_PRCM_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) |=
          CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_PRCM_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
           CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL) !=
           CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP);    /*lint !e722*/

    HWREG(SOC_PRCM_REGS + CM_PER_GPMC_CLKCTRL) |=
          CM_PER_GPMC_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_PRCM_REGS + CM_PER_GPMC_CLKCTRL) &
           CM_PER_GPMC_CLKCTRL_MODULEMODE) != 
           CM_PER_GPMC_CLKCTRL_MODULEMODE_ENABLE);          /*lint !e722*/

    HWREG(SOC_PRCM_REGS + CM_PER_ELM_CLKCTRL) |=
          CM_PER_ELM_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_PRCM_REGS + CM_PER_ELM_CLKCTRL) &
           CM_PER_ELM_CLKCTRL_MODULEMODE) != 
           CM_PER_ELM_CLKCTRL_MODULEMODE_ENABLE);           /*lint !e722*/


    while(!(HWREG(SOC_PRCM_REGS + CM_PER_L3S_CLKSTCTRL) &
            CM_PER_L3S_CLKSTCTRL_CLKACTIVITY_L3S_GCLK));    /*lint !e722*/

    while(!(HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKSTCTRL) &
            CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK));      /*lint !e722*/

} /* GPMCClkConfig */

/*************************************************************************/
/*  GPIO0ModuleClkConfig                                                 */
/*                                                                       */
/*  This function enables the functional clocks for GPIO0 instance.      */
/*  Based on the StarterWare example (platform\evmskAM335x\gpio.c).      */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void GPIO0ModuleClkConfig(void)
{
   /* Writing to MODULEMODE field of CM_WKUP_GPIO0_CLKCTRL register. */
   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_GPIO0_CLKCTRL) |=
         CM_WKUP_GPIO0_CLKCTRL_MODULEMODE_ENABLE;

   /* Waiting for MODULEMODE field to reflect the written value. */
   while(CM_WKUP_GPIO0_CLKCTRL_MODULEMODE_ENABLE !=
         (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_GPIO0_CLKCTRL) &
          CM_WKUP_GPIO0_CLKCTRL_MODULEMODE));   /*lint !e722*/

   /*
   ** Writing to OPTFCLKEN_GPIO0_GDBCLK field of CM_WKUP_GPIO0_CLKCTRL
   ** register.
   */
   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_GPIO0_CLKCTRL) |=
         CM_WKUP_GPIO0_CLKCTRL_OPTFCLKEN_GPIO0_GDBCLK;

   /* Waiting for OPTFCLKEN_GPIO0_GDBCLK field to reflect the written value. */
   while(CM_WKUP_GPIO0_CLKCTRL_OPTFCLKEN_GPIO0_GDBCLK !=
         (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_GPIO0_CLKCTRL) &
          CM_WKUP_GPIO0_CLKCTRL_OPTFCLKEN_GPIO0_GDBCLK));   /*lint !e722*/

   /* Writing to IDLEST field in CM_WKUP_GPIO0_CLKCTRL register. */
   while((CM_WKUP_GPIO0_CLKCTRL_IDLEST_FUNC <<
          CM_WKUP_GPIO0_CLKCTRL_IDLEST_SHIFT) !=
         (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_GPIO0_CLKCTRL) &
          CM_WKUP_GPIO0_CLKCTRL_IDLEST));    /*lint !e722*/

   /*
   ** Waiting for CLKACTIVITY_GPIO0_GDBCLK field in CM_WKUP_GPIO0_CLKCTRL
   ** register to attain desired value.
   */
   while(CM_WKUP_CLKSTCTRL_CLKACTIVITY_GPIO0_GDBCLK !=
         (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) &
          CM_WKUP_CLKSTCTRL_CLKACTIVITY_GPIO0_GDBCLK));  /*lint !e722*/
          
} /* GPIO0ModuleClkConfig */

/*************************************************************************/
/*  GPIO1ModuleClkConfig                                                 */
/*                                                                       */
/*  This function enables the functional clocks for GPIO1 instance.      */
/*  Based on the StarterWare example (platform\evmskAM335x\gpio.c).      */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void GPIO1ModuleClkConfig (void)
{
   /* Writing to MODULEMODE field of CM_PER_GPIO1_CLKCTRL register. */
   HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1_CLKCTRL) |=
         CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE;

   /* Waiting for MODULEMODE field to reflect the written value. */
   while(CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE !=
         (HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1_CLKCTRL) &
          CM_PER_GPIO1_CLKCTRL_MODULEMODE));    /*lint !e722*/
          
   /*
   ** Writing to OPTFCLKEN_GPIO_1_GDBCLK bit in CM_PER_GPIO1_CLKCTRL
   ** register.
   */
   HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1_CLKCTRL) |=
         CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK;

   /*
   ** Waiting for OPTFCLKEN_GPIO_1_GDBCLK bit to reflect the desired
   ** value.
   */
   while(CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK !=
         (HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1_CLKCTRL) &
          CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK));   /*lint !e722*/

   /*
   ** Waiting for IDLEST field in CM_PER_GPIO1_CLKCTRL register to attain the
   ** desired value.
   */
   while((CM_PER_GPIO1_CLKCTRL_IDLEST_FUNC <<
          CM_PER_GPIO1_CLKCTRL_IDLEST_SHIFT) !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1_CLKCTRL) &
           CM_PER_GPIO1_CLKCTRL_IDLEST));    /*lint !e722*/

   /*
   ** Waiting for CLKACTIVITY_GPIO_1_GDBCLK bit in CM_PER_L4LS_CLKSTCTRL
   ** register to attain desired value.
   */
   while(CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_GPIO_1_GDBCLK !=
         (HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKSTCTRL) &
          CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_GPIO_1_GDBCLK));   /*lint !e722*/
          
} /* GPIO1ModuleClkConfig */

/*************************************************************************/
/*  GPIO2ModuleClkConfig                                                 */
/*                                                                       */
/*  This function enables the functional clocks for GPIO2 instance.      */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void GPIO2ModuleClkConfig (void)
{
   /* Writing to MODULEMODE field of CM_PER_GPIO2_CLKCTRL register. */
   HWREG(SOC_CM_PER_REGS + CM_PER_GPIO2_CLKCTRL) |=
         CM_PER_GPIO2_CLKCTRL_MODULEMODE_ENABLE;

   /* Waiting for MODULEMODE field to reflect the written value. */
   while(CM_PER_GPIO2_CLKCTRL_MODULEMODE_ENABLE !=
         (HWREG(SOC_CM_PER_REGS + CM_PER_GPIO2_CLKCTRL) &
          CM_PER_GPIO2_CLKCTRL_MODULEMODE));    /*lint !e722*/
          
   /*
   ** Writing to OPTFCLKEN_GPIO_2_GDBCLK bit in CM_PER_GPIO1_CLKCTRL
   ** register.
   */
   HWREG(SOC_CM_PER_REGS + CM_PER_GPIO2_CLKCTRL) |=
         CM_PER_GPIO2_CLKCTRL_OPTFCLKEN_GPIO_2_GDBCLK;

   /*
   ** Waiting for OPTFCLKEN_GPIO_2_GDBCLK bit to reflect the desired
   ** value.
   */
   while(CM_PER_GPIO2_CLKCTRL_OPTFCLKEN_GPIO_2_GDBCLK !=
         (HWREG(SOC_CM_PER_REGS + CM_PER_GPIO2_CLKCTRL) &
          CM_PER_GPIO2_CLKCTRL_OPTFCLKEN_GPIO_2_GDBCLK));   /*lint !e722*/

   /*
   ** Waiting for IDLEST field in CM_PER_GPIO2_CLKCTRL register to attain the
   ** desired value.
   */
   while((CM_PER_GPIO1_CLKCTRL_IDLEST_FUNC <<
          CM_PER_GPIO1_CLKCTRL_IDLEST_SHIFT) !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_GPIO2_CLKCTRL) &
           CM_PER_GPIO2_CLKCTRL_IDLEST));    /*lint !e722*/

   /*
   ** Waiting for CLKACTIVITY_GPIO_2_GDBCLK bit in CM_PER_L4LS_CLKSTCTRL
   ** register to attain desired value.
   */
   while(CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_GPIO_2_GDBCLK !=
         (HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKSTCTRL) &
          CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_GPIO_2_GDBCLK));   /*lint !e722*/
          
} /* GPIO2ModuleClkConfig */

/*************************************************************************/
/*  Name  : EnableGPIOClocks                                             */
/*                                                                       */
/*  This function enables the GPIO 0, 1 and 2 clocks.                    */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void EnableGPIOClocks (void)
{
   GPIO0ModuleClkConfig();
   GPIO1ModuleClkConfig();
   GPIO2ModuleClkConfig();
   
} /* EnableGPIOClocks */

/*************************************************************************/
/*  Name  : EnableL3L4Uart0Clocks (based on platform\beaglebone\uart.c)  */
/*                                                                       */
/*  This function enables the system L3 and system L4_WKUP clocks.       */
/*  This also enables the clocks for UART0 instance.                     */
/*  (Based on the UART0ModuleClkConfig function from the BeagleBone)     */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void EnableL3L4Uart0Clocks (void)
{ 
   /* Configuring L3 Interface Clocks. */

   /* Writing to MODULEMODE field of CM_PER_L3_CLKCTRL register. */
   HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) |=
         CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE;

   /* Waiting for MODULEMODE field to reflect the written value. */
   while(CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE !=
         (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) &
          CM_PER_L3_CLKCTRL_MODULEMODE));                /*lint !e722*/

   /* Writing to MODULEMODE field of CM_PER_L3_INSTR_CLKCTRL register. */
   HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) |=
         CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE;

   /* Waiting for MODULEMODE field to reflect the written value. */
   while(CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE !=
         (HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) &
          CM_PER_L3_INSTR_CLKCTRL_MODULEMODE));          /*lint !e722*/

   /* Writing to CLKTRCTRL field of CM_PER_L3_CLKSTCTRL register. */
   HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) |=
         CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

   /* Waiting for CLKTRCTRL field to reflect the written value. */
   while(CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
         (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) &
          CM_PER_L3_CLKSTCTRL_CLKTRCTRL));               /*lint !e722*/

   /* Writing to CLKTRCTRL field of CM_PER_OCPWP_L3_CLKSTCTRL register. */
   HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) |=
         CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

   /* Waiting for CLKTRCTRL field to reflect the written value. */
   while(CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
         (HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
          CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL));         /*lint !e722*/

   /* Writing to CLKTRCTRL field of CM_PER_L3S_CLKSTCTRL register. */
   HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) |=
         CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

   /* Waiting for CLKTRCTRL field to reflect the written value. */
   while(CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
         (HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) &
          CM_PER_L3S_CLKSTCTRL_CLKTRCTRL));              /*lint !e722*/

   /* Checking fields for necessary values.  */

   /* Waiting for IDLEST field in CM_PER_L3_CLKCTRL register to be set to 0x0. */
   while((CM_PER_L3_CLKCTRL_IDLEST_FUNC << CM_PER_L3_CLKCTRL_IDLEST_SHIFT)!=
         (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) &
          CM_PER_L3_CLKCTRL_IDLEST));                    /*lint !e722*/

   /*
   ** Waiting for IDLEST field in CM_PER_L3_INSTR_CLKCTRL register to attain the
   ** desired value.
   */
   while((CM_PER_L3_INSTR_CLKCTRL_IDLEST_FUNC <<
          CM_PER_L3_INSTR_CLKCTRL_IDLEST_SHIFT)!=
         (HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) &
          CM_PER_L3_INSTR_CLKCTRL_IDLEST));              /*lint !e722*/

   /*
   ** Waiting for CLKACTIVITY_L3_GCLK field in CM_PER_L3_CLKSTCTRL register to
   ** attain the desired value.
   */
   while(CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK !=
         (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) &
          CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK));     /*lint !e722*/

   /*
   ** Waiting for CLKACTIVITY_OCPWP_L3_GCLK field in CM_PER_OCPWP_L3_CLKSTCTRL
   ** register to attain the desired value.
   */
   while(CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L3_GCLK !=
         (HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
          CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L3_GCLK));  /*lint !e722*/

   /*
   ** Waiting for CLKACTIVITY_L3S_GCLK field in CM_PER_L3S_CLKSTCTRL register
   ** to attain the desired value.
   */
   while(CM_PER_L3S_CLKSTCTRL_CLKACTIVITY_L3S_GCLK !=
         (HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) &
         CM_PER_L3S_CLKSTCTRL_CLKACTIVITY_L3S_GCLK));    /*lint !e722*/

   /* Configuring registers related to Wake-Up region. */

   /* Writing to MODULEMODE field of CM_WKUP_CONTROL_CLKCTRL register. */
   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CONTROL_CLKCTRL) |=
         CM_WKUP_CONTROL_CLKCTRL_MODULEMODE_ENABLE;

   /* Waiting for MODULEMODE field to reflect the written value. */
   while(CM_WKUP_CONTROL_CLKCTRL_MODULEMODE_ENABLE !=
         (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CONTROL_CLKCTRL) &
          CM_WKUP_CONTROL_CLKCTRL_MODULEMODE));          /*lint !e722*/

   /* Writing to CLKTRCTRL field of CM_PER_L3S_CLKSTCTRL register. */
   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) |=
         CM_WKUP_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

   /* Waiting for CLKTRCTRL field to reflect the written value. */
   while(CM_WKUP_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
         (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) &
          CM_WKUP_CLKSTCTRL_CLKTRCTRL));                 /*lint !e722*/

   /* Writing to CLKTRCTRL field of CM_L3_AON_CLKSTCTRL register. */
   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L3_AON_CLKSTCTRL) |=
         CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

   /* Waiting for CLKTRCTRL field to reflect the written value. */
   while(CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
         (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L3_AON_CLKSTCTRL) &
          CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKTRCTRL));       /*lint !e722*/

   /* Verifying if the other bits are set to required settings. */

   /*
   ** Waiting for IDLEST field in CM_WKUP_CONTROL_CLKCTRL register to attain
   ** desired value.
   */
   while((CM_WKUP_CONTROL_CLKCTRL_IDLEST_FUNC <<
          CM_WKUP_CONTROL_CLKCTRL_IDLEST_SHIFT) !=
         (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CONTROL_CLKCTRL) &
          CM_WKUP_CONTROL_CLKCTRL_IDLEST));              /*lint !e722*/

   /*
   ** Waiting for CLKACTIVITY_L3_AON_GCLK field in CM_L3_AON_CLKSTCTRL
   ** register to attain desired value.
   */
   while(CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKACTIVITY_L3_AON_GCLK !=
         (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L3_AON_CLKSTCTRL) &
          CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKACTIVITY_L3_AON_GCLK));  /*lint !e722*/

   /*
   ** Waiting for IDLEST field in CM_WKUP_L4WKUP_CLKCTRL register to attain
   ** desired value.
   */
   while((CM_WKUP_L4WKUP_CLKCTRL_IDLEST_FUNC <<
          CM_WKUP_L4WKUP_CLKCTRL_IDLEST_SHIFT) !=
         (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_L4WKUP_CLKCTRL) &
          CM_WKUP_L4WKUP_CLKCTRL_IDLEST));               /*lint !e722*/

   /*
   ** Waiting for CLKACTIVITY_L4_WKUP_GCLK field in CM_WKUP_CLKSTCTRL register
   ** to attain desired value.
   */
   while(CM_WKUP_CLKSTCTRL_CLKACTIVITY_L4_WKUP_GCLK !=
         (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) &
          CM_WKUP_CLKSTCTRL_CLKACTIVITY_L4_WKUP_GCLK));  /*lint !e722*/

   /*
   ** Waiting for CLKACTIVITY_L4_WKUP_AON_GCLK field in CM_L4_WKUP_AON_CLKSTCTRL
   ** register to attain desired value.
   */
   while(CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL_CLKACTIVITY_L4_WKUP_AON_GCLK !=
         (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL) &
          CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL_CLKACTIVITY_L4_WKUP_AON_GCLK)); /*lint !e722*/

          
   /* Enable the L4LS clock (from DMTimer2ModuleClkConfig) */

   /* Writing to CLKTRCTRL field of CM_PER_L4LS_CLKSTCTRL register. */
   HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKSTCTRL) =
         CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

   /* Waiting for CLKTRCTRL field to reflect the written value. */
   while((HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKSTCTRL) &
          CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL) !=
          CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL_SW_WKUP);      /*lint !e722*/

   /* Writing to MODULEMODE field of CM_PER_L4LS_CLKCTRL register. */
   HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKCTRL) =
         CM_PER_L4LS_CLKCTRL_MODULEMODE_ENABLE;

   /* Waiting for MODULEMODE field to reflect the written value. */
   while((HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKCTRL) &
          CM_PER_L4LS_CLKCTRL_MODULEMODE) != 
          CM_PER_L4LS_CLKCTRL_MODULEMODE_ENABLE);        /*lint !e722*/

} /* EnableL3L4Uart0Clocks */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  tal_CPUInit                                                          */
/*                                                                       */
/*  "Initialize" the CPU.                                                */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void tal_CPUInit (void)
{
   uint32_t dValue;

   /*
    * Here the CrossWorks for ARM code is used for initialize Cache and
    * MMU. This will be done in Sitara_Startup.s. The "alignment fault"
    * MUST not enabled.
    *
    * The code will look like:
    *
    *    Enable the MMU and caches
    *    mrc p15, 0, r0, c1, c0, 0      Read MMU control register
    *    orr r0, r0, #0x00001000        Enable ICache
    *    orr r0, r0, #0x00000005        Enable DCache and MMU
    *    mcr p15, 0, r0, c1, c0, 0      Write MMU control register
    */

   /* GPMC initialization */
   GPMCClkConfig();
   
   /* Enable L3, L4 and UART0 clocks */
   EnableL3L4Uart0Clocks();
   
   /* Enable GPIO 0, 1, and 2 clocks */
   EnableGPIOClocks();
   
   /* Initialize the interrupt controller */   
   IntAINTCInit();
   
   /* Initialize the SysTick */
   dHiResPeriod = SysTickInit();

   dValue = tal_CPUGetFrequencyCPU();
   dValue = tal_CPUGetFrequencyCore(); /*lint !e838*/
   dValue = tal_CPUGetFrequencyDDR();  /*lint !e838*/
   dValue = tal_CPUGetFrequencyPeri(); /*lint !e838*/
   
   dValue = tal_CPUGetFrequencyM4();   /*lint !e838*/
   dValue = tal_CPUGetFrequencyM5();   /*lint !e838*/
   dValue = tal_CPUGetFrequencyM6();   /*lint !e838*/

   (void)dValue;
   
   CPUGetRandomData();

} /* tal_CPUInit */

/*************************************************************************/
/*  tal_CPUSysTickStart                                                  */
/*                                                                       */
/*  Start the SysTick.                                                   */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void tal_CPUSysTickStart (void)
{
   /* Start timer */
   DMTimerEnable(SOC_DMTIMER_2_REGS);   
} /* tal_CPUSysTickStart */

/*************************************************************************/
/*  tal_CPUIrqEnable                                                     */
/*                                                                       */
/*  Enable the given IRQ.                                                */
/*                                                                       */
/*  In    : IRQ                                                          */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void tal_CPUIrqEnable (int IRQ)
{
   IntSystemEnable((unsigned int)IRQ);
} /* tal_CPUIrqEnable */

/*************************************************************************/
/*  tal_CPUIrqDisable                                                    */
/*                                                                       */
/*  Disable the given IRQ.                                               */
/*                                                                       */
/*  In    : IRQ                                                          */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void tal_CPUIrqDisable (int IRQ)
{
   IntSystemDisable((unsigned int)IRQ);
} /* tal_CPUIrqDisable */

/*************************************************************************/
/*  tal_CPUIrqDisableAll                                                 */
/*                                                                       */
/*  Disable all interrupts.                                              */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void tal_CPUIrqDisableAll (void)
{
   /*lint +rw(_to_semi) */
   /*lint -d__disable_irq=_to_semi */

   __disable_irq();
   
} /* tal_CPUIrqDisableAll */

/*************************************************************************/
/*  tal_CPUIrqSetPriority                                                */
/*                                                                       */
/*  Set priority of the given IRQ.                                       */
/*                                                                       */
/*  In    : IRQ, Priority                                                */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void tal_CPUIrqSetPriority (int IRQ, int Priority)
{
   /* Disable the system interrupt in AINTC first. */
   IntSystemDisable((unsigned int)IRQ);    
   
   /* Setting the priority for the system interrupt in AINTC. */
   IntPrioritySet((unsigned int)IRQ, (unsigned int)Priority, AINTC_HOSTINT_ROUTE_IRQ);

} /* tal_CPUIrqSetPriority */

/*************************************************************************/
/*  tal_CPUIrqSetFunction                                                */
/*                                                                       */
/*  Set the handler of the given IRQ.                                    */
/*                                                                       */
/*  In    : IRQ, fnHandler                                               */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void tal_CPUIrqSetFunction (int IRQ, tal_irq_fnt Function) 
{
   /* Disable the system interrupt in AINTC first. */
   IntSystemDisable((unsigned int)IRQ);    
    
   /* Registering the Interrupt Service Routine(ISR). */
   IntRegister((unsigned int)IRQ, Function);

} /* tal_CPUIrqSetFunction */

/*************************************************************************/
/*  tal_CPUStatGetHiResPeriod                                            */
/*                                                                       */
/*  Return the HiResPeriod value.                                        */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: HiResPeriod                                                  */
/*************************************************************************/
uint32_t tal_CPUStatGetHiResPeriod (void)
{
   return(dHiResPeriod);
} /* tal_CPUStatGetHiResPeriod */

/*************************************************************************/
/*  tal_CPUStatGetHiResCnt                                               */
/*                                                                       */
/*  Return the HiRes counter.                                            */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: HiRes counter                                                */
/*************************************************************************/
uint32_t tal_CPUStatGetHiResCnt (void)
{
   uint32_t dValue;
   
   /* Get milliseconds */
   dValue  = (OS_TimeGet() << 16);
   
   /* The timer counter counts down from dTimerReload to 0xFFFFFFFF */
   dValue |= (uint16_t)(DMTimerCounterGet(SOC_DMTIMER_2_REGS) - dTimerReload);
   
   return(dValue);
} /* tal_CPUStatGetHiResCnt */

/*************************************************************************/
/*  tal_CPUGetFrequencyCPU                                               */
/*                                                                       */
/*  Return the CPU clock frequency in MHz.                               */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: Frequency                                                    */
/*************************************************************************/
uint32_t tal_CPUGetFrequencyCPU (void)
{
   uint32_t dFrequency = OSC_CLOCK_FREQUENCY;
   uint32_t dValue;
   uint32_t dCLKMode;
   uint32_t dPLLInfo;
   uint32_t dM2Div;
   uint32_t dDivider;
   uint32_t dMultiplier;
   
   dCLKMode = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKMODE_DPLL_MPU) & 
                    CM_WKUP_CM_CLKMODE_DPLL_MPU_DPLL_EN;
   
   /* Check for bypass mode */
   if (dCLKMode != CM_WKUP_CM_CLKMODE_DPLL_MPU_DPLL_EN_DPLL_MN_BYP_MODE)
   {
      /* Get PLL divider and multiplier */
      dPLLInfo = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKSEL_DPLL_MPU);
      
      dDivider = ((dPLLInfo & CM_WKUP_CM_CLKSEL_DPLL_MPU_DPLL_DIV) >> 
                   CM_WKUP_CM_CLKSEL_DPLL_MPU_DPLL_DIV_SHIFT) + 1;
                  
      dMultiplier = (dPLLInfo & CM_WKUP_CM_CLKSEL_DPLL_MPU_DPLL_MULT) >> 
                     CM_WKUP_CM_CLKSEL_DPLL_MPU_DPLL_MULT_SHIFT;

      
      /* Get the CLKOUT2 divider */
      dValue = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_DIV_M2_DPLL_MPU);
      dM2Div = (dValue & CM_WKUP_CM_DIV_M2_DPLL_MPU_DPLL_CLKOUT_DIV) >> 
                CM_WKUP_CM_DIV_M2_DPLL_MPU_DPLL_CLKOUT_DIV_SHIFT;
   
      dFrequency = ((OSC_CLOCK_FREQUENCY / dDivider) * dMultiplier) / dM2Div;
   }
   
   return(dFrequency);
} /* tal_CPUGetFrequencyCPU */

/*************************************************************************/
/*  Name  : tal_CPUGetFrequencyCore                                      */
/*                                                                       */
/*  Return the Core clock frequency in MHz.                              */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: Frequency                                                    */
/*************************************************************************/
uint32_t tal_CPUGetFrequencyCore (void)
{
   uint32_t dFrequency = OSC_CLOCK_FREQUENCY;
   uint32_t dCLKMode;
   uint32_t dPLLInfo;
   uint32_t dDivider;
   uint32_t dMultiplier;
   
   dCLKMode = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKMODE_DPLL_CORE) &
                    CM_WKUP_CM_CLKMODE_DPLL_PER_DPLL_EN;
   
   /* Check for bypass mode */
   if (dCLKMode != CM_WKUP_CM_CLKMODE_DPLL_PER_DPLL_EN_DPLL_MN_BYP_MODE)
   {
      /* Get PLL divider and multiplier */
      dPLLInfo = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKSEL_DPLL_CORE);

      dDivider = ((dPLLInfo & CM_WKUP_CM_CLKSEL_DPLL_CORE_DPLL_DIV) >> 
                   CM_WKUP_CM_CLKSEL_DPLL_CORE_DPLL_DIV_SHIFT) + 1;

      dMultiplier = (dPLLInfo & CM_WKUP_CM_CLKSEL_DPLL_CORE_DPLL_MULT) >> 
                     CM_WKUP_CM_CLKSEL_DPLL_CORE_DPLL_MULT_SHIFT;
                    
                    
      dFrequency = (OSC_CLOCK_FREQUENCY / dDivider) * dMultiplier;
   }
   
   return(dFrequency);
} /* tal_CPUGetFrequencyCore */

/*************************************************************************/
/*  Name  : tal_CPUGetFrequencyPeri                                      */
/*                                                                       */
/*  Return the Peripheral clock frequency in MHz.                        */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: Frequency                                                    */
/*************************************************************************/
uint32_t tal_CPUGetFrequencyPeri (void)
{
   uint32_t dFrequency = OSC_CLOCK_FREQUENCY;
   uint32_t dValue;
   uint32_t dCLKMode;
   uint32_t dPLLInfo;
   uint32_t dM2Div;
   uint32_t dDivider;
   uint32_t dMultiplier;
   
   dCLKMode = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKMODE_DPLL_PER) &
                    CM_WKUP_CM_CLKMODE_DPLL_PER_DPLL_EN;
   
   /* Check for bypass mode */
   if (dCLKMode != CM_WKUP_CM_CLKMODE_DPLL_PER_DPLL_EN_DPLL_MN_BYP_MODE)
   {
      /* Get PLL divider and multiplier */
      dPLLInfo = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKSEL_DPLL_PERIPH);
      
      dDivider = ((dPLLInfo & CM_WKUP_CM_CLKSEL_DPLL_PERIPH_DPLL_DIV) >> 
                   CM_WKUP_CM_CLKSEL_DPLL_PERIPH_DPLL_DIV_SHIFT) + 1;
                  
      dMultiplier = (dPLLInfo & CM_WKUP_CM_CLKSEL_DPLL_PERIPH_DPLL_MULT) >> 
                     CM_WKUP_CM_CLKSEL_DPLL_PERIPH_DPLL_MULT_SHIFT;


      /* Get the CLKOUT2 divider */
      dValue = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_DIV_M2_DPLL_PER);
      dM2Div = (dValue & CM_WKUP_CM_DIV_M2_DPLL_PER_DPLL_CLKOUT_DIV) >> 
                CM_WKUP_CM_DIV_M2_DPLL_PER_DPLL_CLKOUT_DIV_SHIFT;
   
      dFrequency = ((OSC_CLOCK_FREQUENCY / dDivider) * dMultiplier) / dM2Div;
   }
   
   return(dFrequency);
} /* tal_CPUGetFrequencyPeri */

/*************************************************************************/
/*  Name  : tal_CPUGetFrequencyM4                                        */
/*                                                                       */
/*  Return the M4 clock frequency in MHz.                                */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: Frequency                                                    */
/*************************************************************************/
uint32_t tal_CPUGetFrequencyM4 (void)
{
   uint32_t dFrequency;
   uint32_t dDivider;
   
   dFrequency = tal_CPUGetFrequencyCore();

   /* Get PLL divider */
   dDivider = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_DIV_M4_DPLL_CORE);
   dDivider = (dDivider & CM_WKUP_CM_DIV_M4_DPLL_CORE_HSDIVIDER_CLKOUT1_DIV) >>
               CM_WKUP_CM_DIV_M4_DPLL_CORE_HSDIVIDER_CLKOUT1_DIV_SHIFT;
   
   dFrequency = (dFrequency / dDivider);
   
   return(dFrequency);
} /* tal_CPUGetFrequencyM4 */

/*************************************************************************/
/*  Name  : tal_CPUGetFrequencyM5                                        */
/*                                                                       */
/*  Return the M5 clock frequency in MHz.                                */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: Frequency                                                    */
/*************************************************************************/
uint32_t tal_CPUGetFrequencyM5 (void)
{
   uint32_t dFrequency;
   uint32_t dDivider;
   
   dFrequency = tal_CPUGetFrequencyCore();

   /* Get PLL divider */
   dDivider = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_DIV_M5_DPLL_CORE);
   dDivider = (dDivider & CM_WKUP_CM_DIV_M5_DPLL_CORE_HSDIVIDER_CLKOUT2_DIV) >>
               CM_WKUP_CM_DIV_M5_DPLL_CORE_HSDIVIDER_CLKOUT2_DIV_SHIFT;
   
   dFrequency = (dFrequency / dDivider);
   
   return(dFrequency);
} /* tal_CPUGetFrequencyM5 */

/*************************************************************************/
/*  Name  : tal_CPUGetFrequencyM6                                        */
/*                                                                       */
/*  Return the M6 clock frequency in MHz.                                */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: Frequency                                                    */
/*************************************************************************/
uint32_t tal_CPUGetFrequencyM6 (void)
{
   uint32_t dFrequency;
   uint32_t dDivider;
   
   dFrequency = tal_CPUGetFrequencyCore();

   /* Get PLL divider */
   dDivider = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_DIV_M6_DPLL_CORE);
   dDivider = (dDivider & CM_WKUP_CM_DIV_M6_DPLL_CORE_HSDIVIDER_CLKOUT3_DIV) >>
               CM_WKUP_CM_DIV_M6_DPLL_CORE_HSDIVIDER_CLKOUT3_DIV_SHIFT;
   
   dFrequency = (dFrequency / dDivider);
   
   return(dFrequency);
} /* tal_CPUGetFrequencyM6 */

/*************************************************************************/
/*  Name  : tal_CPUGetFrequencyDDR                                       */
/*                                                                       */
/*  Return the DDR clock frequency in MHz.                               */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: Frequency                                                    */
/*************************************************************************/
uint32_t tal_CPUGetFrequencyDDR (void)
{
   uint32_t dFrequency;
   uint32_t dValue;
   uint32_t dPLLInfo;
   uint32_t dM2Div;
   uint32_t dDivider;
   uint32_t dMultiplier;
   
   /* Get PLL divider and multiplier */
   dPLLInfo = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKSEL_DPLL_DDR);
   
   dDivider = ((dPLLInfo & CM_WKUP_CM_CLKSEL_DPLL_DDR_DPLL_DIV) >>
                CM_WKUP_CM_CLKSEL_DPLL_DDR_DPLL_DIV_SHIFT) + 1;
   
   dMultiplier = (dPLLInfo & CM_WKUP_CM_CLKSEL_DPLL_DDR_DPLL_MULT) >> 
                  CM_WKUP_CM_CLKSEL_DPLL_DDR_DPLL_MULT_SHIFT;
   
   
   /* Get the CLKOUT2 divider */
   dValue = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_DIV_M2_DPLL_DDR);
   dM2Div = (dValue & CM_WKUP_CM_DIV_M2_DPLL_DDR_DPLL_CLKOUT_DIV) >>
             CM_WKUP_CM_DIV_M2_DPLL_DDR_DPLL_CLKOUT_DIV_SHIFT;

   dFrequency = ((OSC_CLOCK_FREQUENCY / dDivider) * dMultiplier) / dM2Div;
   
   return(dFrequency);
} /* tal_CPUGetFrequencyDDR */

/*************************************************************************/
/*  Name  : tal_CPUGetFrequencyDisp                                      */
/*                                                                       */
/*  Return the Display clock frequency in MHz.                           */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: Frequency                                                    */
/*************************************************************************/
uint32_t tal_CPUGetFrequencyDisp (void)
{
   uint32_t dFrequency;
   uint32_t dValue;
   uint32_t dPLLInfo;
   uint32_t dM2Div;
   uint32_t dDivider;
   uint32_t dMultiplier;

   /* Get PLL divider and multiplier */
   dPLLInfo = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKSEL_DPLL_DISP);
   
   dDivider = ((dPLLInfo & CM_WKUP_CM_CLKSEL_DPLL_DISP_DPLL_DIV) >>
                CM_WKUP_CM_CLKSEL_DPLL_DISP_DPLL_DIV_SHIFT) + 1;
   
   dMultiplier = (dPLLInfo & CM_WKUP_CM_CLKSEL_DPLL_DISP_DPLL_MULT) >> 
                  CM_WKUP_CM_CLKSEL_DPLL_DISP_DPLL_MULT_SHIFT;


   /* Get the CLKOUT2 divider */
   dValue = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_DIV_M2_DPLL_DISP);
   dM2Div = (dValue & CM_WKUP_CM_CLKMODE_DPLL_DISP_DPLL_EN) >>
             CM_WKUP_CM_CLKMODE_DPLL_DISP_DPLL_EN_SHIFT;

   dFrequency = ((OSC_CLOCK_FREQUENCY / dDivider) * dMultiplier) / dM2Div;
   
   return(dFrequency);
} /* tal_CPUGetFrequencyDisp */

/*************************************************************************/
/*  tal_CPUReboot                                                        */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void tal_CPUReboot (void)
{
   term_printf("*** Reboot ***\r\n");
   OS_TimeDly(500);

   tal_BoardReset();
   
} /* tal_CPUReboot */

/*************************************************************************/
/*  tal_CPURngInit                                                       */
/*                                                                       */
/*  Initialize the random number generator.                              */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void tal_CPURngInit (void)
{
   /* Empty function */
} /* tal_CPURngInit */
   
/*************************************************************************/
/*  tal_CPURngDeInit                                                     */
/*                                                                       */
/*  DeInitialize the random number generator.                            */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void tal_CPURngDeInit (void)
{
   /* Empty function */
} /* tal_CPURngDeInit */
   
/*************************************************************************/
/*  tal_CPURngHardwarePoll                                               */
/*                                                                       */
/*  Generates a 32-bit random number.                                    */
/*                                                                       */
/*  In    : pData, dSize                                                 */
/*  Out   : pData                                                        */
/*  Return: TAL_OK / TAL_ERROR                                           */
/*************************************************************************/
TAL_RESULT tal_CPURngHardwarePoll (uint8_t *pData, uint32_t dSize)
{
   uint64_t Random;
   
   while (dSize)
   {
      Random = CPUGetRandomData();
      
      if (dSize >= sizeof(uint64_t))
      {
         *pData++ = (Random & 0xFF); Random = Random >> 8;
         *pData++ = (Random & 0xFF); Random = Random >> 8;
         *pData++ = (Random & 0xFF); Random = Random >> 8;
         *pData++ = (Random & 0xFF); Random = Random >> 8;
         *pData++ = (Random & 0xFF); Random = Random >> 8;
         *pData++ = (Random & 0xFF); Random = Random >> 8;
         *pData++ = (Random & 0xFF); Random = Random >> 8;
         *pData++ = (Random & 0xFF); Random = Random >> 8;
         
         dSize = dSize - sizeof(uint64_t);
      }
      else
      {
         while (dSize)
         {
            *pData++ = (Random & 0xFF); Random = Random >> 8;
            dSize--;
         }
      }
   }

   return(TAL_OK);   
} /* tal_CPURngHardwarePoll */  

/*************************************************************************/
/*  Name  : CPU_IRQHandler                                               */
/*                                                                       */
/*  CPU IRQ handler.                                                     */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void CPU_IRQHandler (void)
{
   uint32_t    ActiveIRQ;
   tal_irq_fnt IRQFunction;
   
   /* Get the currently active IRQ interrupt number */
   ActiveIRQ = HWREG(SOC_AINTC_REGS + INTC_SIR_IRQ) & 0x7f;

   /*
    * Read the interrupt vector
    */
   IRQFunction = fnRAMVectors[ActiveIRQ];      
   if (IRQFunction != (tal_irq_fnt)0)  /* Make sure we don't have a NULL pointer */          
   {
      (*IRQFunction)(); 
   }
   
   /* Reset IRQ output and enable new IRQ generation */
   HWREG(SOC_AINTC_REGS + INTC_CONTROL) = 1;                         

} /* CPU_IRQHandler */

/*** EOF ***/

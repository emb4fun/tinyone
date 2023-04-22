/**************************************************************************
*  This file is part of the TAL project (Tiny Abstraction Layer)
*
*  Copyright (c) 2015-2023 by Michael Fischer (www.emb4fun.de).
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
**************************************************************************/
#define __B3GPMC_C__

/*=======================================================================*/
/*  Include                                                              */
/*=======================================================================*/
#include <stdint.h>
#include "tal.h"
#include "cpu_avm.h"

#include "soc_AM335x.h"
#include "hw_control_AM335x.h"
#include "hw_types.h"
#include "hw_cm_per.h"
#include "hw_gpio_v2.h"
#include "gpmc.h"
#include "interrupt.h"
#include "gpio_v2.h"

/*=======================================================================*/
/*  All extern data                                                      */
/*=======================================================================*/

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

/*
 * Set GPMC_SUPPORT_WAIT to 1 if wait states should be supported.
 */
#define GPMC_SUPPORT_WAIT              0


/*
 * Control register
 */
#define CONTROL_CONF_MMODE_SHIFT       0
#define CONTROL_CONF_PUDEN_SHIFT       3
#define CONTROL_CONF_PUTYPESEL_SHIFT   4
#define CONTROL_CONF_RXACTIVE_SHIFT    5

/*
 * The GPMC frequency is 100MHz (10ns).
 * If a value of 80ns is needed, use 8 for the setting.
 */
 
/* CS# */ 
#define CSWROFFTIME        8                 /* De-assertion time from start cycle time for write accesses */
#define CSRDOFFTIME        8                 /* De-assertion time from start cycle time for read accesses */

#define CSEXTRADELAY       0                 /* Add Extra Half GPMC.FCLK cycle */
#define CSONTIME           0                 /* Assertion time from start cycle time */

/* ADV# */
#define ADVWROFFTIME       0                 /* De-assertion time from start cycle time for write accesses */
#define ADVRDOFFTIME       0                 /* De-assertion time from start cycle time for read accesses */
#define ADVONTIME          0                 /* Assertion time from start cycle time */

/* WE#, OE# */
#define WEOFFTIME          (CSWROFFTIME - 0) /* De-assertion time from start cycle time */
#define WEONTIME           0                 /* Assertion time from start cycle time */
#define OEOFFTIME          (CSRDOFFTIME - 0) /* De-assertion time from start cycle time */
#define OEONTIME           0                 /* Assertion time from start cycle time */

#define RDACCESSTIME       (OEOFFTIME - 0)   /* Delay between start cycle time and first data valid */

#define WRCYCLETIME        CSWROFFTIME       /* Total write cycle time */ 
#define RDCYCLETIME        CSRDOFFTIME       /* Total read cycle time */

#define WRACCESSTIME       CSWROFFTIME       /* Delay from StartAccessTime to the GPMC.FCLK rising edge */
                                             /* corresponding the the GPMC.CLK rising edge used by the attached */
                                             /* memory for the first data capture. */

#define CYCLE2CYCLEDELAY   3                 /* Chip select high pulse delay between two successive accesses */
#define BUSTURNAROUND      0                 /* Bus turn around latency between two successive accesses to the */
                                             /* same chip-select (read to write) or to a different chip-select (read to */
                                             /* read and read to write) */

/*=======================================================================*/
/*  Definition of all global Data                                        */
/*=======================================================================*/   

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

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
          CM_PER_GPIO2_CLKCTRL_MODULEMODE));
          
   /*
   ** Writing to OPTFCLKEN_GPIO_2_GDBCLK bit in CM_PER_GPIO2_CLKCTRL
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
          CM_PER_GPIO2_CLKCTRL_OPTFCLKEN_GPIO_2_GDBCLK));

   /*
   ** Waiting for IDLEST field in CM_PER_GPIO2_CLKCTRL register to attain the
   ** desired value.
   */
   while((CM_PER_GPIO1_CLKCTRL_IDLEST_FUNC <<
          CM_PER_GPIO1_CLKCTRL_IDLEST_SHIFT) !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_GPIO2_CLKCTRL) &
           CM_PER_GPIO2_CLKCTRL_IDLEST));

   /*
   ** Waiting for CLKACTIVITY_GPIO_2_GDBCLK bit in CM_PER_L4LS_CLKSTCTRL
   ** register to attain desired value.
   */
   while(CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_GPIO_2_GDBCLK !=
         (HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKSTCTRL) &
          CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_GPIO_2_GDBCLK));
          
} /* GPIO2ModuleClkConfig */

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
          CM_PER_GPIO1_CLKCTRL_MODULEMODE));
          
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
          CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK));

   /*
   ** Waiting for IDLEST field in CM_PER_GPIO1_CLKCTRL register to attain the
   ** desired value.
   */
   while((CM_PER_GPIO1_CLKCTRL_IDLEST_FUNC <<
          CM_PER_GPIO1_CLKCTRL_IDLEST_SHIFT) !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1_CLKCTRL) &
           CM_PER_GPIO1_CLKCTRL_IDLEST));

   /*
   ** Waiting for CLKACTIVITY_GPIO_1_GDBCLK bit in CM_PER_L4LS_CLKSTCTRL
   ** register to attain desired value.
   */
   while(CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_GPIO_1_GDBCLK !=
         (HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKSTCTRL) &
          CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_GPIO_1_GDBCLK));
          
} /* GPIO1ModuleClkConfig */

/*************************************************************************/
/*  GPMCPinMuxSetup                                                      */
/*                                                                       */
/*  This function selects the GPMC pins for the external memory bus.     */
/*  The GPMC pins are multiplexed with pins of other peripherals in the  */
/*  SoC. Based on the StarterWare example (platform\evmAM335x\nand.c).   */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void GPMCPinMuxSetup (void)
{
   uint32_t Value;
   
   /*
    * Setup Data lines 0 .. 15
    */
   for(uint32_t Index = 0; Index <= 15; Index++)
   {
      HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_AD(Index)) = 
         (0 << CONTROL_CONF_MMODE_SHIFT)     |     /* Mode 0 */
         (0 << CONTROL_CONF_PUDEN_SHIFT)     |     /* Pull enabled */
         (0 << CONTROL_CONF_PUTYPESEL_SHIFT) |     /* Pulldown */
         (1 << CONTROL_CONF_RXACTIVE_SHIFT);       /* Receiver enabled */
   }

   /*
    * Setup Control lines like:
    *    WAIT1, CS0, OE and WE
    */
   
   /* Use WAIT1 instead of WAIT0 */
   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_CLK) =
      (2 << CONTROL_CONF_MMODE_SHIFT)     |        /* Mode 2 */
      (0 << CONTROL_CONF_PUDEN_SHIFT)     |        /* Pull enabled */
      (1 << CONTROL_CONF_PUTYPESEL_SHIFT) |        /* Pullup */
      (1 << CONTROL_CONF_RXACTIVE_SHIFT);          /* Receiver enabled */
    
   /* Set the value for the Control Register */ 
   Value = (0 << CONTROL_CONF_MMODE_SHIFT)     |   /* Mode 0 */
           (0 << CONTROL_CONF_PUDEN_SHIFT)     |   /* Pull enabled */
           (1 << CONTROL_CONF_PUTYPESEL_SHIFT) |   /* Pullup */
           (0 << CONTROL_CONF_RXACTIVE_SHIFT);     /* Receiver disabled */

   /* CS0 */
   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_CSN(0)) = Value;

   /* OE */
   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_OEN_REN) = Value;

   /* WE */
   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_WEN) = Value;

   /*
    * Compared to the NAND examples, the Address lines will be used too.
    */

   /*
    * Setup Address lines 1 .. 11.
    * Address line 0 is not needed because of 16 bit access only.
    */
    
   /* Set the value for the Control Register */ 
   Value = (1 << CONTROL_CONF_MMODE_SHIFT)     |   /* Mode 1 */
           (1 << CONTROL_CONF_PUDEN_SHIFT)     |   /* Pull disabled */
           (1 << CONTROL_CONF_PUTYPESEL_SHIFT) |   /* Pullup */
           (0 << CONTROL_CONF_RXACTIVE_SHIFT);     /* Receiver disabled */

   for(uint32_t Index = 1; Index <= 7; Index++)
   {
      HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_DATA(Index)) = Value;
   }
   
   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_VSYNC)      = Value; /* A8  */
   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_HSYNC)      = Value; /* A9  */
   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_PCLK)       = Value; /* A10 */
   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_AC_BIAS_EN) = Value; /* A11 */

} /* GPMCPinMuxSetup */

/*************************************************************************/
/*  TimingSetup                                                          */
/*                                                                       */
/*  For the timing information take a look in the AM335x Technical       */
/*  Reference Manual, here section "Memory Subsystem".                   */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void TimingSetup (void)
{
   uint32_t Value;
   uint32_t Config;
   uint32_t Config1, Config2, Config3, Config4, Config5, Config6, Config7;
   
   /*
    * For the setup of the timing the following registers are needed:
    *    CONFIG, CONFIG1...CONFIG7
    *
    * Now we will setup all these registers here.
    */
   
   /*
    * The configuration register allows global configuration of the GPMC.
    */
   Config = (0 << 9) |                    /* WAIT1PINPOLARITY */
            (0 << 8) |                    /* WAIT0PINPOLARITY */
            (0 << 4) |                    /* WRITEPROTECT */
            (0 << 1) |                    /* LIMITEDADDRESS */
            (0 << 0);                     /* NANDFORCEPOSTEDWRITE */

   /* 
    * The configuration 1 register sets signal control parameters per chip select.
    */ 
#if (GPMC_SUPPORT_WAIT == 0)   
   Config1 = (1 << 16) |                  /* WAITPINSELECT, Wait1 */
             (1 << 12);                   /* DEVICESIZE */
#else
   Config1 = (1 << 22) |                  /* WAITREADMONITORING */
             (1 << 21) |                  /* WAITWRITEMONITORING */
             (0 << 18) |                  /* WAITMONITORINGTIME */
             (1 << 16) |                  /* WAITPINSELECT, Wait1 */
             (1 << 12);                   /* DEVICESIZE */
#endif             

   /*
    * Chip-select signal timing parameter configuration. 
    */
   Config2 = (CSWROFFTIME  << 16) |       /* CSWROFFTIME */
             (CSRDOFFTIME  <<  8) |       /* CSRDOFFTIME */
             (CSEXTRADELAY <<  7) |       /* CSEXTRADELAY */
             (CSONTIME     <<  0);        /* CSONTIME    */ 

   /*
    * ADV# signal timing parameter configuration.
    */   
   Config3 = (ADVWROFFTIME << 16) |       /* ADVWROFFTIME */
             (ADVRDOFFTIME <<  8) |       /* ADVRDOFFTIME */
             (ADVONTIME    <<  0);        /* ADVONTIME    */   
   
   /*
    * WE# and OE# signals timing parameter configuration.
    */
   Config4 = (WEOFFTIME << 24) |          /* WEOFFTIME */
             (WEONTIME  << 16) |          /* WEONTIME  */
             (OEOFFTIME <<  8) |          /* OEOFFTIME */
             (OEONTIME  <<  0);           /* OEONTIME  */

   /*
    * RdAccessTime and CycleTime timing parameters configuration.
    */
   Config5 = (RDACCESSTIME << 16) |       /* RDACCESSTIME  */
             (WRCYCLETIME  <<  8) |       /* WRCYCLETIME */
             (RDCYCLETIME  <<  0);        /* RDCYCLETIME */   
   
   /*
    * WrAccessTime, WrDataOnADmuxBus, Cycle2Cycle, and BusTurnAround parameters configuration.
    */
   Config6 = (WRACCESSTIME     << 24) |   /* WRACCESSTIME  */
             (CYCLE2CYCLEDELAY <<  8) |   /* CYCLE2CYCLEDELAY */
             (3                <<  6) |   /* Enable CYCLE2CYCLESAMECSEN, CYCLE2CYCLEDIFFCSEN */
             (BUSTURNAROUND    <<  0);    /* BUSTURNAROUND */ 

   /* 
    * Chip-select address mapping configuration.
    * Chip-select size of 16 Mbytes, CS (active low) enabled
    */
   Value   = GPMC_CS0_START_ADDRESS >> 24;
   Config7 = (0x0F  << 8) |               /* MASKADDRESS */
             (0x01  << 6) |               /* CSVALID     */
             (Value << 0);                /* BASEADDRESS */

   /* Set configuration registers */
   HWREG(SOC_GPMC_0_REGS + GPMC_CONFIG)     = Config;
   HWREG(SOC_GPMC_0_REGS + GPMC_CONFIG1(0)) = Config1;
   HWREG(SOC_GPMC_0_REGS + GPMC_CONFIG2(0)) = Config2;
   HWREG(SOC_GPMC_0_REGS + GPMC_CONFIG3(0)) = Config3;
   HWREG(SOC_GPMC_0_REGS + GPMC_CONFIG4(0)) = Config4;
   HWREG(SOC_GPMC_0_REGS + GPMC_CONFIG5(0)) = Config5;
   HWREG(SOC_GPMC_0_REGS + GPMC_CONFIG6(0)) = Config6;
   HWREG(SOC_GPMC_0_REGS + GPMC_CONFIG7(0)) = Config7;

} /* TimingSetup */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  GPMCInit                                                             */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void GPMCInit (void)
{
   uint32_t Status;

   /* 
    * Initialize pin configuration of the GPMC module.
    */
   GPMCPinMuxSetup();
//   GPMCClkConfig();   // Still done by tal_CPUInit
   
   /*
    * Soft reset GPMC module
    */
   GPMCModuleSoftReset(SOC_GPMC_0_REGS);
   
   /* Wait for reset done */
   Status = GPMCModuleResetStatusGet(SOC_GPMC_0_REGS);
   while ((Status & GPMC_SYSSTATUS_RESETDONE) != GPMC_SYSSTATUS_RESETDONE)
   {
      Status = GPMCModuleResetStatusGet(SOC_GPMC_0_REGS);
   }
   
   /* 
    * Setup the GPMC timing 
    */
   TimingSetup();    
   
   /* 
    * Init GPIO clocks 
    */
   GPIO1ModuleClkConfig();    
   GPIO2ModuleClkConfig();
          
   /*
    * Configure reset signal, gpio2[5] 
    */   
    
   /* Setup Pad Control Registers. */
   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_BE0N_CLE) = 
      (7 << CONTROL_CONF_MMODE_SHIFT)     |     /* Mode 7 */
      (0 << CONTROL_CONF_PUDEN_SHIFT)     |     /* Pull enabled */
      (1 << CONTROL_CONF_PUTYPESEL_SHIFT) |     /* PullUp */
      (0 << CONTROL_CONF_RXACTIVE_SHIFT);       /* Receiver disabled */

   /* Module is enabled, clocks are not gated. */
   HWREG(SOC_GPIO_2_REGS + GPIO_CTRL) &= ~GPIO_CTRL_DISABLEMODULE;

#if 0
   /* Clear the corresponding bit in the GPIO_DATAOUT register, set output to low. */
   HWREG(SOC_GPIO_2_REGS + GPIO_CLEARDATAOUT) = (1<<5);
#else
   HWREG(SOC_GPIO_2_REGS + GPIO_SETDATAOUT) = (1<<5);
#endif
   
   /* The corresponding GPIO port is configured as an output. */
   HWREG(SOC_GPIO_2_REGS + GPIO_OE) &= ~(1<<5);

} /* GPMCInit */

/*************************************************************************/
/*  GPMCResetSet                                                         */
/*                                                                       */
/*  Assert reset signal which is low active.                             */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void GPMCResetSet (void)
{
   HWREG(SOC_GPIO_2_REGS + GPIO_CLEARDATAOUT) = (1<<5);
} /* GPMCResetSet */

/*************************************************************************/
/*  GPMCResetClear                                                       */
/*                                                                       */
/*  Deassert reset signal which is low active.                           */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void GPMCResetClear (void)
{
   HWREG(SOC_GPIO_2_REGS + GPIO_SETDATAOUT) = (1<<5);
} /* GPMCResetClear */

/*************************************************************************/
/*  GPMCConfigureIRQ                                                     */
/*                                                                       */
/*  Configure the GPIO1_31 for the IRQ pin.                              */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void GPMCConfigureIRQ (void (*IrqHandler)(void))
{
   uint32_t Value;

   /*
    * Configure the GPIO1_31 for the IRQ pin
    */
   
   /* Setup Pad Control Registers */
   Value = (7 << CONTROL_CONF_MMODE_SHIFT)     |   /* Mode 7 */
           (0 << CONTROL_CONF_PUDEN_SHIFT)     |   /* Pull enabled */
           (1 << CONTROL_CONF_PUTYPESEL_SHIFT) |   /* Pullup */
           (1 << CONTROL_CONF_RXACTIVE_SHIFT);     /* Receiver enabled */
      
   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_CSN(2)) = Value;

   /* Configure the GPIO1_31 to input */
   GPIODirModeSet(SOC_GPIO_1_REGS, 31, GPIO_DIR_INPUT);

          
   /* 
    * Configure the INTC
    */

   /* Register the Interrupt Service Routine(ISR). */
   IntRegister(SYS_INT_GPIOINT1A, IrqHandler);

   /* Set the priority for the GPIO1 system interrupt in INTC. */
   IntPrioritySet(SYS_INT_GPIOINT1A, 1, AINTC_HOSTINT_ROUTE_IRQ);

   /* Enable the GPIO1 system interrupt in INTC. */
   IntSystemEnable(SYS_INT_GPIOINT1A);
   
   /* Enable interrupt generation on detection of a logic HIGH or LOW levels. */
   GPIOIntTypeSet(SOC_GPIO_1_REGS, 31, GPIO_INT_TYPE_LEVEL_LOW);

   /* Disable interrupt generation on detection of a rising or a falling edge. */
   GPIOIntTypeSet(SOC_GPIO_1_REGS, 31, GPIO_INT_TYPE_NO_EDGE);

   /* Enable interrupt for the specified GPIO Input Pin. */
   GPIOPinIntEnable(SOC_GPIO_1_REGS, GPIO_INT_LINE_1, 31);

} /* GPMCConfigureIRQ */

/*** EOF ***/

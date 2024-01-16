/**************************************************************************
*  Copyright (c) 2017 by Michael Fischer (www.emb4fun.de).
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
*  12.02.2017  mifi  First Version, tested with a BeagleBone Black (B3).
*  14.07.2019  mifi  Change UART_STDIO_PORT to port 0.
**************************************************************************/
#define __MAIN_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/
#include <stdint.h>
#include "tal.h"

#include "bl.h"
#include "bl_platform.h"
#include "bl_copy.h"
#include "uartStdio.h"
#include "hw_intc.h"
#include "hw_types.h"
#include "soc_AM335x.h"

void EnableGPIOClocks (void);

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all global Data                                        */
/*=======================================================================*/

unsigned int entryPoint = 0;

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  OutputBootMessage                                                    */
/*                                                                       */
/*  Output boot message.                                                 */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void OutputBootMessage (void)
{
   const char         ResetScreen[] = { 0x1B, 'c', 0 };
   uint32_t          dDelayCnt;
   volatile uint32_t dValue = 0;
   
   UARTprintf("%s", ResetScreen);
   for (dDelayCnt = 0; dDelayCnt < 1000000; dDelayCnt++)
   {
      dValue++;
   }

   UARTprintf("\r\n");
   UARTprintf("*********************************\r\n");
   UARTprintf("  Project: %s\r\n", PROJECT_NAME);
   UARTprintf("  Board  : %s\r\n", tal_BoardGetName());
   UARTprintf("  Version: v%s\r\n", PROJECT_VER_STRING);
   UARTprintf("  Build  : "__DATE__ " " __TIME__"\r\n");
   UARTprintf("*********************************\r\n");
   
   (void)dValue;
} /* OutputBootMessage */


/*************************************************************************/
/*  Name  : DisableAllSystemInts                                         */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void DisableAllSystemInts (void)
{
   /* Reset the ARM interrupt controller */
   HWREG(SOC_AINTC_REGS + INTC_SYSCONFIG) = INTC_SYSCONFIG_SOFTRESET;

   /* Wait for the reset to complete */
   while((HWREG(SOC_AINTC_REGS + INTC_SYSSTATUS) 
         & INTC_SYSSTATUS_RESETDONE) != INTC_SYSSTATUS_RESETDONE);   /*lint !e722*/    
} /* DisableAllSystemInts */

/*************************************************************************/
/*  Name  : StartApplication                                             */
/*                                                                       */
/*  Giving control to the application.                                   */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void StartApplication (void)
{
   void (*AppEntry)(void);
   
   /*
    * Check if a valid application is available
    */
   AppEntry = (void (*)(void))entryPoint;     
   if (AppEntry != NULL)
   {
      unsigned sctlr = 1; /* remove warning */
   
      TAL_CPU_DISABLE_ALL_INTS();
      
      DisableAllSystemInts();
      
      /*lint +rw(_to_semi)    */
      /*lint -d__mrc=_to_semi */
      /*lint -d__mcr=_to_semi */
      
      /* Disable Insruction, Data cache and MMU */  
      sctlr  = __mrc(15, 0, 1, 0, 0);
      sctlr &= ~((1<<0) | (1<<2) | (1<<12)); /*lint !e564*/
      __mcr(15, 0, sctlr, 1, 0, 0);
      __mcr(15, 0, 0, 7, 5, 0);
      
      AppEntry(); /* Start application */
      
      TAL_CPU_ENABLE_ALL_INTS();
      
      (void)sctlr;
   }      
} /* StartApplication */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  main                                                                 */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: never                                                        */
/*************************************************************************/
int main (void)
{
   /* 
    * Configures PLL and DDR controller 
    */
   BlPlatformConfig();
   
   /*
    * Initializes the UART instance for the console operations
    */
   UARTStdioInit(UART_STDIO_PORT);
   
   /* 
    * LED init 
    */
   EnableGPIOClocks();
   tal_LEDInit();   
   
   /*
    * Output boot messages
    */
   OutputBootMessage();
   
   /* 
    * Copies application from non-volatile flash memory to RAM 
    */
   ImageCopy();
   
   /* 
    * Do any post-copy config before leaving boot loader 
    */
   BlPlatformConfigPostBoot();
   
   /* 
    * Start application if available
    */
   StartApplication();
   
   /*
    * !!! Boot error !!!
    */
   BootAbort();
   
   /*
    * This return here make no sense.
    * But to prevent the compiler warning:
    *    "return type of 'main' is not 'int'
    * We use an int as return :-)
    */ 
   return(0); /*lint !e527*/
} /* main */

/*************************************************************************/
/*  Name  : BlPlatformConfigPostBoot                                     */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void BlPlatformConfigPostBoot (void)
{
} /* BlPlatformConfigPostBoot */

/*************************************************************************/
/*  Name  : BootAbort                                                    */
/*                                                                       */
/*  This function is called in case of an boot error from the            */
/*  TI StarterWare Boot Loader code.                                     */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: never                                                        */
/*************************************************************************/
void BootAbort (void)
{
   uint32_t          dLoopCnt;
   uint32_t          dDelayCnt;
   volatile uint32_t dValue = 0;
            
   /*
    * Toggle all LEDs to show that there exist a problem.
    */      
   for (dLoopCnt = 0; dLoopCnt < 5; dLoopCnt++)
   {
      tal_LEDSet(TAL_LED_CHANNEL_5_R);
      tal_LEDSet(TAL_LED_CHANNEL_6_R);
      tal_LEDSet(TAL_LED_CHANNEL_7_R);
      tal_LEDSet(TAL_LED_CHANNEL_8_R);
      
      for (dDelayCnt = 0; dDelayCnt < 1000000; dDelayCnt++)
      {
         dValue++;
      }
      
      tal_LEDClear(TAL_LED_CHANNEL_5_R);
      tal_LEDClear(TAL_LED_CHANNEL_6_R);
      tal_LEDClear(TAL_LED_CHANNEL_7_R);
      tal_LEDClear(TAL_LED_CHANNEL_8_R);
      
      for (dDelayCnt = 0; dDelayCnt < 1000000; dDelayCnt++)
      {
         dValue++;
      }
   }
   
   (void)dValue;

   /*
    * Reset now...
    */
#ifdef DEBUG
   /* DEBUG mode */
   while (1)
   {
      __asm("nop");
   }
#else
   /* Release mode */
   tal_BoardReset();
#endif   
} /* BootAbort */

#ifdef NDEBUG  /* CrossWorks for ARM Release mode */
/*************************************************************************/
/*  Name  : xxxx_handler                                                 */
/*                                                                       */
/*  Default exception handler of the ARM system.                         */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: never                                                        */
/*************************************************************************/
void undef_handler (void)
{
   tal_BoardReset();
}

void swi_handler (void)
{
   tal_BoardReset();
}

void pabort_handler (void)
{
   tal_BoardReset();
}

void dabort_handler (void)
{
   tal_BoardReset();
}

void fiq_handler (void)
{
   tal_BoardReset();
}
#endif /* CrossWorks for ARM Release mode */

/*** EOF ***/

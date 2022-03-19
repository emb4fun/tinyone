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
#if defined(USE_BOARD_B3)
#define __TALBOARD_C__

/*=======================================================================*/
/*  Include                                                              */
/*=======================================================================*/
#include <string.h>
#include "tal.h"

#include "soc_AM335x.h"
#include "hw_control_AM335x.h"
#include "hw_types.h"
#include "hw_cm_wkup.h"
#include "hw_cm_per.h"
#include "watchdog.h"

extern void cpsw_GetMACAddress (uint8_t *pAddress);

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

/*
 * Control register
 */
#define CONTROL_CONF_MMODE_SHIFT       0
#define CONTROL_CONF_PUDEN_SHIFT       3
#define CONTROL_CONF_PUTYPESEL_SHIFT   4
#define CONTROL_CONF_RXACTIVE_SHIFT    5

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  tal_BoardEnableCOMx                                                  */
/*                                                                       */
/*  Board and hardware depending functionality to enable the COM port.   */
/*  If this port is not supported by the board, return an error.         */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: TAL_OK / error cause                                         */
/*************************************************************************/
TAL_RESULT tal_BoardEnableCOM1 (void)
{
   /* 
    * Configure the system clocks for UARTx instance.
    * This is still done by configuring the L3, and clock in talcpu.c 
    */
   
   /* Writing to MODULEMODE field of CM_WKUP_UART0_CLKCTRL register. */
   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_UART0_CLKCTRL) |=
         CM_WKUP_UART0_CLKCTRL_MODULEMODE_ENABLE;

   /* Waiting for MODULEMODE field to reflect the written value. */
   while(CM_WKUP_UART0_CLKCTRL_MODULEMODE_ENABLE !=
         (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_UART0_CLKCTRL) &
          CM_WKUP_UART0_CLKCTRL_MODULEMODE)); /*lint !e722*/
          
   /*
   ** Waiting for CLKACTIVITY_UART0_GFCLK field in CM_WKUP_CLKSTCTRL
   ** register to attain desired value.
   */
   while(CM_WKUP_CLKSTCTRL_CLKACTIVITY_UART0_GFCLK !=
         (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) &
          CM_WKUP_CLKSTCTRL_CLKACTIVITY_UART0_GFCLK)); /*lint !e722*/
          
    
   /*
    * Performing the Pin Multiplexing for UARTx instance.
    */

   /* RXD */
   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_RXD(0)) = 
         (CONTROL_CONF_UART0_RXD_CONF_UART0_RXD_PUTYPESEL | 
          CONTROL_CONF_UART0_RXD_CONF_UART0_RXD_RXACTIVE);  /*lint !e845*/

   /* TXD */
   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_TXD(0)) = 
         CONTROL_CONF_UART0_TXD_CONF_UART0_TXD_PUTYPESEL;   /*lint !e845*/

   return(TAL_OK);
} /* tal_BoardEnableCOM1 */

TAL_RESULT tal_BoardEnableCOM2 (void)
{
   /*
    * Enable clock
    */
    
   /* Writing to MODULEMODE field of CM_PER_UART1_CLKCTRL register. */
   HWREG(SOC_CM_PER_REGS + CM_PER_UART1_CLKCTRL) = 
         CM_PER_UART1_CLKCTRL_MODULEMODE_ENABLE;

   /* Waiting for MODULEMODE field to reflect the written value. */
   while(CM_PER_UART1_CLKCTRL_MODULEMODE_ENABLE !=
         (HWREG(SOC_CM_PER_REGS + CM_PER_UART1_CLKCTRL) & 
          CM_PER_UART1_CLKCTRL_MODULEMODE)); /*lint !e722*/

   /* Waiting for activ UART clock */          
   while(!(HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKSTCTRL) &
           (CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_L4LS_GCLK |
           CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_UART_GFCLK))); /*lint !e722*/
          
   
   /*
    * Performing the Pin Multiplexing for UARTx instance.
    */

   /* RXD */
   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_RXD(1)) = 
         (CONTROL_CONF_UART1_RXD_CONF_UART1_RXD_PUTYPESEL | 
          CONTROL_CONF_UART1_RXD_CONF_UART1_RXD_RXACTIVE);  /*lint !e845*/

   /* TXD */
   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_TXD(1)) = 
         CONTROL_CONF_UART1_TXD_CONF_UART1_TXD_PUTYPESEL;   /*lint !e845*/

   return(TAL_OK);
} /* tal_BoardEnableCOM2 */

TAL_RESULT tal_BoardEnableCOM3 (void)
{
   return(TAL_ERR_COM_PORT_NOHW);
} /* tal_BoardEnableCOM3 */

TAL_RESULT tal_BoardEnableCOM4 (void)
{
   return(TAL_ERR_COM_PORT_NOHW);
} /* tal_BoardEnableCOM4 */

TAL_RESULT tal_BoardEnableCOM5 (void)
{
   return(TAL_ERR_COM_PORT_NOHW);
} /* tal_BoardEnableCOM5 */

TAL_RESULT tal_BoardEnableCOM6 (void)
{
   return(TAL_ERR_COM_PORT_NOHW);
} /* tal_BoardEnableCOM6 */

/*************************************************************************/
/*  tal_BoardEnableCANx                                                  */
/*                                                                       */
/*  Board and hardware depending functionality to enable the CAN port.   */
/*  If this port is not supported by the board, return an error.         */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: TAL_OK / error cause                                         */
/*************************************************************************/
TAL_RESULT tal_BoardEnableCAN1 (void)
{
   uint32_t Value;

   /*
    * Enable clock
    */

   /* Writing to MODULEMODE field of CM_PER_DCAN0_CLKCTRL register. */
   HWREG(SOC_CM_PER_REGS + CM_PER_DCAN0_CLKCTRL) = 
         CM_PER_DCAN0_CLKCTRL_MODULEMODE_ENABLE;

   /* Waiting for MODULEMODE field to reflect the written value. */
   while(CM_PER_DCAN0_CLKCTRL_MODULEMODE_ENABLE !=
         (HWREG(SOC_CM_PER_REGS + CM_PER_DCAN0_CLKCTRL) & 
          CM_PER_DCAN0_CLKCTRL_MODULEMODE));                /*lint !e722*/
          
   /* Waiting for activ CAN clock */          
   while(!(HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKSTCTRL) &
           (CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_L4LS_GCLK |
           CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_CAN_CLK)));    /*lint !e722*/


   /*
    * Performing the Pin Multiplexing for DCANx instance.
    */

   /* Setup Pad Control Registers, DCAN0 TX */
   Value = (2 << CONTROL_CONF_MMODE_SHIFT)     |   /* Mode 2 */
           (1 << CONTROL_CONF_PUDEN_SHIFT)     |   /* Pull disabled */
           (0 << CONTROL_CONF_PUTYPESEL_SHIFT) |   /* Pulldown */
           (0 << CONTROL_CONF_RXACTIVE_SHIFT);     /* Receiver disabled */ /*lint !e845*/
   
   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_CTSN(1)) = Value;


   /* Setup Pad Control Registers, DCAN0 RX */
   Value = (2 << CONTROL_CONF_MMODE_SHIFT)     |   /* Mode 2 */
           (0 << CONTROL_CONF_PUDEN_SHIFT)     |   /* Pull enabled */
           (1 << CONTROL_CONF_PUTYPESEL_SHIFT) |   /* Pullup */
           (1 << CONTROL_CONF_RXACTIVE_SHIFT);     /* Receiver enabled */  /*lint !e845*/
   
   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_RTSN(1)) = Value;

   return(TAL_OK);
} /* tal_BoardEnableCAN1 */

TAL_RESULT tal_BoardEnableCAN2 (void)
{
   return(TAL_ERR_CAN_PORT_NOHW);
} /* tal_BoardEnableCAN2 */

/*************************************************************************/
/*  tal_BoardGetMACAddress                                               */
/*                                                                       */
/*  Retrieve the MAC address of the board.                               */
/*  In case of an error, a default address will be used.                 */
/*                                                                       */
/*  In    : pAddress                                                     */
/*  Out   : pAddress                                                     */
/*  Return: TAL_OK / TAL_ERROR                                           */
/*************************************************************************/
TAL_RESULT tal_BoardGetMACAddress (uint8_t *pAddress)
{
   TAL_RESULT      Error         = TAL_ERROR;
   static uint8_t bMACRetrieved  = TAL_FALSE;
   static uint8_t  MACAddress[6] = {0x00,0x11,0x22,0x33,0x44,0x55};
   
   if (TAL_FALSE == bMACRetrieved)
   {
      bMACRetrieved = TAL_TRUE;
      
      cpsw_GetMACAddress(MACAddress);
   }
   
   /* Return MAC address */
   memcpy(pAddress, MACAddress, 6);
   
   return(Error);
} /* tal_BoardGetMACAddress */

/*************************************************************************/
/*  tal_BoardReset        (examples\beaglebone\watchdogTimer\wdtReset.c) */
/*                                                                       */
/*  Board and hardware depending functionality to reset the board/cpu.   */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: never                                                        */
/*************************************************************************/
void tal_BoardReset (void)
{

/*
 * The watchdog use a 32 bit timer clocked with 32kHz
 */
#define RESET_TIME_IN_MS      1000
#define INITIAL_COUNT_VALUE   (0xFFFFFFFF - (RESET_TIME_IN_MS * 32))
#define RELOAD_COUNT_VALUE    INITIAL_COUNT_VALUE

   /* Configure and enable the pre-scaler clock, use 32kHz */
   WatchdogTimerPreScalerClkEnable(SOC_WDT_1_REGS, WDT_PRESCALER_CLK_DIV_1);

   /* Set/Write the Counter register with the counter value */
   WatchdogTimerCounterSet(SOC_WDT_1_REGS, INITIAL_COUNT_VALUE);

   /* Set the reload count value in the Watchdog timer load register */
   WatchdogTimerReloadSet(SOC_WDT_1_REGS, RELOAD_COUNT_VALUE);
   
   /* Starts/Enables the watchdog timer */
   WatchdogTimerEnable(SOC_WDT_1_REGS);
   
   /* Wait for the reset by the watchdog */
   TAL_CPU_DISABLE_ALL_INTS();
   while(1)
   {
      __asm("nop");
   }
   TAL_CPU_ENABLE_ALL_INTS(); /*lint !e527*/

} /* tal_BoardReset */

/*************************************************************************/
/*  tal_BoardGetName                                                     */
/*                                                                       */
/*  Return the board name like "BeagleBone" or "BeagleBone Black".       */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: "BeagleBone" / "BeagleBone Black"                            */
/*************************************************************************/
char *tal_BoardGetName (void)
{
   uint32_t dDeviceIsB3;
   
   dDeviceIsB3 = (HWREG(SOC_CONTROL_REGS + CONTROL_DEVICE_ID) >> CONTROL_DEVICE_ID_DEVREV_SHIFT);    

   if (0 == dDeviceIsB3)
   {
      return("BeagleBone");
   }
   else
   {
      return("BeagleBone Black");
   }
   
} /* tal_BoardGetName */

#endif /* USE_BOARD_B3 */

/*** EOF ***/

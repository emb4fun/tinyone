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
*  31.08.2019  mifi  Added support for USE_BOARD_B3_HIL01.
**************************************************************************/
#if defined(USE_BOARD_B3)
#define __TALLED_C__

/*=======================================================================*/
/*  Include                                                              */
/*=======================================================================*/
#include <stdint.h>
#include <string.h>
#include "tal.h"

#include "soc_AM335x.h"
#include "hw_control_AM335x.h"
#include "hw_types.h"
#include "hw_gpio_v2.h"

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


/*
 * LED status
 */
#define LED_OFF   0
#define LED_ON    1

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

static uint8_t LedStatus[TAL_LED_CHANNEL_MAX];

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

#if defined(USE_BOARD_B3_CAPE01)   
/*************************************************************************/
/*  InitCape01                                                           */
/*                                                                       */
/*  Initialize the LEDs of the cape.                                     */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void InitCape01 (void)
{
   uint32_t Value;
   
   /* Setup Pad Control Registers */
   Value = (7 << CONTROL_CONF_MMODE_SHIFT)     |   /* Mode 7 */
           (1 << CONTROL_CONF_PUDEN_SHIFT)     |   /* Pull disabled */
           (1 << CONTROL_CONF_PUTYPESEL_SHIFT) |   /* Pullup */
           (0 << CONTROL_CONF_RXACTIVE_SHIFT);     /* Receiver disabled */ /*lint !e845*/
           
   /*
    * Initialize GPIO0 led's
    */

   /* Switch off all LEDs first to prevent glitches */   
   HWREG(SOC_GPIO_0_REGS + GPIO_CLEARDATAOUT) = ((1<<26) | (1<<27));
   
   /* Setup Pad Control Registers */
   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_AD(10)) = Value; /* GPIO0_26 */
   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_AD(11)) = Value; /* GPIO0_27 */

   /* Module is enabled, clocks are not gated. */
   HWREG(SOC_GPIO_0_REGS + GPIO_CTRL) &= ~GPIO_CTRL_DISABLEMODULE;

   /* Clear the corresponding bits in the GPIO_DATAOUT register, set outputs to low. */
   HWREG(SOC_GPIO_0_REGS + GPIO_CLEARDATAOUT) = ((1<<26) | (1<<27));
   
   /* The corresponding GPIO ports are configured as an output. */
   HWREG(SOC_GPIO_0_REGS + GPIO_OE) &= ~((1<<26) | (1<<27));


   /*
    * Initialize GPIO1 led's
    */
   
   /* Switch off all LEDs first to prevent glitches */   
   HWREG(SOC_GPIO_1_REGS + GPIO_CLEARDATAOUT) = ((1<<12) | (1<<13) | (1<<14) | (1<<15) | (1<<29));

   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_AD(12)) = Value; /* GPIO1_12 */
   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_AD(13)) = Value; /* GPIO1_13 */
   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_AD(14)) = Value; /* GPIO1_14 */
   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_AD(15)) = Value; /* GPIO1_15 */
   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_CSN(0)) = Value; /* GPIO1_29 */
   
   /* Module is enabled, clocks are not gated. */
   HWREG(SOC_GPIO_1_REGS + GPIO_CTRL) &= ~GPIO_CTRL_DISABLEMODULE;

   /* Clear the corresponding bits in the GPIO_DATAOUT register, set outputs to low. */
   HWREG(SOC_GPIO_1_REGS + GPIO_CLEARDATAOUT) = ((1<<12) | (1<<13) | (1<<14) | (1<<15) | (1<<29));
   
   /* The corresponding GPIO ports are configured as an output. */
   HWREG(SOC_GPIO_1_REGS + GPIO_OE) &= ~((1<<12) | (1<<13) | (1<<14) | (1<<15) | (1<<29));


   /*
    * Initialize GPIO2 led's
    */
    
   /* Switch off all LEDs first to prevent glitches */   
   HWREG(SOC_GPIO_2_REGS + GPIO_CLEARDATAOUT) = (1<<1);

   /* Setup Pad Control Registers */
   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_CLK) = Value; /* GPIO2_1 */

   /* Module is enabled, clocks are not gated. */
   HWREG(SOC_GPIO_2_REGS + GPIO_CTRL) &= ~GPIO_CTRL_DISABLEMODULE;

   /* Clear the corresponding bits in the GPIO_DATAOUT register, set outputs to low. */
   HWREG(SOC_GPIO_2_REGS + GPIO_CLEARDATAOUT) = (1<<1);
   
   /* The corresponding GPIO ports are configured as an output. */
   HWREG(SOC_GPIO_2_REGS + GPIO_OE) &= ~(1<<1);

} /* InitCape01 */
#endif /* USE_BOARD_B3_CAPE01 */

#if defined(USE_BOARD_B3_HIL01)   
/*************************************************************************/
/*  InitHil01                                                            */
/*                                                                       */
/*  Initialize the LEDs of the cape.                                     */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void InitHil01 (void)
{
   uint32_t Value;
   
   /* Setup Pad Control Registers */
   Value = (7 << CONTROL_CONF_MMODE_SHIFT)     |   /* Mode 7 */
           (1 << CONTROL_CONF_PUDEN_SHIFT)     |   /* Pull disabled */
           (1 << CONTROL_CONF_PUTYPESEL_SHIFT) |   /* Pullup */
           (0 << CONTROL_CONF_RXACTIVE_SHIFT);     /* Receiver disabled */ /*lint !e845*/
           
   /*
    * Initialize GPIO0 led's
    */

   /* Switch off all LEDs first to prevent glitches */   
   HWREG(SOC_GPIO_0_REGS + GPIO_CLEARDATAOUT) = ((1<<8) | (1<<9) | (1<<10));
   
   /* Setup Pad Control Registers */
   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_DATA(12)) = Value; /* GPIO0_8  */
   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_DATA(13)) = Value; /* GPIO0_9  */
   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_LCD_DATA(14)) = Value; /* GPIO0_10 */

   /* Module is enabled, clocks are not gated. */
   HWREG(SOC_GPIO_0_REGS + GPIO_CTRL) &= ~GPIO_CTRL_DISABLEMODULE;

   /* Clear the corresponding bits in the GPIO_DATAOUT register, set outputs to low. */
   HWREG(SOC_GPIO_0_REGS + GPIO_CLEARDATAOUT) = ((1<<8) | (1<<9) | (1<<10));
   
   /* The corresponding GPIO ports are configured as an output. */
   HWREG(SOC_GPIO_0_REGS + GPIO_OE) &= ~((1<<8) | (1<<9) | (1<<10));
   
} /* InitHil01 */
#endif /* USE_BOARD_B3_HIL01 */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  tal_LEDInit                                                          */
/*                                                                       */
/*  Initialize the LEDs of the board.                                    */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void tal_LEDInit (void)
{
   uint32_t Value;

   /* Clear status information */
   memset(LedStatus, LED_OFF, sizeof(LedStatus));
   
   /* Setup Pad Control Registers */
   Value = (7 << CONTROL_CONF_MMODE_SHIFT)     |   /* Mode 7 */
           (1 << CONTROL_CONF_PUDEN_SHIFT)     |   /* Pull disabled */
           (1 << CONTROL_CONF_PUTYPESEL_SHIFT) |   /* Pullup */
           (0 << CONTROL_CONF_RXACTIVE_SHIFT);     /* Receiver disabled */ /*lint !e845*/

   
   /*
    * Initialize GPIO1 led's
    */
   
   /* Switch off all LEDs first to prevent glitches */   
   HWREG(SOC_GPIO_1_REGS + GPIO_CLEARDATAOUT) = ((1<<21) | (1<<22) | (1<<23) | (1<<24));

   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_A(5)) = Value; /* GPIO1_21 */
   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_A(6)) = Value; /* GPIO1_22 */
   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_A(7)) = Value; /* GPIO1_23 */
   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_GPMC_A(8)) = Value; /* GPIO1_24 */
   
   /* Module is enabled, clocks are not gated. */
   HWREG(SOC_GPIO_1_REGS + GPIO_CTRL) &= ~GPIO_CTRL_DISABLEMODULE;

   /* Clear the corresponding bits in the GPIO_DATAOUT register, set outputs to low. */
   HWREG(SOC_GPIO_1_REGS + GPIO_CLEARDATAOUT) = ((1<<21) | (1<<22) | (1<<23) | (1<<24));
   
   /* The corresponding GPIO ports are configured as an output. */
   HWREG(SOC_GPIO_1_REGS + GPIO_OE) &= ~((1<<21) | (1<<22) | (1<<23) | (1<<24));


   /*
    * Init LEDs of the cape
    */

#if defined(USE_BOARD_B3_CAPE01)   
   InitCape01();
#endif   

#if defined(USE_BOARD_B3_HIL01)   
   InitHil01();
#endif

} /* tal_LEDInit */

/*************************************************************************/
/*  tal_LEDSet                                                           */
/*                                                                       */
/*  Set the given LED.                                                   */
/*                                                                       */
/*  In    : eChannel                                                     */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void tal_LEDSet (TAL_LED_CHANNEL eChannel)
{
   /* Check for valid range */
   if ((uint8_t)eChannel < (uint8_t)TAL_LED_CHANNEL_MAX)
   {
      switch (eChannel)
      {
         case TAL_LED_CHANNEL_1:   HWREG(SOC_GPIO_1_REGS + GPIO_SETDATAOUT) = (1<<21); break;
         case TAL_LED_CHANNEL_2:   HWREG(SOC_GPIO_1_REGS + GPIO_SETDATAOUT) = (1<<22); break;
         case TAL_LED_CHANNEL_3:   HWREG(SOC_GPIO_1_REGS + GPIO_SETDATAOUT) = (1<<23); break;
         case TAL_LED_CHANNEL_4:   HWREG(SOC_GPIO_1_REGS + GPIO_SETDATAOUT) = (1<<24); break;
         
#if defined(USE_BOARD_B3_CAPE01)
         case TAL_LED_CHANNEL_5_G: HWREG(SOC_GPIO_1_REGS + GPIO_SETDATAOUT) = (1<<12); break;
         case TAL_LED_CHANNEL_5_R: HWREG(SOC_GPIO_1_REGS + GPIO_SETDATAOUT) = (1<<13); break;
         case TAL_LED_CHANNEL_6_G: HWREG(SOC_GPIO_0_REGS + GPIO_SETDATAOUT) = (1<<26); break;
         case TAL_LED_CHANNEL_6_R: HWREG(SOC_GPIO_1_REGS + GPIO_SETDATAOUT) = (1<<15); break;
         case TAL_LED_CHANNEL_7_G: HWREG(SOC_GPIO_1_REGS + GPIO_SETDATAOUT) = (1<<14); break;
         case TAL_LED_CHANNEL_7_R: HWREG(SOC_GPIO_0_REGS + GPIO_SETDATAOUT) = (1<<27); break;
         case TAL_LED_CHANNEL_8_G: HWREG(SOC_GPIO_2_REGS + GPIO_SETDATAOUT) = (1<< 1); break;
         case TAL_LED_CHANNEL_8_R: HWREG(SOC_GPIO_1_REGS + GPIO_SETDATAOUT) = (1<<29); break;
#endif

#if defined(USE_BOARD_B3_HIL01)
         case TAL_LED_CHANNEL_5_R: HWREG(SOC_GPIO_0_REGS + GPIO_SETDATAOUT) = (1<<10); break;
         case TAL_LED_CHANNEL_6_Y: HWREG(SOC_GPIO_0_REGS + GPIO_SETDATAOUT) = (1<< 9); break;
         case TAL_LED_CHANNEL_7_G: HWREG(SOC_GPIO_0_REGS + GPIO_SETDATAOUT) = (1<< 8); break;
#endif
                                   
         default:
         {
            /* Do nothing */
            break;
         }
      }
      
      LedStatus[eChannel] = LED_ON;
   }
   
} /* hal_LEDSet */

/*************************************************************************/
/*  tal_LEDClear                                                         */
/*                                                                       */
/*  Clear the given LED.                                                 */
/*                                                                       */
/*  In    : eChannel                                                     */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void tal_LEDClear (TAL_LED_CHANNEL eChannel)
{
   /* Check for valid range */
   if ((uint8_t)eChannel < (uint8_t)TAL_LED_CHANNEL_MAX)
   {
      switch (eChannel)
      {
         case TAL_LED_CHANNEL_1:   HWREG(SOC_GPIO_1_REGS + GPIO_CLEARDATAOUT) = (1<<21); break;
         case TAL_LED_CHANNEL_2:   HWREG(SOC_GPIO_1_REGS + GPIO_CLEARDATAOUT) = (1<<22); break;
         case TAL_LED_CHANNEL_3:   HWREG(SOC_GPIO_1_REGS + GPIO_CLEARDATAOUT) = (1<<23); break;
         case TAL_LED_CHANNEL_4:   HWREG(SOC_GPIO_1_REGS + GPIO_CLEARDATAOUT) = (1<<24); break;
         
#if defined(USE_BOARD_B3_CAPE01)
         case TAL_LED_CHANNEL_5_G: HWREG(SOC_GPIO_1_REGS + GPIO_CLEARDATAOUT) = (1<<12); break;
         case TAL_LED_CHANNEL_5_R: HWREG(SOC_GPIO_1_REGS + GPIO_CLEARDATAOUT) = (1<<13); break;
         case TAL_LED_CHANNEL_6_G: HWREG(SOC_GPIO_0_REGS + GPIO_CLEARDATAOUT) = (1<<26); break;
         case TAL_LED_CHANNEL_6_R: HWREG(SOC_GPIO_1_REGS + GPIO_CLEARDATAOUT) = (1<<15); break;
         case TAL_LED_CHANNEL_7_G: HWREG(SOC_GPIO_1_REGS + GPIO_CLEARDATAOUT) = (1<<14); break;
         case TAL_LED_CHANNEL_7_R: HWREG(SOC_GPIO_0_REGS + GPIO_CLEARDATAOUT) = (1<<27); break;
         case TAL_LED_CHANNEL_8_G: HWREG(SOC_GPIO_2_REGS + GPIO_CLEARDATAOUT) = (1<< 1); break;
         case TAL_LED_CHANNEL_8_R: HWREG(SOC_GPIO_1_REGS + GPIO_CLEARDATAOUT) = (1<<29); break;
#endif

#if defined(USE_BOARD_B3_HIL01)
         case TAL_LED_CHANNEL_5_R: HWREG(SOC_GPIO_0_REGS + GPIO_CLEARDATAOUT) = (1<<10); break;
         case TAL_LED_CHANNEL_6_Y: HWREG(SOC_GPIO_0_REGS + GPIO_CLEARDATAOUT) = (1<< 9); break;
         case TAL_LED_CHANNEL_7_G: HWREG(SOC_GPIO_0_REGS + GPIO_CLEARDATAOUT) = (1<< 8); break;
#endif

         default:
         {
            /* Do nothing */
            break;
         }
      }
      
      LedStatus[eChannel] = LED_OFF;
   }
   
} /* tal_LEDClear */

/*************************************************************************/
/*  tal_LEDToggle                                                        */
/*                                                                       */
/*  Toggle the given LED.                                                */
/*                                                                       */
/*  In    : eChannel                                                     */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void tal_LEDToggle (TAL_LED_CHANNEL eChannel)
{
   /* Check for valid range */
   if ((uint8_t)eChannel < (uint8_t)TAL_LED_CHANNEL_MAX)
   {
      if (LED_ON == LedStatus[eChannel])
      { 
         tal_LEDClear(eChannel); 
      }
      else
      {
         tal_LEDSet(eChannel); 
      }         
   }
   
} /* tal_LEDClear */

#endif /* USE_BOARD_B3 */

/*** EOF ***/

/**************************************************************************
*  This file is part of the TAL project (Tiny Abstraction Layer)
*
*  Copyright (c) 2016-2023 by Michael Fischer (www.emb4fun.de).
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
#if !defined(__TALBOARD_H__) && defined(USE_BOARD_B3)
#define __TALBOARD_H__

/**************************************************************************
*  Includes
**************************************************************************/
#include <stdint.h>
#include "project.h"
#include "taltypes.h"

#include "b3gpmc.h"

/**************************************************************************
*  Global Definitions
**************************************************************************/

/*
 * External oscillator frequency in Hz
 */
#define OSC_CLOCK_FREQUENCY   24000000UL


/*
 * Defines for board and cpu name
 */
//#define TAL_BOARD       "BeagleBone Black"
/* 
 * For board name use tal_BoardGetName()
 */
#define TAL_CPU         "AM335x"


/*
 * With 6 bits of interrupt priority 63 levels are
 * available. Priority 0 is the highest one, 63 the lowest.
 */

#if !defined(CPU_TINT2_PRIO)
#define CPU_TINT2_PRIO        4     /* SysTick */
#endif

#if !defined(CPU_ENET_PRIO)
#define CPU_ENET_PRIO         8
#endif

#if !defined(CPU_UART0_PRIO)
#define CPU_UART0_PRIO        10
#endif

#if !defined(CPU_UART1_PRIO)
#define CPU_UART1_PRIO        10
#endif

#if !defined(CPU_CAN0_PRIO)
#define CPU_CAN0_PRIO         9
#endif
 
#if !defined(CPU_CAN1_PRIO)
#define CPU_CAN1_PRIO         9
#endif


#define SYSTICK_INT     CPU_TINT2_INT 
#define SYSTICK_PRIO    CPU_TINT2_PRIO


/*
 * The BeagleBone Black board support 4 LEDs
 */
typedef enum _tal_led_channel_
{
   TAL_LED_CHANNEL_1 = 0,
   TAL_LED_CHANNEL_2,
   TAL_LED_CHANNEL_3,
   TAL_LED_CHANNEL_4,

#if defined(USE_BOARD_B3_CAPE01)
   TAL_LED_CHANNEL_5_G,
   TAL_LED_CHANNEL_5_R,
   TAL_LED_CHANNEL_6_G,
   TAL_LED_CHANNEL_6_R,
   TAL_LED_CHANNEL_7_G,
   TAL_LED_CHANNEL_7_R,
   TAL_LED_CHANNEL_8_G,
   TAL_LED_CHANNEL_8_R,
#endif

#if defined(USE_BOARD_B3_HIL01)
   TAL_LED_CHANNEL_5_R,
   TAL_LED_CHANNEL_6_Y,
   TAL_LED_CHANNEL_7_G,
#endif
   
   /* TAL_LED_CHANNEL_MAX must be the last one */
   TAL_LED_CHANNEL_MAX
} TAL_LED_CHANNEL;

/*
 * Some LED name's
 */
#define LED1   TAL_LED_CHANNEL_1
#define LED2   TAL_LED_CHANNEL_2
#define LED3   TAL_LED_CHANNEL_3
#define LED4   TAL_LED_CHANNEL_4


/*
 * COM port used for the terminal
 */
#if !defined(TERM_COM_PORT)
#define TERM_COM_PORT   TAL_COM_PORT_1
#endif


/*
 * Configure external memory for the HEAP
 */
#define TAL_HEAP_MEM1_START   __SDRAM_segment_used_end__
#define TAL_HEAP_MEM1_END     __SDRAM_segment_end__


/* Support heap memory more than 256 MBytes */   
#define TAL_MEM_SUPPORT_BIG_MEM     1

/**************************************************************************
*  Macro Definitions
**************************************************************************/

/* No support RTC support */
#define tal_BoardRTCSetTM(_a)
#define tal_BoardRTCSetUnixtime(_a)
#define tal_BoardRTC2System()

/**************************************************************************
*  Functions Definitions
**************************************************************************/

TAL_RESULT tal_BoardEnableCOM1 (void);
TAL_RESULT tal_BoardEnableCOM2 (void);
TAL_RESULT tal_BoardEnableCOM3 (void);
TAL_RESULT tal_BoardEnableCOM4 (void);
TAL_RESULT tal_BoardEnableCOM5 (void);
TAL_RESULT tal_BoardEnableCOM6 (void);

TAL_RESULT tal_BoardEnableCAN1 (void);
TAL_RESULT tal_BoardEnableCAN2 (void);

TAL_RESULT tal_BoardGetMACAddress (uint8_t *pAddress);

void       tal_BoardReset (void);
char      *tal_BoardGetName (void);


#define SD_PRESENT         ((uint8_t)0x01)
#define SD_NOT_PRESENT     ((uint8_t)0x00)

uint8_t SD_Detect (void);

extern int32_t NutHeapUsedRawMemory;

#endif /* !__TALBOARD_H__ */

/*** EOF ***/

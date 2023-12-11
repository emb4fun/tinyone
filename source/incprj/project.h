/**************************************************************************
*  Copyright (c) 2019-2023 by Michael Fischer (www.emb4fun.de).
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
#if !defined(__PROJECT_H__)
#define __PROJECT_H__

/**************************************************************************
*  Includes
**************************************************************************/

/**************************************************************************
*  Global Definitions
**************************************************************************/

/*
 * Project name and version info
 */
#define PROJECT_NAME          "TinyONE"
#define PROJECT_NAME_LONG     "Tiny ONE Server"

#define PROJECT_VER_MAJOR     1
#define PROJECT_VER_MINOR_1   2
#define PROJECT_VER_MINOR_2   2

#define PROJECT_VER_NUMBER    ((PROJECT_VER_MAJOR * 100) + (PROJECT_VER_MINOR_1 * 10) + PROJECT_VER_MINOR_2)

#define PROJECT_VER_STRING    (XSTR(PROJECT_VER_MAJOR) "." \
                               XSTR(PROJECT_VER_MINOR_1)   \
                               XSTR(PROJECT_VER_MINOR_2))


/*-----------------------------------------------------------------------*/
/*    Task priorities and delays                                         */
/*-----------------------------------------------------------------------*/

/*
 * Define priority and stack size of the tasks.
 * In case of CTS, valid priorities are in the range 
 * from HIGHPRIO (1) ... (255) LOWPRIO.
 */

#define TASK_START_PRIORITY               1           /* This is the start priority */
#define TASK_START_PRIORITY_IDLE          OS_PRIO_MAX /* The START thread is not needed later */
#define TASK_START_STK_SIZE               (4*1024)

#define TASK_TERM_PRIORITY                64
#define TASK_TERM_STK_SIZE                1024

#define TASK_LED_PRIORITY                 64
#define TASK_LED_STK_SIZE                 512

/**************************************************************/

#define TASK_IP_IPERF_PRIORITY            64
#define TASK_IP_IPERF_SERVER_STK_SIZE     768
#define TASK_IP_IPERF_CLIENT_STK_SIZE     768

#define TASK_IP_WEB_TLS_SERVER_PRIORITY   64
#define TASK_IP_WEB_TLS_SERVER_STK_SIZE   768
#define TASK_IP_WEB_TLS_CLIENT_STK_SIZE   (6*1024)

#define TASK_IP_WEB_SERVER_PRIORITY       44
#define TASK_IP_WEB_SERVER_STK_SIZE       768
#define TASK_IP_WEB_CLIENT_STK_SIZE       (6*1024)

#define TASK_IP_SNTP_CLIENT_PRIORITY      42
#define TASK_IP_SNTP_CLIENT_STK_SIZE      1024

#define TASK_IP_SNTP_SERVER_PRIORITY      41
#define TASK_IP_SNTP_SERVER_STK_SIZE      1024

#define TASK_IP_ELCAC_PRIORITY            40
#define TASK_IP_ELCAC_STK_SIZE            (8*1024)

#define TASK_IP_TNP_PRIORITY              39
#define TASK_IP_TNP_STK_SIZE              1536

#define TASK_IP_MDNS_PRIORITY             38
#define TASK_IP_MDNS_STK_SIZE             768

#define TASK_IP_DHCP_SERVER_PRIORITY      37
#define TASK_IP_DHCP_SERVER_STK_SIZE      1024

/**************************************************************/

#define TASK_IP_PRIORITY                  (TASK_IP_RX_PRIORITY + 1)
#define TASK_IP_STK_SIZE                  2048

/*
 * The priority of the IP_RX task must be the highest one of
 * all IP tasks. Than comes the IP task and all IP application.
 */
#define TASK_IP_RX_PRIORITY               32
#define TASK_IP_RX_STK_SIZE               2048

/**************************************************************/

#define TASK_SD0FS_PRIORITY               30
#define TASK_SD0FS_STK_SIZE               1536


/*
 * Define delay times of the tasks
 */
#define TASK_START_DELAY_MS            1000
#define TASK_TERM_DELAY_MS             100
#define TASK_LED_DELAY_MS              500


/*-----------------------------------------------------------------------*/
/* Terminal stuff                                                        */
/*-----------------------------------------------------------------------*/

/*
 * TERM_COM_PORT is defined in talboard.h
 */
#define TERM_COM_SPEED                 115200 


/*-----------------------------------------------------------------------*/
/* lwIP and other project specific                                       */
/*-----------------------------------------------------------------------*/
//#define PRJ_LWIP_IPV6                  1
//#define USE_IP_DEFAULT_MAC_ADDR


/**************************************************************************
*  Macro Definitions
**************************************************************************/

/**************************************************************************
*  Functions Definitions
**************************************************************************/
 
#endif /* !__PROJECT_H__ */

/*** EOF ***/

/**************************************************************************
*  Copyright (c) 2019 by Michael Fischer (www.emb4fun.de).
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
*  14.07.2019  mifi  First version for the BeagleBone Black.
**************************************************************************/
#define __MAIN_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/
#include <stdio.h>
#include "tal.h"
#include "terminal.h"
#include "fs.h"
#include "etc.h"
#include "nvm.h"
#include "ipstack.h"
#include "ipweb.h"
#include "cert.h"
#include "xmempool.h"
#include "mbedtls/version.h"
#include "iperf.h"

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

#define LED_MODE_WAIT   0
#define LED_MODE_READY  1

/*=======================================================================*/
/*  Definition of all global Data                                        */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all extern Data                                        */
/*=======================================================================*/

extern int nNumThreadsMax;
extern int nNumThreadsMaxTLS;

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

/* 
 * Some TASK variables like stack and task control block.
 */
static OS_STACK (StartStack,  TASK_START_STK_SIZE);
static OS_STACK (LEDStack,    TASK_LED_STK_SIZE);

static OS_TCB TCBStartTask;
static OS_TCB TCBLed;

/*
 * mDNS configuration
 */
static ip_mdns_config_t MDNSConfig = 
{
   .Hostname  = "tiny",
   .Hostname2 = NULL,
   .TTL       = 0
};
static char MDNSName2[32] = "tiny";

static int nDHCPCallbackBound = 0;
static int nLedMode = LED_MODE_WAIT; 

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  DHCPCallbackBound                                                    */
/*                                                                       */
/*  Callback is called whenever the DHCP is bound.                       */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void DHCPCallbackBound (void)
{
   nDHCPCallbackBound = 1;
   
} /* DHCPCallbackBound */

/*************************************************************************/
/*  TimeInit                                                             */
/*                                                                       */
/*  Initialize the system time.                                          */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void TimeInit (void)
{
   NVM_TIME Time;

   /* 
    * Configure Time settings 
    */
   nvm_TimeGet(&Time);

   OS_TimezoneIDSet((int16_t)Time.nZoneID);
   OS_TimezoneMinSet((int16_t)Time.nZoneOffset);
   OS_TimezoneDstSet((int8_t)Time.nZoneDst);

   IP_SNTP_ServerSet(Time.dNTPAddr);
   IP_SNTP_RefreshSet(Time.dNTPRefresh);   

   /* Set the system time from the RTC */
   tal_BoardRTC2System();

} /* TimeInit */

/*************************************************************************/
/*  SecFunction                                                          */
/*                                                                       */
/*  This function wil be called each second.                             */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void SecFunction (void)
{
   static int nOldMode = 1;
   static int nNewMode = 1;
   
   if (1 == nDHCPCallbackBound)
   {
      nDHCPCallbackBound = 0;
      
      IP_mDNS_Start(&MDNSConfig); 
      IP_TNP_SetName(MDNSConfig.Hostname2, (uint8_t)strlen(MDNSConfig.Hostname2));

      IP_TNP_DHCPCallbackBound(0);
      IP_SNTP_TimeSync();
      
      nLedMode = LED_MODE_READY;;
   }
   
   /********************************************/   
   
   nNewMode = IP_IF_IsReady(IFACE_ANY);
   if (nOldMode != nNewMode)
   {
      /* Check for lost */
      if ((1 == nOldMode) && (0 == nNewMode))
      {
         nLedMode = LED_MODE_WAIT;
         IP_DHCP_Stop(0);
      }
        
      /* Check for available */
      if ((0 == nOldMode) && (1 == nNewMode))
      {
         if (etc_IPDhcpIsUsed() != 0)
         {
            /* LED mode will be set in DHCPCallbackBound */
            IP_DHCP_CallbackSet(0, DHCPCallbackBound);   
            IP_DHCP_Start(0);
         }
         else
         {
            nLedMode = LED_MODE_READY;
         }
      }
  
      nOldMode = nNewMode;
   }
   
} /* SecFunction */

/*************************************************************************/
/*  EthernetInit                                                         */
/*                                                                       */
/*  Initialize the ethernet interface.                                   */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void EthernetInit (void)
{
   uint8_t  MACAddr[6];
   uint32_t Addr;
   char     Name2[16];

   IP_Init();

   /* Configure the MAC address */
   tal_BoardGetMACAddress(MACAddr);

   /* Configure the IP interface */
   IP_IF_MACSet(0, MACAddr);
   IP_IF_AddrSet(0,  etc_IPAddrGet(ETC_IP_ADDR) );
   IP_IF_MaskSet(0,  etc_IPAddrGet(ETC_IP_MASK) );
   IP_IF_GWSet(0,  etc_IPAddrGet(ETC_IP_GW) );
//   IP_IF_DNSSet(0, etc_IPAddrGet(ETC_IP_DNS) );
//   IP_IF_DNS2Set(0, etc_IPAddrGet(ETC_IP_DNS2) );
   
   /* Build mDNS hostname 2 */   
   sprintf(Name2, "%02x%02x%02x", MACAddr[3], MACAddr[4], MACAddr[5]);
   strcat(MDNSName2, Name2);
   MDNSConfig.Hostname2 = MDNSName2;
                
   /* Start the IP stack */
   IP_IF_Start(0);

   /* Start the DHCP service */   
   if (etc_IPDhcpIsUsed() != 0)
   {
      /* mDNS wil be started later, if DHCP is bound */
      IP_DHCP_CallbackSet(0, DHCPCallbackBound);   
      IP_DHCP_Start(0);
   }
   else
   {
      /* A fix IP address is used, start the mDNS service */
      IP_mDNS_Start(&MDNSConfig); 
      IP_TNP_SetName(MDNSConfig.Hostname2, (uint8_t)strlen(MDNSConfig.Hostname2));
   }      

   /* Start TNP service */
   IP_TNP_Start();

   /* Syslog server */
   Addr = etc_IPAddrGet(ETC_IP_SYSLOG);
   if (Addr != 0)
   {
      IP_SYSL_ServerSet(0, Addr);
   }   
   
} /* EthernetInit */

/*************************************************************************/
/*  EntropyInit                                                          */
/*                                                                       */
/*  Initialize the entropy source.                                       */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void EntropyInit (void)
{
   uint64_t Seed1, Seed2;
   
   Seed1 = 0;
   Seed2 = 0;

   /*
    * Get the seed from the hardware random number generator
    */   
   tal_CPURngInit();
   tal_CPURngHardwarePoll((uint8_t*)&Seed1, sizeof(Seed1));
   tal_CPURngHardwarePoll((uint8_t*)&Seed2, sizeof(Seed2));
   
   (void)Seed1;
   (void)Seed2;
   
} /* EntropyInit */   

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
   const char ResetScreen[] = { 0x1B, 'c', 0 };
   
   term_printf("%s", ResetScreen);
   OS_TimeDly(50);   
   
   term_printf("\r\n");
   term_printf("*********************************\r\n");
   term_printf("  Project: %s\r\n", PROJECT_NAME_LONG);
   term_printf("  Board  : %s\r\n", tal_BoardGetName());
   term_printf("  Version: v%s\r\n", PROJECT_VER_STRING);
   term_printf("  Build  : "__DATE__ " " __TIME__"\r\n");
   term_printf("*********************************\r\n");
   term_printf("\r\n");
   
} /* OutputBootMessage */

/*************************************************************************/
/*  OutputVersionInfo                                                    */
/*                                                                       */
/*  Output version information.                                          */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void OutputVersionInfo (void)
{
   term_printf("*** Version ***\r\n");
   term_printf("Board: %s\r\n", tal_BoardGetName());
   term_printf("CPU  : %s\r\n", TAL_CPU);
   term_printf("OS   : %s v%s\r\n", OS_NAME, OS_VER_STRING);
   term_printf("TAL  : v%s\r\n", TAL_CORE_VER_STRING);
   term_printf("TLS  : %s\r\n", MBEDTLS_VERSION_STRING_FULL);
   term_printf("\r\n");
   
} /* OutputVersionInfo */

/*************************************************************************/
/*  OutputFrequencyInfo                                                  */
/*                                                                       */
/*  Output frequency information.                                        */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void OutputFrequencyInfo (void)
{
   term_printf("*** Frequency info ***\r\n");
   term_printf("Core: %3d MHz\r\n", tal_CPUGetFrequencyCore() / 1000000); 
   term_printf("CPU : %3d MHz\r\n", tal_CPUGetFrequencyCPU() / 1000000); 
   term_printf("DDR : %3d MHz\r\n", tal_CPUGetFrequencyDDR() / 1000000); 
   term_printf("Peri: %3d MHz\r\n", tal_CPUGetFrequencyPeri() / 1000000); 
   term_printf("M4  : %3d MHz\r\n", tal_CPUGetFrequencyM4() / 1000000); 
   term_printf("M5  : %3d MHz\r\n", tal_CPUGetFrequencyM5() / 1000000); 
   term_printf("M6  : %3d MHz\r\n", tal_CPUGetFrequencyM6() / 1000000); 
   term_printf("Disp: %3d MHz\r\n", tal_CPUGetFrequencyDisp() / 1000000); 
   term_printf("\r\n");
   
} /* OutputFrequencyInfo */

/*************************************************************************/
/*  OutputCPULoad                                                        */
/*                                                                       */
/*  Output load information.                                             */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void OutputCPULoad (void)
{
   term_printf("CPU load: %d%%\r\n\r\n", OS_StatGetCPULoad());
   
} /* OutputCPULoad */

/*************************************************************************/
/*  OutputUsageInfo                                                      */
/*                                                                       */
/*  Output usage information.                                            */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void OutputUsageInfo (void)
{
   term_printf("*** Usage ***\r\n");
   term_printf("h: output this info\r\n");
   term_printf("?: output this info\r\n");
   term_printf("c: output cpu load info\r\n");
   term_printf("f: output frequency info\r\n");
   term_printf("r: output runtime stack info\r\n");
   term_printf("t: output task info\r\n");
   term_printf("m: output memory info\r\n");
   term_printf("v: output version info\r\n");

   term_printf("n: output network configuration info\r\n");
#if defined(LWIP_DEBUG)
   term_printf("l: output lwIP statistics\r\n");
#endif 
   term_printf("\r\n");
   
} /* OutputUsageInfo */

#if defined(LWIP_DEBUG)
/*************************************************************************/
/*  OutputLWIPStatistics                                                 */
/*                                                                       */
/*  Output lwIP Statistics                                               */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void OutputLWIPStatistics (void)
{
   uint8_t i;

   term_printf("*** lwIP Statistics ***\r\n");
   term_printf("\r\n");

   term_printf("Name              Used   Size    Max   Err\r\n");
   term_printf("===========================================\r\n");

   term_printf("%-16s %5d  %5d  %5d  %4d\r\n", 
               lwip_stats.mem.name,
               lwip_stats.mem.used,
               lwip_stats.mem.avail,
               lwip_stats.mem.max,
               lwip_stats.mem.err);

   for (i = 0; i < MEMP_MAX; i++)
   {
      term_printf("%-16s %5d  %5d  %5d  %4d\r\n",
                  lwip_stats.memp[i]->name,
                  lwip_stats.memp[i]->used,
                  lwip_stats.memp[i]->avail,
                  lwip_stats.memp[i]->max,
                  lwip_stats.memp[i]->err);
   }
   
   term_printf("\r\n");
   
} /* OutputLWIPStatistics */
#endif /* defined(LWIP_DEBUG) */

/*************************************************************************/
/*  LEDTask                                                              */
/*                                                                       */
/*  This is the LED task.                                                */
/*                                                                       */
/*  In    : task parameter                                               */
/*  Out   : none                                                         */
/*  Return: never                                                        */
/*************************************************************************/
static void LEDTask (void *p)
{
   (void)p;
   
   while (1)
   {
      if (LED_MODE_WAIT == nLedMode)
      {
         tal_LEDSet(LED1);
         OS_TimeDly(TASK_LED_DELAY_MS);
         tal_LEDClear(LED1);
         OS_TimeDly(TASK_LED_DELAY_MS);
      }
      else
      {
         tal_LEDSet(LED1);
         OS_TimeDly(1000);
      }   
      
      //term_printf("Heap used: %d\n", tal_MEMGetUsedRawMemory());
   }

} /* LEDTask */

/*************************************************************************/
/*  StartTask                                                            */
/*                                                                       */
/*  This is the Start task.                                              */
/*                                                                       */
/*  In    : task parameter                                               */
/*  Out   : none                                                         */
/*  Return: never                                                        */
/*************************************************************************/
static void StartTask (void *p)
{
   (void)p;
   
   /*
    * The StartTask will be used to start all other tasks in the system.
    * At the end the priority will be set to "IDLE" priority.
    */
    
   OS_SysTickStart();   /* Start the System ticker */
   OS_StatEnable();     /* Enable the statistic function */

   term_Start();        /* Start the Terminal functionality */
   
   /*
    * Output startup messages
    */
   OutputBootMessage();
   OutputUsageInfo();

   /* Create the LED task */
   OS_TaskCreate(&TCBLed, LEDTask, NULL, TASK_LED_PRIORITY,
                 LEDStack, sizeof(LEDStack), 
                 "LEDTask");

   fs_Init();           /* Initialize the file system */
   etc_Init();          /* Initialize the ETC system */
   cert_Init();         /* Initialize the CERT system */

   TimeInit();          /* Initialize the system time */
   EntropyInit();       /* Initialize the entropy source */
   EthernetInit();      /* Configure and start the IP interface */

   /*
    * The web server must be initialized before all other web services
    */   
   IP_WEBS_Init();        /* Initialize the web server */

   IP_DHCP_ServerInit();  /* Initialize the DHCP server */
   IP_SNTP_ServerInit();  /* Initialize the SNTP server */

   IP_WEBS_Start(80);     /* Start the web server */
   IP_WEBS_SSLStart(443);

   /*******************************************************************/

   /*
    * All other services which use the web server 
    * must be started after the web server.
    */

   /*
    * Start the DHCP server only if the own DHCP client is not used
    */
    
   if ((0 == etc_IPDhcpIsUsed()) && (nvm_DhcpServerUseGet() != 0))
   {
      IP_DHCP_ServerStart();
   }      

   IP_SNTP_Start();
   IP_SNTP_ServerStart();
   
   iperf_Start();       /* Start the IPerf service */

   OS_TaskChangePriority(TASK_START_PRIORITY_IDLE);

   while (1)
   {
      tal_LEDToggle(LED2);
      OS_TimeDly(1000);
      SecFunction();
   }

} /* StartTask */

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
    * Init the Tiny Abstraction Layer 
    */
   tal_Init();
   
   /* 
    * Initialize the memory pool
    */
   xmem_Init();
   
   /* 
    * Create the StartTask.
    * The StartTask is the one and only task which 
    * will start all other tasks of the system.
    */
   OS_TaskCreate(&TCBStartTask, StartTask, NULL, TASK_START_PRIORITY,
                 StartStack, sizeof(StartStack), 
                 "StartTask");
   
   /* 
    * OSStart must be the last function here.
    *  
    * Fasten your seatbelt, engine will be started...
    */        
   OS_Start();
   
   /*
    * This return here make no sense.
    * But to prevent the compiler warning:
    *    "return type of 'main' is not 'int'
    * We use an int as return :-)
    */ 
   return(0); /*lint !e527*/
} /* main */

/*************************************************************************/
/*  term_RxCallback                                                      */
/*                                                                       */
/*  Will be called from TermTask in case a char is received.             */
/*                                                                       */
/*  In    : bData                                                        */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void term_RxCallback (uint8_t bData)
{
   switch (bData)
   {
      case 'h':
      case '?':
      {
         OutputUsageInfo();
         break;
      } 
           
      case 'c':
      {
         OutputCPULoad();
         break;
      }

      case 'f':
      {
         OutputFrequencyInfo();
         break;
      }

      case 'r':
      {
         OS_OutputRuntimeStackInfo();
         break;
      }
      
      case 't':
      {
         OS_OutputTaskInfo();
         break;
      }
      
      case 'm':
      {
         tal_MEMOutputMemoryInfo();
         break;
      }
      
      case 'v':
      {
         OutputVersionInfo();
         break;
      }

#if defined(LWIP_DEBUG)
      case 'l':
         {
         OutputLWIPStatistics();
         break;
      }
#endif 
      
      /**************************************************/
      
      case 'n':
      {
         IP_IF_OutputConfig(0);
         break;
      }
      
      case '1':
      {
         term_printf("\r\n");
         term_printf("Threads Max HTTP : %d\r\n", nNumThreadsMax);
         term_printf("Threads Max HTTPs: %d\r\n", nNumThreadsMaxTLS);
         nNumThreadsMax    = 0;
         nNumThreadsMaxTLS = 0;
         break;
      }
      
      default:
      {
         /* Do nothing */
         break;
      }
   } /* end switch (bData) */

} /* term_RxCallback */

/*************************************************************************/
/*  mbedtls_hardware_poll                                                */
/*                                                                       */
/*  mbedTLS support function to retrieve RNG data.                       */
/*                                                                       */
/*  In    : data, output, olen                                           */
/*  Out   : output, olen                                                 */
/*  Return: 0 = OK                                                       */
/*************************************************************************/
int mbedtls_hardware_poll (void *data, unsigned char *output, size_t len, size_t *olen)
{
   (void)data;

   tal_CPURngHardwarePoll(output, len);
   *olen = len;
   
   return(0);
} /* mbedtls_hardware_poll */

/*** EOF ***/

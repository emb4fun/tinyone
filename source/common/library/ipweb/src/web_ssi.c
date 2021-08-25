/**************************************************************************
*  Copyright (c) 2015 by Michael Fischer (www.emb4fun.de).
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
*  25.01.2015  mifi  First Version.
*  13.03.2016  mifi  Renamed to web_asp.c.
*  29.12.2018  mifi  Renamed to web_ssi.c.
**************************************************************************/
#define __WEB_SSI_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "tcts.h"
#include "ipstack.h"
#include "ipweb.h"
#include "fs_xfile.h"
#include "web_sid.h"
#include "nvm.h"
#include "etc.h"
#include "fs.h"

#include <pro/uhttp/uhttpd.h>
#include <pro/uhttp/streamio.h>

#if !defined(IP_WEB_SSI_EXT_CST) 
#define _IP_WEB_SSI_EXT_CST   0
#else
#define _IP_WEB_SSI_EXT_CST   IP_WEB_SSI_EXT_CST
#endif  

#if (_IP_WEB_SSI_EXT_CST >= 1)
#include "web_ssi_ext_cst.h"
#endif

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

#define MAX_SSI_LIST_ENTRY    32

/*=======================================================================*/
/*  Definition of all global Data                                        */
/*=======================================================================*/

char   *LastUpdateName  = NULL;
uint8_t LastUpdateError = 0;

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

static SSI_EXT_LIST_ENTRY *ListTable[MAX_SSI_LIST_ENTRY];

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  TNPTable                                                             */
/*                                                                       */
/*  In    : hs, nListAll                                                 */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void TNPTable (HTTPD_SESSION *hs, int nListAll)
{
   char        String[80];
   char        IPAddress[16];
   IP_TNP_INFO TNPInfo;
   int         Err;
   uint16_t    Index;

   Index = 0;   
   while(1)
   {
      Err = IP_TNP_DeviceGet(Index, &TNPInfo);
      if (Err != 0)
      {
         break;
      }
      else
      {      
         if (0 == (Index & 0x01))
         {
            s_printf(hs->s_stream, "<tr>\r\n");
         }
         else
         {
            s_printf(hs->s_stream, "<tr class=\"dim\">\r\n");
         }
         
         s_printf(hs->s_stream, "  <td>&nbsp;</td>\r\n");
         
         /* MAC address */
         snprintf(String, sizeof(String), "%02X:%02X:%02X:%02X:%02X:%02X",
                  TNPInfo.bMACAddress[0], TNPInfo.bMACAddress[1],
                  TNPInfo.bMACAddress[2], TNPInfo.bMACAddress[3],
                  TNPInfo.bMACAddress[4], TNPInfo.bMACAddress[5]);
         s_printf(hs->s_stream, "  <td>%s</td>\r\n", String);
         s_printf(hs->s_stream, "  <td>&nbsp;</td>\r\n");

         /* IP address */
         htoa(TNPInfo.dAddress, IPAddress, sizeof(IPAddress));
         
         if( (TNPInfo.dAddress != IP_IF_AddrGet(0)) || (1 == nListAll))
         {
            /* Check if mDNS name is available */
            if (TNPInfo.MDNSName[0] != 0)
            {
               /* Use mDNS name */
               snprintf(String, sizeof(String), "<a href=\"//%s\">%s</a>&nbsp;</td>\n", TNPInfo.MDNSName, IPAddress);
            }
            else
            {
               /* Use IP address */
               snprintf(String, sizeof(String), "<a href=\"//%s\">%s</a>&nbsp;</td>\n", IPAddress, IPAddress);
            }   
            s_printf(hs->s_stream, "  <td>%s</td>\r\n", String);
         }
         else
         {
            s_printf(hs->s_stream, "  <td>%s</td>\r\n", IPAddress);
         }   
         s_printf(hs->s_stream, "  <td>&nbsp;</td>\r\n");
         
         /* Hostname */
         s_printf(hs->s_stream, "  <td>%s</td>\r\n", TNPInfo.Name);
         s_printf(hs->s_stream, "  <td>&nbsp;</td>\r\n");
         
         /* FW version */
         s_printf(hs->s_stream, "  <td>%d.%02d</td>\r\n", TNPInfo.dFWVersion / 100, TNPInfo.dFWVersion % 100);
         s_printf(hs->s_stream, "  <td>&nbsp;</td>\r\n");
         
         /* Location */
         s_printf(hs->s_stream, "  <td>%s</td>\r\n", TNPInfo.Location);
         s_printf(hs->s_stream, "  <td>&nbsp;</td>\r\n");
         s_printf(hs->s_stream, "</tr>\r\n");

         s_flush(hs->s_stream);
      }

      Index++;
   }
   
   IP_TNP_SendRequest(0);

} /* TNPTable */

/*************************************************************************/
/*  sys_boot_time                                                        */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_boot_time (HTTPD_SESSION *hs)
{
   s_printf(hs->s_stream, "30");

   s_flush(hs->s_stream);

   return(0);
} /* sys_boot_time */

/*************************************************************************/
/*  sys_shortname                                                        */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_shortname (HTTPD_SESSION *hs)
{
   s_printf(hs->s_stream, etc_ShortNameGet());

   s_flush(hs->s_stream);

   return(0);
} /* sys_shortname */

/*************************************************************************/
/*  sys_longname                                                         */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_longname (HTTPD_SESSION *hs)
{
   s_printf(hs->s_stream, etc_LongNameGet());

   s_flush(hs->s_stream);

   return(0);
} /* sys_longname */

/*************************************************************************/
/*  sys_location                                                         */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_location (HTTPD_SESSION *hs)
{
   s_printf(hs->s_stream, etc_LocationGet());

   s_flush(hs->s_stream);

   return(0);
} /* sys_location */

/*************************************************************************/
/*  sys_fw_name                                                          */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_fw_name (HTTPD_SESSION *hs)
{
   s_printf(hs->s_stream, PROJECT_NAME);

   s_flush(hs->s_stream);

   return(0);
} /* sys_fw_name */

/*************************************************************************/
/*  sys_fw_ver                                                           */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_fw_ver (HTTPD_SESSION *hs)
{
   uint16_t Version = PROJECT_VER_NUMBER; /*lint !e845*/
   
   s_printf(hs->s_stream, "%d.%02d", Version / 100, Version % 100);

   s_flush(hs->s_stream);

   return(0);
} /* sys_fw_ver */

/*************************************************************************/
/*  sys_web_name                                                         */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_web_name (HTTPD_SESSION *hs)
{
   s_puts(xfile_GetName(), hs->s_stream);

   s_flush(hs->s_stream);

   return(0);
} /* sys_web_name */

/*************************************************************************/
/*  sys_web_ver                                                          */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_web_ver (HTTPD_SESSION *hs)
{
   uint32_t dVersion = xfile_GetVersion();
   
   if (dVersion < 10000)
   {
      s_printf(hs->s_stream, "%d.%02d", dVersion / 100, dVersion % 100);
   }
   else
   {
      uint32_t dDigit1, dDigit2, dDigit3;
      
      dDigit1  = dVersion / 10000;
      dVersion = dVersion % 10000;
      dDigit2  = dVersion / 100;
      dDigit3  = dVersion % 100;
      
      s_printf(hs->s_stream, "%02d.%02d.%02d", dDigit1, dDigit2, dDigit3);
   }

   s_flush(hs->s_stream);

   return(0);
} /* sys_web_ver */

/*************************************************************************/
/*  sys_web1_name                                                        */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_web1_name (HTTPD_SESSION *hs)
{
   s_puts(fs_WebGetName(1), hs->s_stream);

   s_flush(hs->s_stream);

   return(0);
} /* sys_web1_name */

/*************************************************************************/
/*  sys_web1_ver                                                         */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_web1_ver (HTTPD_SESSION *hs)
{
   uint32_t dVersion = fs_WebGetVersion(1);
   
   if (dVersion < 10000)
   {
      s_printf(hs->s_stream, "%d.%02d", dVersion / 100, dVersion % 100);
   }
   else
   {
      uint32_t dDigit1, dDigit2, dDigit3;
      
      dDigit1  = dVersion / 10000;
      dVersion = dVersion % 10000;
      dDigit2  = dVersion / 100;
      dDigit3  = dVersion % 100;
      
      s_printf(hs->s_stream, "%02d.%02d.%02d", dDigit1, dDigit2, dDigit3);
   }

   s_flush(hs->s_stream);

   return(0);
} /* sys_web1_ver */

/*************************************************************************/
/*  sys_web2_name                                                        */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_web2_name (HTTPD_SESSION *hs)
{
   s_puts(fs_WebGetName(2), hs->s_stream);

   s_flush(hs->s_stream);

   return(0);
} /* sys_web2_name */

/*************************************************************************/
/*  sys_web2_ver                                                         */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_web2_ver (HTTPD_SESSION *hs)
{
   uint32_t dVersion = fs_WebGetVersion(2);
   
   if (dVersion < 10000)
   {
      s_printf(hs->s_stream, "%d.%02d", dVersion / 100, dVersion % 100);
   }
   else
   {
      uint32_t dDigit1, dDigit2, dDigit3;
      
      dDigit1  = dVersion / 10000;
      dVersion = dVersion % 10000;
      dDigit2  = dVersion / 100;
      dDigit3  = dVersion % 100;
      
      s_printf(hs->s_stream, "%02d.%02d.%02d", dDigit1, dDigit2, dDigit3);
   }

   s_flush(hs->s_stream);

   return(0);
} /* sys_web2_ver */

/*************************************************************************/
/*  sys_fw1_name                                                         */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_fw1_name (HTTPD_SESSION *hs)
{
   s_puts(fs_FwGetName(1), hs->s_stream);

   s_flush(hs->s_stream);

   return(0);
} /* sys_fw1_name */

/*************************************************************************/
/*  sys_fw1_ver                                                          */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_fw1_ver (HTTPD_SESSION *hs)
{
   uint32_t dVersion = fs_FwGetVersion(1);
   
   if (dVersion != 0)
   {
      s_printf(hs->s_stream, "%d.%02d", dVersion / 100, dVersion % 100);
   }      

   s_flush(hs->s_stream);

   return(0);
} /* sys_fw1_ver */

/*************************************************************************/
/*  sys_fw2_name                                                         */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_fw2_name (HTTPD_SESSION *hs)
{
   s_puts(fs_FwGetName(2), hs->s_stream);

   s_flush(hs->s_stream);

   return(0);
} /* sys_fw2_name */

/*************************************************************************/
/*  sys_fw2_ver                                                          */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_fw2_ver (HTTPD_SESSION *hs)
{
   uint32_t dVersion = fs_FwGetVersion(2);
   
   if (dVersion != 0)
   {
      s_printf(hs->s_stream, "%d.%02d", dVersion / 100, dVersion % 100);
   }      

   s_flush(hs->s_stream);

   return(0);
} /* sys_fw2_ver */

/*************************************************************************/
/*  sys_mem_usage                                                        */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_mem_usage (HTTPD_SESSION *hs)
{
   s_printf(hs->s_stream, "sys_mem_usage");

   s_flush(hs->s_stream);

   return(0);
} /* sys_mem_usage */

/*************************************************************************/
/*  sys_uptime                                                           */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_uptime (HTTPD_SESSION *hs)
{
   uint32_t Uptime = OS_TimeGetSeconds();
   int      Day  = 0;
   int      Hour = 0;
   int      Min  = 0;
   int      Sec  = 0;
   
   Day     = (int)(Uptime / 86400); /*lint !e838*/
   Uptime %= 86400;
   Hour    = (int)(Uptime / 3600);  /*lint !e838*/
   Uptime %= 3600;
   Min     = (int)(Uptime / 60);    /*lint !e838*/
   Sec     = (int)(Uptime % 60);    /*lint !e838*/
   
   s_printf(hs->s_stream, "%dd %02dh %02dm %02ds", Day, Hour, Min, Sec);

   s_flush(hs->s_stream);

   return(0);
} /* sys_uptime */

/*************************************************************************/
/*  sys_up_name                                                          */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_up_name (HTTPD_SESSION *hs)
{          
   s_printf(hs->s_stream, "%s", LastUpdateName);
   s_flush(hs->s_stream);

   return(0);
} /* sys_up_name */

/*************************************************************************/
/*  sys_up_err                                                           */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_up_err (HTTPD_SESSION *hs)
{          
   s_printf(hs->s_stream, "%d", LastUpdateError);
   s_flush(hs->s_stream);

   return(0);
} /* sys_up_err */

/*************************************************************************/
/*  sys_ip_mac                                                           */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_ip_mac (HTTPD_SESSION *hs)
{
   char MACAddr[6];
   
   IP_IF_MACGet(0, (uint8_t*)MACAddr, sizeof(MACAddr));   
   s_printf(hs->s_stream, "%02X:%02X:%02X:%02X:%02X:%02X",
            MACAddr[0], MACAddr[1], MACAddr[2], MACAddr[3], MACAddr[4], MACAddr[5]);

   s_flush(hs->s_stream);

   return(0);
} /* sys_ip_mac */

/*************************************************************************/
/*  sys_ip_addr                                                          */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_ip_addr (HTTPD_SESSION *hs)
{
   char String[16];
   
   htoa(IP_IF_AddrGet(0), String, sizeof(String));
   s_puts(String, hs->s_stream);

   s_flush(hs->s_stream);

   return(0);
} /* sys_ip_addr */

/*************************************************************************/
/*  sys_ip_mask                                                          */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_ip_mask (HTTPD_SESSION *hs)
{
   char String[16];
   
   htoa(IP_IF_MaskGet(0), String, sizeof(String));
   s_puts(String, hs->s_stream);

   s_flush(hs->s_stream);

   return(0);
} /* sys_ip_mask */

/*************************************************************************/
/*  sys_ip_gate                                                          */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_ip_gate (HTTPD_SESSION *hs)
{
   char String[16];
   
   htoa(IP_IF_GWGet(0), String, sizeof(String));
   s_puts(String, hs->s_stream);

   s_flush(hs->s_stream);

   return(0);
} /* sys_ip_gate */

/*************************************************************************/
/*  sys_ip_dhcp                                                          */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_ip_dhcp (HTTPD_SESSION *hs)
{
   char String[16];
   
   htoa(IP_DHCP_ServerGet(0), String, sizeof(String));
   s_puts(String, hs->s_stream);

   s_flush(hs->s_stream);

   return(0);
} /* sys_ip_dhcp */

/*************************************************************************/
/*  sys_ip_dns                                                           */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_ip_dns (HTTPD_SESSION *hs)
{
   char String[16];
   
   htoa(IP_DNS_ServerGet(0), String, sizeof(String));
   s_puts(String, hs->s_stream);

   s_flush(hs->s_stream);

   return(0);
} /* sys_ip_dns */

/*************************************************************************/
/*  sys_ip_dns_sec                                                       */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_ip_dns_sec (HTTPD_SESSION *hs)
{
   char String[16];
   
   htoa(IP_DNS_ServerGet(1), String, sizeof(String));
   s_puts(String, hs->s_stream);

   s_flush(hs->s_stream);

   return(0);
} /* sys_ip_dns_sec */

/*************************************************************************/
/*  sys_ip_syslog                                                        */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_ip_syslog (HTTPD_SESSION *hs)
{
   char String[16];
   
   htoa(IP_SYSL_ServerGet(0), String, sizeof(String));
   s_puts(String, hs->s_stream);

   s_flush(hs->s_stream);

   return(0);
} /* sys_ip_syslog */

/*************************************************************************/
/*  sys_ip_cfg_addr                                                      */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_ip_cfg_addr (HTTPD_SESSION *hs)
{
   char   String[16];
   NVM_IP IP;
   
   nvm_IPGet(&IP);

   htoa(IP.dIPAddr, String, sizeof(String));
   s_puts(String, hs->s_stream);

   s_flush(hs->s_stream);

   return(0);
} /* sys_ip_cfg_addr */

/*************************************************************************/
/*  sys_ip_dhcp_chk                                                      */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_ip_dhcp_chk (HTTPD_SESSION *hs)
{
   if (1 == IP_DHCP_IsStarted(0))
   {
      s_printf(hs->s_stream, "1");
   }
   else
   {
      s_printf(hs->s_stream, "0");
   }      

   s_flush(hs->s_stream);

   return(0);
} /* sys_ip_dhcp_chk */

/*************************************************************************/
/*  sys_ip_tnpes_chk                                                     */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_ip_tnpes_chk (HTTPD_SESSION *hs)
{
   if (1 == IP_TNP_IsES())
   {
      s_printf(hs->s_stream, "1");
   }
   else
   {
      s_printf(hs->s_stream, "0");
   }      

   s_flush(hs->s_stream);

   return(0);
} /* sys_ip_tnpes_chk */

/*************************************************************************/
/*  sys_ip_tnp_table                                                     */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_ip_tnp_table (HTTPD_SESSION *hs)
{
   TNPTable(hs, 0);

   return(0);
} /* sys_ip_tnp_table */

/*************************************************************************/
/*  sys_ip_tnp_jump_table                                                */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_ip_tnp_jump_table (HTTPD_SESSION *hs)
{
   TNPTable(hs, 1);

   return(0);
} /* sys_ip_tnp_jump_table */

/*************************************************************************/
/*  sys_time_date                                                        */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_time_date (HTTPD_SESSION *hs)
{
   uint32_t  Unixtime;
   struct tm TM;

   Unixtime = OS_UnixtimeGet() + OS_TimezoneSecGet() + OS_TimezoneDstSecGet();   /*lint !e737*/
   gmtime_r((const time_t*)&Unixtime, &TM);

   s_printf(hs->s_stream, "%02d.%02d.%04d",
            TM.tm_mday, TM.tm_mon+1, TM.tm_year+1900);
               
   s_flush(hs->s_stream);

   return(0);
} /* sys_time_date */

/*************************************************************************/
/*  sys_time_time                                                        */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_time_time (HTTPD_SESSION *hs)
{
   uint32_t  Unixtime;
   struct tm TM;

   Unixtime = OS_UnixtimeGet() + OS_TimezoneSecGet() + OS_TimezoneDstSecGet();   /*lint !e737*/
   gmtime_r((const time_t*)&Unixtime, &TM);

   s_printf(hs->s_stream, "%02d:%02d:%02d",
            TM.tm_hour, TM.tm_min, TM.tm_sec);
               
   s_flush(hs->s_stream);

   return(0);
} /* sys_time_time */

/*************************************************************************/
/*  sys_time_zone                                                        */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_time_zone (HTTPD_SESSION *hs)
{
   int32_t Offset = OS_TimezoneMinGet();
   
   if (Offset >= 0)
   {
      s_printf(hs->s_stream, "GMT+%02d:%02d", Offset/60, Offset%60);
   }
   else
   {
      Offset = abs(Offset);
      s_printf(hs->s_stream, "GMT-%02d:%02d", Offset/60, Offset%60);
   }   

   s_flush(hs->s_stream);

   return(0);
} /* sys_time_zone */

/*************************************************************************/
/*  sys_time_zone_id                                                     */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_time_zone_id (HTTPD_SESSION *hs)
{
   s_printf(hs->s_stream, "%d", OS_TimezoneIDGet());

   s_flush(hs->s_stream);

   return(0);
} /* sys_time_zone_id */

/*************************************************************************/
/*  sys_time_dst                                                         */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_time_dst (HTTPD_SESSION *hs)
{
   int32_t Offset = OS_TimezoneDstSecGet();
   
   if (0 == Offset)
   {
      s_printf(hs->s_stream, "0");
   }
   else
   {
      s_printf(hs->s_stream, "1");
   }      
   
   s_flush(hs->s_stream);

   return(0);
} /* sys_time_dst */

/*************************************************************************/
/*  sys_time_dst_checked                                                 */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_time_dst_checked (HTTPD_SESSION *hs)
{
   int32_t Offset = OS_TimezoneDstSecGet();
   
   if (0 == Offset)
   {
      s_printf(hs->s_stream, "");
   }
   else
   {
      s_printf(hs->s_stream, "checked");
   }      
   
   s_flush(hs->s_stream);

   return(0);
} /* sys_time_dst_checked */

/*************************************************************************/
/*  sys_ntp_chk                                                          */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_ntp_chk (HTTPD_SESSION *hs)
{
   if (1 == IP_SNTP_IsStarted())
   {
      s_printf(hs->s_stream, "1");
   }
   else
   {
      s_printf(hs->s_stream, "0");
   }      

   s_flush(hs->s_stream);

   return(0);
} /* sys_ntp_chk */

/*************************************************************************/
/*  sys_ntp_manual_ip                                                    */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_ntp_manual_ip (HTTPD_SESSION *hs)
{
   char     String[16];
   NVM_TIME Time;

   nvm_TimeGet(&Time);
   htoa(Time.dNTPAddr, String, sizeof(String));

   s_puts(String, hs->s_stream);

   s_flush(hs->s_stream);

   return(0);
} /* sys_ntp_manual_ip */

/*************************************************************************/
/*  sys_ntp_ip                                                           */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_ntp_ip (HTTPD_SESSION *hs)
{
   char String[16];
   
   htoa(IP_SNTP_ServerGet(), String, sizeof(String));
   s_puts(String, hs->s_stream);

   s_flush(hs->s_stream);

   return(0);
} /* sys_ntp_ip */

/*************************************************************************/
/*  sys_ntp_refresh                                                      */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_ntp_refresh (HTTPD_SESSION *hs)
{
   s_printf(hs->s_stream, "%d", IP_SNTP_RefreshGet());

   s_flush(hs->s_stream);

   return(0);
} /* sys_ntp_refresh */

/*
 * SSI variable list
 */
static const SSI_EXT_LIST_ENTRY SSIList[] =
{
   { "sys_boot_time",            sys_boot_time          },

   /* General system variables */ 
   { "sys_shortname",            sys_shortname          },
   { "sys_longname",             sys_longname           },
   { "sys_location",             sys_location           },
   { "sys_fw_name",              sys_fw_name            },
   { "sys_fw_ver",               sys_fw_ver             },
   { "sys_web_name",             sys_web_name           },
   { "sys_web_ver",              sys_web_ver            },
   { "sys_web1_name",            sys_web1_name          },
   { "sys_web1_ver",             sys_web1_ver           },
   { "sys_web2_name",            sys_web2_name          },
   { "sys_web2_ver",             sys_web2_ver           },
   { "sys_fw1_name",             sys_fw1_name           },
   { "sys_fw1_ver",              sys_fw1_ver            },
   { "sys_fw2_name",             sys_fw2_name           },
   { "sys_fw2_ver",              sys_fw2_ver            },
   { "sys_mem_usage",            sys_mem_usage          },
   { "sys_uptime",               sys_uptime             },
   { "sys_up_name",              sys_up_name            },
   { "sys_up_err",               sys_up_err             },

   /* IP variables */
   { "sys_ip_mac",               sys_ip_mac             },
   { "sys_ip_addr",              sys_ip_addr            },
   { "sys_ip_mask",              sys_ip_mask            },
   { "sys_ip_gate",              sys_ip_gate            },
   { "sys_ip_dhcp",              sys_ip_dhcp            },
   { "sys_ip_dns",               sys_ip_dns             },
   { "sys_ip_dns_sec",           sys_ip_dns_sec         },
   { "sys_ip_syslog",            sys_ip_syslog          },
   { "sys_ip_cfg_addr",          sys_ip_cfg_addr        },
   { "sys_ip_dhcp_chk",          sys_ip_dhcp_chk        },
   { "sys_ip_tnpes_chk",         sys_ip_tnpes_chk       },
   { "sys_ip_tnp_table",         sys_ip_tnp_table       },
   { "sys_ip_tnp_jump_table",    sys_ip_tnp_jump_table  },

   /* Time variables */
   { "sys_time_date",            sys_time_date          },
   { "sys_time_time",            sys_time_time          },
   { "sys_time_zone",            sys_time_zone          },
   { "sys_time_zone_id",         sys_time_zone_id       },
   { "sys_time_dst",             sys_time_dst           },
   { "sys_time_dst_checked",     sys_time_dst_checked   },

   /* SNTP client variables */   
   { "sys_ntp_chk",              sys_ntp_chk            },
   { "sys_ntp_manual_ip",        sys_ntp_manual_ip      },
   { "sys_ntp_ip",               sys_ntp_ip             },
   { "sys_ntp_refresh",          sys_ntp_refresh        },
      
#if (_IP_WEB_SSI_EXT_CST >= 1)
   WEB_SSI_EXT_CST
#endif   
      
   {NULL, NULL}
};

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  IP_WEBS_SSIInit                                                      */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_WEBS_SSIInit (void)
{
   memset(ListTable, 0x00, sizeof(ListTable));
   
   IP_WEBS_SSIListAdd((SSI_EXT_LIST_ENTRY*)SSIList);
   
} /* IP_WEBS_SSIInit */

/*************************************************************************/
/*  IP_WEBS_SSIListAdd                                                   */
/*                                                                       */
/*  In    : pList                                                        */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_WEBS_SSIListAdd (SSI_EXT_LIST_ENTRY *pList)
{
   uint16_t Index;
   
   for(Index = 0; Index < MAX_SSI_LIST_ENTRY; Index++)
   {
      if (0 == ListTable[Index])
      {
         ListTable[Index] = pList;
         break;   
      }
   }

} /* IP_WEBS_SSIListAdd */

/*************************************************************************/
/*  IP_WEBS_SSIStart                                                     */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_WEBS_SSIStart (void)
{
   /* Do nothing here */
} /* IP_WEBS_SSIStart */

/*************************************************************************/
/*  HttpSsiParseExt                                                      */
/*                                                                       */
/*  In    : hs, buf, len                                                 */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = Error                                          */
/*************************************************************************/
int HttpSsiParseExt (HTTPD_SESSION *hs, char *buf, int len)
{
   int                  res = -1;
   uint16_t             ListIndex = 0;
   SSI_EXT_LIST_ENTRY *pList;
   
   buf[len] = 0;

   /* Loop over the registered lists */   
   while ((ListIndex != MAX_SSI_LIST_ENTRY) && (ListTable[ListIndex] != NULL))
   {
      /* Get list */
      pList = ListTable[ListIndex];   

      /* Loop over the actual list */   
      while(pList->Var != NULL)
      {
         if (0 == strcasecmp(pList->Var, buf))
         {
            res = pList->pFunc(hs);
            return(res);
            break;   /*lint !e527*/
         }
         
         /* Switch to next entry inside the list */
         pList++;
      }
    
      /* Switch to next list */  
      ListIndex++;
   }      
   
   return(-1);
} /* HttpSsiParseExt */

/*** EOF ***/



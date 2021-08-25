/**************************************************************************
*  Copyright (c) 2018 by Michael Fischer (www.emb4fun.de).
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
*  03.02.2018  mif   First Version.
*  06.06.2020  mifi  Rename API.
**************************************************************************/
#define __IP_DHCPSERVER_C__

/*=======================================================================*/
/*  Include                                                              */
/*=======================================================================*/
#include <string.h>
#include "tal.h"
#include "ipstack.h"

#if (IP_DEFAULT_OPT_DHCPS >= 1)

#include "terminal.h"
#include "ipweb.h"
#include "nvm.h"

#include "lwip\prot\dhcp.h"
#include "lwip\prot\iana.h"
#include "lwip\netifapi.h"
#include "lwip\priv\sockets_priv.h"

/*=======================================================================*/
/*  All extern definitions                                               */
/*=======================================================================*/

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

#define PERMISSION_ADMIN      0xFFFFFFFF

#define DHCP_OPTION_SYSL      7     /* Syslog */

#if !defined(MIN)
#define MIN(a,b) ((a) < (b) ? a : b)
#endif

#define SOCKET_RCVTIMEO_MS    1000

/*
 * Some external swithes are not ready to use if the link
 * is available. Therefore wait a short time.
 */
#define DELAY_AFTER_LINK_MS   2000


/*
 * Reference is made to RFC2131 and:
 * https://de.wikipedia.org/wiki/Dynamic_Host_Configuration_Protocol
 */
 
#define DHCP_SERVER_PORT      67
#define DHCP_CLIENT_PORT      68
#define DHCP_IP_MAX_SIZE      256
 
#define RX_BUFFER_SIZE        576
#define TX_BUFFER_SIZE        1460
#define REQUEST_LIST_SIZE     15
#define HOST_NAME_SIZE        255 
#define CLIENT_ID_SIZE        255 


typedef struct _dhcp_server_
{
   int      InUse;
   uint32_t PoolStart;
   uint32_t PoolSize;
   uint32_t LeaseTimeSec;
   uint32_t NETMask;
   uint32_t GWAddr;
   uint32_t DNSAddr;
   uint32_t DNS2Addr;
   uint32_t NTPAddr;
   uint32_t SyslAddr;
} dhcp_server_t;


typedef struct _dhcp_entry_
{
   uint8_t  Used;
   uint8_t  MACAddr[6];
   uint32_t IPAddr;
   uint32_t StartTime;
   uint32_t RemainingTime;
   char     HostName[HOST_NAME_SIZE + 1];
   uint8_t  ClientIDLen;
   char     ClientID[CLIENT_ID_SIZE + 1];
} dhcp_entry_t;


typedef struct _msg_options_
{
   uint8_t  MsgType;                               /* 53 */
   uint32_t RequestedIP;                           /* 50 */
   uint32_t LeaseTime;                             /* 51 */
   uint8_t  RequestList[REQUEST_LIST_SIZE + 1];    /* 55 */
   char     HostName[HOST_NAME_SIZE + 1];          /* 12 */
   uint8_t  ClientIDLen;                           /* 61 */
   char     ClientID[CLIENT_ID_SIZE + 1];          /* 61 */
} msg_options_t;

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

/* 
 * Some TASK variables like stack and task control block.
 */
static OS_STACK (DHCPStack, TASK_IP_DHCP_SERVER_STK_SIZE);

static OS_TCB  TCBDHCP;

static dhcp_server_t Server;

static char          RxBuffer[RX_BUFFER_SIZE];
static char          TxBuffer[TX_BUFFER_SIZE];
static msg_options_t MsgRxOptions;
static uint8_t       MsgTxType;

static dhcp_entry_t  IPList[DHCP_IP_MAX_SIZE];

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

static int ssi_dhcps_ip_chk (HTTPD_SESSION *hs)
{
   uint32_t ClientUse;
   uint32_t ServerUse;

   /* Get client info */
   ClientUse = nvm_IPUseDHCPGet();   

   /* Get server info */
   ServerUse = nvm_DhcpServerUseGet();   

   /*
    * The server is only allowed to run if the client is disabled
    */
   if (0 == ClientUse)
   {
      if (1 == ServerUse)
      {
         s_printf(hs->s_stream, "1");
      }
      else
      {
         s_printf(hs->s_stream, "0");
      }
   }
   else
   {
      /* The client is enabled, server is not allowed */
      s_printf(hs->s_stream, "0");
   }            

   s_flush(hs->s_stream);

   return(0);
}

static int ssi_dhcps_ip_start (HTTPD_SESSION *hs)
{
   uint32_t dStart;
   uint32_t dSize;
   char      String[16];
   NVM_DHCPS DHCPS;

   nvm_DhcpServerGet(&DHCPS); 

   dStart = DHCPS.dPoolStart;
   dSize  = DHCPS.dPoolSize;

   htoa(dStart, String, sizeof(String));
   s_puts(String, hs->s_stream);

   s_flush(hs->s_stream);
   
   (void)dSize;

   return(0);
}

static int ssi_dhcps_ip_size (HTTPD_SESSION *hs)
{
   uint32_t dStart;
   uint32_t dSize;
   NVM_DHCPS DHCPS;

   nvm_DhcpServerGet(&DHCPS); 

   dStart = DHCPS.dPoolStart;
   dSize  = DHCPS.dPoolSize;

   s_printf(hs->s_stream, "%d", dSize);

   (void)dStart;

   return(0);
}

static int ssi_dhcps_ip_lease (HTTPD_SESSION *hs)
{
   uint32_t dLease;
   NVM_DHCPS DHCPS;

   nvm_DhcpServerGet(&DHCPS); 
   
   dLease = DHCPS.dLease;

   s_printf(hs->s_stream, "%d", dLease);

   return(0);
}

static int ssi_dhcps_ip_gate (HTTPD_SESSION *hs)
{
   char String[16];
   NVM_DHCPS DHCPS;

   nvm_DhcpServerGet(&DHCPS); 
   
   htoa(DHCPS.dIPGate, String, sizeof(String));
   s_puts(String, hs->s_stream);

   s_flush(hs->s_stream);

   return(0);
}

static int ssi_dhcps_ip_dns (HTTPD_SESSION *hs)
{
   char String[16];
   NVM_DHCPS DHCPS;

   nvm_DhcpServerGet(&DHCPS); 
   
   htoa(DHCPS.dIPDns, String, sizeof(String));
   s_puts(String, hs->s_stream);

   s_flush(hs->s_stream);

   return(0);
}

static int ssi_dhcps_ip_dns2 (HTTPD_SESSION *hs)
{
   char String[16];
   NVM_DHCPS DHCPS;

   nvm_DhcpServerGet(&DHCPS); 
   
   htoa(DHCPS.dIPDns2, String, sizeof(String));
   s_puts(String, hs->s_stream);

   s_flush(hs->s_stream);

   return(0);
}

static int ssi_dhcps_ip_ntp (HTTPD_SESSION *hs)
{
   char      String[16];
   NVM_DHCPS DHCPS;

   nvm_DhcpServerGet(&DHCPS); 
   
   htoa(DHCPS.dIPNtp, String, sizeof(String));
   s_puts(String, hs->s_stream);

   s_flush(hs->s_stream);

   return(0);
}

static int ssi_dhcps_ip_sysl (HTTPD_SESSION *hs)
{
   char      String[16];
   NVM_DHCPS DHCPS;

   nvm_DhcpServerGet(&DHCPS); 
   
   htoa(DHCPS.dIPSysl, String, sizeof(String));
   s_puts(String, hs->s_stream);

   s_flush(hs->s_stream);

   return(0);
}

/*************************************************************************/
/*  cgi_dhcps_table                                                      */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int cgi_dhcps_table (HTTPD_SESSION *hs)
{
   uint16_t       Index;
   uint16_t       Dim;
   int            Len;
   int            CopySize;
   char           String[32];
   dhcp_entry_t *pList;

   IP_WEBS_CGISendHeader(hs);

   Dim = 0;
   for(Index = 0; Index < Server.PoolSize; Index++)
   {
      pList = &IPList[Index];

      /* Only used entries must be handled */
      if (pList->Used != 0)
      {
         if (0 == (Dim & 0x01))
         {
            s_printf(hs->s_stream, "<tr>\r\n");
         }
         else
         {
            s_printf(hs->s_stream, "<tr class=\"dim\">\r\n");
         }

         s_printf(hs->s_stream, "  <td class=\"tbl\">&nbsp;</td>\r\n");
         s_printf(hs->s_stream, "  <td>%d</td>\r\n", Index);

         s_printf(hs->s_stream, "  <td>%02X:%02X:%02X:%02X:%02X:%02X</td>\r\n",
                  pList->MACAddr[0], pList->MACAddr[1], pList->MACAddr[2],
                  pList->MACAddr[3], pList->MACAddr[4], pList->MACAddr[5]);
         s_printf(hs->s_stream, "  <td>%s</td>\r\n", htoa(pList->IPAddr, String, sizeof(String)));
         s_printf(hs->s_stream, "  <td>%d</td>\r\n", pList->RemainingTime);

         if (pList->HostName[0] != 0)
         {
            Len = (int)strlen(pList->HostName);   
            CopySize = MIN(Len, (int)(sizeof(String)-1));
            memcpy(String, pList->HostName, CopySize); /*lint !e732*/   
            String[CopySize] = 0;
            s_printf(hs->s_stream, "  <td>%s</td>\r\n", String);
         }
         else
         {
            s_printf(hs->s_stream, "  <td>---</td>\r\n");
         }

         if ((pList->ClientIDLen != 0) && (0 == pList->ClientID[0]))
         {
            CopySize = MIN(pList->ClientIDLen, sizeof(String)-1); /*lint !e665*/
            memcpy(String, pList->ClientID, CopySize); /*lint !e732*/
            String[CopySize] = 0;
            s_printf(hs->s_stream, "  <td>0x%02X:%s</td>\r\n", String[0], &String[1]);
         }
         else
         {
            s_printf(hs->s_stream, "  <td>---</td>\r\n");
         }

         s_printf(hs->s_stream, "  <td class=\"tbr\">&nbsp;</td>\r\n");
         s_printf(hs->s_stream, "</tr>\r\n");

         s_flush(hs->s_stream);

         Dim++; 
      } /* end if (IPList[Index].Used != 0) */
   } /* end for */

   s_flush(hs->s_stream);

   return(0);
} /* cgi_dhcps_table */

/*************************************************************************/
/*  cgi_dhcps_set                                                        */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int cgi_dhcps_set (HTTPD_SESSION *hs)
{
   long      Avail;
   char    *pArg;
   char    *pVal;
   char    *pRedir = NULL;
   uint32_t dPermission = hs->s_req.req_sid_perm;
   NVM_DHCPS Config;
   int      dEnable    = 0;
   uint32_t dPoolStart = 0;
   uint32_t dPoolSize  = 0;
   uint32_t dLease     = 0;
   uint32_t dIPMask    = 0;
   uint32_t dIPGate    = 0;
   uint32_t dIPDns     = 0;
   uint32_t dIPDns2    = 0;
   uint32_t dIPNtp     = 0;
   uint32_t dIPSysl    = 0;

   Avail = hs->s_req.req_length;
   while (Avail) 
   {
      pArg = HttpArgReadNext(hs, &Avail);
      if (pArg != NULL)
      {
         pVal = HttpArgValue(&hs->s_req);
         if (pVal)
         {
            if      (strcmp(pArg, "dhcps_ena") == 0)
            {
               dEnable = 1;
            }
            else if (strcmp(pArg, "ip_start") == 0)
            {
               dPoolStart = atoh(pVal);
            }
            else if (strcmp(pArg, "ip_size") == 0)
            {
               dPoolSize = (uint32_t)atol(pVal);
            }
            else if (strcmp(pArg, "ip_lease") == 0)
            {
               dLease = (uint32_t)atol(pVal);
            }
            else if (strcmp(pArg, "ip_gate") == 0)
            {
               dIPGate = atoh(pVal);
            }
            else if (strcmp(pArg, "ip_dns") == 0)
            {
               dIPDns = atoh(pVal);
            }
            else if (strcmp(pArg, "ip_dns2") == 0)
            {
               dIPDns2 = atoh(pVal);
            }
            else if (strcmp(pArg, "ip_ntp") == 0)
            {
               dIPNtp = atoh(pVal);
            }
            else if (strcmp(pArg, "ip_sysl") == 0)
            {
               dIPSysl = atoh(pVal);
            }
            else if (strcmp(pArg, "redir") == 0)
            {
               pRedir = xstrdup(XM_ID_IP, pVal);
            }
         }            
      }
   } 

   if (pRedir != NULL)
   {
      if (PERMISSION_ADMIN == dPermission)
      {
         /* Check and set default */
         if (0 == dPoolStart) dEnable   = 0;
         if (0 == dPoolSize)  dPoolSize = 10;
         if (0 == dLease)     dLease    = 10;

         nvm_DhcpServerGet(&Config);
         Config.dEnable    = (dEnable != 0) ? 1 : 0;
         Config.dPoolStart = dPoolStart;
         Config.dPoolSize  = dPoolSize;
         Config.dLease     = dLease;
         Config.dIPMask    = dIPMask;
         Config.dIPGate    = dIPGate;
         Config.dIPDns     = dIPDns;
         Config.dIPDns2    = dIPDns2;
         Config.dIPNtp     = dIPNtp;
         Config.dIPSysl    = dIPSysl;
         nvm_DhcpServerSet(&Config);

         HttpSendRedirection(hs, 303, pRedir, NULL);

         /*
          * Update new settings and reboot
          */ 
         nvm_Write();
         OS_TimeDly(2000);
         tal_CPUReboot();
      }
      else
      {
         HttpSendRedirection(hs, 303, "/403.htm", NULL);
      }         
   }      

   xfree(pRedir);

   return(0);
} /* cgi_dhcps_set */

/*
 * SSI variable list
 */
static const SSI_EXT_LIST_ENTRY SSIList[] =
{
   { "sys_dhcps_ip_chk",         ssi_dhcps_ip_chk   },
   { "sys_dhcps_ip_start",       ssi_dhcps_ip_start },
   { "sys_dhcps_ip_size",        ssi_dhcps_ip_size  },
   { "sys_dhcps_ip_lease",       ssi_dhcps_ip_lease },
   { "sys_dhcps_ip_gate",        ssi_dhcps_ip_gate  },
   { "sys_dhcps_ip_dns",         ssi_dhcps_ip_dns   },
   { "sys_dhcps_ip_dns2",        ssi_dhcps_ip_dns2  },
   { "sys_dhcps_ip_ntp",         ssi_dhcps_ip_ntp   },
   { "sys_dhcps_ip_sysl",        ssi_dhcps_ip_sysl  },
   
   {NULL, NULL}
};

/*
 * CGI variable list
 */
static const CGI_LIST_ENTRY CGIList[] =
{
   { "cgi-bin/dhcps_table.cgi",  cgi_dhcps_table },
   { "cgi-bin/dhcps_set.cgi",    cgi_dhcps_set   },

   {NULL, NULL}
};


/*************************************************************************/
/*  DebugInfoIn                                                          */
/*                                                                       */
/*  Output some debug information, like Type and XID.                    */
/*                                                                       */
/*  In    : MsgType, XID                                                 */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void DebugInfoIn (uint8_t MsgType, uint32_t XID)
{
   XID = ntohl(XID);

   switch (MsgType)
   {
      case DHCP_DISCOVER: TAL_DEBUG(TAL_DBG_DHCPs, "DHCPs: => DHCP Discover - Transaction ID 0x%08X", XID); break;
      case DHCP_OFFER:    TAL_DEBUG(TAL_DBG_DHCPs, "DHCPs: => DHCP Offer    - Transaction ID 0x%08X", XID); break; 
      case DHCP_REQUEST:  TAL_DEBUG(TAL_DBG_DHCPs, "DHCPs: => DHCP Request  - Transaction ID 0x%08X", XID); break;
      case DHCP_DECLINE:  TAL_DEBUG(TAL_DBG_DHCPs, "DHCPs: => DHCP Decline  - Transaction ID 0x%08X", XID); break;
      case DHCP_ACK:      TAL_DEBUG(TAL_DBG_DHCPs, "DHCPs: => DHCP ACK      - Transaction ID 0x%08X", XID); break;
      case DHCP_NAK:      TAL_DEBUG(TAL_DBG_DHCPs, "DHCPs: => DHCP NAK      - Transaction ID 0x%08X", XID); break;
      case DHCP_RELEASE:  TAL_DEBUG(TAL_DBG_DHCPs, "DHCPs: => DHCP Release  - Transaction ID 0x%08X", XID); break;
      case DHCP_INFORM:   TAL_DEBUG(TAL_DBG_DHCPs, "DHCPs: => DHCP Inform   - Transaction ID 0x%08X", XID); break;
      
      default:
      {
         /* Do nothing */
         break;
      }
   }

} /* DebugInfoIn */

/*************************************************************************/
/*  DebugInfoOut                                                         */
/*                                                                       */
/*  Output some debug information, like Type and XID.                    */
/*                                                                       */
/*  In    : MsgRx                                                        */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void DebugInfoOut (uint8_t MsgType, uint32_t XID)
{
   XID = ntohl(XID);

   switch (MsgType)
   {
      case DHCP_DISCOVER: TAL_DEBUG(TAL_DBG_DHCPs, "DHCPs: <= DHCP Discover - Transaction ID 0x%08X", XID); break;
      case DHCP_OFFER:    TAL_DEBUG(TAL_DBG_DHCPs, "DHCPs: <= DHCP Offer    - Transaction ID 0x%08X", XID); break; 
      case DHCP_REQUEST:  TAL_DEBUG(TAL_DBG_DHCPs, "DHCPs: <= DHCP Request  - Transaction ID 0x%08X", XID); break;
      case DHCP_DECLINE:  TAL_DEBUG(TAL_DBG_DHCPs, "DHCPs: <= DHCP Decline  - Transaction ID 0x%08X", XID); break;
      case DHCP_ACK:      TAL_DEBUG(TAL_DBG_DHCPs, "DHCPs: <= DHCP ACK      - Transaction ID 0x%08X", XID); break;
      case DHCP_NAK:      TAL_DEBUG(TAL_DBG_DHCPs, "DHCPs: <= DHCP NAK      - Transaction ID 0x%08X", XID); break;
      case DHCP_RELEASE:  TAL_DEBUG(TAL_DBG_DHCPs, "DHCPs: <= DHCP Release  - Transaction ID 0x%08X", XID); break;
      case DHCP_INFORM:   TAL_DEBUG(TAL_DBG_DHCPs, "DHCPs: <= DHCP Inform   - Transaction ID 0x%08X", XID); break;
      
      default:
      {
         /* Do nothing */
         break;
      }
   }

} /* DebugInfoOut */

/*************************************************************************/
/*  CheckRequestList                                                     */
/*                                                                       */
/*  Check if the requested option is available in the RequestList.       */
/*                                                                       */
/*  In    : Option                                                       */
/*  Out   : none                                                         */
/*  Return: 0 = not available / 1 = available                            */
/*************************************************************************/
static int CheckRequestList (uint8_t Option)
{
   uint16_t Index;
   int      Available = 0;

   for(Index = 0; Index < REQUEST_LIST_SIZE; Index++)
   {
      if (0 == MsgRxOptions.RequestList[Index])
      {
         break;
      }

      if (Option == MsgRxOptions.RequestList[Index])
      {
         Available = 1;
         break;
      }
   }

   return(Available);
} /* CheckRequestList */

/*************************************************************************/
/*  CheckLeaseTime                                                       */
/*                                                                       */
/*  Check the lease time of the IP addresses.                            */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void CheckLeaseTime (void)
{
   uint16_t Index;
   uint32_t Actual;
   uint32_t Diff;

   Actual = OS_TimeGetSeconds();

   for(Index = 0; Index < Server.PoolSize; Index++)
   {
      /* Only used entries must be handled */
      if (IPList[Index].Used != 0)
      {
         /* Check timeout */       
         if (OS_TEST_TIMEOUT(Actual, IPList[Index].StartTime, Server.LeaseTimeSec))
         {
            /* This entry can be reused */
            IPList[Index].Used  = 0;
            memset(IPList[Index].MACAddr, 0x00, 6);
            IPList[Index].StartTime = 0;
         }
         else
         {
            Diff = Actual - IPList[Index].StartTime;
            IPList[Index].RemainingTime = Server.LeaseTimeSec - Diff;
         }   
      }
   }

} /* CheckLeaseTime */

/*************************************************************************/
/*  FindFreeIP                                                           */
/*                                                                       */
/*  Find a free IP address for the given MAC address.                    */
/*                                                                       */
/*  In    : MACAddr                                                      */
/*  Out   : none                                                         */
/*  Return: IPAddr or 0 in case of an ERROR                              */
/*************************************************************************/
static uint32_t FindFreeIP (uint8_t *MACAddr)
{
   uint16_t Index;
   uint32_t IPAddr      = 0;
   uint32_t RequestedIP = 0;
   uint32_t ValidIP;

   /* 
    * In case of a DHCP_REQUEST, check if the requested IP is in a valid range
    */ 
   if ((DHCP_REQUEST == MsgRxOptions.MsgType) && (MsgRxOptions.RequestedIP != 0))
   {
      ValidIP = Server.PoolStart & Server.NETMask;

      RequestedIP = ntohl(MsgRxOptions.RequestedIP);
      if ((RequestedIP & Server.NETMask) != ValidIP)
      {
         /* Error, requested IP is out of range in case of DHCP_REQUEST  */
         goto FindFreeIPExit; /*lint !e801*/   
      }
   }

   /*
    * In case of a DHCP_DISCOVER, check if the requested IP can be used
    */
   if ((DHCP_DISCOVER == MsgRxOptions.MsgType) && (MsgRxOptions.RequestedIP != 0))
   {
      ValidIP = Server.PoolStart & Server.NETMask;

      RequestedIP = ntohl(MsgRxOptions.RequestedIP);
      if ((RequestedIP & Server.NETMask) != ValidIP)
      {
         /* The requested IP is out of range, but no error in case of DHCP_DISCOVER */
         RequestedIP = 0;
      }
   }

   /* Check if the MAC address is in the list */
   for(Index = 0; Index < Server.PoolSize; Index++)
   {
      if ((IPList[Index].Used != 0) && (0 == memcmp(IPList[Index].MACAddr, MACAddr, 6)))
      {
         IPAddr = Server.PoolStart + Index;

         IPList[Index].IPAddr    = IPAddr;
         IPList[Index].StartTime = OS_TimeGetSeconds(); 
         IPList[Index].RemainingTime = IPList[Index].StartTime;

         /* Clear first */
         memset(IPList[Index].HostName, 0x00, sizeof(IPList[Index].HostName));
         memset(IPList[Index].ClientID, 0x00, sizeof(IPList[Index].ClientID));

         /* Copy data */
         memcpy(IPList[Index].HostName, MsgRxOptions.HostName, HOST_NAME_SIZE);
         memcpy(IPList[Index].ClientID, MsgRxOptions.ClientID, CLIENT_ID_SIZE);
         IPList[Index].ClientIDLen = MsgRxOptions.ClientIDLen;
         break;
      } 
   }
   
   if (0 == IPAddr)
   {
      /* MAC address is not in the list, use a free entry */
      for(Index = 0; Index < Server.PoolSize; Index++)
      {
         /* Check if this entry is free */
         if (0 == IPList[Index].Used)
         {
            IPAddr = Server.PoolStart + Index;

            IPList[Index].Used = 1;
            memcpy(IPList[Index].MACAddr, MACAddr, 6); 
            IPList[Index].IPAddr    = IPAddr;
            IPList[Index].StartTime = OS_TimeGetSeconds(); 
            IPList[Index].RemainingTime = IPList[Index].StartTime;

            /* Clear first */
            memset(IPList[Index].HostName, 0x00, sizeof(IPList[Index].HostName));
            memset(IPList[Index].ClientID, 0x00, sizeof(IPList[Index].ClientID));

            /* Copy data */
            memcpy(IPList[Index].HostName, MsgRxOptions.HostName, HOST_NAME_SIZE);
            memcpy(IPList[Index].ClientID, MsgRxOptions.ClientID, CLIENT_ID_SIZE);
            IPList[Index].ClientIDLen = MsgRxOptions.ClientIDLen;
            break;
         } 
      }
   }

   (void)RequestedIP;

FindFreeIPExit:
   return(IPAddr);
} /* FindFreeIP */

/*************************************************************************/
/*  AddOption                                                            */
/*                                                                       */
/*  Add an option to the option list.                                    */
/*                                                                       */
/*  In    : Option, Data, Size, OpType                                   */
/*  Out   : none                                                         */
/*  Return: Option                                                       */
/*************************************************************************/
static uint8_t *AddOption (uint8_t *Option, uint8_t *Data, uint8_t Size, uint8_t OpType)
{
   *Option++ = OpType;  /* Type */
   if (Size != 0)
   {
      *Option++ = Size;    /* Len */
      memcpy(Option, Data, Size);
      Option += Size;
   }  
   
   return(Option); 
} /* AddOption */

/*************************************************************************/
/*  DecodeOptions                                                        */
/*                                                                       */
/*  Decode DHCP options.                                                 */
/*                                                                       */
/*  In    : Msg, MsgSize                                                 */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static msg_options_t *DecodeOptions (struct dhcp_msg *Msg, uint16_t MsgSize)
{
   uint8_t *Options;
   uint8_t *End;
   uint8_t  Len;
   uint8_t  CopySize;
   int      OptionsSize;
   int      OptionsEnd;

   Options     = &Msg->options[0];
   OptionsSize = MsgSize - (int)DHCP_OPTIONS_OFS;
   End         = Options + OptionsSize;

   /* Clear MsgOptions first */
   memset(&MsgRxOptions, 0x00, sizeof(MsgRxOptions));
   
   OptionsEnd = FALSE;
   while(Options < End)
   {
      Len = 0;
      
      switch (*Options)
      {
         case DHCP_OPTION_MESSAGE_TYPE:            /* 53 */
         {
            Options++;
            Len = *Options++;
            
            MsgRxOptions.MsgType = *Options;
            break;
         }
         
         case DHCP_OPTION_HOSTNAME:                /* 12 */
         {
            Options++;
            Len = *Options++;
            
            CopySize = MIN(Len, HOST_NAME_SIZE);
            memcpy(MsgRxOptions.HostName, Options, CopySize); /*lint !e670*/
            break;
         }

         case DHCP_OPTION_REQUESTED_IP:            /* 50 */
         {
            Options++;
            Len = *Options++;

            memcpy(&MsgRxOptions.RequestedIP, Options, 4);
            break;
         }

         case DHCP_OPTION_LEASE_TIME:              /* 51 */
         {
            Options++;
            Len = *Options++;
            
            memcpy(&MsgRxOptions.LeaseTime, Options, 4);
            MsgRxOptions.LeaseTime = ntohl(MsgRxOptions.LeaseTime);
            break;
         }
         
         case DHCP_OPTION_PARAMETER_REQUEST_LIST:  /* 55 */
         {
            Options++;
            Len = *Options++;
            
            CopySize = MIN(Len, REQUEST_LIST_SIZE);
            memcpy(MsgRxOptions.RequestList, Options, CopySize);
            break;
         }
         
         case DHCP_OPTION_CLIENT_ID:               /* 61 */
         {
            Options++;
            Len = *Options++;

            CopySize = MIN(Len, CLIENT_ID_SIZE); 
            memcpy(MsgRxOptions.ClientID, Options, Len); /*lint !e670*/
            MsgRxOptions.ClientIDLen = CopySize;
            break;
         }
         
         case DHCP_OPTION_END:                     /* 255 */
         {
            OptionsEnd = TRUE;
            break;
         }
          
         default:
         {
            Options++;
            Len = *Options++;
            break;
         }
      }
      
      if (TRUE == OptionsEnd)
      {
         break;
      }
      
      /* Switch to next option */      
      Options += Len;
   }
   
   return(&MsgRxOptions);
} /* DecodeOptions */ 

/*************************************************************************/
/*  BuildResponseMsg                                                     */
/*                                                                       */
/*  Build a DHCP response message.                                       */
/*                                                                       */
/*  In    : MsgRx, MsgRxSize, MsgTx, MsgTxSize, Type                     */
/*  Out   : MsgTxSize                                                    */
/*  Return: none                                                         */
/*************************************************************************/
static void BuildResponseMsg (struct dhcp_msg *MsgRx, uint16_t  MsgRxSize, 
                              struct dhcp_msg *MsgTx, uint16_t *MsgTxSize, uint8_t Type)
{
   uint8_t  *Options;
   uint16_t  OptionsSize;
   uint32_t  YourIP;
   uint32_t  Value;
   
   (void)MsgRxSize;

   /* Check if we have a free IP */
   YourIP = FindFreeIP(MsgRx->chaddr);
   if (0 == YourIP)
   {
      Type = DHCP_NAK;
   }

   *MsgTxSize = 0;
   MsgTxType  = Type;

   /* Clear message first */
   memset(TxBuffer, 0x00, sizeof(TxBuffer));

   /* Copy RX => TX */
   memcpy(MsgTx, MsgRx, DHCP_OPTIONS_OFS);

   MsgTx->op     = DHCP_BOOTREPLY;
   MsgTx->htype  = LWIP_IANA_HWTYPE_ETHERNET;
   MsgTx->hlen   = NETIF_MAX_HWADDR_LEN;
   MsgTx->xid    = MsgRx->xid;
   MsgTx->secs   = 0;
   MsgTx->flags  = MsgRx->flags;
   MsgTx->cookie = htonl(DHCP_MAGIC_COOKIE);
   
   ip4_addr_set_u32(ip_2_ip4(&MsgTx->ciaddr), 0);
   ip4_addr_set_u32(ip_2_ip4(&MsgTx->yiaddr), htonl(YourIP));
   ip4_addr_set_u32(ip_2_ip4(&MsgTx->siaddr), 0);
   ip4_addr_set_u32(ip_2_ip4(&MsgTx->giaddr), 0);
   
   Options = &MsgTx->options[0];
   
   /* 53 */
   Value = Type; 
   Options = AddOption(Options, (uint8_t*)&Value, 1, DHCP_OPTION_MESSAGE_TYPE); 
   
   /* 54 */      
   Value = htonl(IP_IF_AddrGet(0));
   Options = AddOption(Options, (uint8_t*)&Value, sizeof(Value), DHCP_OPTION_SERVER_ID); 
   
   if (Type != DHCP_NAK) 
   {
      /* 51 */      
      Value = htonl(Server.LeaseTimeSec);
      Options = AddOption(Options, (uint8_t*)&Value, sizeof(Value), DHCP_OPTION_LEASE_TIME); 
 
      /* 1 */      
      Value = htonl(Server.NETMask);
      Options = AddOption(Options, (uint8_t*)&Value, sizeof(Value), DHCP_OPTION_SUBNET_MASK); 
 
      /* 3 */      
      Value = htonl(Server.GWAddr);
      Options = AddOption(Options, (uint8_t*)&Value, sizeof(Value), DHCP_OPTION_ROUTER); 
 
      /* 6 */      
      Value = htonl(Server.DNSAddr);
      Options = AddOption(Options, (uint8_t*)&Value, sizeof(Value), DHCP_OPTION_DNS_SERVER); 

      /* 42 */
      if ((CheckRequestList(DHCP_OPTION_NTP)) && (Server.NTPAddr != 0))
      {
         Value = htonl(Server.NTPAddr);
         Options = AddOption(Options, (uint8_t*)&Value, sizeof(Value), DHCP_OPTION_NTP); 
      }

      /* 7 */
      if ((CheckRequestList(DHCP_OPTION_SYSL)) && (Server.SyslAddr != 0))
      {
         Value = htonl(Server.SyslAddr);
         Options = AddOption(Options, (uint8_t*)&Value, sizeof(Value), DHCP_OPTION_SYSL); 
      }
   }
 
   /* End of list */  
   Options = AddOption(Options, 0, 0, DHCP_OPTION_END); 
   
   /* Calculate options size */
   OptionsSize = (uint16_t)(Options - &MsgTx->options[0]);
   
   /* Calculate total size */
   *MsgTxSize = (uint16_t)(DHCP_OPTIONS_OFS + OptionsSize);
   
} /* BuildResponseMsg */

/*************************************************************************/
/*  HandleDiscover                                                       */
/*                                                                       */
/*  Handle the DISCOVER message.                                         */
/*                                                                       */
/*  In    : Msg, MsgSize, MsgTxSize                                      */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static struct dhcp_msg *HandleDiscover (struct dhcp_msg *MsgRx, uint16_t MsgRxSize, uint16_t *MsgTxSize)
{
   struct dhcp_msg *MsgTx = (struct dhcp_msg*)TxBuffer;
   
   BuildResponseMsg(MsgRx, MsgRxSize, MsgTx, MsgTxSize, DHCP_OFFER);
   
   return(MsgTx);
} /* HandleDiscover */

/*************************************************************************/
/*  HandleRequest                                                        */
/*                                                                       */
/*  Handle the REQUEST message.                                          */
/*                                                                       */
/*  In    : Msg, MsgSize, MsgTxSize                                      */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static struct dhcp_msg *HandleRequest (struct dhcp_msg *MsgRx, uint16_t MsgRxSize, uint16_t *MsgTxSize)
{
   struct dhcp_msg *MsgTx = (struct dhcp_msg*)TxBuffer;

   BuildResponseMsg(MsgRx, MsgRxSize, MsgTx, MsgTxSize, DHCP_ACK);
   
   return(MsgTx);
} /* HandleRequest */

/*************************************************************************/
/*  DHCPTask                                                             */
/*                                                                       */
/*  This is the DHCP task.                                               */
/*                                                                       */
/*  In    : arg                                                          */
/*  Out   : none                                                         */
/*  Return: never                                                        */
/*************************************************************************/
static void DHCPTask (void *arg)
{
   int                Err;
   int                Value;
   int                RxSock;
   int                TxSock;
   struct sockaddr_in ServerAddr;
   struct sockaddr_in SourceAddr;
   struct sockaddr_in ClientAddr;
   int                SourceLen; 
   int                Size;
   struct lwip_sock  *Sock; 
   msg_options_t     *Options;
   struct dhcp_msg   *MsgRx;
   uint16_t           MsgRxSize;
   struct dhcp_msg   *MsgTx;
   uint16_t           MsgTxSize;
   
   (void)arg;
   
   /* Wait that the IP interface is ready for use */
   while(0 == IP_IF_IsReady(IFACE_ANY))
   {
      OS_TimeDly(500);
   }

   /* Wait some time for the external switch */
   OS_TimeDly(DELAY_AFTER_LINK_MS);
   
   TAL_DEBUG(TAL_DBG_DHCPs, "DHCPs: Server started...");
   
   /* Clear DHCP list first */
   memset(IPList, 0x00, sizeof(IPList));
   
   /*
    * Create Rx socket 
    */
   RxSock = socket(AF_INET, SOCK_DGRAM, 0);
   TAL_ASSERT(RxSock != SOCKET_ERROR);

   /* Receive broadcast messages too */
   Value = 1;
   Err = setsockopt(RxSock, SOL_SOCKET, SO_BROADCAST, &Value, sizeof(Value));
   TAL_ASSERT(0 == Err);

   /* Set receive timeout */   
   Value = SOCKET_RCVTIMEO_MS;
   Err = setsockopt(RxSock, SOL_SOCKET, SO_RCVTIMEO, &Value, sizeof(Value));
   TAL_ASSERT(0 == Err);


   /* 
    * Create Tx socket 
    */
   TxSock = socket(AF_INET, SOCK_DGRAM, 0);
   TAL_ASSERT(TxSock != SOCKET_ERROR);
   
   /* Send broadcast messages too */
   Value = 1;
   Err = setsockopt(TxSock, SOL_SOCKET, SO_BROADCAST, &Value, sizeof(Value));
   TAL_ASSERT(0 == Err);
   
   /* Set local UDP port */
   Sock = lwip_socket_dbg_get_socket(TxSock); /* Hack Alert */
   Sock->conn->pcb.udp->local_port = 67;      /* Hack Alert */
   
   /*
    * Create server 
    */
   ServerAddr.sin_addr.s_addr = INADDR_ANY;
   ServerAddr.sin_port        = htons(DHCP_SERVER_PORT);
   ServerAddr.sin_family      = AF_INET;

   /* Assign a name (port) to an unnamed socket */
   Err = bind(RxSock, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr));   /*lint !e740*/
   TAL_ASSERT(0 == Err);


   while(1)
   {
      SourceLen = sizeof(SourceAddr);
      Size = recvfrom(RxSock, RxBuffer, RX_BUFFER_SIZE, 0,
                      (struct sockaddr *)&SourceAddr, (socklen_t*)&SourceLen); /*lint !e740*/

      if (Size >= (int)DHCP_OPTIONS_OFS)
      {
         MsgRx     = (struct dhcp_msg *)RxBuffer;
         MsgRxSize = (uint16_t)Size;
         
         MsgTx     = NULL;
         MsgTxSize = 0;
         
         /* Check for a valid DHCP frame */
         if( (DHCP_BOOTREQUEST          == MsgRx->op)            && 
             (LWIP_IANA_HWTYPE_ETHERNET == MsgRx->htype)         && 
             (NETIF_MAX_HWADDR_LEN      == MsgRx->hlen)          && 
             (DHCP_MAGIC_COOKIE         == ntohl(MsgRx->cookie)) )
         {
            Options = DecodeOptions(MsgRx, MsgRxSize);

            DebugInfoIn(Options->MsgType, MsgRx->xid); 

            switch (Options->MsgType)
            {
               case DHCP_DISCOVER:
               {
                  MsgTx = HandleDiscover(MsgRx, MsgRxSize, &MsgTxSize);
                  break;
               }
               
               case DHCP_REQUEST:
               {
                  MsgTx = HandleRequest(MsgRx, MsgRxSize, &MsgTxSize);
                  break;
               } 
               
               case DHCP_RELEASE:
               {
                  /* Must be tested with a PC */
                  break;
               }  
               
               default:
               {
                  /* Do nothing */
                  break;
               }
            }
            
            /* Check if an answer must be send */
            if ((MsgTx != NULL) && (MsgTxSize != 0))
            {
               DebugInfoOut(MsgTxType, MsgTx->xid);

               /* Send message */
               ClientAddr.sin_addr.s_addr = IPADDR_BROADCAST;
               ClientAddr.sin_port        = htons(DHCP_CLIENT_PORT);
               ClientAddr.sin_family      = AF_INET;
               sendto(TxSock, MsgTx, (size_t)MsgTxSize, 0, (struct sockaddr *)&ClientAddr, sizeof(ClientAddr)); /*lint !e740*/
            }
            
         } /* end if "valid frame" */   
      } /* end if (Size >= (int)DHCP_OPTIONS_OFS) */

      CheckLeaseTime();

   } /* end while(1) */

} /* DHCPTask */  

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  IP_DHCP_ServerInit                                                   */
/*                                                                       */
/*  Initialize the DHCP server.                                          */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_DHCP_ServerInit (void)
{
   memset(&Server, 0x00, sizeof(Server));
   
   IP_WEBS_SSIListAdd((SSI_EXT_LIST_ENTRY*)SSIList);
   IP_WEBS_CGIListAdd((CGI_LIST_ENTRY*)CGIList);
   
} /* IP_DHCP_ServerInit */

/*************************************************************************/
/*  IP_DHCP_ServerStart                                                  */
/*                                                                       */
/*  Start the DHCP server.                                               */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_DHCP_ServerStart (void)
{
   uint32_t      AddrLow;
   uint32_t      PoolSize;
   NVM_DHCPS     Config;
   struct netif *netif;
   
   if (0 == Server.InUse)
   {
      netif = netif_find("en0");
      if (netif != NULL)
      {
         /* DHCP server is in use */
         Server.InUse = 1;

         /* Get configuration */
         nvm_DhcpServerGet(&Config);
         
         /* 
          * Configure DHCPS settings 
          */
         AddrLow = (Config.dPoolStart & 0x000000FF);
         if ((AddrLow + Config.dPoolSize) >= 254)
         {
            PoolSize = (254 - AddrLow) + 1;
         }
         else
         {
            PoolSize = Config.dPoolSize;
         } 

         Server.PoolStart = Config.dPoolStart;
         Server.PoolSize  = PoolSize;
   
         Server.LeaseTimeSec = Config.dLease * 60;
         Server.NETMask      = IP_IF_MaskGet(0);
         Server.GWAddr       = Config.dIPGate;
         Server.DNSAddr      = Config.dIPDns;
         Server.DNS2Addr     = Config.dIPDns2;
         Server.NTPAddr      = Config.dIPNtp;
         Server.SyslAddr     = Config.dIPSysl;
                  
         OS_TaskCreate(&TCBDHCP, DHCPTask, NULL, TASK_IP_DHCP_SERVER_PRIORITY, 
                       DHCPStack, sizeof(DHCPStack), 
                       "DHCPServer"); 
      }   
   }   
   
} /* IP_DHCP_ServerStart */

/*************************************************************************/
/*  IP_DHCP_ServerStop                                                   */
/*                                                                       */
/*  Stop the DHCP server.                                                */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_DHCP_ServerStop (void)
{
   struct netif *netif;
   
   if (1 == Server.InUse)
   {
      netif = netif_find("en0");
      if (netif != NULL)
      {
         //Server.InUse = 0;
      }   
   }
} /* IP_DHCP_ServerStop */

/*************************************************************************/
/*  IP_DHCP_ServerIsStarted                                              */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 0 not started / 1 otherwise                                  */
/*************************************************************************/
int IP_DHCP_ServerIsStarted (void)
{
   return(Server.InUse);
} /* IP_DHCP_ServerIsStarted */

#endif /* (IP_DEFAULT_OPT_DHCPS >= 1) */

/*** EOF ***/

/**************************************************************************
*  Copyright (c) 2014 by Michael Fischer (www.emb4fun.de).
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
*  08.02.2014  mifi  First Version.
*  07.02.2015  mifi  Rework to use lwip.
*  06.06.2020  mifi  Rework to support 2 interfaces and rename API.
**************************************************************************/
#define __IP_DHCP_C__

/*=======================================================================*/
/*  Include                                                              */
/*=======================================================================*/
#include "tal.h"
#include "ipstack.h"
#include "terminal.h"

#include "lwip\prot\dhcp.h"
#include "lwip\netifapi.h"
#include "lwip\dns.h"

#if (IP_DEFAULT_OPT_DHCP >= 1)
/*=======================================================================*/
/*  All extern definitions                                               */
/*=======================================================================*/

void dhcp_set_timeout (u32_t timeout_msec);
void dhcp_set_clientid (uint8_t *id, uint8_t len);

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

#if !defined(ETH_MAX_IFACE)
#define ETH_MAX_IFACE   1
#endif

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

static int DhcpInUse[2] = {0,0}; 

static ip_dhcp_callback_t DhcpCallback = NULL;

static uint32_t NTPServerList[ETH_MAX_IFACE][LWIP_DHCP_MAX_NTP_SERVERS];

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  _IP_DHCP_CallbackBound                                               */
/*                                                                       */
/*  Callback is called whenever the DHCP is bound.                       */
/*                                                                       */
/*  In    : netif                                                        */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void _IP_DHCP_CallbackBound (void)
{
   TAL_DEBUG(TAL_DBG_DHCP, "DHCP is bound");
      
   if (DhcpCallback != NULL)
   {
      DhcpCallback();
   }

} /* _IP_DHCP_CallbackBound */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  dhcp_set_ntp_servers                                     (callback)  */
/*                                                                       */
/*  In    : num_servers, server_addrs                                    */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void dhcp_set_ntp_servers(struct netif *netif, u8_t num_servers, const ip4_addr_t* server_addrs)
{
   int     Iface;
   uint8_t Index;
   
   Iface = netif->num;
   if (Iface > ETH_MAX_IFACE)
   {
      /* Error, protect array */
      Iface = 0;
   }

   /* Check for max range */
   if (num_servers > LWIP_DHCP_MAX_NTP_SERVERS)
   {
      num_servers = LWIP_DHCP_MAX_NTP_SERVERS;
   }

   /* Copy data */
   for (Index = 0; Index < num_servers; Index++)
   {
      NTPServerList[Iface][Index] = ntohl(server_addrs[Index].addr);
   }

} /* dhcp_set_ntp_servers */

/*************************************************************************/
/*  IP_DHCP_Start                                                        */
/*                                                                       */
/*  Start DHCP negotiation for the network interface.                    */
/*                                                                       */
/*  In    : iface                                                        */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_DHCP_Start (uint8_t iface)
{
   struct netif *netif;
   char          name[] = "enx";

   if (iface < ETH_MAX_IFACE)
   {   
      if (0 == DhcpInUse[iface])
      {
         name[2] = 0x30 + iface; /*lint !e734*/
      
         netif = netif_find(name);
         if (netif != NULL)
         {
            DhcpInUse[iface] = 1;

            /* Clear NTP server infos */
            memset(NTPServerList, 0x00, sizeof(NTPServerList));

            /* Start DHCP */
            netifapi_dhcp_start(netif);
         }   
      }   
   }
         
} /* IP_DHCP_Start */

/*************************************************************************/
/*  IP_DHCP_Stop                                                         */
/*                                                                       */
/*  Stop the DHCP client of the network interface.                       */
/*                                                                       */
/*  In    : iface                                                        */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_DHCP_Stop (uint8_t iface)
{
   struct netif *netif;
   char          name[] = "enx";
   ip4_addr_t    ipaddr;
   ip4_addr_t    netmask;
   ip4_addr_t    gw;

   (void)iface;

   if (iface < ETH_MAX_IFACE)
   {   
      if (1 == DhcpInUse[iface])
      {
         name[2] = 0x30 + iface; /*lint !e734*/
      
         netif = netif_find(name);
         if (netif != NULL)
         {
            DhcpInUse[iface] = 0;
            DhcpCallback     = NULL;
            netifapi_dhcp_stop(netif);
         
            OS_TimeDly(100);
         
            /* Set startup values */
            IP_IF_StartupValuesGet(0, &ipaddr.addr, &netmask.addr, &gw.addr);
            netif_set_addr(netif, &ipaddr, &netmask, &gw);         
         
            ipaddr.addr = 0;
            dns_setserver(0, &ipaddr); 
            dns_setserver(1, &ipaddr); 
         }   
      }
   }
         
} /* IP_DHCP_Stop */

/*************************************************************************/
/*  IP_DHCP_IsStarted                                                    */
/*                                                                       */
/*  In    : iface                                                        */
/*  Out   : none                                                         */
/*  Return: 0 not started / 1 otherwise                                  */
/*************************************************************************/
int IP_DHCP_IsStarted (uint8_t iface)
{
   int IsStarted = 0;
   
   if (iface < ETH_MAX_IFACE)
   {
      IsStarted = DhcpInUse[iface];   
   }

   return(IsStarted);
} /* IP_DHCP_IsStarted */

/*************************************************************************/
/*  IP_DHCP_ServerGet                                                    */
/*                                                                       */
/*  Get the DHCP server address of the network interface in host order.  */
/*                                                                       */
/*  In    : iface                                                        */
/*  Out   : none                                                         */
/*  Return: Address in host order                                        */
/*************************************************************************/
uint32_t IP_DHCP_ServerGet (uint8_t iface)
{
   struct netif *netif;
   char          name[] = "enx";
   struct dhcp  *dhcp;
   uint32_t      addr = 0;

   if (iface < ETH_MAX_IFACE)
   {
      if (1 == DhcpInUse[iface])
      {
         name[2] = 0x30 + iface; /*lint !e734*/
      
         netif = netif_find(name);
         if (netif != NULL)
         {
            dhcp = netif_dhcp_data(netif);
            addr = dhcp->server_ip_addr.addr;
         }
      }
   }      

   return( ntohl(addr) );
} /* IP_DHCP_ServerGet */

/*************************************************************************/
/*  IP_DHCP_CallbackSet                                                  */
/*                                                                       */
/*  In    : iface, Callback                                              */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_DHCP_CallbackSet (uint8_t iface, ip_dhcp_callback_t callback)
{
   (void)iface;

   DhcpCallback = callback;
   
} /* IP_DHCP_CallbackSet */

/*************************************************************************/
/*  IP_DHCP_NTPServerGet                                                 */
/*                                                                       */
/*  Get the NTP server address of the network interface in host order.   */
/*                                                                       */
/*  In    : iface                                                        */
/*  Out   : none                                                         */
/*  Return: Address in host order                                        */
/*************************************************************************/
uint32_t IP_DHCP_NTPServerGet (uint8_t iface)
{
   uint32_t addr = 0;

   if (iface < ETH_MAX_IFACE)
   {
      if (1 == DhcpInUse[iface])
      {
         addr = NTPServerList[iface][0];
      }
   }      

   return(addr);
} /* IP_DHCP_NTPServerGet */

/*************************************************************************/
/*  IP_DHCP_TimeoutSet                                                   */
/*                                                                       */
/*  Set the given timeout in msec.                                       */
/*                                                                       */
/*  In    : iface, timeout_msec                                          */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_DHCP_TimeoutSet (uint8_t iface, uint32_t timeout_msec)
{
   (void)iface;
   dhcp_set_timeout(timeout_msec);
   
} /* IP_DHCP_TimeoutSet */

/*************************************************************************/
/*  TP_DHCP_HostnameSet                                                  */
/*                                                                       */
/*  Set the given hostname.                                              */
/*                                                                       */
/*  In    : iface, hostname                                              */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_DHCP_HostnameSet (uint8_t iface, char *hostname)
{
   struct netif *netif;
   
   if ((iface < ETH_MAX_IFACE) && (hostname != NULL))
   {
      netif = IP_IF_NetIfGet(iface);
      if (netif != NULL)
      {
         netif->hostname = hostname;
      }
   }      

} /* TP_DHCP_HostnameSet */

/*************************************************************************/
/*  IP_DHCP_ClientIDSet                                                  */
/*                                                                       */
/*  Set the given clientid.                                              */
/*                                                                       */
/*  In    : iface, id, len                                               */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_DHCP_ClientIDSet (uint8_t iface, uint8_t *id, uint8_t len)
{
   if ((iface < ETH_MAX_IFACE) && (id != NULL) && (len != 0))
   {
      dhcp_set_clientid(id, len);
   }      

} /* IP_DHCP_ClientIDSet */

#else


int IP_DHCP_IsStarted (int uint8_t iface)
{
   (void)iface;
    
   return(0);
} /* IP_DHCP_IsStarted */

uint32_t IP_DHCP_ServerGet (uint8_t iface)
{
   (void)iface;

   return(0);
} /* IP_DHCP_ServerGet */

#endif /* (IP_DEFAULT_OPT_DHCP >= 1) */

/*** EOF ***/

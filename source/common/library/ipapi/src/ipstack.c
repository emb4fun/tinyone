/**************************************************************************
*  Copyright (c) 2013 by Michael Fischer (www.emb4fun.de).
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
*  19.05.2013  mifi  First Version.
*  19.08.2013  mifi  Some rework and cleanup.
*  18.04.2014  mifi  Some rework, HW init of ETH will be done by
*                    ethernetif_init.
*  07.02.2015  mifi  Some rework and cleanup.
*  06.06.2020  mifi  Rework to support 2 interfaces, and rename API.
**************************************************************************/
#define __IPSTACK_C__

/*=======================================================================*/
/*  Include                                                              */
/*=======================================================================*/
#include <string.h>
#include "tal.h"
#include "ipstack.h"
#include "terminal.h"

#include "lwip\api.h"
#include "lwip\tcpip.h"
#include "lwip\inet.h"
#include "lwip\netif.h"
#include "netif\etharp.h"

#include "ethernetif.h"

/*=======================================================================*/
/*  Extern                                                               */
/*=======================================================================*/

/*=======================================================================*/
/*  Global                                                               */
/*=======================================================================*/

int errno = 0; /* Needed by lwIP */

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

#if !defined(ETH_MAX_IFACE)
#define ETH_MAX_IFACE   1
#endif

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

static uint8_t        InitDone = 0;

static struct netif   NetIf[ETH_MAX_IFACE];
static struct netif *pNetIf[ETH_MAX_IFACE];

static uint8_t    MACDefaultAddr[ETH_MAX_IFACE][6];
static uint8_t    MACAddr[ETH_MAX_IFACE][6];

static uint32_t   IPAddr[ETH_MAX_IFACE];
static uint32_t   NETMask[ETH_MAX_IFACE];
static uint32_t   GWAddr[ETH_MAX_IFACE];

static int        NetIfIsUp[ETH_MAX_IFACE];
static int        LinkIsUp[ETH_MAX_IFACE];

static uint32_t   StartupIPAddr[ETH_MAX_IFACE];
static uint32_t   StartupNETMask[ETH_MAX_IFACE];
static uint32_t   StartupGWAddr[ETH_MAX_IFACE];

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  NetIfStatusCallback0                                                 */
/*                                                                       */
/*  Callback is called whenever an interface changes its up/down status  */ 
/*  (i.e., due to DHCP IP acquistion)                                    */
/*                                                                       */
/*  In    : netif                                                        */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void NetIfStatusCallback0 (struct netif *netif)
{
   if(netif_is_up(netif))
   {
      if (0 == NetIfIsUp[0])
      {
         NetIfIsUp[0] = 1;
         TAL_PRINTF("NetIf 0 is up\r\n");
      }   
   }
   else
   {
      if (1 == NetIfIsUp[0])
      {
         NetIfIsUp[0] = 0;
         TAL_PRINTF("NetIf 0 is down\r\n");
      }   
   }
   
} /* NetIfStatusCallback0 */

#if (ETH_MAX_IFACE == 2)
static void NetIfStatusCallback1 (struct netif *netif)
{
   if(netif_is_up(netif))
   {
      if (0 == NetIfIsUp[1])
      {
         NetIfIsUp[1] = 1;
         TAL_PRINTF("NetIf 1 is up\r\n");
      }         
   }
   else
   {
      if (1 == NetIfIsUp[1])
      {
         NetIfIsUp[1] = 0;
         TAL_PRINTF("NetIf 1 is down\r\n");
      }         
   }
   
} /* NetIfStatusCallback1 */
#endif

/*************************************************************************/
/*  NetIfLinkCallback0                                                   */
/*                                                                       */
/*  Callback is called when link is brought up/down                      */
/*                                                                       */
/*  In    : netif                                                        */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void NetIfLinkCallback0 (struct netif *netif)
{
   if (netif_is_link_up(netif))
   {
      TAL_PRINTF("Link 0 is up\r\n");
      LinkIsUp[0] = 1;
   }
   else
   {
      TAL_PRINTF("Link 0 is down\r\n");
      LinkIsUp[0] = 0;
   }
   
} /* NetIfLinkCallback0 */

#if (ETH_MAX_IFACE == 2)
static void NetIfLinkCallback1 (struct netif *netif)
{
   if (netif_is_link_up(netif))
   {
      TAL_PRINTF("Link 1 is up\r\n");
      LinkIsUp[1] = 1;
   }
   else
   {
      TAL_PRINTF("Link 1 is down\r\n");
      LinkIsUp[1] = 0;
   }
   
} /* NetIfLinkCallback1 */
#endif

/*************************************************************************/
/*  IP_TNP_IsStarted                                                     */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 0 not started / 1 otherwise                                  */
/*************************************************************************/
int __attribute__((weak)) IP_TNP_IsStarted (void)
{
   return(0);
} /* IP_TNP_IsStarted */

/*************************************************************************/
/*  IP_TNP_IsES                                                          */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 0 not started / 1 otherwise                                  */
/*************************************************************************/
int __attribute__((weak)) IP_TNP_IsES (void)
{
   return(0);
} /* IP_TNP_IsES */

/*************************************************************************/
/*  IP_DHCP_ServerGet                                                    */
/*                                                                       */
/*  Get the DHCP server address of the network interface in host order.  */
/*                                                                       */
/*  In    : iface                                                        */
/*  Out   : none                                                         */
/*  Return: Address in host order                                        */
/*************************************************************************/
uint32_t __attribute__((weak)) IP_DHCP_ServerGet (uint8_t iface)
{
   (void)iface;
   
   return(0);
} /* IP_DHCP_ServerGet */ 

/*************************************************************************/
/*  IP_DHCP_IsStarted                                                    */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 0 not started / 1 otherwise                                  */
/*************************************************************************/
int __attribute__((weak)) IP_DHCP_IsStarted (uint8_t iface)
{
   (void)iface;

   return(0);
} /* IP_DHCP_IsStarted */

/*************************************************************************/
/*  IP_mDNS_IsStarted                                                    */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 0 not started / 1 otherwise                                  */
/*************************************************************************/
int __attribute__((weak)) IP_mDNS_IsStarted (void)
{
   return(0);
} /* IP_mDNS_IsStarted */

/*************************************************************************/
/*  IP_DNS_ServerGet                                                     */
/*                                                                       */
/*  Get the DNS server address of the network interface in host order.   */
/*                                                                       */
/*  In    : index                                                        */
/*  Out   : none                                                         */
/*  Return: Address in host order                                        */
/*************************************************************************/
uint32_t __attribute__((weak)) IP_DNS_ServerGet (uint8_t index)
{
   (void)index;
   
   return(0);   
} /* IP_DNS_ServerGet */

/*************************************************************************/
/*  IP_SNTP_ServerGet                                                    */
/*                                                                       */
/*  Get the SNTP address of the network interface in host order.         */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: IP Address                                                   */
/*************************************************************************/
uint32_t __attribute__((weak)) IP_SNTP_ServerGet (void)
{
   return(0);
} /* IP_SNTP_ServerGet */

/*************************************************************************/
/*  IP_SYSL_ServerGet                                                    */
/*                                                                       */
/*  Get the IP address of the syslog server in host order.               */
/*                                                                       */
/*  In    : iface                                                        */
/*  Out   : none                                                         */
/*  Return: Address in host order                                        */
/*************************************************************************/
uint32_t __attribute__((weak)) IP_SYSL_ServerGet (uint8_t iface)
{
   (void)iface;

   return(0);
} /* IP_SYSL_ServerGet */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  IP_Init                                                              */
/*                                                                       */
/*  Init the network system.                                             */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_Init (void)
{ 
   uint8_t MACDefault[6] = IP_DEFAULT_MAC_ADDR;
   
   InitDone = 1;
   
   memset(NetIf, 0x00, sizeof(NetIf));
   
   memset(MACDefaultAddr, 0x00, sizeof(MACDefaultAddr));
   memset(MACAddr,        0x00, sizeof(MACAddr));

   memset(IPAddr,         0x00, sizeof(IPAddr));
   memset(NETMask,        0x00, sizeof(NETMask));
   memset(GWAddr,         0x00, sizeof(GWAddr));

   memset(NetIfIsUp,      0x00, sizeof(NetIfIsUp));
   memset(LinkIsUp,       0x00, sizeof(LinkIsUp));
   
   memset(StartupIPAddr,  0x00, sizeof(StartupIPAddr));
   memset(StartupNETMask, 0x00, sizeof(StartupNETMask));
   memset(StartupGWAddr,  0x00, sizeof(StartupGWAddr));
   
   memcpy(&MACDefaultAddr[0][0], MACDefault, ETHARP_HWADDR_LEN);
   MACDefaultAddr[0][5] &= ~0x01;
   pNetIf[0] = NULL;
   
#if (ETH_MAX_IFACE == 2)
   memcpy(&MACDefaultAddr[1][0], MACDefault, ETHARP_HWADDR_LEN);
   MACDefaultAddr[1][5] |=  0x01;
   pNetIf[1] = NULL;
#endif   
   
   /* Initialize lwIP */
   tcpip_init(NULL, NULL);

} /* IP_Init */

/*************************************************************************/
/*  IP_IF_NetIfGet                                                       */
/*                                                                       */
/*  In    : iface                                                        */
/*  Out   : none                                                         */
/*  Return: netif                                                        */
/*************************************************************************/
struct netif *IP_IF_NetIfGet (uint8_t iface)
{
   struct netif *pNetIF = NULL;
   
   if (iface < ETH_MAX_IFACE)
   {
      pNetIF = pNetIf[iface];
   }
   
   return(pNetIF);

} /* IP_IF_NetIfGet */

/*************************************************************************/
/*  atoh                                                                 */
/*                                                                       */
/*  Convert a ASCII internet address into host order.                    */
/*                                                                       */
/*  In    : pAsciiAddr                                                   */
/*  Out   : none                                                         */
/*  Return: host order address                                           */
/*************************************************************************/
uint32_t atoh (char *pAsciiAddr)
{
   uint32_t addr = inet_addr(pAsciiAddr);
   
   return( ntohl(addr) );
} /* atoh */

/*************************************************************************/
/*  htoa                                                                 */
/*                                                                       */
/*  Convert a host IP address into decimal dotted ASCII representation.  */
/*                                                                       */
/*  In    : Addr, pBuf, BufLen                                           */
/*  Out   : none                                                         */
/*  Return: ASCII address                                                */
/*************************************************************************/
char *htoa (uint32_t Addr, char *pBuf, int BufLen)
{
   ip_addr_t ipaddr;

   ipaddr.addr = htonl(Addr);

   return( ipaddr_ntoa_r(&ipaddr, pBuf, BufLen) );
} /* htoa */

/*************************************************************************/
/*  IP_IF_MACSet                                                         */
/*                                                                       */
/*  Set the MAC address of the network interface.                        */
/*                                                                       */
/*  In    : iface, mac                                                   */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_IF_MACSet (uint8_t iface, uint8_t *mac)
{
   if (iface < ETH_MAX_IFACE)
   {
      memcpy(&MACAddr[iface][0], mac, 6);
   }   
   
} /* IP_IF_MACSet */

/*************************************************************************/
/*  IP_IF_AddrSet                                                        */
/*                                                                       */
/*  Set the IP address of the network interface in host order.           */
/*                                                                       */
/*  In    : iface, addr                                                  */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_IF_AddrSet (uint8_t iface, uint32_t addr)
{
   if (iface < ETH_MAX_IFACE)
   {
      IPAddr[iface] = htonl(addr);
   }
      
} /* IP_IF_AddrSet */

/*************************************************************************/
/*  IP_IF_MaskSet                                                        */
/*                                                                       */
/*  Set the network mask of the network interface in host order.         */
/*                                                                       */
/*  In    : iface, addr                                                  */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_IF_MaskSet (uint8_t iface, uint32_t addr)
{
   if (iface < ETH_MAX_IFACE)
   {
      NETMask[iface] = htonl(addr);
   }
      
} /* IP_IF_MaskSet */

/*************************************************************************/
/*  IP_IF_GWSet                                                          */
/*                                                                       */
/*  Set the gateway address of the network interface in host order.      */
/*                                                                       */
/*  In    : iface, addr                                                  */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_IF_GWSet (uint8_t iface, uint32_t addr)
{
   if (iface < ETH_MAX_IFACE)
   {
      GWAddr[iface] = htonl(addr);
   }
         
} /* IP_IF_GWSet */

/*************************************************************************/
/*  IP_IF_MACGet                                                         */
/*                                                                       */
/*  Get the MAC address of the network interface.                        */
/*                                                                       */
/*  In    : iface, mac, size                                             */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_IF_MACGet (uint8_t iface, uint8_t *mac, uint8_t size)
{
   if ((iface < ETH_MAX_IFACE) && (size >= ETHARP_HWADDR_LEN))
   {
      memcpy(mac, pNetIf[iface]->hwaddr, ETHARP_HWADDR_LEN);
   }
} /* IP_IF_MACGet */

/*************************************************************************/
/*  IP_IF_AddrGet                                                        */
/*                                                                       */
/*  Get the IP address of the network interface in host order.           */
/*                                                                       */
/*  In    : iface                                                        */
/*  Out   : none                                                         */
/*  Return: Address in host order                                        */
/*************************************************************************/
uint32_t IP_IF_AddrGet (uint8_t iface)
{
   uint32_t addr = 0;

   if ((iface < ETH_MAX_IFACE) && (pNetIf[iface] != NULL))
   {
      addr = pNetIf[iface]->ip_addr.addr;
   }

   return(ntohl(addr));
} /* IP_IF_AddrGet */

/*************************************************************************/
/*  IP_IF_MaskGet                                                        */
/*                                                                       */
/*  Get the network mask of the network interface in host order.         */
/*                                                                       */
/*  In    : iface                                                        */
/*  Out   : none                                                         */
/*  Return: Address in host order                                        */
/*************************************************************************/
uint32_t IP_IF_MaskGet (uint8_t iface)
{
   uint32_t addr = 0;

   if ((iface < ETH_MAX_IFACE) && (pNetIf[iface] != NULL))
   {
      addr = pNetIf[iface]->netmask.addr;
   }

   return(ntohl(addr));
} /* IP_IF_MaskGet */

/*************************************************************************/
/*  IP_IF_GWGet                                                          */
/*                                                                       */
/*  Get the gateway address of the network interface in host order.      */
/*                                                                       */
/*  In    : iface                                                        */
/*  Out   : none                                                         */
/*  Return: Address in host order                                        */
/*************************************************************************/
uint32_t IP_IF_GWGet (uint8_t iface)
{
   uint32_t addr = 0;

   if ((iface < ETH_MAX_IFACE) && (pNetIf[iface] != NULL))
   {
      addr = pNetIf[iface]->gw.addr;
   }

   return(ntohl(addr));
} /* IP_IF_GWGet */

/*************************************************************************/
/*  IP_IF_LinkSpeedDuplexGet                                             */
/*                                                                       */
/*  Get the gateway address of the network interface in host order.      */
/*                                                                       */
/*  In    : iface                                                        */
/*  Out   : none                                                         */
/*  Return: Address in host order                                        */
/*************************************************************************/
void IP_IF_LinkSpeedDuplexGet (uint8_t iface, uint16_t *speed, uint8_t *duplex)
{
   *speed  = 0;
   *duplex = 0;
   
   if ((iface < ETH_MAX_IFACE) && (pNetIf[iface] != NULL))
   {
      *speed  = (uint16_t)pNetIf[iface]->link_speed;
      *duplex = (uint8_t)pNetIf[iface]->link_duplex_full;
   }

} /* IP_IF_LinkSpeedDuplexGet */

/*************************************************************************/
/*  IP_IF_Start                                                          */
/*                                                                       */
/*  Start the TCP/IP stack.                                              */
/*                                                                       */
/*  In    : iface                                                        */
/*  Out   : none                                                         */
/*  Return: 0 on success / -1 otherwise                                  */
/*************************************************************************/
int IP_IF_Start (uint8_t iface)
{
   int                      Error = -1;
   ip_addr_t                ipaddr;
   ip_addr_t                netmask;
   ip_addr_t                gw; 
   netif_init_fn            init;
   netif_status_callback_fn status_cb;
   netif_status_callback_fn link_cb;

   if ((iface >= ETH_MAX_IFACE) || (0 == InitDone))
   {
      return(-1);
   }
   
   /* Check if IP address or network mask is 0 */ 
   if ((0 == IPAddr[iface]) || (0 == NETMask[iface]))
   {
      /* Error, use default values */
      IPAddr[iface]  = inet_addr(IP_DEFAULT_ADDRESS);
      NETMask[iface] = inet_addr(IP_DEFAULT_NETMASK);
      GWAddr[iface]  = inet_addr(IP_DEFAULT_GATEWAY);
   }

   /* Check if MAC address is 0 */
   if( (0 == MACAddr[iface][0]) && (0 == MACAddr[iface][1]) && (0 == MACAddr[iface][2]) &&
       (0 == MACAddr[iface][3]) && (0 == MACAddr[iface][4]) && (0 == MACAddr[iface][5]) )
   {
      /* Error, use default values */
      memcpy(&MACAddr[iface][0], &MACDefaultAddr[iface][0], ETHARP_HWADDR_LEN);   
   }
   
   /* IP address setting */
   ipaddr.addr  = IPAddr[iface];
   netmask.addr = NETMask[iface];
   gw.addr      = GWAddr[iface];

   StartupIPAddr[iface]  = ipaddr.addr; 
   StartupNETMask[iface] = netmask.addr;
   StartupGWAddr[iface]  = gw.addr;
   
   /* Add the network interface to the list of lwIP netifs. */
   if (0 == iface)
   {
      init      = ethernetif_init;
      status_cb = NetIfStatusCallback0;
      link_cb   = NetIfLinkCallback0;
   }
#if (ETH_MAX_IFACE == 2)
   else
   {
      init      = ethernetif1_init;
      status_cb = NetIfStatusCallback1;
      link_cb   = NetIfLinkCallback1;
   }
#endif   
   
   pNetIf[iface] = netif_add(&NetIf[iface], &ipaddr, &netmask, &gw, &MACAddr[iface][0], init, tcpip_input); /*lint !e644*/

   if (pNetIf[iface] != NULL)
   {
      if (0 == iface)
      {
         /* Set the network interface as the default network interface. */
         netif_set_default(pNetIf[iface]);
      }         

      /*
       * Set the callback function whenever an interface
       * changes its up/down status (i.e., due to DHCP IP acquistion)
       */
      netif_set_status_callback(pNetIf[iface], status_cb);  /*lint !e644*/

      /* 
       * Set the callback function from an interface 
       * whenever the link changes (i.e., link down)
       */      
      netif_set_link_callback(pNetIf[iface], link_cb);      /*lint !e644*/      

      /* Bring the interface up. */
      netif_set_up(pNetIf[iface]); 
      
      Error = 0;
   }      
   
   return(Error);
} /* IP_IF_Start */    

/*************************************************************************/
/*  IP_IF_Stop                                                           */
/*                                                                       */
/*  Stop the TCP/IP stack.                                               */
/*                                                                       */
/*  In    : iface                                                        */
/*  Out   : none                                                         */
/*  Return: 0 on success / -1 otherwise                                  */
/*************************************************************************/
int IP_IF_Stop (uint8_t iface)
{
   int Error = -1;

   if ((iface < ETH_MAX_IFACE) && (pNetIf[iface] != NULL))
   {
      netif_set_down(pNetIf[iface]);
   }
   
   return(Error);
} /* IP_IF_Stop */      

/*************************************************************************/
/*  IP_IF_IsReady                                                        */
/*                                                                       */
/*  Check if the interface and link is Up.                               */
/*                                                                       */
/*  In    : iface                                                        */
/*  Out   : none                                                         */
/*  Return: 0 / 1                                                        */
/*************************************************************************/
int IP_IF_IsReady (uint8_t iface)
{
   int NetIfUp = 0;
   int Index;

   /*  Check if any iface has a link and is up */   
   if (IFACE_ANY == iface)
   {
      for (Index = 0; Index < ETH_MAX_IFACE; Index++)
      {
         if (pNetIf[Index] != NULL)
         {
            if ((netif_is_up(pNetIf[Index])) && (netif_is_link_up(pNetIf[Index])))
            {
               NetIfUp = 1;
               break;
            }
         }
      }
   }
   else
   {
      /* Check the given iface */
      if ((iface < ETH_MAX_IFACE) && (pNetIf[iface] != NULL))
      {
         if ((netif_is_up(pNetIf[iface])) && (netif_is_link_up(pNetIf[iface])))
         {
            NetIfUp = 1;
         }
      }      
   }      
   
   return(NetIfUp);
} /* IP_IF_IsReady */

/*************************************************************************/
/*  IP_IF_StartupValuesGet                                               */
/*                                                                       */
/*  In    : iface, ipaddr, netmask, gw                                   */
/*  Out   : ipaddr, netmask, gw                                          */
/*  Return: none                                                         */
/*************************************************************************/
void IP_IF_StartupValuesGet (uint8_t iface, uint32_t *ipaddr, uint32_t *netmask, uint32_t *gw)
{
   if ((iface < ETH_MAX_IFACE) && (ipaddr != NULL) && (netmask != NULL) && (gw != NULL))
   {
      *ipaddr  = StartupIPAddr[iface];
      *netmask = StartupNETMask[iface];
      *gw      = StartupGWAddr[iface]; 
   }
         
} /* IP_IF_StartupValuesGet */

/*************************************************************************/
/*  IP_IF_OutputConfig                                                   */
/*                                                                       */
/*  Display IP configuration.                                            */
/*                                                                       */
/*  In    : iface                                                        */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_IF_OutputConfig (uint8_t iface)
{
   char Buffer[24];

   if (iface < ETH_MAX_IFACE)
   {
      term_printf("*** Network configuration for iface: %d ***\r\n", iface);
      term_printf("\r\n");
   
   
      term_printf("Interface      : %s\r\n", (NetIfIsUp[iface]) ? "up" : "down");
      term_printf("Link           : %s\r\n", (LinkIsUp[iface]) ? "up" : "down");
   
      IP_IF_MACGet(iface, (uint8_t*)Buffer, ETHARP_HWADDR_LEN);   
      term_printf("MAC Address    : %02X:%02X:%02X:%02X:%02X:%02X\r\n",
                  Buffer[0], Buffer[1], Buffer[2], Buffer[3], Buffer[4], Buffer[5]);
               
      term_printf("TNPES Enabled  : %s\r\n", (IP_TNP_IsES() == 0) ? "no" : "yes");
      term_printf("DHCP Enabled   : %s\r\n", (IP_DHCP_IsStarted(iface) == 0) ? "no" : "yes");
      term_printf("mDNS Enabled   : %s\n",   (IP_mDNS_IsStarted() == 0) ? "no" : "yes");
      term_printf("IP Address     : %s\r\n", htoa(IP_IF_AddrGet(iface), Buffer, sizeof(Buffer)));
      term_printf("Subnet Mask    : %s\r\n", htoa(IP_IF_MaskGet(iface), Buffer, sizeof(Buffer)));
      term_printf("Gateway        : %s\r\n", htoa(IP_IF_GWGet(iface), Buffer, sizeof(Buffer)));
      term_printf("DHCP Server    : %s\r\n", htoa(IP_DHCP_ServerGet(iface), Buffer, sizeof(Buffer)));
      term_printf("DNS 1 Servcer  : %s\r\n", htoa(IP_DNS_ServerGet(0), Buffer, sizeof(Buffer)));
      term_printf("DNS 2 Server   : %s\r\n", htoa(IP_DNS_ServerGet(1), Buffer, sizeof(Buffer)));
      term_printf("NTP Server     : %s\r\n", htoa(IP_SNTP_ServerGet(), Buffer, sizeof(Buffer)));
      term_printf("Syslog Server  : %s\r\n", htoa(IP_SYSL_ServerGet(iface), Buffer, sizeof(Buffer)));
      term_printf("\r\n");
   }      

} /* IP_IF_OutputConfig */

/*************************************************************************/
/*  Name  : xrand                                                        */
/*                                                                       */
/*  Return a random value.                                               */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: Random                                                       */
/*************************************************************************/
unsigned long xrand (void)
{
   uint32_t Value;

   /* Create a new seed for a sequence of pseudo-random numbers */   
   Value  = OS_TimeGet();
   Value  = Value << 16;
   Value |= ((MACAddr[0][4] <<  8) | MACAddr[0][5]);
   srand(Value);

   /* Create the random value */
   Value  = (rand() & 0xFF) << 24;  /*lint !e701*/
   Value |= (rand() & 0xFF) << 16;
   Value |=  rand() & 0xFFFF;
   
   return(Value);   
} /* xrand */

/*** EOF ***/

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
*  06.06.2020  mifi  Rename API.
**************************************************************************/
#define __IP_DNS_C__

/*=======================================================================*/
/*  Include                                                              */
/*=======================================================================*/
#include "ipstack.h"

#include "lwip\dns.h"

#if (IP_DEFAULT_OPT_DNS >= 1)
/*=======================================================================*/
/*  All extern definitions                                               */
/*=======================================================================*/

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

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
/*  IP_DNS_ServerSet                                                     */
/*                                                                       */
/*  Set the DNS server address of the network interface in host order.   */
/*                                                                       */
/*  In    : index, addr                                                  */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_DNS_ServerSet (uint8_t index, uint32_t addr)
{
   ip_addr_t ipaddr;

   switch(index)
   {
      case 0:
      {
         ipaddr.addr = htonl(addr);
         dns_setserver(0, &ipaddr); 
         break;
      }   
      case 1:
      {
         ipaddr.addr = htonl(addr);
         dns_setserver(1, &ipaddr); 
         break;
      }   
      default:
      {
         /* Do nothing */
         break;
      }
   }      

} /* IP_DNS_ServerSet */

/*************************************************************************/
/*  IP_DNS_ServerGet                                                     */
/*                                                                       */
/*  Get the DNS server address of the network interface in host order.   */
/*                                                                       */
/*  In    : Index                                                        */
/*  Out   : none                                                         */
/*  Return: Address in host order                                        */
/*************************************************************************/
uint32_t IP_DNS_ServerGet (uint8_t index)
{
   const ip_addr_t *ipaddr;
   uint32_t         addr;
   
   switch(index)
   {
      case 0:
      {
         ipaddr = dns_getserver(0);
         addr = ntohl(ipaddr->addr);
         break;
      }
      case 1:
      {
         ipaddr = dns_getserver(1);
         addr = ntohl(ipaddr->addr);
         break;
      }
      default:
      {
         addr = 0;
         break;
      }
   }
   
   return(addr);
} /* IP_DNS_ServerGet */

/*************************************************************************/
/*  IP_DNS_GetHostByName                                                 */
/*                                                                       */
/*  Get the IP address in host order for the given name.                 */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: Address in host order                                        */
/*************************************************************************/
uint32_t IP_DNS_GetHostByName (char *name)
{
   struct hostent *hostent;
   uint32_t        addr;
   
   /* Check if a DNS server is available */   
   addr = IP_DNS_ServerGet(0);
   if (addr != 0)
   {
      hostent = gethostbyname(name);
      if (hostent != NULL)
      {
         if (hostent->h_addr != NULL)
         {
            addr = *((uint32_t*)hostent->h_addr);  /*lint !e826*/
         }
      }
   }      

   return( ntohl(addr) );
} /* IP_DNS_GetHostByName */

#else

uint32_t IP_DNS_ServerGet (uint8_t index)
{
   (void)index;
   
   return(0);
} /* IP_DNS_ServerGet */

#endif /* (IP_DEFAULT_OPT_DNS >= 1) */

/*** EOF ***/

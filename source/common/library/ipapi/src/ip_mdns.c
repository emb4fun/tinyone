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
*  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED                                                                         
*  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
*  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF 
*  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
*  SUCH DAMAGE.
*
***************************************************************************
*  History:
*
*  19.05.2018  mifi  First Version.
*  06.06.2020  mifi  Rework to support 2 interfaces, and rename API.
**************************************************************************/
#define __IP_MDNS_C__

/*=======================================================================*/
/*  Include                                                              */
/*=======================================================================*/
#include <stdlib.h>
#include <ctype.h>
#include "tal.h"
#include "terminal.h"
#include "ipstack.h"

#if (IP_DEFAULT_OPT_MDNS >= 1)
/*=======================================================================*/
/*  All extern definitions                                               */
/*=======================================================================*/

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

#if !defined(ETH_MAX_IFACE)
#define ETH_MAX_IFACE   1
#endif

#define MDNS_IP               0xFB0000E0  /* 224.0.0.251 */
#define LLMNR_IP              0xFC0000E0  /* 224.0.0.252 */ 

#define MDNS_PORT             5353
#define LLMNR_PORT            5355

#define SOCKET_RCVTIMEO_MS    16
#define DEFAULT_TTL           30
#define BUFFER_SIZE           256

/*
 * Some external swithes are not ready to use if the link
 * is available. Therefore wait a short time.
 */
#define DELAY_AFTER_LINK_MS   2000

#define DNS_QUERY_TYPE_A      0x0001
#define DNS_QUERY_TYPE_SOA    0x0006
#define DNS_QUERY_TYPE_AAAA   0x001C
#define DNS_QUERY_CLASS_INET  0x0001
#define DNS_QUERY_CLASS_FLUSH 0x8000


/* See RFC4795 */
typedef struct __attribute__((__packed__)) _dns_header_
{
   uint16_t XID;
   uint16_t Flags;
   uint16_t QDCount;
   uint16_t ANCount;
   uint16_t NSCount;
   uint16_t ARCount;
} dns_header_t;


#define BP2W(_a)     ((_a[0] << 8) | _a[1])
#define W2PB(_a,_b)  {_b[0] = (uint8_t)((_a & 0xFF00) >> 8); \
                      _b[1] = (uint8_t)_a; }

#define D2PB(_a,_b)  {_b[0] = (uint8_t)((_a & 0xFF000000) >> 24); \
                      _b[1] = (uint8_t)((_a & 0x00FF0000) >> 16); \
                      _b[2] = (uint8_t)((_a & 0x0000FF00) >>  8); \
                      _b[3] = (uint8_t)((_a & 0x000000FF));     }

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

/* 
 * Some TASK variables like stack and task control block.
 */
static OS_STACK (MDNSStack, TASK_IP_MDNS_STK_SIZE);
static OS_TCB TCBMDNS;

static int      mDNSInUse = 0;
static int      StopRequest = 0;
static uint32_t TTL = DEFAULT_TTL;
static uint8_t  RxBuffer[BUFFER_SIZE];
static uint8_t  TxBuffer[BUFFER_SIZE];


static char    *pMDNSHostname     = NULL;
static uint8_t  bMDNSHostnameLen  = 0;

static char    *pMDNSHostname2    = NULL;
static uint8_t  bMDNSHostnameLen2 = 0;

static char    *pLLMNRHostname     = NULL;
static uint8_t  bLLMNRHostnameLen  = 0;

static char    *pLLMNRHostname2    = NULL;
static uint8_t  bLLMNRHostnameLen2 = 0;

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  SetupHostname                                                        */
/*                                                                       */
/*  In    : pHostname1, pHostname2                                       */
/*  Out   : none                                                         */
/*  Return: 0 / -1                                                       */
/*************************************************************************/
static int SetupHostname (char *pHostname, char *pHostname2)
{
   int       rc = -1;
   uint8_t  bLen;
   uint16_t wSize;

   bMDNSHostnameLen   = 0;
   bMDNSHostnameLen2  = 0; 
   bLLMNRHostnameLen  = 0;
   bLLMNRHostnameLen2 = 0;
   
   /*
    * Setup mDNS hostname (.hostname.local'0')
    */
    
   /* mDNS Hostname 1 */
   bLen  = (uint8_t)strlen(pHostname);
   wSize = 1 + bLen + 1 + 5 + 1;    
   
   bMDNSHostnameLen = (uint8_t)wSize;
      
   pMDNSHostname = (char*)xmalloc(XM_ID_IP, wSize);
   if (NULL == pMDNSHostname) goto CleanUp; /*lint !e801*/
   
   pMDNSHostname[0] = (char)bLen;
   memcpy(&pMDNSHostname[1], pHostname, bLen);
   
   pMDNSHostname[1+bLen] = 5;
   memcpy(&pMDNSHostname[1+bLen+1], "local", 6);

   /* mDNS Hostname 2 */   
   if (pHostname2 != NULL)
   {
      bLen  = (uint8_t)strlen(pHostname2);
      wSize = 1 + bLen + 1 + 5 + 1;    
   
      bMDNSHostnameLen2 = (uint8_t)wSize;
   
      pMDNSHostname2 = (char*)xmalloc(XM_ID_IP, wSize);
      if (NULL == pMDNSHostname2) goto CleanUp; /*lint !e801*/
   
      pMDNSHostname2[0] = (char)bLen;
      memcpy(&pMDNSHostname2[1], pHostname2, bLen);
   
      pMDNSHostname2[1+bLen] = 5;
      memcpy(&pMDNSHostname2[1+bLen+1], "local", 6);
   }      


   /*
    * Setup LLMNR hostname (.hostname'0')
    */

   /* LLMNR Hostname 1 */
   bLen  = (uint8_t)strlen(pHostname);
   wSize = 1 + bLen + 1;
   
   bLLMNRHostnameLen = (uint8_t)wSize;
      
   pLLMNRHostname = (char*)xmalloc(XM_ID_IP, wSize);
   if (NULL == pLLMNRHostname) goto CleanUp; /*lint !e801*/
   
   pLLMNRHostname[0] = (char)bLen;
   memcpy(&pLLMNRHostname[1], pHostname, bLen+1);

   /* LLMNR Hostname 2 */
   if (pHostname2 != NULL)
   {
      bLen  = (uint8_t)strlen(pHostname2);
      wSize = 1 + bLen + 1;
   
      bLLMNRHostnameLen2 = (uint8_t)wSize;
      
      pLLMNRHostname2 = (char*)xmalloc(XM_ID_IP, wSize);
      if (NULL == pLLMNRHostname2) goto CleanUp; /*lint !e801*/
   
      pLLMNRHostname2[0] = (char)bLen;
      memcpy(&pLLMNRHostname2[1], pHostname2, bLen+1);
   }      

   rc = 0;   
   
CleanUp:
   if (rc != 0)
   {
      xfree(pMDNSHostname);
      xfree(pMDNSHostname2);
      xfree(pLLMNRHostname);
      xfree(pLLMNRHostname2);
      
      pMDNSHostname  = NULL;
      pMDNSHostname2  = NULL;
      pLLMNRHostname = NULL;
      pLLMNRHostname2 = NULL;
   }
      
   return(rc);   
} /* SetupHostname */

/*************************************************************************/
/*  BuildDNSTypeSOAResponse                                              */
/*                                                                       */
/*  In    : pPtr, pName, bNameLen                                        */
/*  Out   : none                                                         */
/*  Return: pPtr                                                         */
/*************************************************************************/
static uint8_t *BuildDNSTypeSOAResponse (uint8_t *pPtr, char *pName, uint8_t bNameLen)
{
   int i;
   
   /*lint -save -e778 -e831 -e845 */ 
   
   memcpy(pPtr, pName, bNameLen);      /* Name */
   pPtr += bNameLen;
   W2PB(DNS_QUERY_TYPE_SOA, pPtr);     /* Type */
   pPtr += 2;
   W2PB(DNS_QUERY_CLASS_INET, pPtr);   /* Class */
   pPtr += 2;
   D2PB(TTL, pPtr);                    /* TTL */
   pPtr += 4;
   W2PB((21 + bNameLen), pPtr);        /* Data length */
   pPtr += 2;
   
   i = 1;
   while(pName[i] != 0)
   {
      pName[i] = (char)tolower(pName[i]);
      i++;
   }
   
   memcpy(pPtr, pName, bNameLen);      /* Primary name server */
   pPtr += bNameLen;  
   *pPtr++ = 0;                        /* Responsible authoritys mailbox: root */
   D2PB(0, pPtr);                      /* Serial number: 0 */
   pPtr += 4;
   D2PB(0, pPtr);                      /* Refresh interval: 0 */
   pPtr += 4;
   D2PB(0, pPtr);                      /* Retry interval: 0 */
   pPtr += 4;
   D2PB(0, pPtr);                      /* Expire limit: 0 */
   pPtr += 4;
   D2PB(TTL, pPtr);                    /* Minimum TTL */
   pPtr += 4;

   /*lint -restore */

   return(pPtr);
} /* BuildDNSTypeSOAResponse */

/*************************************************************************/
/*  BuildDNSTypeAResponse                                                */
/*                                                                       */
/*  In    : pPtr, pName, bNameLen, dIPAddress, MDNS                      */
/*  Out   : none                                                         */
/*  Return: pPtr                                                         */
/*************************************************************************/
static uint8_t *BuildDNSTypeAResponse (uint8_t *pPtr, char *pName, uint8_t bNameLen, uint32_t dIPAddress, int MDNS)
{
   /*lint -save -e778 -e831 -e845 */ 

   memcpy(pPtr, pName, bNameLen);      /* Name */
   pPtr += bNameLen;
   W2PB(DNS_QUERY_TYPE_A, pPtr);       /* Type */
   pPtr += 2;
   
   if (0 == MDNS)                      /* Class */
   {
      W2PB(DNS_QUERY_CLASS_INET, pPtr);               
   }
   else
   {
      W2PB((DNS_QUERY_CLASS_FLUSH | DNS_QUERY_CLASS_INET), pPtr);
   }   
   pPtr += 2;
   
   D2PB(TTL, pPtr);                    /* TTL */
   pPtr += 4;
   W2PB(4, pPtr);                      /* Data length */
   pPtr += 2;
   D2PB(dIPAddress, pPtr);             /* Address */
   pPtr += 4;

   /*lint -restore */

   return(pPtr);
} /* BuildDNSTypeAResponse */

/*************************************************************************/
/*  HandleMDNSRequest                                                    */
/*                                                                       */
/*  In    : Sock, Size, pSource, pBuffer, bIface                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void HandleMDNSRequest (int Sock, int Size, struct sockaddr *pSource, uint8_t *pBuffer, uint8_t bIface)
{
   dns_header_t     *pDNSRequest  = (dns_header_t *)pBuffer; /*lint !e826*/
   dns_header_t     *pDNSResponse;
   uint8_t           bNameLen;
   char             *pName;
   uint8_t          *pPtrIn;
   uint8_t          *pPtrOut;
   uint16_t          wCount;
   uint16_t          wType;
   uint16_t          wClass;
   uint16_t          wOffsetHostname;
   uint32_t          dMyIPAddress;
   struct sockaddr_in Server;

   (void)pSource;
   
   /* MDNS response must be sent to the multicast address */
   Server.sin_addr.s_addr = inet_addr("224.0.0.251");
   Server.sin_port        = htons(MDNS_PORT);
   Server.sin_family      = AF_INET;
   

   if ((0 == pDNSRequest->Flags)          && 
       (0 == ntohs(pDNSRequest->ANCount)) &&
       (0 == ntohs(pDNSRequest->NSCount)))
   {
      wOffsetHostname = 0;
      
      wCount = ntohs(pDNSRequest->QDCount);

      /* First data after header */
      pPtrIn   = &pBuffer[sizeof(dns_header_t)];
      pName    = (char*)pPtrIn;
      bNameLen = 0;

      while(wCount != 0)
      {
         wCount--;
         
         if (wOffsetHostname != 0xC00C)
         {
            /* Query: Name */
            pName = (char*)pPtrIn;
         
            bNameLen = 0;
            while(pName[bNameLen] != 0)
            {
               bNameLen++;
            }
            bNameLen++;
      
            pPtrIn = (uint8_t*)&pName[bNameLen];
         }
         else
         {
            /* Jump over "offset hostname" */
            pPtrIn += 2;
         }            

         /* Query: Type */
         wType   = BP2W(pPtrIn);
         pPtrIn += 2;

         /* Query: Class */
         wClass  = BP2W(pPtrIn) & ~DNS_QUERY_CLASS_FLUSH;
         pPtrIn += 2;
      
         Size = sizeof(dns_header_t);
      
         /* Check for HOST IPv4 address request */    
         if ((DNS_QUERY_TYPE_A == wType) && (DNS_QUERY_CLASS_INET == wClass))
         {
            if( ((bMDNSHostnameLen  == bNameLen) && (0 == strncasecmp(pName, pMDNSHostname,  bNameLen))) ||
                ((bMDNSHostnameLen2 == bNameLen) && (0 == strncasecmp(pName, pMDNSHostname2, bNameLen))) )
            {
               dMyIPAddress = IP_IF_AddrGet(bIface);
         
               /* Build response header */
               pDNSResponse = (dns_header_t *)TxBuffer;
               memcpy(pDNSResponse, pDNSRequest, (size_t)Size); 
            
               /*lint -save -e778 -e831 -e845 */ 
               pDNSResponse->Flags   = htons(0x8400);
               pDNSResponse->QDCount = htons(0);
               pDNSResponse->ANCount = htons(1);
               pDNSResponse->NSCount = htons(0);
               pDNSResponse->ARCount = htons(0);
               /*lint -restore */
            
               pPtrOut = &TxBuffer[Size];
            
               /* Build response type A */
               pPtrOut = BuildDNSTypeAResponse(pPtrOut, pName, bNameLen, dMyIPAddress, 1);
            
               Size = (pPtrOut - TxBuffer);
            
               sendto(Sock, TxBuffer, (size_t)Size, 0, (struct sockaddr *)&Server, sizeof(struct sockaddr)); /*lint !e740*/
               break;
            }
         }
         
         if (wCount != 0)
         {
            wOffsetHostname = BP2W(pPtrIn);
         }   
      } /* end while(wCount != 0) */

   }   

} /* HandleMDNSRequest */

/*************************************************************************/
/*  HandleLLMNRRequest                                                   */
/*                                                                       */
/*  In    : Sock, Size, pSource, pBuffer, bIface                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void HandleLLMNRRequest (int Sock, int Size, struct sockaddr *pSource, uint8_t *pBuffer, uint8_t bIface)
{
   dns_header_t  *pDNSRequest  = (dns_header_t *)pBuffer; /*lint !e826*/
   dns_header_t  *pDNSResponse;
   uint8_t        bNameLen;
   char          *pName;
   uint8_t       *pPtr;
   uint16_t       wType;
   uint16_t       wClass;
   uint32_t       dMyIPAddress;

   
   if ((0 == pDNSRequest->Flags)          && 
       (1 == ntohs(pDNSRequest->QDCount)) &&
       (0 == ntohs(pDNSRequest->ANCount)) &&
       (0 == ntohs(pDNSRequest->NSCount)) &&
       (0 == ntohs(pDNSRequest->ARCount)))
   {
      /* Query: Name */
      pName = (char*)&pBuffer[sizeof(dns_header_t)];
      
      bNameLen = 0;
      while(pName[bNameLen] != 0)
      {
         bNameLen++;
      }
      bNameLen++;
      
      pPtr = (uint8_t*)&pName[bNameLen];

      /* Query: Type */
      wType = BP2W(pPtr);
      pPtr += 2;

      /* Query: Class */
      wClass  = BP2W(pPtr);
      pPtr   += 2;
      
      Size = pPtr - (uint8_t*)pBuffer;
      
      /* Check for HOST IPv4 address request */    
      if ((DNS_QUERY_TYPE_A == wType) && (DNS_QUERY_CLASS_INET == wClass))
      {
         if( ((bLLMNRHostnameLen  == bNameLen) && (0 == strncasecmp(pName, pLLMNRHostname,  bNameLen))) || 
             ((bLLMNRHostnameLen2 == bNameLen) && (0 == strncasecmp(pName, pLLMNRHostname2, bNameLen))) )
         {
            dMyIPAddress = IP_IF_AddrGet(bIface);
         
            /* Build response header */
            pDNSResponse = (dns_header_t *)TxBuffer;
            memcpy(pDNSResponse, pDNSRequest, (size_t)Size); 
            
            /*lint -save -e778 -e831 -e845 */ 
            pDNSResponse->Flags   = htons(0x8000);
            pDNSResponse->QDCount = htons(1);
            pDNSResponse->ANCount = htons(2);
            pDNSResponse->NSCount = htons(0);
            pDNSResponse->ARCount = htons(0);
            /*lint -restore */
            
            pPtr = &TxBuffer[Size];
            
            /* Build response type A */
            pPtr = BuildDNSTypeAResponse(pPtr, pName, bNameLen, dMyIPAddress, 0);
            
            /* Build response type SOA */
            pPtr = BuildDNSTypeSOAResponse(pPtr, pName, bNameLen);
            
            Size = (pPtr - TxBuffer);
            
            sendto(Sock, TxBuffer, (size_t)Size, 0, pSource, sizeof(struct sockaddr));
         }
      }
      
      /* Check for HOST IPv6 address request */    
      if ((DNS_QUERY_TYPE_AAAA == wType) && (DNS_QUERY_CLASS_INET == wClass))
      {
         if( ((bLLMNRHostnameLen  == bNameLen) && (0 == strncasecmp(pName, pLLMNRHostname,  bNameLen))) || 
             ((bLLMNRHostnameLen2 == bNameLen) && (0 == strncasecmp(pName, pLLMNRHostname2, bNameLen))) )
         {
            /* Build response header */
            pDNSResponse = (dns_header_t *)TxBuffer;
            memcpy(pDNSResponse, pDNSRequest, (size_t)Size); 
            
            /*lint -save -e778 -e831 -e845 */ 
            pDNSResponse->Flags   = htons(0x8000);
            pDNSResponse->QDCount = htons(1);
            pDNSResponse->ANCount = htons(0);
            pDNSResponse->NSCount = htons(1);
            pDNSResponse->ARCount = htons(0);
            /*lint -restore */
            
            pPtr = &TxBuffer[Size];
            
            /* Build response type SOA */
            pPtr = BuildDNSTypeSOAResponse(pPtr, pName, bNameLen);
            
            Size = (pPtr - TxBuffer);
            
            sendto(Sock, TxBuffer, (size_t)Size, 0, pSource, sizeof(struct sockaddr));
         }
      }
      
   }   
} /* HandleLLMNRRequest */

/*************************************************************************/
/*  MDNSTask                                                             */
/*                                                                       */
/*  In    : task parameter                                               */
/*  Out   : none                                                         */
/*  Return: never                                                        */
/*************************************************************************/
static void MDNSTask (void *arg)
{
   int                Err;
   int                Size;
   int                Optval;
   int                SockMDNS;
   int                SockMDNS_2;
   int                SockLLMNR;
   int                SockLLMNR_2;
   struct sockaddr_in ServerMDNS;
   struct sockaddr_in ServerLLMNR;
   struct sockaddr_in Source;
   int                SourceLen;  
   struct ip_mreq     mreq;
   uint8_t           *Buffer = RxBuffer;
   struct ifreq       if_bind;
   
   (void)arg;

   /* Wait that the IP interface is ready for use */
   while (0 == IP_IF_IsReady(IFACE_ANY))
   {
      OS_TimeDly(100);
   }

   /* Wait some time for the external switch */
   OS_TimeDly(DELAY_AFTER_LINK_MS);


   /*
    * Setup mDNS
    */
    
   /* Create socket */
   SockMDNS = socket(AF_INET, SOCK_DGRAM, 0);
   TAL_ASSERT(SockMDNS != SOCKET_ERROR);
   
   /* Set receive timeout */   
   Optval = SOCKET_RCVTIMEO_MS;
   Err = setsockopt(SockMDNS, SOL_SOCKET, SO_RCVTIMEO, &Optval, sizeof(Optval));
   TAL_ASSERT(0 == Err);
   
   /* Use SO_REUSEADDR */   
   Optval = 1;
   Err = setsockopt(SockMDNS, SOL_SOCKET, SO_REUSEADDR, &Optval, sizeof(Optval));
   TAL_ASSERT(0 == Err);
   
#if (ETH_MAX_IFACE == 2)
   /* Create socket */
   SockMDNS_2 = socket(AF_INET, SOCK_DGRAM, 0);
   TAL_ASSERT(SockMDNS_2 != SOCKET_ERROR);
   
   /* Set receive timeout */   
   Optval = SOCKET_RCVTIMEO_MS;
   Err = setsockopt(SockMDNS_2, SOL_SOCKET, SO_RCVTIMEO, &Optval, sizeof(Optval));
   TAL_ASSERT(0 == Err);
   
   /* Use SO_REUSEADDR */   
   Optval = 1;
   Err = setsockopt(SockMDNS_2, SOL_SOCKET, SO_REUSEADDR, &Optval, sizeof(Optval));
   TAL_ASSERT(0 == Err);
#else
   (void)SockMDNS_2;
   (void)SockLLMNR_2;
#endif

   /* Assign a name (port) to an unnamed socket */
   ServerMDNS.sin_addr.s_addr = INADDR_ANY;
   ServerMDNS.sin_port        = htons(MDNS_PORT);
   ServerMDNS.sin_family      = AF_INET;

   Err = bind(SockMDNS, (struct sockaddr *)&ServerMDNS, sizeof(ServerMDNS)); /*lint !e740*/
   TAL_ASSERT(0 == Err);
   
   strncpy(if_bind.ifr_name, "en0", IFNAMSIZ);   
   Err = setsockopt(SockMDNS, SOL_SOCKET, SO_BINDTODEVICE, (char *)&if_bind,  sizeof(if_bind));
   TAL_ASSERT(0 == Err);
   
#if (ETH_MAX_IFACE == 2)
   Err = bind(SockMDNS_2, (struct sockaddr *)&ServerMDNS, sizeof(ServerMDNS)); /*lint !e740*/
   TAL_ASSERT(0 == Err);
   
   strncpy(if_bind.ifr_name, "en1", IFNAMSIZ);   
   Err = setsockopt(SockMDNS_2, SOL_SOCKET, SO_BINDTODEVICE, (char *)&if_bind,  sizeof(if_bind));
   TAL_ASSERT(0 == Err);
#endif


   /*
    * Setup LLMNR
    */   
    
   /* Create socket */
   SockLLMNR = socket(AF_INET, SOCK_DGRAM, 0);
   TAL_ASSERT(SockLLMNR != SOCKET_ERROR);

   /* Set receive timeout */   
   Optval = SOCKET_RCVTIMEO_MS;
   Err = setsockopt(SockLLMNR, SOL_SOCKET, SO_RCVTIMEO, &Optval, sizeof(Optval));
   TAL_ASSERT(0 == Err);

   /* Set SO_REUSEADDR */   
   Optval = 1;
   Err = setsockopt(SockLLMNR, SOL_SOCKET, SO_REUSEADDR, &Optval, sizeof(Optval));
   TAL_ASSERT(0 == Err);

#if (ETH_MAX_IFACE == 2)
   /* Create socket */
   SockLLMNR_2 = socket(AF_INET, SOCK_DGRAM, 0);
   TAL_ASSERT(SockLLMNR_2 != SOCKET_ERROR);

   /* Set receive timeout */   
   Optval = SOCKET_RCVTIMEO_MS;
   Err = setsockopt(SockLLMNR_2, SOL_SOCKET, SO_RCVTIMEO, &Optval, sizeof(Optval));
   TAL_ASSERT(0 == Err);

   /* Set SO_REUSEADDR */   
   Optval = 1;
   Err = setsockopt(SockLLMNR_2, SOL_SOCKET, SO_REUSEADDR, &Optval, sizeof(Optval));
   TAL_ASSERT(0 == Err);
#endif

   /* Assign a name (port) to an unnamed socket */
   ServerLLMNR.sin_addr.s_addr = INADDR_ANY;
   ServerLLMNR.sin_port        = htons(LLMNR_PORT);
   ServerLLMNR.sin_family      = AF_INET;

   Err = bind(SockLLMNR, (struct sockaddr *)&ServerLLMNR, sizeof(ServerLLMNR)); /*lint !e740*/
   TAL_ASSERT(0 == Err);
   
   strncpy(if_bind.ifr_name, "en0", IFNAMSIZ);   
   Err = setsockopt(SockLLMNR, SOL_SOCKET, SO_BINDTODEVICE, (char *)&if_bind,  sizeof(if_bind));
   TAL_ASSERT(0 == Err);
   
#if (ETH_MAX_IFACE == 2)
   Err = bind(SockLLMNR_2, (struct sockaddr *)&ServerLLMNR, sizeof(ServerLLMNR)); /*lint !e740*/
   TAL_ASSERT(0 == Err);
   
   strncpy(if_bind.ifr_name, "en1", IFNAMSIZ);   
   Err = setsockopt(SockLLMNR_2, SOL_SOCKET, SO_BINDTODEVICE, (char *)&if_bind,  sizeof(if_bind));
   TAL_ASSERT(0 == Err);
#endif


   /* 
    * Set mDNS/LLMNR multicast address 
    */
    
   /* Set mDNS multicast address */
   mreq.imr_multiaddr.s_addr = MDNS_IP;
   mreq.imr_interface.s_addr = INADDR_ANY; //htonl(IP_IF_AddrGet(Iface));
   Err = setsockopt(SockMDNS, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
   TAL_ASSERT(0 == Err);

#if (ETH_MAX_IFACE == 2)
   Err = setsockopt(SockMDNS_2, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
   TAL_ASSERT(0 == Err);
#endif   
   

   /* Set LLMNR multicast address */
   mreq.imr_multiaddr.s_addr = LLMNR_IP;
   mreq.imr_interface.s_addr = INADDR_ANY; //htonl(IP_IF_AddrGet(Iface));
   Err = setsockopt(SockLLMNR, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
   TAL_ASSERT(0 == Err);

#if (ETH_MAX_IFACE == 2)
   Err = setsockopt(SockLLMNR_2, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
   TAL_ASSERT(0 == Err);
#endif   


   /*
    * Wait for mDNS/LLMNR messages
    */
   while(0 == StopRequest)
   {
      SourceLen = sizeof(Source);
      Size = recvfrom(SockMDNS, Buffer, BUFFER_SIZE, 0,
                      (struct sockaddr *)&Source, (socklen_t*)&SourceLen); /*lint !e740*/
      if (Size > 0) 
      {
         HandleMDNSRequest(SockMDNS, Size, (struct sockaddr *)&Source, Buffer, 0);  /*lint !e740*/
      }


      SourceLen = sizeof(Source);
      Size = recvfrom(SockLLMNR, Buffer, BUFFER_SIZE, 0,
                      (struct sockaddr *)&Source, (socklen_t*)&SourceLen); /*lint !e740*/
      if (Size > 0) 
      {
         HandleLLMNRRequest(SockLLMNR, Size, (struct sockaddr *)&Source, Buffer, 0);   /*lint !e740*/
      }

#if (ETH_MAX_IFACE == 2)
      SourceLen = sizeof(Source);
      Size = recvfrom(SockMDNS_2, Buffer, BUFFER_SIZE, 0,
                      (struct sockaddr *)&Source, (socklen_t*)&SourceLen); /*lint !e740*/
      if (Size > 0) 
      {
         HandleMDNSRequest(SockMDNS_2, Size, (struct sockaddr *)&Source, Buffer, 1);   /*lint !e740*/
      }


      SourceLen = sizeof(Source);
      Size = recvfrom(SockLLMNR_2, Buffer, BUFFER_SIZE, 0,
                      (struct sockaddr *)&Source, (socklen_t*)&SourceLen); /*lint !e740*/
      if (Size > 0) 
      {
         HandleLLMNRRequest(SockLLMNR_2, Size, (struct sockaddr *)&Source, Buffer, 1);  /*lint !e740*/
      }
#endif
   }


   /*
    * Free hostnames
    */
   xfree(pMDNSHostname);
   xfree(pMDNSHostname2);
   xfree(pLLMNRHostname);
   xfree(pLLMNRHostname2);
   
   pMDNSHostname   = NULL;
   pMDNSHostname2  = NULL;
   pLLMNRHostname  = NULL;
   pLLMNRHostname2 = NULL;
   
   StopRequest = 0;
   mDNSInUse   = 0;

} /* MDNSTask */  

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  IP_mDNS_Start                                                        */
/*                                                                       */
/*  Start the mDNS client of the network interface.                      */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_mDNS_Start (ip_mdns_config_t *pConfig)
{
   int rc;
   
   if ((0 == mDNSInUse) && (pConfig != NULL))
   {
      rc = SetupHostname(pConfig->Hostname, pConfig->Hostname2);
      if (0 == rc)
      {
         mDNSInUse = 1;
      
         TTL = (0 == pConfig->TTL) ? DEFAULT_TTL : pConfig->TTL;
      
         /* Create the MDNS Server task */
         OS_TaskCreate(&TCBMDNS, MDNSTask, NULL, TASK_IP_MDNS_PRIORITY,
                       MDNSStack, TASK_IP_MDNS_STK_SIZE, "mDNS");
      }                      
   }   
   
} /* IP_mDNS_Start */

/*************************************************************************/
/*  IP_mDNS_Stop                                                         */
/*                                                                       */
/*  Stop the mDNS client of the network interface.                       */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_mDNS_Stop (void)
{
   if (1 == mDNSInUse)
   {
      StopRequest = 1;
   }
   
} /* IP_mDNS_Stop */

/*************************************************************************/
/*  IP_mDNS_IsStarted                                                    */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 0 not started / 1 otherwise                                  */
/*************************************************************************/
int IP_mDNS_IsStarted (void)
{
   return(mDNSInUse);
} /* IP_mDNS_IsStarted */

#else

int IP_mDNS_IsStarted (void)
{
   return(0);
} /* IP_mDNS_IsStarted */

#endif /* (IP_DEFAULT_OPT_MDNS >= 1) */

/*** EOF ***/

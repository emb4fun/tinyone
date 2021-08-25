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
*  08.02.2015  mifi  First Version.
*  01.06.2019  mifi  Added functionality, based on "NUTSetup":
*                    (https://www.emb4fun.de/archive/nutsetup/index.html)
*  05.10.2019  mifi  Rename SAC to TNP (Tiny Network Protocol)
*  06.06.2020  mifi  Rework to support 2 interfaces, and rename API.
**************************************************************************/
#define __IP_TNP_C__

/*=======================================================================*/
/*  Include                                                              */
/*=======================================================================*/
#include "tal.h"
#include "terminal.h"
#include "ipstack.h"

#if (IP_DEFAULT_OPT_TNP >= 1)

#include "etc.h"
#include "nvm.h"

/*=======================================================================*/
/*  All extern definitions                                               */
/*=======================================================================*/

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

#if !defined(ETH_MAX_IFACE)
#define ETH_MAX_IFACE   1
#endif

/*
 * Some external swithes are not ready to use if the link
 * is available. Therefore wait a short time.
 */
#define DELAY_AFTER_LINK_MS   2000

#define SOCKET_RCVTIMEO_MS    100

#define MAX_TNP_LIST_ENTRY    64

typedef struct _tnp_list
{
   uint8_t  bMACAddress[6];
   uint32_t dIPAddress;
   uint32_t dFWVersion;
   char      Name[TNP_MAX_NAME_LEN];
   char      Location[TNP_MAX_LOCATION_LEN];
   char      MDNSName[TNP_MAX_MDNS_NAME_LEN];
} TNP_LIST;

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

/* 
 * Some TASK variables like stack and task control block.
 */
static OS_STACK (TNPStack, TASK_IP_TNP_STK_SIZE);
static OS_TCB TCBTNP;

static int TnpInUse = 0;
static int TnpReady = 0;

static TNP_LIST TNPList[MAX_TNP_LIST_ENTRY];

static uint8_t MyMACAddress[6];
static uint8_t MyMACAddress2[6] = {0};
static char    Name[TNP_MAX_NAME_LEN];
static char    Location[TNP_MAX_LOCATION_LEN];
static char    MDNSName[TNP_MAX_MDNS_NAME_LEN];

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  Name  : Add2List                                                     */
/*                                                                       */
/*  Add a entry to the SIP list.                                         */
/*  If no space is left, the entry will not added.                       */
/*                                                                       */
/*  In    : pMACAddress, dIPAddress, dVersion, pName, pLoc, pMDNS        */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void Add2List (uint8_t *pMACAddress, uint32_t dIPAddress, uint32_t dVersion, char *pName, char *pLoc, char *pMDNS)
{
   uint16_t wIndex;
   uint16_t wAvailable = FALSE;
   
   /* Check if entry is still in the list */
   for (wIndex=0; wIndex<MAX_TNP_LIST_ENTRY; wIndex++)
   {
      if (TNPList[wIndex].dIPAddress == dIPAddress)
      {
         wAvailable = TRUE;
         break;   
      }
   }
   
   if (FALSE == wAvailable)
   {
      /* The entry is not in the list, add it if possible */
      for (wIndex=0; wIndex<MAX_TNP_LIST_ENTRY; wIndex++)
      {
         /* Search free entry and copy data */
         if (0 == TNPList[wIndex].dIPAddress)
         {
            /* Copy MAC address */
            memcpy(TNPList[wIndex].bMACAddress, pMACAddress, 6);
         
            /* Copy IP address */
            TNPList[wIndex].dIPAddress = dIPAddress;
            
            /* Copy FW version */
            TNPList[wIndex].dFWVersion = dVersion;
         
            /* Copy Name, ignore source len, copy max size of dest */
            memcpy(TNPList[wIndex].Name, pName, TNP_MAX_NAME_LEN);
            
            /* Copy Loccation, ignore source len, copy max size of dest */
            memcpy(TNPList[wIndex].Location, pLoc, TNP_MAX_LOCATION_LEN);
            
            /* Copy mDNS, ignore source len, copy max size of dest */
            memcpy(TNPList[wIndex].MDNSName, pMDNS, TNP_MAX_MDNS_NAME_LEN);
            break;
         }
      }
   }      
   
} /* Add2List */

/*************************************************************************/
/*  Name  : InitList                                                     */
/*                                                                       */
/*  Init the SIP list.                                                   */
/*                                                                       */
/*  In    : bIface                                                       */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void InitList (void)
{
   uint32_t IPAddr1  = IP_IF_AddrGet(0);
#if (ETH_MAX_IFACE == 2)
   uint32_t IPAddr2  = IP_IF_AddrGet(1);
#endif   
   uint32_t FWVersion;

   /* Build version */
   FWVersion = PROJECT_VER_NUMBER;  /*lint !e845*/
   
   /* Clear the list */
   memset(TNPList, 0x00, sizeof(TNPList));
   
   /* Add itself to the list */
   Add2List(MyMACAddress, IPAddr1, FWVersion, Name, Location, MDNSName);
#if (ETH_MAX_IFACE == 2)
   Add2List(MyMACAddress2, IPAddr2, FWVersion, Name, Location, MDNSName);
#endif   

} /* InitList */

/*************************************************************************/
/*  SendRequest                                                          */
/*                                                                       */
/*  Send a TNP request to the network.                                   */
/*                                                                       */
/*  In    : bIface                                                       */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void SendRequest (uint8_t bIface)
{
   int                Err;
   int               nOptionValue;
   int               nResult;
   int                Socket;
   struct sockaddr_in Dest;
   IP_TNP_INFO        TNPInfo;
   struct ifreq       if_bind;
  
   /* Create the socket */ 
   Socket = socket(AF_INET, SOCK_DGRAM, 0);
   if (Socket != SOCKET_ERROR)
   {
      /* Change option to BROADCAST */
      nOptionValue = TRUE;
      setsockopt(Socket, SOL_SOCKET, SO_BROADCAST, (void *)&nOptionValue, sizeof(int));
      
      if (0 == bIface)
      {
         strncpy(if_bind.ifr_name, "en0", IFNAMSIZ);   
      }
#if (ETH_MAX_IFACE == 2)
      else
      {
         strncpy(if_bind.ifr_name, "en1", IFNAMSIZ);   
      }
#endif      
      Err = setsockopt(Socket, SOL_SOCKET, SO_BINDTODEVICE, (char *)&if_bind,  sizeof(if_bind)); /*lint !e645*/
      TAL_ASSERT(0 == Err);
    
      /* Set address and port for the socket */
      Dest.sin_addr.s_addr = INADDR_BROADCAST;
      Dest.sin_port        = htons(TNP_UDP_PORT);
      Dest.sin_family      = AF_INET;
    
      /* Setup SIPSetup structure */
      memset(&TNPInfo, 0x00, sizeof(IP_TNP_INFO));
  
      TNPInfo.dMagic1  = TNP_HEADER_MAGIC_1;
      TNPInfo.dMagic2  = TNP_HEADER_MAGIC_2;
      TNPInfo.wSize    = sizeof(IP_TNP_INFO);
      TNPInfo.wVersion = TNP_HEADER_VERSION;
    
      /* Send the packet */
      nResult = sendto(Socket, (const char *)&TNPInfo, sizeof(IP_TNP_INFO), 0, 
                       (struct sockaddr *)&Dest, sizeof(struct sockaddr)); /*lint !e740*/    
                       
      if (nResult != (int)sizeof(IP_TNP_INFO))
      {
         TAL_FAILED();  
      }
    
      /* Close the socket */
      closesocket(Socket);    
   }   
} /* SendRequest */

/*************************************************************************/
/*  SendResponse                                                         */
/*                                                                       */
/*  Send a TNP response "hello" to the network.                          */
/*                                                                       */
/*  In    : bIface                                                       */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void SendResponse (uint8_t bIface)
{
   int                Err;
   int               nOptionValue;
   int               nResult;
   int                Socket;
   uint8_t           bIndex;
   struct sockaddr_in Dest;
   IP_TNP_INFO        TNPInfo;
   uint32_t           IPAddr;
   struct ifreq       if_bind;
  
   /* Create the socket */ 
   Socket = socket(AF_INET, SOCK_DGRAM, 0);
   if (Socket != SOCKET_ERROR)
   {
      /* Change option to BROADCAST */
      nOptionValue = TRUE;
      setsockopt(Socket, SOL_SOCKET, SO_BROADCAST, (void *)&nOptionValue, sizeof(int));
      
      if (0 == bIface)
      {
         strncpy(if_bind.ifr_name, "en0", IFNAMSIZ);   
      }
#if (ETH_MAX_IFACE == 2)
      else
      {
         strncpy(if_bind.ifr_name, "en1", IFNAMSIZ);   
      }
#endif      
      Err = setsockopt(Socket, SOL_SOCKET, SO_BINDTODEVICE, (char *)&if_bind,  sizeof(if_bind)); /*lint !e645*/
      TAL_ASSERT(0 == Err);

      /* Set address and port for the socket */
      Dest.sin_addr.s_addr = INADDR_BROADCAST;
      Dest.sin_port        = htons(TNP_UDP_PORT);
      Dest.sin_family      = AF_INET;
    
      /* Setup SIPSetup structure */
      memset(&TNPInfo, 0x00, sizeof(IP_TNP_INFO));
  
      TNPInfo.dMagic1  = TNP_HEADER_MAGIC_1;
      TNPInfo.dMagic2  = TNP_HEADER_MAGIC_2;
      TNPInfo.wSize    = sizeof(IP_TNP_INFO);
      TNPInfo.wVersion = TNP_HEADER_VERSION;

      /* Check for normal or ES mode */
      if (1 == etc_IPTnpIsES())
      {
         TNPInfo.bMode = TNP_SETUP_RESPONSE_ES;
      }
      else
      {
         TNPInfo.bMode = TNP_SETUP_RESPONSE;
      }         
      
      TNPInfo.bUseDHCP = (1 == etc_IPDhcpIsUsed()) ? 1 : 0;
  
      for(bIndex=0; bIndex<6; bIndex++) 
      {
         if(0 == bIface)
         {
            TNPInfo.bMACAddress[bIndex] = MyMACAddress[bIndex];
         }
         else
         {
            TNPInfo.bMACAddress[bIndex] = MyMACAddress2[bIndex];
         }   
      }
    
      IPAddr = IP_IF_AddrGet(bIface);
      
      TNPInfo.dAddress   = htonl(IPAddr);
      TNPInfo.dMask      = htonl(IP_IF_MaskGet(bIface));
      TNPInfo.dGateway   = htonl(IP_IF_GWGet(bIface));
      TNPInfo.dFWVersion = PROJECT_VER_NUMBER;  /*lint !e845*/
            
      if (strlen(Name) <= TNP_NAME_LEN) 
      {
         memcpy(TNPInfo.Name, Name, strlen(Name));  
      }
    
      if (strlen(Location) <= TNP_LOCATION_LEN) 
      {
         memcpy(TNPInfo.Location, Location, strlen(Location));  
      }
      
      if ((MDNSName[0] != 0) && (strlen(MDNSName) <= TNP_MDNS_NAME_LEN))
      {
         memcpy(TNPInfo.MDNSName, MDNSName, strlen(MDNSName));  
      }
      
    
      /* 
       * Wait before sending the response.
       * We get problem if all devices on the network
       * send the response at the same time. Therefore
       * a delay is needed. Here we use the low byte of the
       * IP address and divide it by 2. This is now the delay.
       * Using the IP address should be prevent that all the 
       * devices calculated the same delay.
       */
      IPAddr &= 0x00FF;
      IPAddr = IPAddr >> 1;
      OS_TimeDly(OS_MS_2_TICKS((uint16_t)IPAddr));    
    
      /* Send the packet */
      nResult = sendto(Socket, (const char *)&TNPInfo, sizeof(IP_TNP_INFO), 0, 
                       (struct sockaddr *)&Dest, sizeof(struct sockaddr)); /*lint !e740*/    
                       
      (void)nResult;
    
      /* Close the socket */
      closesocket(Socket);    
   }
} /* SendResponse */

/*************************************************************************/
/*  HandleResponse                                                       */
/*                                                                       */
/*  Handle the incoming TNP packet.                                      */
/*                                                                       */
/*  In    : pTNPSetupIn, pSource, bIface                                 */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void HandleResponse (IP_TNP_INFO *pTNPSetupIn, struct sockaddr_in *pSource, uint8_t bIface)
{
   char IPAddress[16];
   /* We do not need to check the parameters here */
   
   switch (pTNPSetupIn->bMode)
   {
      case TNP_SETUP_REQUEST:
      {
         /* 
          * An other note from the network had send a request.
          * Now we must send the response to the network. Even we 
          * can use the request to clear the SIP list, and use all 
          * response's we receive to build the new SIP list. 
          */
         
         /* Send our response */         
         SendResponse(bIface);
         
         /* Init the SIP list */
         InitList();
         
#if 0 //!defined(TAL_NO_DEBUG)
         htoa(htonl(pSource->sin_addr.s_addr), IPAddress, sizeof(IPAddress));
         TAL_DEBUG(TAL_DBG_TNP, "TNP: Request from: %s", IPAddress);
#endif         
         break;
      }
      
      case TNP_SETUP_RESPONSE:
      case TNP_SETUP_RESPONSE_ES:
      {
         Add2List(pTNPSetupIn->bMACAddress, 
                  htonl(pTNPSetupIn->dAddress),
                  pTNPSetupIn->dFWVersion,
                  pTNPSetupIn->Name,
                  pTNPSetupIn->Location,
                  pTNPSetupIn->MDNSName);
                  
#if 0 //!defined(TAL_NO_DEBUG)
         htoa(htonl(pSource->sin_addr.s_addr), IPAddress, sizeof(IPAddress));
         TAL_DEBUG(TAL_DBG_TNP, "TNP: Response from: %s", IPAddress);
#endif         
         break;
      }

      case TNP_SETUP_SET:
      {
         NVM_IP IP;
         
#if 0 //!defined(TAL_NO_DEBUG)
         htoa(htonl(pSource->sin_addr.s_addr), IPAddress, sizeof(IPAddress));
         TAL_DEBUG(TAL_DBG_TNP, "TNP: Setup from: %s", IPAddress);
#endif         
         
         /* Check if this is a command for us */
         if( (1 == etc_IPTnpIsES()) && (0 == memcmp(pTNPSetupIn->bMACAddress, MyMACAddress, 6)) )
         {
            /* Check for "resetcfg" command */
            if (0 == strcmp(pTNPSetupIn->Location, "resetcfg"))
            {
               nvm_Reset();
            }
            else
            {
               /* Save the new values in the Fram */   
               nvm_IPUseDHCPSet(pTNPSetupIn->bUseDHCP);
               nvm_IPGet(&IP);
               
               IP.dIPAddr  = ntohl(pTNPSetupIn->dAddress);
               IP.dNETMask = ntohl(pTNPSetupIn->dMask);
               IP.dGWAddr  = ntohl(pTNPSetupIn->dGateway);
               memcpy(IP.Location, pTNPSetupIn->Location, NVM_IP_LOC_LEN);
               
               nvm_IPSet(&IP);
               nvm_Write();
            }      
            
            OS_TimeDly(2000);
                              
            /* Now we need a reset */                                    
            tal_CPUReboot();
         }   
         break;
      }
      
      default:
      {
         /* Do noting */
         break;
      }
   }
   
   (void)IPAddress;
   (void)pSource;

} /* HandleResponse */

/*************************************************************************/
/*  TNPTask                                                              */
/*                                                                       */
/*  In    : task parameter                                               */
/*  Out   : none                                                         */
/*  Return: never                                                        */
/*************************************************************************/
static void TNPTask (void *arg)
{
   int                Err;
   int                Size;
   int                Optval;
   int                Socket;
   int                Socket_2;
   struct sockaddr_in Server;
   struct sockaddr_in Source;
   int                SourceLen;  
   IP_TNP_INFO        TNPInfo; 
   struct ifreq       if_bind;

   (void)arg;
   
   IP_IF_MACGet(0, MyMACAddress,  sizeof(MyMACAddress));
   
#if (ETH_MAX_IFACE == 2)
   IP_IF_MACGet(1, MyMACAddress2, sizeof(MyMACAddress));
#endif   
   
   memcpy(Name,     etc_ShortNameGet(), TNP_MAX_NAME_LEN);
   memcpy(Location, etc_LocationGet(),  TNP_MAX_LOCATION_LEN);

   /* Wait that the IP interface is ready for use */
   while (0 == IP_IF_IsReady(IFACE_ANY))
   {
      OS_TimeDly(100);
   }

   /* Wait some time for the external switch */
   OS_TimeDly(DELAY_AFTER_LINK_MS);
   
   /* Clear the SIP list */
   InitList();
   
   /* Create socket */
   Socket = socket(AF_INET, SOCK_DGRAM, 0);
   TAL_ASSERT(Socket != SOCKET_ERROR);
   
   /* Receive broadcast messages too */
   Optval = 1;
   Err = setsockopt(Socket, SOL_SOCKET, SO_BROADCAST, &Optval, sizeof(Optval));
   TAL_ASSERT(0 == Err);
   
   /* Set receive timeout */   
   Optval = SOCKET_RCVTIMEO_MS;
   Err = setsockopt(Socket, SOL_SOCKET, SO_RCVTIMEO, &Optval, sizeof(Optval));
   TAL_ASSERT(0 == Err);
   
   /* Use SO_REUSEADDR */   
   Optval = 1;
   Err = setsockopt(Socket, SOL_SOCKET, SO_REUSEADDR, &Optval, sizeof(Optval));
   TAL_ASSERT(0 == Err);
   
#if (ETH_MAX_IFACE == 2)
   /* Create socket */
   Socket_2 = socket(AF_INET, SOCK_DGRAM, 0);
   TAL_ASSERT(Socket_2 != SOCKET_ERROR);
   
   /* Receive broadcast messages too */
   Optval = 1;
   Err = setsockopt(Socket_2, SOL_SOCKET, SO_BROADCAST, &Optval, sizeof(Optval));
   TAL_ASSERT(0 == Err);
   
   /* Set receive timeout */   
   Optval = SOCKET_RCVTIMEO_MS;
   Err = setsockopt(Socket_2, SOL_SOCKET, SO_RCVTIMEO, &Optval, sizeof(Optval));
   TAL_ASSERT(0 == Err);
   
   /* Use SO_REUSEADDR */   
   Optval = 1;
   Err = setsockopt(Socket_2, SOL_SOCKET, SO_REUSEADDR, &Optval, sizeof(Optval));
   TAL_ASSERT(0 == Err);
#endif
   
   /* Assign a name (port) to an unnamed socket */
   Server.sin_addr.s_addr = INADDR_ANY;
   Server.sin_port        = htons(TNP_UDP_PORT);
   Server.sin_family      = AF_INET;

   Err = bind(Socket, (struct sockaddr *)&Server, sizeof(Server)); /*lint !e740*/
   TAL_ASSERT(0 == Err);

   strncpy(if_bind.ifr_name, "en0", IFNAMSIZ);   
   Err = setsockopt(Socket, SOL_SOCKET, SO_BINDTODEVICE, (char *)&if_bind,  sizeof(if_bind));
   TAL_ASSERT(0 == Err);

#if (ETH_MAX_IFACE == 2)
   Err = bind(Socket_2, (struct sockaddr *)&Server, sizeof(Server)); /*lint !e740*/
   TAL_ASSERT(0 == Err);

   strncpy(if_bind.ifr_name, "en1", IFNAMSIZ);   
   Err = setsockopt(Socket_2, SOL_SOCKET, SO_BINDTODEVICE, (char *)&if_bind,  sizeof(if_bind));
   TAL_ASSERT(0 == Err);
#endif
   

   /* 
    * At this point the ServerSocket is 
    * created an can be used 
    */
    
   /* Send "hello" to the network */
   SendResponse(0);
   OS_TimeDly(OS_MS_2_TICKS(500));
   SendResponse(0);
   OS_TimeDly(OS_MS_2_TICKS(500));
   
#if (ETH_MAX_IFACE == 2)
   SendResponse(1);
   OS_TimeDly(OS_MS_2_TICKS(500));
   SendResponse(1);
   OS_TimeDly(OS_MS_2_TICKS(500));
#endif   
   
   /* Request information from the network */
   SendRequest(0);
#if (ETH_MAX_IFACE == 2)
   SendRequest(1);
#endif   

   TnpReady = 1;

   while(1)
   {
      SourceLen = sizeof(Source);
      Size = recvfrom(Socket, (uint8_t*)&TNPInfo, sizeof(TNPInfo), 0,
                      (struct sockaddr *)&Source, (socklen_t*)&SourceLen); /*lint !e740*/
                      
      if( (Size             == sizeof(IP_TNP_INFO)) &&   /*lint !e737*/
          (TNPInfo.dMagic1  == TNP_HEADER_MAGIC_1)  &&
          (TNPInfo.dMagic2  == TNP_HEADER_MAGIC_2)  &&
          (TNPInfo.wSize    == sizeof(IP_TNP_INFO)) &&
          (TNPInfo.wVersion == TNP_HEADER_VERSION)  )
      {
         HandleResponse(&TNPInfo, &Source, 0);
      }

#if (ETH_MAX_IFACE == 2)
      SourceLen = sizeof(Source);
      Size = recvfrom(Socket_2, (uint8_t*)&TNPInfo, sizeof(TNPInfo), 0,
                      (struct sockaddr *)&Source, (socklen_t*)&SourceLen); /*lint !e740*/
                      
      if( (Size             == sizeof(IP_TNP_INFO)) &&   /*lint !e737*/
          (TNPInfo.dMagic1  == TNP_HEADER_MAGIC_1)  &&
          (TNPInfo.dMagic2  == TNP_HEADER_MAGIC_2)  &&
          (TNPInfo.wSize    == sizeof(IP_TNP_INFO)) &&
          (TNPInfo.wVersion == TNP_HEADER_VERSION)  )
      {
         HandleResponse(&TNPInfo, &Source, 1);
      }
#else
      (void)Socket_2; /*lint !e530*/
#endif
      
   }

} /* TNPTask */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  IP_TNP_Start                                                         */
/*                                                                       */
/*  Start the "Search And Config" client of the network interface.       */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_TNP_Start (void)
{
   if (0 == TnpInUse)
   {
      TnpInUse = 1;
      
      /* Create the TNP Server task */
      OS_TaskCreate(&TCBTNP, TNPTask, NULL, TASK_IP_TNP_PRIORITY,
                    TNPStack, TASK_IP_TNP_STK_SIZE, "TNP");
   }   
   
} /* IP_TNP_Start */

/*************************************************************************/
/*  IP_TNP_IsReady                                                       */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 0 not ready / 1 ready                                        */
/*************************************************************************/
int IP_TNP_IsReady (void)
{
   return(TnpReady);
} /* IP_TNP_IsReady */

/*************************************************************************/
/*  IP_TNP_IsES                                                          */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 0 = TnpIsES not enabled / 1 = TnpIsES enabled                */
/*************************************************************************/
int IP_TNP_IsES (void)
{
   return( etc_IPTnpIsES() );
} /* IP_TNP_IsES */

/*************************************************************************/
/*  IP_TNP_DeviceGet                                                     */
/*                                                                       */
/*  Return the entry if available.                                       */
/*                                                                       */
/*  In    : wIndex, pInfo                                                */
/*  Out   : pInfo                                                        */
/*  Return: 0 on success / -1 otherwise                                  */
/*************************************************************************/
int IP_TNP_DeviceGet (uint32_t index, IP_TNP_INFO *pInfo)
{
   int Result = -1;
   
   if (index < MAX_TNP_LIST_ENTRY)
   {
      if( (pInfo != NULL) && (TNPList[index].dIPAddress != 0) )
      {
         memcpy(pInfo->bMACAddress, TNPList[index].bMACAddress, 6);
         pInfo->dAddress   = TNPList[index].dIPAddress;
         pInfo->dFWVersion = TNPList[index].dFWVersion;
         memcpy(pInfo->Name, TNPList[index].Name, TNP_MAX_NAME_LEN);
         memcpy(pInfo->Location, TNPList[index].Location, TNP_MAX_LOCATION_LEN);
         memcpy(pInfo->MDNSName, TNPList[index].MDNSName, TNP_MAX_MDNS_NAME_LEN);
         
         Result = 0; 
      }
   }
   
   return(Result);
} /* IP_TNP_DeviceGet */

/*************************************************************************/
/*  IP_TNP_DeviceFind                                                    */
/*                                                                       */
/*  Return the IP address for the given device.                          */
/*                                                                       */
/*  In    : pDevice, pAddr                                               */
/*  Out   : pAddr                                                        */
/*  Return: 0 on success / -1 otherwise                                  */
/*************************************************************************/
int IP_TNP_DeviceFind (char *pDevice, uint32_t *pAddr)
{
   int Result = -1;
   
   if( (pDevice != NULL ) && (pAddr != NULL) )
   {
      *pAddr = 0;
      
      for (int index = 0; index < MAX_TNP_LIST_ENTRY; index++)
      {
         if( (TNPList[index].dIPAddress != 0) && (0 == strcmp(TNPList[index].Name, pDevice)) )
         {
            *pAddr = TNPList[index].dIPAddress;
         
            Result = 0;
            break;
         }
      }
   }      
   
   return(Result);
} /* IP_TNP_DeviceFind */

/*************************************************************************/
/*  IP_TNP_SendRequest                                                   */
/*                                                                       */
/*  Send a TNP request to the network.                                   */
/*                                                                       */
/*  In    : bIface                                                       */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_TNP_SendRequest (uint8_t bIface)
{
   /* Init the SIP list */
   InitList();

   if (bIface < ETH_MAX_IFACE)
   {
      /* Send the request to the network */
      SendRequest(bIface);
   
      /* 
       * Send the response to the network too, because with the request 
       * before the list is cleared in all other nodes. And with the 
       * response here, the other one can add us again.
       */
      SendResponse(bIface);   
   }      
   
} /* IP_TNP_SendRequest */

/*************************************************************************/
/*  IP_TNP_DHCPCallbackBound                                             */
/*                                                                       */
/*  In    : bIface                                                       */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_TNP_DHCPCallbackBound (uint8_t bIface)
{
   InitList();

   if (bIface < ETH_MAX_IFACE)
   {
      /* Send "hello" to the network */
      SendResponse(bIface);
      OS_TimeDly(OS_MS_2_TICKS(500));
      SendResponse(bIface);
      OS_TimeDly(OS_MS_2_TICKS(500));
   
      /* Request information from the network */
      SendRequest(bIface);
   }      
   
} /* IP_TNP_DHCPCallbackBound */

/*************************************************************************/
/*  IP_TNP_SetName                                                       */
/*                                                                       */
/*  Set mDNS name.                                                       */
/*                                                                       */
/*  In    : pName, bLen                                                  */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_TNP_SetName (char *pName, uint8_t bLen)
{
   (void)bLen;
   
   snprintf(MDNSName, sizeof(MDNSName), "%s.local", pName);
   
} /* IP_TNP_SetName */

#else

int IP_TNP_IsStarted (void)
{
   return(0);
} /* IP_TNP_IsStarted */

#endif /* (IP_DEFAULT_OPT_TNP >= 1) */

/*** EOF ***/

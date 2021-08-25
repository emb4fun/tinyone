/****************************************************************************
*  Copyright (c) 2011 by Michael Fischer (www.emb4fun.de).
*  All rights reserved.
*
*  This work based on source from proconX Pty Ltd. Therefore
*  partial copyright by: Copyright (c) 2010 proconX Pty Ltd.
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
*****************************************************************************
*  History:
*
*  27.05.2011  mifi  First Version based on source from proconX Pty Ltd.
*                    Information about UPnP can be found at "www.upnp.org".
*                    Note: I had no success with the UPnP Device Validator
*                    from the "Open Software Projects". The old original
*                    Intel tools from the book "UPnP Design by Example"
*                    works a little bit better, but has problems too.
*
*  16.12.2013  mifi  Ported to BSD sockets.
*  08.02.2014  mifi  Renamed functions which are exported.
*  08.02.2015  mifi  Use a semaphore to trigger AddrUpdate.
****************************************************************************/
#define __IP_UPNP_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tal.h"
#include "ipstack.h"
#include "ipweb.h"

int snprintf(char *s, size_t n, const char *format, ...);

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

/*
 * UPnP manufacurer and model description
 */
#ifndef UPNP_FRIENDLY_NAME
#define UPNP_FRIENDLY_NAME    "emb4fun UPnP Demo"
#endif

#ifndef UPNP_MANUFACTURER
#define UPNP_MANUFACTURER     "emb4fun"
#endif

#ifndef UPNP_MANUFACTURER_URL
#define UPNP_MANUFACTURER_URL "http://www.emb4fun.de"
#endif

#ifndef UPNP_MODEL_DESC
#define UPNP_MODEL_DESC       "Web server with UPnP"
#endif

#ifndef UPNP_MODEL_NAME
#define UPNP_MODEL_NAME       "TAL"
#endif

#ifndef UPNP_MODEL_URL
#define UPNP_MODEL_URL        "http://www.emb4fun.de"
#endif
 

/*
 * Some external swithes are not ready to use if the link
 * is available. Therefore wait a short time.
 */
#define DELAY_AFTER_LINK_MS   2000


/*
 * XML buffer size
 */
#define XML_BUF_SIZE 1536


/*
 * Enable to fulfil the UPnP specification for notify messages.
 * I think this is not needed to advertise only the device on
 * windows networks.
 */
#define UPNP_FULFIL_SPEC      0

#define UPNP_UUID_PART1       "56F9C1D5-5083-4ee5-A6B3-"

#define SSDP_IP               0xfaffffefU /* 239.255.255.250 */
#define SSDP_PORT             1900
#define SSDP_BUFFER_SIZE      512
#define SSDP_NOTIFY_TIMEOUT   300000U     /* 5 minutes */


/* *INDENT-OFF* */

static char NOTIFY_ALIVE[] =
   "NOTIFY * HTTP/1.1\r\n"
   "Host: 239.255.255.250:1900\r\n"
   "Cache-Control: max-age=900\r\n"
   "Location: http://%s/cgi-bin/upnp.cgi\r\n"
   "NT: %s\r\n"
   "NTS: ssdp:alive\r\n"
   "USN: %s%s\r\n"
   "Server: %s\r\n"
   "Content-Length: 0\r\n"
   "\r\n";

static char NOTIFY_BYE[] =
   "NOTIFY * HTTP/1.1\r\n"
   "Host: 239.255.255.250:1900\r\n"
   "NT: upnp:rootdevice\r\n"
   "NTS: ssdp:byebye\r\n"
   "USN: %s::upnp:rootdevice\r\n"
   "Content-Length: 0\r\n"
   "\r\n";

static char MSEARCH_RESPONSE[] =
   "HTTP/1.1 200 OK\r\n"
   "Cache-Control: max-age=900\r\n"
   "Ext:\r\n"
   "Location: http://%s/cgi-bin/upnp.cgi\r\n"
   "Server: %s\r\n"
   "ST: upnp:rootdevice\r\n"
   "USN: %s::upnp:rootdevice\r\n"
   "Content-Length: 0\r\n"
   "\r\n";

/*
 * All listed elements which are not tagged are "REQUIRED" as per
 * UPnP Device Architecture 1.0. Some other are "OPTIONAL" or "RECOMMENDED".
 */
static char HTML_UPNP[] =
"<?xml version=\"1.0\"?>\r\n"
"<root xmlns=\"urn:schemas-upnp-org:device-1-0\">\r\n"
   "<specVersion>\r\n"
      "<major>1</major>\r\n"
      "<minor>0</minor>\r\n"
   "</specVersion>\r\n"
   "<URLBase>http://%s</URLBase>\r\n"                                                  /* OPTIONAL */
   "<device>\r\n"
      "<deviceType>urn:schemas-upnp-org:device:Basic:1</deviceType>\r\n"
      "<friendlyName>%s (%02X%02X%02X%02X%02X%02X)</friendlyName>\r\n"
      "<manufacturer>%s</manufacturer>\r\n"
      "<manufacturerURL>%s</manufacturerURL>\r\n"                                      /* OPTIONAL */
      "<modelDescription>%s</modelDescription>\r\n"                                    /* RECOMMENDED */
      "<modelName>%s</modelName>\r\n"
      "<modelNumber>%02X%02X%02X%02X%02X%02X</modelNumber>\r\n"                        /* RECOMMENDED */
      "<modelURL>%s</modelURL>\r\n"                                                    /* OPTIONAL */
      "<serialNumber>%02X%02X%02X%02X%02X%02X</serialNumber>\r\n"                      /* RECOMMENDED */
      "<UDN>uuid:" UPNP_UUID_PART1 "%02X%02X%02X%02X%02X%02X</UDN>\r\n"

      /*
       * Despite being not required by the UPnP standard, we must
       * define at leat one service to make the device show up on Windows XP.
       */
      "<serviceList>\r\n"
         "<service>\r\n"
            "<serviceType>urn:schemas-upnp-org:service:Dummy:1</serviceType>\r\n"
            "<serviceId>urn:upnp-org:serviceId:Dummy</serviceId>\r\n"
            "<SCPDURL>/cgi-bin/upnp/dummy.cgi</SCPDURL>\r\n"
            "<controlURL>/upnp</controlURL>\r\n"
            "<eventSubURL></eventSubURL>\r\n"
         "</service>\r\n"
      "</serviceList>\r\n"
      "<presentationURL>/index.htm</presentationURL>\r\n"                              /* RECOMMENDED */
   "</device>\r\n"
"</root>\r\n";

static char HTML_UPNP_DUMMY[] =
"<?xml version=\"1.0\" encoding=\"utf-8\"?>"
"<scpd xmlns=\"urn:schemas-upnp-org:service-1-0\">"
   "<specVersion>"
      "<major>1</major>"
      "<minor>0</minor>"
   "</specVersion>"
   "<serviceStateTable>"
      "<stateVariable>"
         "<name>Dummy</name>"
         "<dataType>i1</dataType>"
      "</stateVariable>"
   "</serviceStateTable>"
" </scpd>";

/* *INDENT-ON* */

/*=======================================================================*/
/*  Definition of all global Data                                        */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/
/* 
 * Some TASK variables like stack and task control block.
 */
static OS_STACK (SSDPStack,   TASK_IP_SSDP_STK_SIZE);
static OS_STACK (NotifyStack, TASK_IP_SSDP_NOTIFY_STK_SIZE);

static os_tcb_t   TCBSSDP;
static os_tcb_t   TCBNotify;


static uint8_t    MyMACAddress[6];
static uint32_t   MyIPHost;
static char       MyIPStr[16];

static char       SSDPBuffer[SSDP_BUFFER_SIZE + 1];   /* Add one for the string termination 0 */
static char       NotifyBuffer[SSDP_BUFFER_SIZE + 1]; /* Add one for the string termination 0 */
static char       MyUSN[48];
static char       ServerInfo[32];
static int        UpnpInUse = 0;

static os_sema_t  UpdateSema;

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  GetRand                                                              */
/*                                                                       */
/*  Return a random number between 0...MaxValue.                         */
/*                                                                       */
/*  In    : MaxValue                                                     */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static uint32_t GetRand(uint32_t MaxValue)
{
   uint32_t Value;

   Value = rand() & MaxValue;

   return (Value);
} /* GetRand */

/*************************************************************************/
/*  UPnPCGIDummyHandler                                                  */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static int UPnPCGIDummyHandler (HTTPD_SESSION *hs)
{
   char *buf;

   /*
    * Generate HTTP header
    */
   HttpSendHeaderTop(hs, 200);
   HttpSendHeaderBottom(hs, "text", "xml", -1, 0);

   /*
    * Output XML data
    */
   buf = xmalloc(XM_ID_IP, XML_BUF_SIZE);
   if (buf != NULL)
   {
      snprintf(buf, XML_BUF_SIZE, HTML_UPNP_DUMMY);
      s_puts(buf, hs->s_stream);
      
      xfree(buf);               
   }

   s_flush(hs->s_stream);

   return(0);
} /* UPnPCGIDummyHandler */

/*************************************************************************/
/*  UPnPCGIHandler                                                       */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static int UPnPCGIHandler (HTTPD_SESSION *hs)
{
   char *buf;
   
   /*
    * Generate HTTP header
    */
   HttpSendHeaderTop(hs, 200);
   HttpSendHeaderBottom(hs, "text", "xml", -1, 0);
   
   /*
    * Output XML data
    */
   buf = xmalloc(XM_ID_IP, XML_BUF_SIZE);
   if (buf != NULL)
   {
      snprintf(buf, XML_BUF_SIZE, HTML_UPNP,
               MyIPStr,                          /* URLBase */

               UPNP_FRIENDLY_NAME,               /* friendlyName */
               MyMACAddress[0], MyMACAddress[1], 
               MyMACAddress[2], MyMACAddress[3],
               MyMACAddress[4], MyMACAddress[5],
              
               UPNP_MANUFACTURER,                /* manufacturer */
               UPNP_MANUFACTURER_URL,            /* manufacturer URL */
               UPNP_MODEL_DESC,                  /* model Description */
               UPNP_MODEL_NAME,                  /* model Name */ 

               MyMACAddress[0], MyMACAddress[1], /* model Number */
               MyMACAddress[2], MyMACAddress[3],
               MyMACAddress[4], MyMACAddress[5],

               UPNP_MODEL_URL,                   /* model URL */   

               MyMACAddress[0], MyMACAddress[1], /* serial Number */
               MyMACAddress[2], MyMACAddress[3],
               MyMACAddress[4], MyMACAddress[5],

               MyMACAddress[0], MyMACAddress[1], /* UDN */
               MyMACAddress[2], MyMACAddress[3],
               MyMACAddress[4], MyMACAddress[5]);
              
      s_puts(buf, hs->s_stream);
      
      xfree(buf);               
   }

   s_flush(hs->s_stream);
   
   return(0);
} /* UPnPCGIHandler */

/*************************************************************************/
/*  SendNotifyAliveChunk                                                 */
/*                                                                       */
/*  In    : Buffer, BufSize, TxSock                                      */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void SendNotifyAliveChunk(char *Buffer, int BufSize, int TxSock)
{
   int                Size;
   int                Err;
   struct sockaddr_in Dest;

   /*
    * Send device discovery messages.
    * The first message is really needed.
    * All other to fulfil the UPnP specification.
    */
    
   /* Build destination address */    
   Dest.sin_addr.s_addr = SSDP_IP;
   Dest.sin_port        = htons(SSDP_PORT);
   Dest.sin_family      = AF_INET;

   /* Send first alive message */
   Size = snprintf(Buffer, BufSize, NOTIFY_ALIVE,
                   MyIPStr,
                   "upnp:rootdevice",           /* NT */
                   MyUSN, "::upnp:rootdevice",  /* USN */
                   ServerInfo);
                  
   Err = sendto(TxSock, Buffer, Size, 0, (struct sockaddr *)&Dest, sizeof(Dest));
   TAL_ASSERT(Err == Size);
   OSTimeDly(25);

#if (UPNP_FULFIL_SPEC >= 1)

   /* Send second alive message */
   Size = snprintf(Buffer, BufSize, NOTIFY_ALIVE,
                   MyIPStr,
                   MyUSN,     /* NT */
                   MyUSN, "", /* USN */
                   ServerInfo);
   Err = sendto(TxSock, Buffer, Size, 0, (struct sockaddr *)&Dest, sizeof(Dest));
   TAL_ASSERT(Err == Size);
   tal_OSTimeDly(25);

   /* Send third alive message */
   Size = snprintf(Buffer, BufSize, NOTIFY_ALIVE,
                   MyIPStr,
                   "urn:schemas-upnp-org:device:Basic:1",            /* NT */
                   MyUSN, "::urn:schemas-upnp-org:device:Basic:1",   /* USN */
                   ServerInfo);
   Err = sendto(TxSock, Buffer, Size, 0, (struct sockaddr *)&Dest, sizeof(Dest));
   TAL_ASSERT(Err == Size);
   tal_OSTimeDly(25);


   /*
    * Send embedded device discovery messages
    */

   /* No embedded device available */

   /*
    * Send service discovery messages
    */
   Size = snprintf(Buffer, BufSize, NOTIFY_ALIVE,
                   MyIPStr,
                   "urn:schemas-upnp-org:service:Dummy:1",           /* NT */
                   MyUSN, "::urn:schemas-upnp-org:service:Dummy:1",  /* USN */
                   ServerInfo);
   Err = sendto(TxSock, Buffer, Size, 0, (struct sockaddr *)&Dest, sizeof(Dest));
   TAL_ASSERT(Err == Size);
#endif
} /* SendNotifyAliveChunk */

/*************************************************************************/
/*  SendNotifyAlive                                                      */
/*                                                                       */
/*  In    : Buffer, BufSize, TxSock                                      */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void SendNotifyAlive (char *Buffer, int BufSize, int TxSock)
{
   uint32_t RandomDelay;

   /*
    * Send device discovery messages. Here two chunks will be send
    * with a random delay between each chunk.
    */

   SendNotifyAliveChunk(Buffer, BufSize, TxSock);

   /* Create random value between 500...2000ms */
   RandomDelay = 500 + GetRand(1500);

   OSTimeDly(RandomDelay);

   SendNotifyAliveChunk(Buffer, BufSize, TxSock);

} /* SendNotifyAlive */

/*************************************************************************/
/*  SendByeBye                                                           */
/*                                                                       */
/*  In    : Buffer, BufSize, TxSock                                      */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void SendByeBye (char *Buffer, int BufSize, int TxSock)
{
   int                Err;
   int                Size;
   struct sockaddr_in Dest;
   
   Dest.sin_family      = AF_INET;
   Dest.sin_port        = htons(SSDP_PORT);
   Dest.sin_addr.s_addr = SSDP_IP;
    
   Size = snprintf(Buffer, BufSize, NOTIFY_BYE, MyUSN);
   Err = sendto(TxSock, Buffer, Size, 0, (struct sockaddr *)&Dest, sizeof(Dest));
   TAL_ASSERT(Err == Size);
} /* SendByeBye */ 

/*************************************************************************/
/*  AddrUpdate                                                           */
/*                                                                       */
/*  This is a trigger to update the IP address which is used.            */
/*  Send a "ByeBye" with the old IP address, and a "Hello" with          */
/*  with the new one.                                                    */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void AddrUpdate (void)
{
   char *Buffer = SSDPBuffer;
   int   TxSock;
   
   if (1 == UpnpInUse)
   {
      TxSock = socket(AF_INET, SOCK_DGRAM, 0);
      TAL_ASSERT(TxSock != SOCKET_ERROR);
   
      if (TxSock != SOCKET_ERROR)
      {
         /* Send ByeBye with the old IP address. */
         SendByeBye(Buffer, SSDP_BUFFER_SIZE, TxSock);
         OSTimeDly(100);
         SendByeBye(Buffer, SSDP_BUFFER_SIZE, TxSock);

         /* Get my IP address */
         MyIPHost = IPIfaceAddrGet();   
         htoa(MyIPHost, MyIPStr, sizeof(MyIPStr));
   
         /* Send a "Hello" with the new IP address. */
         SendNotifyAlive(Buffer, SSDP_BUFFER_SIZE, TxSock);
      
         closesocket(TxSock);
      }
   }      
} /* AddrUpdate */

/*************************************************************************/
/*  NotifyTask                                                           */
/*                                                                       */
/*  This is the Notify task.                                             */
/*                                                                       */
/*  In    : arg                                                          */
/*  Out   : none                                                         */
/*  Return: never                                                        */
/*************************************************************************/
static void NotifyTask (void *arg)
{
   TAL_RESULT Error;
   int        TxSock;
   
   (void)arg;
   
   /* Create Tx socket */
   TxSock = socket(AF_INET, SOCK_DGRAM, 0);
   TAL_ASSERT(TxSock != SOCKET_ERROR);
   
   while (1)
   {
      Error = OSSemaWait(&UpdateSema, SSDP_NOTIFY_TIMEOUT);
      if (OS_RC_OK == Error)
      {
         AddrUpdate();
      }
      else
      {
         /* Send alive message. */
         SendNotifyAlive(NotifyBuffer, sizeof(NotifyBuffer), TxSock);
      }         
   }

} /* NotifyTask */

/*************************************************************************/
/*  SSDPTask                                                             */
/*                                                                       */
/*  This is the SSDP task.                                               */
/*                                                                       */
/*  In    : arg                                                          */
/*  Out   : none                                                         */
/*  Return: never                                                        */
/*************************************************************************/
static void SSDPTask (void *arg)
{
   char              *Buffer = SSDPBuffer;
   int                Err;
   int                Size;
   int                RxSock;
   int                TxSock;
   struct sockaddr_in Server;
   struct sockaddr_in Source;
   int                SourceLen;  
   struct ip_mreq     mreq;
   char              *Start;
   char              *End;
   uint32_t           MaxDelay;
   uint32_t           Delay;

   (void)arg;
   
   /* Wait that the IP interface is ready for use */
   while(0 == IPIsNetIfUp())
   {
      OSTimeDly(500);
   }
   
   /* UPnP is in use */
   UpnpInUse = 1;
   
   /* Wait some time for the external switch */
   OSTimeDly(DELAY_AFTER_LINK_MS);
         
   /* Get MAC address */   
   IPIfaceMACGet(MyMACAddress, sizeof(MyMACAddress));
      
   /* Get my IP address */
   MyIPHost = IPIfaceAddrGet();   
   htoa(MyIPHost, MyIPStr, sizeof(MyIPStr));
         
   /* Setup UPnP info */
   snprintf(MyUSN, sizeof(MyUSN),
            "uuid:%s%02X%02X%02X%02X%02X%02X",
            UPNP_UUID_PART1, 
            MyMACAddress[0], MyMACAddress[1], MyMACAddress[2],
            MyMACAddress[3], MyMACAddress[4], MyMACAddress[5]);

   snprintf(ServerInfo, sizeof(ServerInfo),
            "Trinity/%d.%d UPnP/1.0", 0, 0);
   
   /* Create Rx/Tx socket */
   RxSock = socket(AF_INET, SOCK_DGRAM, 0);
   TAL_ASSERT(RxSock != SOCKET_ERROR);
   
   TxSock = socket(AF_INET, SOCK_DGRAM, 0);
   TAL_ASSERT(TxSock != SOCKET_ERROR);
   
   /*
    * First send ByeBye to the network.
    */
   SendByeBye(Buffer, SSDP_BUFFER_SIZE, TxSock);
   
   /*
    * Join the UPnP multicast group
    */ 
   if ((RxSock != SOCKET_ERROR) && (TxSock != SOCKET_ERROR))
   {
      /* Join the UPnP multicast group */
      mreq.imr_multiaddr.s_addr = SSDP_IP;
      mreq.imr_interface.s_addr = htonl(MyIPHost);

      term_printf("Add UPnP multicast address...");
      Err = setsockopt(RxSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
      if (Err != 0) { term_printf("failed\n"); } else { term_printf("OK\n"); }
 
      TAL_ASSERT(0 == Err);
      
      if (0 == Err)
      {
         Server.sin_addr.s_addr = INADDR_ANY;
         Server.sin_port        = htons(SSDP_PORT);
         Server.sin_family      = AF_INET;

         /* Assign a name (port) to an unnamed socket */
         Err = bind(RxSock, (struct sockaddr *)&Server, sizeof(Server));
         TAL_ASSERT(0 == Err);
      }
   }
   else
   {
      TAL_FAILED();
   }
   
   /* Start Notify task */
   OSTaskCreate(&TCBNotify, NotifyTask, NULL, TASK_IP_SSDP_NOTIFY_PRIORITY, 
                NotifyStack, sizeof(NotifyStack), 
                "ssdp-notify"); 
   
   
   /* Wait some time between "ByeBye" and "Hello" */
   OSTimeDly(500);
   
   /* Now we can send a "Hello" to the network, we are back. */
   SendNotifyAlive(Buffer, SSDP_BUFFER_SIZE, TxSock);
   
   while(1)
   {
      SourceLen = sizeof(Source);
      Size = recvfrom(RxSock, Buffer, SSDP_BUFFER_SIZE, 0,
                      (struct sockaddr *)&Source, (socklen_t*)&SourceLen);
      if (Size > 0) 
      {
         Buffer[Size] = 0;   /* Terminate string */

         /*
          * Check if a control point serach for a network device.
          */
         if ((strstr(Buffer, "M-SEARCH") != NULL) &&        /* <= REQUIRED */
             (strstr(Buffer, "ssdp:discover") != NULL)) {   /* <= REQUIRED */
             
            /*
             * Check if the control point search for "all", "root devices"
             * or for a special device.
             */
            if ((strstr(Buffer, "ssdp:all") != NULL) ||
                (strstr(Buffer, "upnp:rootdevice") != NULL) || (strstr(Buffer, MyUSN) != NULL)) 
            {
               Start = strstr(Buffer, "MX:");
               if (Start != NULL) 
               {
                  /* Jump over the "MX:" */
                  Start += 3;

                  /* Get delay in seconds */
                  MaxDelay = strtol(Start, &End, 10);
                  if (MaxDelay == 0) 
                  {
                     MaxDelay = 1;
                  }

                  /* Change to millisecond */
                  MaxDelay *= 1000;

                  /* Get random value */
                  Delay = GetRand(MaxDelay);
               } 
               else 
               {
                  /* Ups, error */
                  Delay = 1234;
               }

               OSTimeDly(Delay);

               /*
                * We must send a M-SEARCH response
                */
               Size = sprintf(Buffer, MSEARCH_RESPONSE,
                              MyIPStr,
                              ServerInfo, MyUSN);
                              
               /* Send first M-SEARCH response */
               Err = sendto(TxSock, Buffer, Size, 0, (struct sockaddr *)&Source, sizeof(Source));
               TAL_ASSERT(Err == Size);
               
               OSTimeDly(25);
               
               /* Send second M-SEARCH response */
               Err = sendto(TxSock, Buffer, Size, 0, (struct sockaddr *)&Source, sizeof(Source));
               TAL_ASSERT(Err == Size);
            }
         }
      }
   }

} /* SSDPTask */  

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  IPUpnpStart                                                          */
/*                                                                       */
/*  Start the UPnP tasks.                                                */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IPUpnpStart (void)
{
   HttpRegisterCgiFunction("cgi-bin/upnp.cgi", UPnPCGIHandler);
   HttpRegisterCgiFunction("cgi-bin/upnp/dummy.cgi", UPnPCGIDummyHandler);
   
   OSSemaCreate(&UpdateSema, 0, 1);

   OSTaskCreate(&TCBSSDP, SSDPTask, NULL, TASK_IP_SSDP_PRIORITY, 
                SSDPStack, sizeof(SSDPStack), 
                "ssdp"); 
                    
} /* IPUpnpStart */

/*************************************************************************/
/*  IPUpnpIsStarted                                                      */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 0 not in used / 1 otherwise                                  */
/*************************************************************************/
int IPUpnpIsStarted (void)
{
   return(UpnpInUse);
} /* IPUpnpIsStarted */

/*************************************************************************/
/*  IPUpnpAddrUpdate                                                     */
/*                                                                       */
/*  This is a trigger to update the IP address.                          */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IPUpnpAddrUpdate (void)
{
   OSSemaSignal(&UpdateSema);
} /* IPUpnpAddrUpdate */

/*** EOF ***/



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
#define __IP_SNTPSERVER_C__

/*=======================================================================*/
/*  Include                                                              */
/*=======================================================================*/
#include <string.h>
#include "tal.h"
#include "ipstack.h"

#if (IP_DEFAULT_OPT_SNTPS >= 1)

#include "terminal.h"
#include "ipweb.h"
#include "nvm.h"

/*=======================================================================*/
/*  All extern definitions                                               */
/*=======================================================================*/

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

/*
 * Some external swithes are not ready to use if the link
 * is available. Therefore wait a short time.
 */
#define DELAY_AFTER_LINK_MS   2000

/* NTP Packet Header, see RFC4330, page 9 */
typedef struct __attribute__ ((__packed__)) _sntp_packet_
{
   uint8_t  bLiVnMode;
   uint8_t  bStratum;
   uint8_t  bPoll;
   uint8_t  bPrecision;
   uint32_t dRootDelay;
   uint32_t dRootDispersion;
   uint32_t dReferenceIdentifier;
   uint32_t dReferenceTs;
   uint32_t dReferenceTsFrac;
   uint32_t dOriginateTs;
   uint32_t dOriginateTsFrac;
   uint32_t dReceiveTs;
   uint32_t dReceiveTsFrac;
   uint32_t dTransmitTs;
   uint32_t dTransmitFrac;
} SNTP_PACKET;

/* NTP/SNTP server port */
#define SNTP_PORT          123

/*
 * NTP starts by 01.01.1900 and Unixtime by 01.01.1970.
 * Here we have an offset of 70 years, this must be removed
 * from dTime to get the Unixtime in UTC.
 *
 * Between 1900 and 1970, we have 17 leap years =>
 *
 *                  days              sec per day
 * offset = (((70ul*365ul)+17ul) * (24ul*60ul*60ul))
 */
#define NTP_OFFSET         ((uint32_t)(((70ul*365ul)+17ul) * (24ul*60ul*60ul)))

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

/* 
 * Some TASK variables like stack and task control block.
 */
static OS_STACK (ServerStack, TASK_IP_SNTP_SERVER_STK_SIZE);

static OS_TCB TCBServer;

static int SntpInUse = 0;

static SNTP_PACKET PacketIn;
static SNTP_PACKET PacketOut;


/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  sys_ntps                                                             */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_ntps_chk (HTTPD_SESSION *hs)
{
   if (1 == SntpInUse)
   {
      s_printf(hs->s_stream, "1");
   }
   else
   {
      s_printf(hs->s_stream, "0");
   }      

   s_flush(hs->s_stream);

   return(0);
} /* sys_ntps */

/*
 * SSI variable list
 */
static const SSI_EXT_LIST_ENTRY SSIList[] =
{
   { "sys_ntps_chk",    sys_ntps_chk },
   
   {NULL, NULL}
};

/*************************************************************************/
/*  HandleSNTPRequest                                                    */
/*                                                                       */
/*  In    : Sock, Size, pSource, pDataIn                                 */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void HandleSNTPRequest (int Sock, int Size, struct sockaddr *pSource, SNTP_PACKET *pIn)
{
   SNTP_PACKET *pOut    = &PacketOut;
   uint32_t     dOffset = NTP_OFFSET;

   (void)pIn;
   (void)Size;

   /* Clear data */
   memset(pOut, 0x00, sizeof(SNTP_PACKET));

   /* Set data */
   pOut->bLiVnMode   = 0x1C;
   pOut->bStratum    = 0x02;
   pOut->dTransmitTs = htonl(OS_UnixtimeGet() + dOffset);

   sendto(Sock, pOut, (size_t)sizeof(SNTP_PACKET), 0, (struct sockaddr *)pSource, sizeof(struct sockaddr)); /*lint !e740*/

} /* HandleSNTPRequest */

/*************************************************************************/
/*  SNTPServerTask                                                       */
/*                                                                       */
/*  This is the SNTP server task.                                        */
/*                                                                       */
/*  In    : arg                                                          */
/*  Out   : none                                                         */
/*  Return: never                                                        */
/*************************************************************************/
static void SNTPServerTask (void *arg)
{
   int                Err;
   int                Size;
   int                Socket;
   struct sockaddr_in Server;
   struct sockaddr_in Source;
   int                SourceLen;  

   (void)arg;

   /* Wait that the IP interface is ready for use */
   while(0 == IP_IF_IsReady(IFACE_ANY))
   {
      OS_TimeDly(500);
   }

   /* Wait some time for the external switch */
   OS_TimeDly(DELAY_AFTER_LINK_MS);
   
   TAL_DEBUG(TAL_DBG_SNTPs, "SNTPs: Server started...");


   /* Create socket */
   Socket = socket(AF_INET, SOCK_DGRAM, 0);
   TAL_ASSERT(Socket != SOCKET_ERROR);

#if 0   
   /* Set receive timeout */   
   Value = SOCKET_RCVTIMEO_MS;
   Err = setsockopt(Socket, SOL_SOCKET, SO_RCVTIMEO, &Value, sizeof(Value));
   TAL_ASSERT(0 == Err);
#endif

   /* Assign a name (port) to an unnamed socket */
   Server.sin_addr.s_addr = INADDR_ANY;
   Server.sin_port        = htons(SNTP_PORT);
   Server.sin_family      = AF_INET;

   Err = bind(Socket, (struct sockaddr *)&Server, sizeof(Server)); /*lint !e740*/
   TAL_ASSERT(0 == Err);


   while(1)
   {
      SourceLen = sizeof(Source);
      Size = recvfrom(Socket, (uint8_t*)&PacketIn, sizeof(PacketIn), 0,
                      (struct sockaddr *)&Source, (socklen_t*)&SourceLen); /*lint !e740*/

      if (Size == (int)sizeof(SNTP_PACKET))
      {
         HandleSNTPRequest(Socket, Size, (struct sockaddr *)&Source, &PacketIn); /*lint !e740*/
         TAL_DEBUG(TAL_DBG_SNTPs, "SNTPs: Handle request");
      }

   }

} /* SNTPServerTask */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  IP_SNTP_ServerInit                                                   */
/*                                                                       */
/*  Initialize the SNTP server.                                          */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_SNTP_ServerInit (void)
{
   IP_WEBS_SSIListAdd((SSI_EXT_LIST_ENTRY*)SSIList);
   
} /* IP_SNTP_ServerInit */

/*************************************************************************/
/*  IP_SNTP_ServerStart                                                  */
/*                                                                       */
/*  Start the SNTP server.                                               */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_SNTP_ServerStart (void)
{
   if (0 == SntpInUse)
   {
      SntpInUse = 1;                  

      /* Create the SNTP Server task */
      OS_TaskCreate(&TCBServer, SNTPServerTask, NULL, TASK_IP_SNTP_SERVER_PRIORITY,
                    ServerStack, TASK_IP_SNTP_SERVER_STK_SIZE, "SNTPServer");
   }   
   
} /* IP_SNTP_ServerStart */

/*************************************************************************/
/*  IP_SNTP_ServerIsStarted                                              */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 0 not started / 1 otherwise                                  */
/*************************************************************************/
int IP_SNTP_ServerIsStarted (void)
{
   return(SntpInUse);
} /* IP_SNTP_ServerIsStarted */

#endif /* (IP_DEFAULT_OPT_SNTPS >= 1) */

/*** EOF ***/

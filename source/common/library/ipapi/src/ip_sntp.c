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
*  08.02.2014  mifi  First Version, reference is RFC1361 and RFC4330.
*  06.06.2020  mifi  Rename API.
**************************************************************************/
#define __IP_SNTP_C__

/*=======================================================================*/
/*  Include                                                              */
/*=======================================================================*/
#include <string.h>
#include <stdlib.h>
#include "tal.h"
#include "ipstack.h"

#if (IP_DEFAULT_OPT_SNTP >= 1)
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

#define TASK_DELAY_MS         500

#define SNTP_PORT    123

/* See RFC1361, page 4 */
typedef struct __attribute__((__packed__)) _sntp_message_header_
{
   uint8_t  LiVnMode;
   uint8_t  Stratum;   
   uint8_t  Poll;
   uint8_t  Precision;
   uint32_t RootDelay;
   uint32_t RootDispersion;
   uint32_t ReferenceIdentifier;
   uint32_t ReferenceTsSec;
   uint32_t ReferenceTsFrac;
   uint32_t OriginateTsSec;
   uint32_t OriginateTsFrac;
   uint32_t ReceiveTsSec;
   uint32_t ReceiveTsFrac;
   uint32_t TransmitTsSec;
   uint32_t TransmitTsFrac;
// uint32_t Authenticator[3];  /* Not used, see note */
} sntp_message_header_t;

/*
 * Note: Authenticator (optional): When the NTP authentication mechanism is
 *       implemented, this contains the authenticator information defined in
 *       Appendix C of RFC-1305. In SNTP this field is ignored for incoming
 *       messages and is not generated for outgoing messages.
 */

#define SNTP_SERVER_NAME_LEN  32


/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

/* 
 * Some TASK variables like stack and task control block.
 */
static OS_STACK (ClientStack, TASK_IP_SNTP_CLIENT_STK_SIZE);

static OS_TCB TCBClient;

static int SntpInUse = 0;

static uint32_t dManualSNTPAddr = 0;
static uint32_t dSNTPRefresh    = 0;
static uint32_t dSyncRequest    = 0;
static uint32_t dSNTPNextSync   = 0;

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  TimeGet                                                              */
/*                                                                       */
/*  Request the unixtime from the given server.                          */
/*                                                                       */
/*  In    : server, *unixtime, timeout                                   */
/*  Out   : none                                                         */
/*  Return: 0 on success / -1 otherwise / -2 in case of "KoD"            */
/*************************************************************************/
static int TimeGet (uint32_t addr, uint32_t *unixtime, uint32_t timeout)
{
   int                    rc = -1;
   sntp_message_header_t *sntp = NULL;
   int                    sock = SOCKET_ERROR;
   uint16_t               bufsize = 256;   
   struct sockaddr_in     dest;
   struct sockaddr_in     source;
   int                    source_len;  
   int                    size;
   uint32_t               sntp_time;
   
   
   /* Check parameters */
   if ((0 == addr) || (NULL == unixtime) || (0 == timeout))
   {
      goto IPSntpTimeGetEnd;  /*lint !e801*/
   }

   /* Get sntp message memory */
   sntp = xmalloc(XM_ID_IP, sizeof(sntp_message_header_t));
   if (NULL == sntp)
   {
      goto IPSntpTimeGetEnd;  /*lint !e801*/
   }
   
   /*
    * 4. SNTP Client Operations
    *
    * The model for an SNTP client operating with either an NTP or SNTP
    * server is a RPC client with no persistent state. The client
    * initializes the SNTP message header, sends the message to the server
    * and strips the time of day from the reply. For this purpose all of
    * the message-header fields shown above are set to zero, except the
    * first octet. In this octet the Leap Indicator is set to zero (no
    * warning) and the Mode to 3 (client). The Version Number must agree
    * with the software version of the NTP or SNTP server.
    */
   memset(sntp, 0x00, sizeof(sntp_message_header_t));   
   
   /* Li = 0, Vn = 3, Mode = 3 */
   sntp->LiVnMode = 0x1B;
   
   /* Create the  socket */
   sock = socket(AF_INET, SOCK_DGRAM, 0);
   if (SOCKET_ERROR == sock)
   {
      goto IPSntpTimeGetEnd;  /*lint !e801*/
   }

   /* Set socket rx buffer size */
   setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &bufsize, sizeof(bufsize));
   
   /* Set destination */
   dest.sin_family      = AF_INET;
   dest.sin_port        = htons(SNTP_PORT);
   dest.sin_addr.s_addr = htonl(addr);
   
   /* Send sntp request */
   rc = sendto(sock, sntp, sizeof(sntp_message_header_t), 0, 
               (struct sockaddr *)&dest, sizeof(dest));  /*lint !e740*/
   if (SOCKET_ERROR == rc)
   {
      goto IPSntpTimeGetEnd;  /*lint !e801*/
   }

   /* Set rx timeout */
   setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

   /* Get SNTP server response, with timeout from above */
   source_len = sizeof(source);
   size = recvfrom(sock, sntp, sizeof(sntp_message_header_t), 0,
                   (struct sockaddr *)&source, (socklen_t*)&source_len);   /*lint !e740*/
   if (size <= 0)
   {
      /* Error or timeout */
      goto IPSntpTimeGetEnd;  /*lint !e801*/
   }   
   
   
   /*
    * Check the requirement of RFC4330, page 15, point 1 to 5
    *
    * *** ToDo ***
    */    
   
   /*
    * Check for Kiss-of-Death (KoD), RFC4330
    */
   if (0 == sntp->Stratum)
   {
      rc = -2;
      goto IPSntpTimeGetEnd;  /*lint !e801*/
   }
      
   /* 
    * Check Leap Indicator (LI) and port. 
    */
   if( ((sntp->LiVnMode & 0xc0) == 0xc0)     || /* <= check if server is not synchronized */
       (source.sin_port != htons(SNTP_PORT)) )  /* <= check port */ 
   {
      goto IPSntpTimeGetEnd;  /*lint !e801*/
   }       
   
   /*
    * SNTP time starts at 1.1.1900, but Unixtime at 1.1.1970.
    * Therefore an offset of 70 years must be subtracted. In these
    * 70 years, there was 17 leap years, the calculation looks like:
    *
    *        days              seconds in a day 
    *  ((70u * 365u) + 17u) * (24u * 60u * 60u)
    */
   sntp_time  = ntohl(sntp->TransmitTsSec);
   sntp_time -= ((70u * 365u) + 17u) * (24u * 60u * 60u);
   
   /* sntp_time is now the correct unixtime, based on 1.1.1970 */
   *unixtime  = sntp_time;

   /* No error */   
   rc = 0;   

IPSntpTimeGetEnd:

   /* Close socket if available */
   if (sock != SOCKET_ERROR)
   {
      closesocket(sock);
   }
      
   /* Free sntp message memory if available */
   if (sntp != NULL)
   {
      xfree(sntp);
   }

   return(rc);   
} /* TimeGet */

/*************************************************************************/
/*  SNTPClientTask                                                       */
/*                                                                       */
/*  This is the SNTP client task.                                        */
/*                                                                       */
/*  In    : arg                                                          */
/*  Out   : none                                                         */
/*  Return: never                                                        */
/*************************************************************************/
static void SNTPClientTask (void *arg)
{
   int      nErr;
   uint32_t dAddress;
   uint32_t dUnixtime;
   uint32_t dRefreshCounterMax;
   uint32_t dRefreshCounter;

   (void)arg;

   /* Wait that the IP interface is ready for use */
   while(0 == IP_IF_IsReady(IFACE_ANY))
   {
      OS_TimeDly(500);
   }

   /* Wait some time for the external switch */
   OS_TimeDly(DELAY_AFTER_LINK_MS);
   
   TAL_DEBUG(TAL_DBG_SNTP, "SNTP: Client started...");

   /* Calculate refresh counter max */
   dRefreshCounterMax = (dSNTPRefresh * 60 * 1000) / TASK_DELAY_MS;
   dRefreshCounter    = dRefreshCounterMax;

   while(1)
   {
      /*
       * Handle refresh counter
       */
      if (dRefreshCounter > 0)
      {
         dRefreshCounter--;
         if (0 == dRefreshCounter)
         {
            dSyncRequest = 1;
         }
         
         dSNTPNextSync = (dRefreshCounter * TASK_DELAY_MS) / 1000;
      }
      
      /*
       * Check if we must sync the time 
       */
      if (dSyncRequest != 0)
      {
         /* Calculate refresh counter max, dSNTPRefresh could be changed */
         dRefreshCounterMax = (dSNTPRefresh * 60 * 1000) / TASK_DELAY_MS;

         /* Set new values */      
         dSyncRequest    = 0;
         dRefreshCounter = dRefreshCounterMax;   

         TAL_PRINTF("SNTP: Client sync...");

         dAddress = IP_SNTP_ServerGet();
         if (dAddress != 0)
         {
            dUnixtime = 0; /* Remove compiler warning */
            nErr = TimeGet(dAddress, &dUnixtime, 3000);
            if (0 == nErr)
            {
               OS_UnixtimeSet(dUnixtime);
               tal_BoardRTCSetUnixtime(dUnixtime);
               TAL_PRINTF("OK\r\n");
            }
            else
            {
               TAL_PRINTF("Error\r\n");
            }
         }
         else
         {
            TAL_PRINTF("error, no address\r\n");
         }            
      }

      OS_TimeDly(TASK_DELAY_MS);
   }

} /* SNTPClientTask */


/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  IP_SNTP_Start                                                        */
/*                                                                       */
/*  Start the SNTP client of the network interface.                      */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_SNTP_Start (void)
{
   if (0 == SntpInUse)
   {
      SntpInUse = 1;
      
      /* Create the SNTP Client task */
      OS_TaskCreate(&TCBClient, SNTPClientTask, NULL, TASK_IP_SNTP_CLIENT_PRIORITY,
                    ClientStack, TASK_IP_SNTP_CLIENT_STK_SIZE, "SNTPClient");
   }   
   
} /* IP_SNTP_Start */

/*************************************************************************/
/*  IP_SNTP_IsStarted                                                    */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 0 not started / 1 otherwise                                  */
/*************************************************************************/
int IP_SNTP_IsStarted (void)
{
   return(SntpInUse);
} /* IP_SNTP_IsStarted */

/*************************************************************************/
/*  IP_SNTP_TimeGet                                                      */
/*                                                                       */
/*  Request the unixtime from the SNTP server.                           */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 0 on success / -1 otherwise / -2 in case of "KoD"            */
/*************************************************************************/
int IP_SNTP_TimeGet (void)
{
   int      rc = -1;
   uint32_t unixtime;
   
   TAL_PRINTF("Get time by SNTP...");

   if (dManualSNTPAddr != 0)
   {
      rc = TimeGet(dManualSNTPAddr, &unixtime, 5000);
      if (0 == rc)
      {
         OS_UnixtimeSet(unixtime);
         tal_BoardRTCSetUnixtime(unixtime);
      }
   }
   
   if (0 == rc)
   {
      TAL_PRINTF("OK\n");
   }
   else
   {
      TAL_PRINTF("Error\n");
   }   

   return(rc);   
} /* IP_SNTP_TimeGet */

/*************************************************************************/
/*  IP_SNTP_TimeGetEx                                                    */
/*                                                                       */
/*  Request the unixtime from the given server.                          */
/*                                                                       */
/*  In    : server, *unixtime, timeout                                   */
/*  Out   : none                                                         */
/*  Return: 0 on success / -1 otherwise / -2 in case of "KoD"            */
/*************************************************************************/
int IP_SNTP_TimeGetEx (uint32_t addr, uint32_t *unixtime, uint32_t timeout)
{
   return( TimeGet(addr, unixtime, timeout) );
} /* IP_SNTP_TimeGetEx */

/*************************************************************************/
/*  IP_SNTP_TimeSync                                                     */
/*                                                                       */
/*  Request a time synchronisation.                                      */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_SNTP_TimeSync (void)
{
   dSyncRequest = 1;

} /* IP_SNTP_TimeSync */

/*************************************************************************/
/*  IP_SNTP_ServerSet                                                    */
/*                                                                       */
/*  Set the SNTP address of the network interface in host order.         */
/*                                                                       */
/*  In    : *server                                                      */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_SNTP_ServerSet (uint32_t addr)
{
   dManualSNTPAddr = addr;
} /* IP_SNTP_ServerSet */

/*************************************************************************/
/*  IP_SNTP_ServerGet                                                    */
/*                                                                       */
/*  Get the SNTP address of the network interface in host order.         */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: IP Address                                                   */
/*************************************************************************/
uint32_t IP_SNTP_ServerGet (void)
{
   uint32_t Addr = 0;

   if (1 == IP_DHCP_IsStarted(0))
   {
      Addr = IP_DHCP_NTPServerGet(0);
      if (0 == Addr)
      {
         Addr = dManualSNTPAddr;
      }
   }
   else
   {
      Addr = dManualSNTPAddr;
   }

   return(Addr);
} /* IP_SNTP_ServerGet */

/*************************************************************************/
/*  IP_SNTP_RefreshSet                                                   */
/*                                                                       */
/*  Set the SNTP refresh time.                                           */
/*                                                                       */
/*  In    : refresh                                                      */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_SNTP_RefreshSet (uint32_t refresh)
{
   dSyncRequest = 1;
   dSNTPRefresh = refresh;

} /* IP_SNTP_RefreshSet */

/*************************************************************************/
/*  IP_SNTP_RefreshGet                                                   */
/*                                                                       */
/*  Return the SNTP refresh time.                                        */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: SNTPRefresh                                                  */
/*************************************************************************/
uint32_t IP_SNTP_RefreshGet (void)
{
   return(dSNTPRefresh);
} /* IP_SNTP_RefreshGet */

/*************************************************************************/
/*  IP_SNTP_NextSyncGet                                                  */
/*                                                                       */
/*  Return the SNTP "NextSync" time.                                     */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: dSNTPNextSync                                                */
/*************************************************************************/
uint32_t IP_SNTP_NextSyncGet (void)
{
   return(dSNTPNextSync);
} /* IP_SNTP_NextSyncGet */

#else

uint32_t IP_SNTP_ServerGet (void)
{
   return(0);
} /* IP_SNTP_ServerGet */

#endif /* (IP_DEFAULT_OPT_SNTP >= 1) */

/*** EOF ***/

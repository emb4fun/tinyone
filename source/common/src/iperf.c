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
*  18.06.2013  mifi  First Version, tested with a STM3240G-EVAL board.
*  19.08.2013  mifi  Some rework and cleanup.
*  21.08.2013  mifi  Changed for ChibiOS.
*  18.04.2014  mifi  Client threads will be created depending of
*                    MAX_IPERF_CLIENT.
*  04.01.2016  mifi  Changed for TinyCTS.
**************************************************************************/
#define __IPERF_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/
#include <stdio.h>
#include <string.h>

#include "tal.h"
#include "project.h"
#include "ipstack.h"

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

#define IPERF_TCP_PORT     5001

#define MAX_IPERF_CLIENT   4

typedef struct _client_info_
{
   OS_TCB    TCB;
   uint8_t  *Stack;
   uint16_t  StackSize;
   int       Sock;
} client_info_t;

/*=======================================================================*/
/*  Definition of all global Data                                        */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

/* 
 * Stack and task definition.
 */
static OS_STACK(IperfServerStack, TASK_IP_IPERF_SERVER_STK_SIZE);

static OS_TCB        TCBIperfServer;

static uint8_t       DummyRxBuffer[4096];
static client_info_t ClientArray[MAX_IPERF_CLIENT];
static uint64_t      ClientStack[MAX_IPERF_CLIENT][TASK_IP_IPERF_CLIENT_STK_SIZE/8];

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  FindFreeClient                                                       */
/*                                                                       */
/*  In    : task parameter                                               */
/*  Out   : none                                                         */
/*  Return: never                                                        */
/*************************************************************************/
static client_info_t *FindFreeClient (void)
{
   client_info_t *Client = NULL;
   
   for (int i=0; i<MAX_IPERF_CLIENT; i++)
   {
      if (OS_TASK_STATE_NOT_IN_USE == ClientArray[i].TCB.State)
      {
         Client = &ClientArray[i];
         break;
      }
   }
   
   return(Client);
} /* FindFreeClient */

/*************************************************************************/
/*  IperfClient                                                          */
/*                                                                       */
/*  In    : task parameter                                               */
/*  Out   : none                                                         */
/*  Return: never                                                        */
/*************************************************************************/
static void IperfClient (void *arg)
{
   int32_t        Sock;
   int            BytesReceived;  
   client_info_t *Client = (client_info_t*)arg;
   
   Sock = Client->Sock;
   
   do 
   {
      BytesReceived = recv(Sock, DummyRxBuffer, sizeof(DummyRxBuffer), 0);
      if (BytesReceived <= 0)
      {
         break;
      }
   } while (1);   /*lint !e506*/

   shutdown(Sock, SHUT_RDWR);
   closesocket(Sock); 
   
} /* IperfClient */

/*************************************************************************/
/*  IperfServer                                                          */
/*                                                                       */
/*  In    : task parameter                                               */
/*  Out   : none                                                         */
/*  Return: never                                                        */
/*************************************************************************/
static void IperfServer (void *p)
{
   int                  ServerSocket;
   int                  Sock;
   struct sockaddr_in   Server;
   struct sockaddr      InAddr;
   socklen_t            InAddrSize;
   client_info_t       *Client = NULL;
   
   (void)p;
   
   while(!IP_IF_IsReady(IFACE_ANY))
   {
      OS_TimeDly(500);
   }
   
   /* Create the IPerf server socket */
   ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
   
   /* Set address and port for the socket */
   memset(&Server, 0x00, sizeof(Server));
   Server.sin_addr.s_addr = INADDR_ANY;
   Server.sin_port        = htons(IPERF_TCP_PORT);
   Server.sin_family      = AF_INET;
   
   /* Assign a name (port) to an unnamed socket */
   bind(ServerSocket, (const struct sockaddr *)&Server, sizeof(Server));
   
   listen(ServerSocket, MAX_IPERF_CLIENT);
   
   while(1)
   {
      InAddrSize = sizeof(InAddr);
      if ((Sock = accept(ServerSocket, &InAddr, &InAddrSize)) == -1)
      {
         continue;   /* Error */
      }

      Client = FindFreeClient();
      if (Client != NULL)
      {
         /* Create the Iperf client task */
         Client->Sock = Sock;

         OS_TaskCreate(&Client->TCB, IperfClient, (void*)Client, (TASK_IP_IPERF_PRIORITY+1),
                       Client->Stack, Client->StackSize, 
                       "IperfClient");
      }
      else
      {
         closesocket(Sock);
      }
   }

} /* IperfServer */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  iperf_Start                                                          */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void iperf_Start (void)
{

   memset(ClientArray, 0x00, sizeof(ClientArray));

   /* Create stack */   
   for(int i=0; i<MAX_IPERF_CLIENT; i++)
   {
      ClientArray[i].Stack     = (uint8_t*)&ClientStack[i][0];
      ClientArray[i].StackSize = TASK_IP_IPERF_CLIENT_STK_SIZE;
   }

   /* Create the IPerf task */
   OS_TaskCreate(&TCBIperfServer, IperfServer, NULL, TASK_IP_IPERF_PRIORITY,
                 IperfServerStack, sizeof(IperfServerStack), 
                 "IperfServer");
                
} /* iperf_Start */

/*** EOF ***/



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
*  24.12.2018  mifi  First Version.
**************************************************************************/
#define __WEB_SSL_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/

#include <string.h>
#include <stdint.h>

#include "tal.h"
#include "ipstack.h"
#include "ipweb.h"
#include "cert.h"

#include "lwip\api.h"
#include "lwip\priv\sockets_priv.h"

#include "mbedtls/config.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/x509.h"
#include "mbedtls/ssl.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl_cache.h"

#if !defined(IP_WEB_TLS_MAX_HTTP_TASKS) 
#define _MAX_WEB_TLS_CLIENT_TASKS   4
#else
#define _MAX_WEB_TLS_CLIENT_TASKS   IP_WEB_TLS_MAX_HTTP_TASKS
#endif

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

static const char               *pers = "ssl_server";
static mbedtls_x509_crt          srvcert;
static mbedtls_pk_context        pkey;
static mbedtls_ctr_drbg_context  ctr_drbg;
static mbedtls_entropy_context   entropy;
static mbedtls_ssl_config        conf;
static mbedtls_ssl_cache_context cache;

/*************************************************************************/

/*
 * Some external swithes are not ready to use if the link
 * is available. Therefore wait a short time.
 */
#define DELAY_AFTER_LINK_MS   2000

typedef struct _CLIENT_THREAD_PARAM 
{
   HTTP_STREAM        *ctp_stream;
   HTTP_CLIENT_HANDLER ctp_handler;
} CLIENT_THREAD_PARAM;

typedef struct _client_tls_info_
{
   OS_TCB               TCB;
   uint8_t             *Stack;
   uint16_t             StackSize;
   int                  Sock;
   CLIENT_THREAD_PARAM *ctp;  
   mbedtls_ssl_context  ssl;
} client_tls_info_t;

/*=======================================================================*/
/*  Definition of all global Data                                        */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all external Data                                      */
/*=======================================================================*/

extern int gRedirectHTTPtoHTTPS;

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

/* 
 * Stack and TCB definition
 */
static OS_STACK (ServerStack, TASK_IP_WEB_TLS_SERVER_STK_SIZE); 
static OS_TCB TCBServerTask;

static client_tls_info_t   ClientArray[_MAX_WEB_TLS_CLIENT_TASKS];
static uint64_t            ClientStack[_MAX_WEB_TLS_CLIENT_TASKS][TASK_IP_WEB_TLS_CLIENT_STK_SIZE/8];

static int       nNumThreads = 0;
static int       nWebsTlsRunning = 0;
static uint16_t  wServerPort;

int nNumThreadsMaxTLS = 0;

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

static void my_debug( void *ctx, int level,
                      const char *file, int line,
                      const char *str )
{
   (void)ctx;
   (void)level;

   term_printf("%s:%04d: %s", file, line, str );
}


int mbedtls_net_recv (void *ctx, unsigned char *buf, size_t len)
{
   int rc;
   int sock = (int)ctx;
   
   rc = recv(sock, buf, len, 0);
   if (rc < 0)
   {
      rc = MBEDTLS_ERR_NET_RECV_FAILED;
   }
   
   if (0 == rc)
   {
      rc = MBEDTLS_ERR_SSL_WANT_READ;
   }
   
   return(rc);   
} /* mbedtls_net_recv */

int mbedtls_net_send (void *ctx, const unsigned char *buf, size_t len)
{
   int rc;
   int sock = (int)ctx;
   
   rc = send(sock, buf, len, 0);
   if (0 == rc)
   {
      rc = MBEDTLS_ERR_SSL_WANT_WRITE;
   }
   
   if (rc < 0)
   {
      rc = MBEDTLS_ERR_NET_SEND_FAILED;
   }
   
   return(rc);   
} /* mbedtls_net_send */

/*************************************************************************/
/*  InitTls                                                              */
/*                                                                       */
/*  Initialize the TLS functionslity.                                    */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 0 == OK / error cause                                        */
/*************************************************************************/
static int InitTls (void)
{
   int    rc;
   char  *buf;
   size_t buflen;

   mbedtls_x509_crt_init(&srvcert);
   mbedtls_pk_init(&pkey);
   mbedtls_ctr_drbg_init(&ctr_drbg);
   mbedtls_entropy_init(&entropy);
   mbedtls_ssl_config_init(&conf);
   mbedtls_ssl_cache_init(&cache);
   
   /*
    * 1. Load the certificates and private RSA key
    */
    
   /* Device certificate */ 
   cert_Get_DeviceCert(&buf, &buflen);
   rc = mbedtls_x509_crt_parse(&srvcert, (const unsigned char *) buf, buflen);
   if(rc != 0) goto exit; /*lint !e801*/

   /* Root or Intermediate certificate */
   cert_Get_IntermediateCert(&buf, &buflen);
   rc = mbedtls_x509_crt_parse(&srvcert, (const unsigned char *) buf, buflen);
   if(rc != 0) goto exit; /*lint !e801*/

   /* Device private key */
   cert_Get_DeviceKey(&buf, &buflen);
   rc = mbedtls_pk_parse_key(&pkey, (const unsigned char *) buf, buflen, NULL, 0);
   if(rc != 0) goto exit; /*lint !e801*/


   /*
    * 2. Seed the RNG
    */
   rc = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)pers, strlen(pers));
   if(rc != 0) goto exit; /*lint !e801*/


   /*
    * 3. Setup stuff
    */
   rc = mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_SERVER, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT);
   if(rc != 0) goto exit; /*lint !e801*/

//   rc = mbedtls_ssl_conf_max_frag_len(&conf, MBEDTLS_SSL_MAX_FRAG_LEN_4096);
//   if(rc != 0) goto exit; /*lint !e801*/

   mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);
   
   //mbedtls_ssl_conf_dbg(&conf, my_debug, 0);
   //mbedtls_debug_set_threshold( 2 );
   
   mbedtls_ssl_conf_session_cache(&conf, &cache, mbedtls_ssl_cache_get, mbedtls_ssl_cache_set);

   mbedtls_ssl_conf_ca_chain(&conf, srvcert.next, NULL);

   rc = mbedtls_ssl_conf_own_cert(&conf, &srvcert, &pkey);
   if(rc != 0) goto exit; /*lint !e801*/

exit:   
   return(rc);
} /* InitTls */

/*************************************************************************/
/*  FindFreeClient                                                       */
/*                                                                       */
/*  In    : task parameter                                               */
/*  Out   : none                                                         */
/*  Return: never                                                        */
/*************************************************************************/
static client_tls_info_t *FindFreeClient (void)
{
   client_tls_info_t *Client = NULL;
   
   for (int i=0; i<_MAX_WEB_TLS_CLIENT_TASKS; i++)
   {
      if ( OS_TEST_STATE_NOT_IN_USED(&ClientArray[i].TCB) )
      {
         Client = &ClientArray[i];
         break;
      }
   }
   
   return(Client);
} /* FindFreeClient */

/*************************************************************************/
/*  WebClientTls                                                         */
/*                                                                       */
/*  In    : task parameter                                               */
/*  Out   : none                                                         */
/*  Return: never                                                        */
/*************************************************************************/
static void WebClientTls (void *p)
{
   int                  ret;
   client_tls_info_t   *Client = (client_tls_info_t*)p;
   CLIENT_THREAD_PARAM *ctp    = (CLIENT_THREAD_PARAM *)Client->ctp;
   
   /* Prepare SSL context */
   mbedtls_ssl_init(&Client->ssl);
   mbedtls_ssl_setup(&Client->ssl, &conf);
   mbedtls_ssl_set_bio(&Client->ssl, (void*)Client->Sock, mbedtls_net_send, mbedtls_net_recv, NULL);
   
   /* SSL handshake */
   ret = mbedtls_ssl_handshake(&Client->ssl);
   if (ret != 0)
   {
      goto exit;  /*lint !e801*/
   }

   /* Client handler */   
   (*ctp->ctp_handler)(ctp->ctp_stream);

   /* Close SSL connection */
   while((ret = mbedtls_ssl_close_notify(&Client->ssl)) < 0)
   {
      if ((ret != MBEDTLS_ERR_SSL_WANT_READ)  &&
          (ret != MBEDTLS_ERR_SSL_WANT_WRITE))
      {
         break;
      }
   }
   
   
exit:   
   /* Free SSL context */
   mbedtls_ssl_session_reset(&Client->ssl);
   mbedtls_ssl_free(&Client->ssl);
   
   closesocket(ctp->ctp_stream->strm_csock);
   xfree(ctp->ctp_stream);
   xfree(ctp);

#if 0 // Set to 1 to enable stack info output 
{   
   uint32_t             Size;
   uint32_t             Free;

   Free  = OSGetStackInfo(&Client->TCB, &Size);
   TAL_PRINTF("%s: S:%4d  U:%4d  F:%4d\n", Client->TCB.Name, Size, (Size - Free), Free);
}   
#endif

   nNumThreads--;
   
} /* WebClientTls */

/*************************************************************************/
/*  WebServerTls                                                         */
/*                                                                       */
/*  In    : task parameter                                               */
/*  Out   : none                                                         */
/*  Return: never                                                        */
/*************************************************************************/
static void WebServerTls (void *p)
{
   SOCKET               sock;
   SOCKET               csock;
   struct sockaddr_in   addr;
   struct sockaddr_in   caddr;
   socklen_t            len;
   CLIENT_THREAD_PARAM *ctp;
   HTTP_STREAM         *ctp_stream;
   unsigned int         tmo;
   client_tls_info_t   *Client;
   struct lwip_sock    *pSock;
   
   (void)p;

   
   /* Setup client data */
   memset(ClientArray, 0x00, sizeof(ClientArray));
   for(int i=0; i<_MAX_WEB_TLS_CLIENT_TASKS; i++)
   {
      OS_TaskSetStateNotInUsed(&ClientArray[i].TCB);
      ClientArray[i].Stack     = (uint8_t*)&ClientStack[i][0];
      ClientArray[i].StackSize = TASK_IP_WEB_TLS_CLIENT_STK_SIZE;
   }
   

   /* Wait that the IP interface is ready for use */
   while(!IP_IF_IsReady(IFACE_ANY))
   {
      OS_TimeDly(500);
   }
   
   /* Wait some time for the external switch */
   OS_TimeDly(DELAY_AFTER_LINK_MS);
   
   /* Create server socket */
   sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (sock != INVALID_SOCKET)
   {
      memset((void*)&addr, 0, sizeof(addr));
      addr.sin_addr.s_addr = htonl(INADDR_ANY);
      addr.sin_port        = htons(wServerPort);
      addr.sin_family      = AF_INET;

      /* Assign a name (port) to an unnamed socket */
      if (bind(sock, (const struct sockaddr*)&addr, sizeof(addr)) == 0) /*lint !e740*/
      {
         if (listen(sock, _MAX_WEB_TLS_CLIENT_TASKS) == 0)
         {
            for (;;) 
            {
               len = sizeof(caddr);
               if ((csock = accept(sock, (struct sockaddr*)&caddr, &len)) == INVALID_SOCKET) /*lint !e740*/
               {
                  continue;   /* Error */
               }

               Client = FindFreeClient();
               if (Client != NULL)
               {
                  /* Create the Http client task */

                  /* Use a higher timeout than for normal HTTP, because TLS need more performance */
                  tmo = 6000;
                  setsockopt(csock, SOL_SOCKET, SO_RCVTIMEO, (char *) &tmo, sizeof(tmo));

                  ctp        = xmalloc(XM_ID_WEB, sizeof(CLIENT_THREAD_PARAM));
                  ctp_stream = xcalloc(XM_ID_WEB, 1, sizeof(HTTP_STREAM));
                  
                  if ((ctp != NULL) && (ctp_stream != NULL))
                  {
                     ctp->ctp_handler = HttpdClientHandler;
                     ctp->ctp_stream = ctp_stream;
                     ctp->ctp_stream->strm_ssock = sock;
                     ctp->ctp_stream->strm_csock = csock;
                     memcpy(&ctp->ctp_stream->strm_saddr, &addr, sizeof(ctp->ctp_stream->strm_saddr));
                     memcpy(&ctp->ctp_stream->strm_caddr, &caddr, sizeof(ctp->ctp_stream->strm_caddr));

                     nNumThreads++;
                     Client->ctp  = ctp;
                     Client->Sock = csock;
                     
                     if (nNumThreads > nNumThreadsMaxTLS)
                     {
                        nNumThreadsMaxTLS = nNumThreads;
                     }
                     
                     pSock = lwip_socket_dbg_get_socket(csock);
                     pSock->conn->ssl = &Client->ssl;

                     OS_TaskCreate(&Client->TCB, WebClientTls, (void*)Client, (TASK_IP_WEB_TLS_SERVER_PRIORITY + 1),
                                   Client->Stack, Client->StackSize, 
                                   "WebClientTls");
                               
                     while (nNumThreads >= _MAX_WEB_TLS_CLIENT_TASKS)
                     {
                        OS_TimeDly(10);
                     }
                  }
                  else
                  {
                     /* Memory error */
                     if (ctp != NULL)        xfree(ctp);
                     if (ctp_stream != NULL) xfree(ctp_stream);
                     
                     closesocket(csock);
                  }                     
               }
               else
               {
                  closesocket(csock);
               } /* end if (Client != NULL) */
            }
         } /* end if "listen" */
      } /* end if "bind" */
      
      closesocket(sock);
   }
   
} /* WebServerTls */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  IP_WEBS_SSLStart                                                     */
/*                                                                       */
/*  Start the web tls server, but only if the normal server is running.  */
/*                                                                       */
/*  In    : wPort                                                        */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 Error                                            */
/*************************************************************************/
int IP_WEBS_SSLStart (uint16_t wPort)
{
   int rc = -1;
   
   if ((1 == IP_WEBS_IsRunnung()) && (0 == nWebsTlsRunning))
   {
      nWebsTlsRunning = 1;
      wServerPort     = wPort;

      /*
       * Initialize the stream interface. 
       */
      StreamInitSsl((ssl_write_t*)mbedtls_ssl_write, (ssl_read_t*)mbedtls_ssl_read);
   
      /*
       * Initialize the TLS functionslity
       */
      rc = InitTls();
      if (0 == rc)
      {
         gRedirectHTTPtoHTTPS = 1;
      }
          
      /* Create the HTTP server tasks */
      OS_TaskCreate(&TCBServerTask, WebServerTls, NULL, TASK_IP_WEB_TLS_SERVER_PRIORITY,
                    ServerStack, sizeof(ServerStack), "WebServerTls");
                   
      rc = 0;                   
   }                   

   return(rc);
} /* IP_WEBS_SSLStart */

/*************************************************************************/
/*  IP_WEBS_IsRunnungSSL                                                 */
/*                                                                       */
/*  Return the state if the web tls server is running.                   */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 1 = running / 0 = not running.                               */
/*************************************************************************/
int IP_WEBS_IsRunnungSSL (void)
{
   return(nWebsTlsRunning);
} /* IP_WEBS_IsRunnungSSL */

/*** EOF ***/

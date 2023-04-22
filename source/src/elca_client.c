/**************************************************************************
*  Copyright (c) 2021-2023 by Michael Fischer (www.emb4fun.de).
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
**************************************************************************/
#define __ELCA_CLIENT_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/
#include <stdio.h>
#include "tal.h"
#include "terminal.h"
#include "ipstack.h"
#include "ip_tnp.h"
#include "elca_client.h"

#include "mbedtls/platform.h"
#include "mbedtls/pkcs5.h"
#include "mbedtls/pk.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/asn1write.h"
#include "mbedtls/oid.h"
#include "mbedtls/x509.h"
#include "mbedtls/x509_csr.h"

/*lint -save -e801*/

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

#define SUBJECT_NAME    "CN=tiny%s.local,OU=ExampleTrust Devices,O=ExampleTrust,C=DE"

#define ALT_NAME        "tiny.local"
#define ALT_NAME_MAC    "tiny%s.local"

#define PRE_SHARED_KEY  "UseyourownPSK"

/*************************************************************************/

/*
 * Some external swithes are not ready to use if the link
 * is available. Therefore wait a short time.
 */
#define DELAY_AFTER_LINK_MS   2000

/*
 * End macro
 */
#define GOTO_END(_a)    { rc = _a; goto end; }


#define KEY_SIZE        1024
#define CSR_SIZE        1024
#define CRT_SIZE        1024

/*=======================================================================*/
/*  Definition of all global Data                                        */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all extern Data                                        */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

/* 
 * Some TASK variables like stack and task control block.
 */
static OS_STACK (ClientStack, TASK_IP_ELCAC_STK_SIZE);
static OS_TCB TCBClient;

static mbedtls_pk_context       pk;
static mbedtls_entropy_context  entropy;
static mbedtls_ctr_drbg_context ctr_drbg;
static mbedtls_x509write_csr    req;

const uint8_t server_outh[] = { 0x30, 0x0A, 0x06, 0x08, 0x2B, 0x06, 0x01, 0x05, 0x05, 0x07, 0x03, 0x01 };

static char KeyData[KEY_SIZE];
static char CSRData[CSR_SIZE];
static char CRTData[CRT_SIZE];
static char InterData[CRT_SIZE];

static ip_elcac_cb_ready_t ReadyCB = NULL;
static int  Ready = 0;             

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*
 * https://github.com/ARMmbed/mbedtls/issues/1878
 */

/*lint -save -e737 -e717 */

#define MBEDTLS_SAN_MAX_LEN    64
typedef struct mbedtls_write_san_node{
    char* host;
    size_t hostlen;
}mbedtls_write_san_node;

typedef struct mbedtls_write_san_list{
    mbedtls_write_san_node node;
    struct mbedtls_write_san_list* next;
}mbedtls_write_san_list;

static size_t mbedtls_get_san_list_deep(const mbedtls_write_san_list* sanlist)
{
    size_t ret = 0;

    const mbedtls_write_san_list* cur = sanlist;
    while ( cur )
    {
        ++ret;
        cur = cur->next;
    }

    return ret;
}

int mbedtls_x509write_crt_set_subject_alternative_name (mbedtls_x509write_csr *ctx, const mbedtls_write_san_list* sanlist, uint32_t ip_address)
{
    int	ret = 0;
    size_t sandeep;
    const mbedtls_write_san_list* cur = sanlist;
    unsigned char* buf;
    unsigned char* pc;
    size_t len;
    size_t buflen;

    /* How many DNS names to be written */
    sandeep = mbedtls_get_san_list_deep(sanlist);
    if ( sandeep == 0 )
        return ret;

    buflen = MBEDTLS_SAN_MAX_LEN * sandeep + sandeep;
    buf = mbedtls_calloc(1, buflen);
    if ( !buf )
        return MBEDTLS_ERR_ASN1_ALLOC_FAILED;

    //mbedtls_zeroize(buf, buflen);
    memset(buf, 0x00, buflen);
    pc = buf + buflen;

    len = 0;

    if (ip_address != 0)
    {
        MBEDTLS_ASN1_CHK_ADD(len, mbedtls_asn1_write_raw_buffer(&pc, buf, (const unsigned char *)&ip_address, sizeof(ip_address)));
        MBEDTLS_ASN1_CHK_ADD(len, mbedtls_asn1_write_len(&pc, buf, sizeof(ip_address)));
        MBEDTLS_ASN1_CHK_ADD(len, mbedtls_asn1_write_tag(&pc, buf, MBEDTLS_ASN1_CONTEXT_SPECIFIC | 7));
    }  

    while ( cur )
    {
        MBEDTLS_ASN1_CHK_ADD(len, mbedtls_asn1_write_raw_buffer(&pc, buf, (const unsigned char *)cur->node.host, cur->node.hostlen));
        MBEDTLS_ASN1_CHK_ADD(len, mbedtls_asn1_write_len(&pc, buf, cur->node.hostlen));
        MBEDTLS_ASN1_CHK_ADD(len, mbedtls_asn1_write_tag(&pc, buf, MBEDTLS_ASN1_CONTEXT_SPECIFIC | 2));

        cur = cur->next;
    }

    MBEDTLS_ASN1_CHK_ADD(len, mbedtls_asn1_write_len(&pc, buf, len));
    MBEDTLS_ASN1_CHK_ADD(len, mbedtls_asn1_write_tag(&pc, buf, MBEDTLS_ASN1_CONSTRUCTED | MBEDTLS_ASN1_SEQUENCE));

    ret = mbedtls_x509write_csr_set_extension(
        ctx,
        MBEDTLS_OID_SUBJECT_ALT_NAME,
        MBEDTLS_OID_SIZE(MBEDTLS_OID_SUBJECT_ALT_NAME),
        buf + buflen - len,
        len);

    mbedtls_free(buf);
    return ret;
}

static int set_basic_constraints (mbedtls_x509write_csr *ctx,
                                  int is_ca, int max_pathlen )
{
    int ret;
    unsigned char buf[9];
    unsigned char *c = buf + sizeof(buf);
    size_t len = 0;

    memset( buf, 0, sizeof(buf) );

    if( is_ca && max_pathlen > 127 )
        return( MBEDTLS_ERR_X509_BAD_INPUT_DATA );

    if( is_ca )
    {
        if( max_pathlen >= 0 )
        {
            MBEDTLS_ASN1_CHK_ADD( len, mbedtls_asn1_write_int( &c, buf, max_pathlen ) );
        }
        MBEDTLS_ASN1_CHK_ADD( len, mbedtls_asn1_write_bool( &c, buf, 1 ) );
    }

    MBEDTLS_ASN1_CHK_ADD( len, mbedtls_asn1_write_len( &c, buf, len ) );
    MBEDTLS_ASN1_CHK_ADD( len, mbedtls_asn1_write_tag( &c, buf, MBEDTLS_ASN1_CONSTRUCTED |
                                                MBEDTLS_ASN1_SEQUENCE ) );

    return mbedtls_x509write_csr_set_extension( ctx, MBEDTLS_OID_BASIC_CONSTRAINTS,
                                        MBEDTLS_OID_SIZE( MBEDTLS_OID_BASIC_CONSTRAINTS ),
                                        buf + sizeof(buf) - len, len );
}

/*lint -restore*/

/*************************************************************************/
/*  HandleGetReq                                                         */
/*                                                                       */
/*  In    : pTxMsg, pRxMsg, dAddress                                     */ 
/*  Out   : pRxMsg                                                       */
/*  Return: 0 on success / -1 otherwise                                  */
/*************************************************************************/
static int HandleGetReq (elca_msg_t *pTxMsg, elca_msg_t *pRxMsg, uint32_t dAddress)
{
   int                   rc = -1;
   int                   Optval;
   int                   Socket;
   struct sockaddr_in    Dest;
   struct sockaddr_in    Source;
   int                   SourceLen;  

   /* Get socket */
   Socket = socket(AF_INET, SOCK_DGRAM, 0);
   if (Socket != INVALID_SOCKET)
   {
      /* Set socket option RCVTIMEO to 1000ms*/
      Optval = 1000;
      setsockopt(Socket, SOL_SOCKET, SO_RCVTIMEO, &Optval, sizeof(Optval));

      /* Set address and port */
      Dest.sin_addr.s_addr = htonl(dAddress);
      Dest.sin_port        = htons(ELCA_SERVER_PORT);
      Dest.sin_family      = AF_INET;
   
      /* Send the request to the server */
      sendto(Socket, (const char *)pTxMsg, ELCA_RPC_HEADER_SIZE + pTxMsg->Header.Len, 0, 
             (struct sockaddr*)&Dest, sizeof(struct sockaddr)); /*lint !e740*/    

      /* Wait for the response from the server */          
      SourceLen = sizeof(Source);
      rc = recvfrom(Socket, (char *)pRxMsg, sizeof(elca_msg_t), 0, 
                    (struct sockaddr*)&Source, (socklen_t*)&SourceLen); /*lint !e740*/
                    
      /* Socket can be closed */
      closesocket(Socket);

      /* Check size of the return data */                 
      if (rc != -1)
      {
         rc = (rc >= (int)ELCA_RPC_HEADER_SIZE) ? 0 : -1; /*lint !e574*/
      }   
   }      
   
   return(rc);
} /* HandleGetReq */

/*************************************************************************/
/*  KeyCreate                                                            */
/*                                                                       */
/*  Create the private key.                                              */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 0 on success / -1 otherwise                                  */
/*************************************************************************/
static int KeyCreate (void)
{
   int         rc;
   const char *pers = "elca";
   
   /* Prepare key generation */
   mbedtls_pk_init(&pk);
   mbedtls_ctr_drbg_init(&ctr_drbg);
   mbedtls_entropy_init(&entropy);

   rc =  mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
                               (const unsigned char *)pers, strlen( pers ));
   if (rc != 0) GOTO_END(-1);

   /* Generate the key */
   rc = mbedtls_pk_setup(&pk, mbedtls_pk_info_from_type(MBEDTLS_PK_ECKEY));
   if (rc != 0) GOTO_END(-1);

   rc = mbedtls_ecp_gen_key(MBEDTLS_ECP_DP_SECP256R1, mbedtls_pk_ec(pk), 
                            mbedtls_ctr_drbg_random, &ctr_drbg);
   if (rc != 0) GOTO_END(-1);

   /* Print private key */
   rc = mbedtls_pk_write_key_pem(&pk, (unsigned char*)KeyData, sizeof(KeyData));
   if (rc != 0) GOTO_END(-1);

end:

   return(rc);   
} /* KeyCreate */ 

/*************************************************************************/
/*  CSRCreate                                                            */
/*                                                                       */
/*  Create the CSR.                                                      */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 0 on success / -1 otherwise                                  */
/*************************************************************************/
static int CSRCreate (void)
{
   int                    rc;
   mbedtls_write_san_list sanlist[2];
   unsigned char          key_usage = 0;
   uint8_t                mac_addr[6];
   char                   mac_str[8];
   static char            name[256];
   
   /* Get the MAC address */
   IP_IF_MACGet(0, mac_addr, sizeof(mac_addr));
   snprintf(mac_str, sizeof(mac_str), "%02x%02x%02x", 
            mac_addr[3], mac_addr[4], mac_addr[5]);

   /* Build subject name */            
   snprintf(name, sizeof(name), SUBJECT_NAME, mac_str);
   
   mbedtls_x509write_csr_init(&req);
   mbedtls_x509write_csr_set_md_alg(&req, MBEDTLS_MD_SHA256);

   rc = mbedtls_x509write_csr_set_subject_name(&req, name);
   if (rc != 0) GOTO_END(-1);


   /*
    * Extensions
    */

   /* X509v3 Basic Constrains */
   rc = set_basic_constraints(&req, 0, -1);
   if (rc != 0) GOTO_END(-1);

   /* X509v3 Key Usage */
   key_usage |= MBEDTLS_X509_KU_DIGITAL_SIGNATURE;
   //key_usage |= MBEDTLS_X509_KU_NON_REPUDIATION;
   key_usage |= MBEDTLS_X509_KU_KEY_ENCIPHERMENT;
   //key_usage |= MBEDTLS_X509_KU_KEY_AGREEMENT;        /* <= ??? is this realy needed */
   mbedtls_x509write_csr_set_key_usage(&req, key_usage);

   /* X509v3 Subject Alternative Name */
   snprintf(name, sizeof(name), ALT_NAME_MAC, mac_str);
   
   sanlist[0].node.host    = ALT_NAME;
   sanlist[0].node.hostlen = strlen(sanlist[0].node.host);
   sanlist[0].next = &sanlist[1];

   sanlist[1].node.host    = name;
   sanlist[1].node.hostlen = strlen(sanlist[1].node.host);
   sanlist[1].next = NULL;

   rc = mbedtls_x509write_crt_set_subject_alternative_name(&req, &sanlist[0], 0xC801A8C0);
   if (rc != 0) GOTO_END(-1);

   /* X509v3 id-kp-serverAuth, see: https://support.apple.com/en-us/HT210176 */
   rc = mbedtls_x509write_csr_set_extension(&req,
                                            MBEDTLS_OID_EXTENDED_KEY_USAGE , MBEDTLS_OID_SIZE(MBEDTLS_OID_EXTENDED_KEY_USAGE),
                                            server_outh, sizeof(server_outh));
   if (rc != 0) GOTO_END(-1);

   /* Set the key for the CSR */
   mbedtls_x509write_csr_set_key(&req, &pk);
   
   /* Create CSR in PEM format */
   rc = mbedtls_x509write_csr_pem(&req, (unsigned char*)CSRData, sizeof(CSRData), mbedtls_ctr_drbg_random, &ctr_drbg);

#if 0
   if (0 == rc)
   {
      OS_TimeDly(2000);
      for(int x=0; x<strlen(CSRData); x++)
      {
         term_putchar(CSRData[x]);
      }
      term_printf("\r\n");
   }
#endif   

   mbedtls_pk_free(&pk);
   mbedtls_ctr_drbg_free(&ctr_drbg);
   mbedtls_entropy_free(&entropy);
   mbedtls_x509write_csr_free(&req);

end:

   return(rc);   
} /* CSRCreate */

/*************************************************************************/
/*  CRTGet                                                               */
/*                                                                       */
/*  Request the CRT with the CSR by the ELCA system.                     */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 0 on success / -1 otherwise                                  */
/*************************************************************************/
static int CRTGet (void)
{
   int                        rc = -1;
   uint8_t                   bIFace = 0; 
   uint32_t                  dAddress; 
   elca_msg_t               *pTxMsg = NULL;
   elca_msg_t               *pRxMsg = NULL;
   mbedtls_md_context_t       sha2_ctx;
   const mbedtls_md_info_t   *info_sha2;
   mbedtls_aes_context        ctx;
   size_t                     Size;
   uint8_t                    Random[16];
   uint8_t                    Key[32];
   

   /* Check if iface is ready */
   if (0 == IP_IF_IsReady(bIFace)) GOTO_END(-1);

   /* Send TNP request and wait */
   IP_TNP_SendRequest(bIFace);
   OS_TimeDly(500);

   /* Check if a TinyELCA server is available */   
   rc = IP_TNP_DeviceFind("TinyELCA", &dAddress);
   if ((rc != 0) || (0 == dAddress)) GOTO_END(-1);

   /* Alloc RPC memory */
   pTxMsg = xmalloc(XM_ID_IP, sizeof(elca_msg_t));
   pRxMsg = xmalloc(XM_ID_IP, sizeof(elca_msg_t));
   if ((NULL == pTxMsg) || (NULL == pRxMsg)) GOTO_END(-1);

   /* Create CRT request */
   memset(pTxMsg, 0x00, sizeof(elca_msg_t));

   pTxMsg->Header.Magic1  = ELCA_RPC_HEADER_MAGIC_1;
   pTxMsg->Header.Magic2  = ELCA_RPC_HEADER_MAGIC_2;
   pTxMsg->Header.SizeVer = ELCA_RPC_SIZEVER;
   pTxMsg->Header.Func    = ELCA_MSG_GET_CRT;   
   pTxMsg->Header.Len     = ELCA_CALL_GET_CRT_SIZE; 

   /* Create AES key */
   info_sha2 = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);
   if (NULL == info_sha2) GOTO_END(-1);

   mbedtls_md_init(&sha2_ctx);
   rc = mbedtls_md_setup(&sha2_ctx, info_sha2, 1);
   if (rc != 0) GOTO_END(-1);
   
   rc = mbedtls_pkcs5_pbkdf2_hmac(&sha2_ctx, 
                                  (uint8_t*)PRE_SHARED_KEY, strlen(PRE_SHARED_KEY), 
                                  NULL, 0,
                                  4096,
                                  sizeof(Key), Key);
   mbedtls_md_free(&sha2_ctx);
   if (rc != 0) GOTO_END(-1);
   
   /* Encrypt CSR first */
   tal_CPURngHardwarePoll(Random, ELCA_RPC_RAND_SIZE);
   memcpy(pTxMsg->Data.cGetCRT.Random, Random, ELCA_RPC_RAND_SIZE);

   Size = strlen(CSRData);
   Size = (Size + 15) & ~15;
   pTxMsg->Data.cGetCRT.CSRLen = (uint16_t)Size;
   
   mbedtls_aes_init(&ctx);
   mbedtls_aes_setkey_enc(&ctx, Key, (sizeof(Key)*8));
   rc = mbedtls_aes_crypt_cbc(&ctx, MBEDTLS_AES_ENCRYPT, 
                              (size_t)Size,
                              Random,
                              (uint8_t*)CSRData,
                              (uint8_t*)pTxMsg->Data.cGetCRT.CSR);
   mbedtls_aes_free(&ctx);
   memset(Key, 0x00, sizeof(Key));
   if (rc != 0) GOTO_END(-1);

   /* Call RPC */
   rc = HandleGetReq(pTxMsg, pRxMsg, dAddress); 
   if (rc != 0) GOTO_END(-1);

   /* Check GetCRT result */
   rc = pRxMsg->Header.Result;
   if (rc != 0) GOTO_END(rc);
   
   /* Copy device CRT */
   if (pRxMsg->Data.rGetCRT.CRTLen > CRT_SIZE) GOTO_END(-1);
   memcpy(CRTData, pRxMsg->Data.rGetCRT.CRT, pRxMsg->Data.rGetCRT.CRTLen);

   rc = 0;
   
end:

   xfree(pTxMsg);
   xfree(pRxMsg);

   return(rc);
} /* CRTGet */

/*************************************************************************/
/*  InterCRTGet                                                          */
/*                                                                       */
/*  Request the intermediate certificate.                                */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 0 on success / -1 otherwise                                  */
/*************************************************************************/
int InterCRTGet (void)
{
   int           rc = -1;
   uint8_t      bIFace = 0; 
   uint32_t     dAddress; 
   elca_msg_t  *pTxMsg = NULL;
   elca_msg_t  *pRxMsg = NULL;
   size_t        CRTSize;
   char        *pCRT   = InterData;
   size_t        Size  = sizeof(InterData);
   
   /* Check if iface is ready */
   if (0 == IP_IF_IsReady(bIFace)) GOTO_END(-1);

   /* Send TNP request and wait */
   IP_TNP_SendRequest(bIFace);
   OS_TimeDly(500);

   /* Check if a TinyELCA server is available */   
   rc = IP_TNP_DeviceFind("TinyELCA", &dAddress);
   if ((rc != 0) || (0 == dAddress)) GOTO_END(ELCA_RPC_ERR_NO_SERVER);

   /* Alloc RPC memory */
   pTxMsg = xmalloc(XM_ID_IP, sizeof(elca_msg_t));
   pRxMsg = xmalloc(XM_ID_IP, sizeof(elca_msg_t));
   if ((NULL == pTxMsg) || (NULL == pRxMsg)) GOTO_END(-1);

   /* Create GetInter request */
   memset(pTxMsg, 0x00, sizeof(elca_msg_t));
   
   pTxMsg->Header.Magic1  = ELCA_RPC_HEADER_MAGIC_1;
   pTxMsg->Header.Magic2  = ELCA_RPC_HEADER_MAGIC_2;
   pTxMsg->Header.SizeVer = ELCA_RPC_SIZEVER;
   pTxMsg->Header.Func    = ELCA_MSG_GET_INTER;   
   pTxMsg->Header.Len     = ELCA_CALL_GET_INTER_SIZE; 

   /* Call RPC */
   rc = HandleGetReq(pTxMsg, pRxMsg, dAddress); 
   if (rc != 0) GOTO_END(-1);

   /* Check GetInter result */
   rc = pRxMsg->Header.Result;
   if (rc != 0) GOTO_END(rc);
   
   /* Check size of InterCRT */
   CRTSize = strlen((char*)pRxMsg->Data.rGetInter.CRT);
   if (CRTSize >= Size) GOTO_END(-1);

   /* Copy InterCRT */
   memset(pCRT, 0x00, Size);
   memcpy(pCRT, pRxMsg->Data.rGetInter.CRT, CRTSize);

   rc = 0;
   
end:

   xfree(pTxMsg);
   xfree(pRxMsg);

   return(rc);
} /* InterCRTGet */ 

/*************************************************************************/
/*  ClientTask                                                           */
/*                                                                       */
/*  This is the ELCA client task.                                        */
/*                                                                       */
/*  In    : task parameter                                               */
/*  Out   : none                                                         */
/*  Return: never                                                        */
/*************************************************************************/
static void ClientTask (void *p)
{
   int rc;
   
   (void)p;
   
   /* Wait that the IP interface is ready for use */
   while (0 == IP_IF_IsReady(0))
   {
      OS_TimeDly(100);
   }

   /* Wait some time for the external switch */
   OS_TimeDly(DELAY_AFTER_LINK_MS);

   /* Wait for TNP is ready */   
   while (0 == IP_TNP_IsReady())
   {
      OS_TimeDly(100);
   }
   
   term_printf("ELCA: Client started...\r\n");

   /* Clear all data first */
   memset(KeyData,   0x00, sizeof(KeyData));
   memset(CSRData,   0x00, sizeof(CSRData));
   memset(CRTData,   0x00, sizeof(CRTData));
   memset(InterData, 0x00, sizeof(InterData));
   
   /* Get intermediate certificate */   
   while (1)
   {
      rc = InterCRTGet();
      if (0 == rc)
      {
         break;
      }
      else
      {
         switch (rc)
         {
            case ELCA_RPC_ERR_NO_SERVER:
            {
               term_printf("ELCA: Error, no ELCA server available\r\n"); 
               break;
            }
            case ELCA_RPC_ERR_LOCKED:
            {
               term_printf("ELCA: Error, the ELCA is currently still locked\r\n"); 
               break;
            }
            default:
            {
               term_printf("ELCA: Error %d\r\n", rc);
            }
         }
      
         /* Wait 10 seconds, and try it again */   
         for (int x = 0; x < 10; x++)
         {
            OS_TimeDly(1000);
         }
      }
   } /* end while (1) */      
   
   /* Create private key */
   rc = KeyCreate();
   if (rc != 0) GOTO_END(-1);
   
   /* Create CSR */
   rc = CSRCreate();
   if (rc != 0) GOTO_END(-1);
   
   /* Get device CRT */   
   rc = CRTGet();
   if (rc != 0) GOTO_END(-1);
   
   Ready = 1;
   
end:

   if (0 == rc)
   {
      term_printf("ELCA: Ready\r\n");
   }
   else
   {
      term_printf("ELCA: Error\r\n");
   }
 
   /* Call ready callback */   
   if ((ReadyCB != NULL) && (0 == rc))
   {
      ReadyCB(rc);
   } 
   
   /*
    * The ELCA client task is not needed any more, bye, bye...
    */     
   OS_TimeDly(1000);
   if (0 == rc)
   {
      /* A certificate was installed, we can do a reset now */
      tal_CPUReboot();
   }

} /* ClientTask */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  IP_ELCAC_Start                                                       */
/*                                                                       */
/*  Start the ELCA client functionality.                                 */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_ELCAC_Start (ip_elcac_cb_ready_t callback)
{
   static int nInitDone = 0;
   
   if (0 == nInitDone)
   {
      ReadyCB = callback;
   
      OS_TaskCreate(&TCBClient, ClientTask, NULL, TASK_IP_ELCAC_PRIORITY,
                    ClientStack, sizeof(ClientStack), 
                    "ELCAc");
                    
      nInitDone = 1;
   }                    

} /* IP_ELCAC_Start */

/*************************************************************************/
/*  IP_ELCAC_KeyGet                                                      */
/*                                                                       */
/*  Request the private key.                                             */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: KeyData                                                      */
/*************************************************************************/
char *IP_ELCAC_KeyGet (void)
{
   return( (1 == Ready) ? KeyData : NULL );
} /* IP_ELCAC_KeyGet */

/*************************************************************************/
/*  IP_ELCAC_CSRGet                                                      */
/*                                                                       */
/*  Return the certificate signing request.                              */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: CSRData                                                      */
/*************************************************************************/
char *IP_ELCAC_CSRGet (void)
{
   return( (1 == Ready) ? CSRData : NULL );
} /* IP_ELCAC_CSRGet */

/*************************************************************************/
/*  IP_ELCAC_DevCRTGet                                                   */
/*                                                                       */
/*  Return the device certificate.                                       */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: CRTData                                                      */
/*************************************************************************/
char *IP_ELCAC_DevCRTGet (void)
{
   return( (1 == Ready) ? CRTData : NULL );
} /* IP_ELCAC_DevCRTGet */

/*************************************************************************/
/*  IP_ELCAC_InterCRTGet                                                 */
/*                                                                       */
/*  Return the intermediate certificate.                                 */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: InterData                                                    */
/*************************************************************************/
char *IP_ELCAC_InterCRTGet (void)
{
   return( (1 == Ready) ? InterData : NULL );
} /* IP_ELCAC_InterCRTGet */

/*lint -restore*/

/*** EOF ***/

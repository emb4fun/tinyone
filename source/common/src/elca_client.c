/**************************************************************************
*  Copyright (c) 2021-2024 by Michael Fischer (www.emb4fun.de).
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
#include "ipweb.h"
#include "cert.h"
#include "ip_tnp.h"
#include "elca_client.h"

#include "mbedtls/platform.h"
#include "mbedtls/base64.h"
#include "mbedtls/pkcs5.h"
#include "mbedtls/pk.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/asn1write.h"
#include "mbedtls/oid.h"
#include "mbedtls/x509.h"
#include "mbedtls/x509_csr.h"
#include "mbedtls/x509_crt.h"

/*lint -save -e801*/

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

#define ELCA_OK               0  
#define ELCA_ERR_USER_PASS    -1
#define ELCA_ERR_BLOCKED      -2
/* The lines above must not changes */
#define ELCA_ERR              -3
#define ELCA_ERR_KEY          -4
#define ELCA_ERR_CRT          -5
#define ELCA_ERR_CRT_CHECK_1  -6
#define ELCA_ERR_CRT_CHECK_2  -7
#define ELCA_ERR_CRT_CHECK_3  -8
#define ELCA_ERR_CRT_CHECK_4  -9
#define ELCA_ERR_CRT_CHECK_5  -10
#define ELCA_ERR_CRT_WRITE    -11
#define ELCA_ERR_CRT_SIZE     -12

#define ELCA_ERR_CRT_SUBJECT  -20
#define ELCA_ERR_CRT_ISSUER   -21
#define ELCA_ERR_CRT_CN       -22
#define ELCA_ERR_CRT_O        -23
#define ELCA_ERR_CRT_OU       -24
#define ELCA_ERR_CRT_C        -25
#define ELCA_ERR_CRT_AN       -26
#define ELCA_ERR_CRT_IC       -27
#define ELCA_ERR_CRT_IO       -28
#define ELCA_ERR_CRT_ICN      -29
#define ELCA_ERR_CRT_S        -30


#define CSR_OU          "ExampleTrust Devices" 
#define CSR_O           "ExampleTrust" 
#define CSR_C           "DE"
#define CSR_IP          "192.168.1.200" 

//#define SUBJECT_NAME    "CN=tiny%s.local,OU=ExampleTrust Devices,O=ExampleTrust,C=DE"
#define SUBJECT_NAME    "CN=tiny%s.local,OU="CSR_OU",O="CSR_O",C="CSR_C

#define ALT_NAME        "tiny.local"
#define ALT_NAME_MAC    "tiny%s.local"

#if !defined(PRE_SHARED_KEY)
/* This is only an example, use your own PSK */
#define PRE_SHARED_KEY  "UseyourownPSK"
#endif

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


typedef struct _oid_an_
{
   uint8_t  OID[3];
   uint8_t  OIDType;
   uint8_t  OIDLen;
   uint8_t  DataType;
   uint8_t  DataLen;
   uint8_t  Data;
} OID_AN;

typedef struct _cert_info_
{
   char CN[256];
   char AN[256];
   char O[256];
   char OU[256];
   char C[256];
   char vf[256];
   char vt[256];
   
   char ICN[256];
   char IO[256];
   char IC[256];
   char I[256];
   
   char S[256];
} CERT_INFO;

typedef struct _san_tag_
{
   uint8_t  tag;
   size_t   hostlen;
   char    *host;
   uint32_t address;
} SAN_TAG;


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

static char CSR_CN[256];    
static char CSR_AN[256];  


static char Month[12][4] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                             "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

static mbedtls_x509_crt chaincert;

static char NewCRT[2048];
static CERT_INFO NewCRTSrv;
static CERT_INFO NewCRTInt;

static int nUpdateStartDone = 0;
static int nUpdateResult    = 1;

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

int mbedtls_x509write_crt_set_subject_alternative_name_ext (mbedtls_x509write_csr *ctx, const mbedtls_write_san_list* sanlist, uint32_t ip_address)
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
        0,
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
                                        0,
                                        buf + sizeof(buf) - len, len );
}

/*lint -restore*/

/*************************************************************************/
/*  JSONSendError                                                        */
/*                                                                       */
/*  In    : hs, nError, pMsg                                             */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void JSONSendError (HTTPD_SESSION *hs, int nError)
{
   s_puts("{", hs->s_stream);
   
   if (0 == nError)
   {
      s_puts("\"err\":0,\"msg\":\"none\"", hs->s_stream);
   }
   else
   {
      s_printf(hs->s_stream, "\"err\":%d,\"msg\":\"error\"", nError);
   }

   s_puts("}", hs->s_stream);
   s_flush(hs->s_stream);

} /* JSONSendError */


/*************************************************************************/
/*  ConvertTextarea                                                      */
/*                                                                       */
/*  Converting the textarea data by replacing the two characters "\n"    */
/*  by 0x0D 0x0A.                                                        */
/*                                                                       */
/*  In    : pData                                                        */
/*  Out   : pData                                                        */
/*  Return: none                                                         */
/*************************************************************************/
static void ConvertTextarea (char *pData)
{
   pData = strstr(pData, "\\n");
   while (pData != NULL)
   {
      pData[0] = 0x0D;
      pData[1] = 0x0A;
      pData += 2;
      pData = strstr(pData, "\\n");
   }

} /* ConvertTextarea */

/*************************************************************************/
/*  GetSubjectTag                                                        */
/*                                                                       */
/*  Find "Alternative Names".                                            */
/*                                                                       */
/*  In    : pSubject, pTag, pBuffer, Size                                */
/*  Out   : pBuffer                                                      */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int GetSubjectTag (char *pSubject, char *pTag, char *pBuffer, size_t BufferSize)
{
   int     rc = ELCA_ERR;
   size_t  Size;
   char   *p;
   
   memset(pBuffer, 0x00, BufferSize);

   p = strstr(pSubject, pTag);
   if (p != NULL)
   {
      p += strlen(pTag);
      Size = 0;
      while ((*p != ',') && (Size < BufferSize))
      {
         pBuffer[Size++] = *p++;
      }
      rc = ELCA_OK;
   }      
   
   return(rc);
} /* GetSubjectTag */

/*************************************************************************/
/*  GetANStr                                                             */
/*                                                                       */
/*  Find "Alternative Names".                                            */
/*                                                                       */
/*  In    : pData, DataSize, pBuffer, BufferSize                         */
/*  Out   : pBuffer                                                      */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int GetANStr (uint8_t *pData, size_t DataSize, char *pBuffer, size_t BufferSize)
{
   int        rc = ELCA_ERR_CRT_AN;
   int        DataLen;
   uint8_t    Len;
   size_t     x;
   uint8_t   *p = NULL;
   OID_AN    *pAN;
   static char Tmp[256];
   
   memset(pBuffer, 0x00, sizeof(BufferSize));
   
   for (x = 0; x < DataSize; x++)
   {
      if( (0x55 == pData[x + 0]) && 
          (0x1D == pData[x + 1]) && 
          (0x11 == pData[x + 2]) && 
          (0x04 == pData[x + 3]) ) 
      {
         p = &pData[x];
         break;
      }          
   }
   
   if (p != NULL)
   {
      rc = ELCA_OK;
      
      /*lint -save -e527 -e661 -e662 -e826*/
   
      pAN = (OID_AN*)p;
      
      (void)pAN->OID;      /* Prevent lint warning */
      (void)pAN->OIDType;
      (void)pAN->OIDLen;   
      (void)pAN->DataType;
      
      DataLen = pAN->DataLen;
      pData   = &pAN->Data;
      while ((DataLen > 0) && (strlen(pBuffer) < BufferSize))
      {
         switch (*pData++)
         {
            case 0x82:  /* DNS: Copy string */
            {
               Len = *pData++;
               strncpy(Tmp, (char*)pData, Len);
               Tmp[Len] = 0;
               pData += Len;
               
               DataLen -= 2;
               DataLen -= (int)Len;
               
               if ((strlen(pBuffer) + Len + 1) < BufferSize)
               {
                  strcat(pBuffer, Tmp);
                  strcat(pBuffer, " ");
               }
               else
               {
                  GOTO_END(ELCA_ERR_CRT_AN);
               }   
               break;
            }
            case 0x87:  /* IP: Copy string */
            {
               Len = *pData++;
               snprintf(Tmp, sizeof(Tmp), "%d.%d.%d.%d ", 
                        pData[0], pData[1], pData[2], pData[3]);
               pData += Len;                        

               DataLen -= 2;
               DataLen -= Len;

               if ((strlen(pBuffer) + strlen(Tmp) + 1) < BufferSize)
               {
                  strcat(pBuffer, Tmp);
               }
               else
               {
                  GOTO_END(ELCA_ERR_CRT_AN);
               }   
               break;               
            }
            default:
            {
               GOTO_END(ELCA_ERR_CRT_AN);
               break;
            }
         }
      }
      /*lint -restore*/
      
      /* Remove space from the end of the string */
      DataLen = (int)strlen(pBuffer);
      if (' ' == pBuffer[DataLen-1])
      {
         pBuffer[DataLen-1] = 0;
      }
   }
   
end:   
   
   return(rc);
} /* GetANStr */

/*************************************************************************/
/*  CRTDecode                                                            */
/*                                                                       */
/*  In    : pCRT                                                         */ 
/*  Out   : none                                                         */
/*  Return: 0 on success / -1 otherwise                                  */
/*************************************************************************/
static int CRTDecode (mbedtls_x509_crt *pCRT, CERT_INFO *pInfo)
{
   int         rc = 0;
   static char subject_name[256];
   static char issuer_name[256];
   
   memset(pInfo, 0x00, sizeof(CERT_INFO));

   rc = mbedtls_x509_dn_gets(subject_name, sizeof(subject_name), &pCRT->subject);
   if (rc < 0) GOTO_END(ELCA_ERR_CRT_SUBJECT);
   
   rc = mbedtls_x509_dn_gets(issuer_name, sizeof(issuer_name), &pCRT->issuer);
   if (rc < 0) GOTO_END(ELCA_ERR_CRT_ISSUER);

   /* Find "Common Name" */
   rc = GetSubjectTag(subject_name, "CN=", pInfo->CN, sizeof(pInfo->CN));
   if (rc != 0) GOTO_END(ELCA_ERR_CRT_CN);

   /* Find "Organization" */
   rc = GetSubjectTag(subject_name, "O=", pInfo->O, sizeof(pInfo->O));
   if (rc != 0) GOTO_END(ELCA_ERR_CRT_O);

   /* Find "Organization Unit" */
   rc = GetSubjectTag(subject_name, "OU=", pInfo->OU, sizeof(pInfo->OU));
   if (rc != 0) pInfo->OU[0] = 0;

   /* Find "Country" */
   rc = GetSubjectTag(subject_name, "C=", pInfo->C, sizeof(pInfo->C));
   if (rc != 0) GOTO_END(ELCA_ERR_CRT_C);

   /* Special check for "Alternative Names" */
   rc = GetANStr(pCRT->raw.p, pCRT->raw.len, pInfo->AN, sizeof(pInfo->AN));
   if (rc != 0) pInfo->AN[0] = 0;

   /* Find "Issuer Country" */
   rc = GetSubjectTag(issuer_name, "C=", pInfo->IC, sizeof(pInfo->IC));
   if (rc != 0) GOTO_END(ELCA_ERR_CRT_IC);

   /* Find "Issuer Organization" */
   rc = GetSubjectTag(issuer_name, "O=", pInfo->IO, sizeof(pInfo->IO));
   if (rc != 0) GOTO_END(ELCA_ERR_CRT_IO);
   
   /* Find "Issuer Common Name" */
   rc = GetSubjectTag(issuer_name, "CN=", pInfo->ICN, sizeof(pInfo->ICN));
   if (rc != 0) GOTO_END(ELCA_ERR_CRT_ICN);
   snprintf(pInfo->I, sizeof(pInfo->I), "%s, %s, %s", pInfo->ICN, pInfo->IO, pInfo->IC);

   /* Not before */
   snprintf(pInfo->vf, sizeof(pInfo->vf), "%s %2d %02d:%02d:%02d %04d GMT", 
            Month[pCRT->valid_from.mon-1], pCRT->valid_from.day,
            pCRT->valid_from.hour, pCRT->valid_from.min, pCRT->valid_from.sec, 
            pCRT->valid_from.year);

   /* Not after */
   snprintf(pInfo->vt, sizeof(pInfo->vt), "%s %2d %02d:%02d:%02d %04d GMT", 
            Month[pCRT->valid_to.mon-1], pCRT->valid_to.day,
            pCRT->valid_to.hour, pCRT->valid_to.min, pCRT->valid_to.sec, 
            pCRT->valid_to.year);

   /* Serial number */   
   rc = mbedtls_x509_serial_gets(pInfo->S, sizeof(pInfo->S), &pCRT->serial);
   if (rc < 0) GOTO_END(ELCA_ERR_CRT_S);
   
   rc = ELCA_OK;

end:
   
   return(rc);
} /* CRTDecode */

/*************************************************************************/
/*  CheckNewChainCRT                                                     */
/*                                                                       */
/*  In    : pCRT                                                         */ 
/*  Out   : none                                                         */
/*  Return: 0 on success / -1 otherwise                                  */
/*************************************************************************/
static int CheckNewChainCRT (char *pCRT)
{
   int                rc  = ELCA_ERR_CRT_CHECK_1;
   size_t             len = strlen(pCRT);
   char             *pStart;
   char             *pEnd;
   mbedtls_x509_crt *pCRT1;
   mbedtls_x509_crt *pCRT2;

   mbedtls_x509_crt_init(&chaincert);

   /*
    * Check if two CRTs are available
    */

   /* Search first "END" */
   pStart = pCRT;
   pEnd   = strstr(pCRT, "-----END CERTIFICATE-----");
   if (pEnd != NULL)
   {
      pEnd += strlen("-----END CERTIFICATE-----");
      if ((pEnd - pStart) < len) 
      {
         /* Search second "END" */
         pStart = pEnd;
         pEnd   = strstr(pStart, "-----END CERTIFICATE-----");
         if (pEnd != NULL)
         {
            pEnd += strlen("-----END CERTIFICATE-----");
            *pEnd = 0;

            rc = 0;
         }
      }
   }
   if (rc != 0) GOTO_END(ELCA_ERR_CRT_CHECK_1);

   /* 
    * Check valid CRTs
    */
   rc = mbedtls_x509_crt_parse(&chaincert, (const unsigned char *) pCRT, strlen(pCRT) + 1);
   if (rc != 0) GOTO_END(ELCA_ERR_CRT_CHECK_2);

   pCRT1 = &chaincert;
   pCRT2 = chaincert.next;
   
   /* Check if two CRTs are available */
   if ((NULL == pCRT1) || (NULL == pCRT2)) GOTO_END(ELCA_ERR_CRT_CHECK_3);

   /* Check if first CRT is not a "ca" */
   if (pCRT1->private_ca_istrue != 0) GOTO_END(ELCA_ERR_CRT_CHECK_4);

   /* Check if second CRT is a "ca" */
   if (pCRT2->private_ca_istrue != 1) GOTO_END(ELCA_ERR_CRT_CHECK_5);

   if ((strlen(pCRT)+1) < sizeof(NewCRT))
   {
      memcpy(NewCRT, pCRT, strlen(pCRT)+1);
      rc = CRTDecode(pCRT1, &NewCRTSrv);
      if (ELCA_OK == rc)
      {
         rc = CRTDecode(pCRT2, &NewCRTInt);
      }
   }
   else
   {  
      rc = ELCA_ERR_CRT_SIZE;
   }   
   
end:

   mbedtls_x509_crt_free(&chaincert);

   return(rc);
} /* CheckNewChainCRT */

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
/*  KeyLoad                                                              */
/*                                                                       */
/*  Load the private key.                                                */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 0 on success / -1 otherwise                                  */
/*************************************************************************/
static int KeyLoad (void)
{
   int    rc;
   char  *buf;
   size_t buflen;
   const char *pers = "elca";
         
   /* Prepare key load */
   mbedtls_pk_init(&pk);
   mbedtls_ctr_drbg_init(&ctr_drbg);
   mbedtls_entropy_init(&entropy);

   rc =  mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
                               (const unsigned char *)pers, strlen( pers ));
   if (rc != 0) GOTO_END(-1);

   /* Load the key */
   cert_Get_DeviceKey(&buf, &buflen);
   rc = mbedtls_pk_parse_key(&pk, (const unsigned char *) buf, buflen, NULL, 0,
                             mbedtls_ctr_drbg_random, &ctr_drbg);
   if(rc != 0) GOTO_END(-1);

end:

   return(rc);   
} /* KeyLoad */

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
   uint32_t               ipaddr = inet_addr(CSR_IP);
   
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
   snprintf(CSR_CN, sizeof(CSR_CN), ALT_NAME_MAC, mac_str); 
   
   sanlist[0].node.host    = ALT_NAME;
   sanlist[0].node.hostlen = strlen(sanlist[0].node.host);
   sanlist[0].next = &sanlist[1];

   sanlist[1].node.host    = name;
   sanlist[1].node.hostlen = strlen(sanlist[1].node.host);
   sanlist[1].next = NULL;

   rc = mbedtls_x509write_crt_set_subject_alternative_name_ext(&req, &sanlist[0], ipaddr);
   if (rc != 0) GOTO_END(-1);
   
   {  
      char buf[20]; 
      htoa(ntohl(ipaddr), buf, sizeof(buf));
      snprintf(CSR_AN, sizeof(CSR_AN), "%s %s %s", CSR_CN, ALT_NAME, buf); 
   }

   /* X509v3 id-kp-serverAuth, see: https://support.apple.com/en-us/HT210176 */
   rc = mbedtls_x509write_csr_set_extension(&req,
                                            MBEDTLS_OID_EXTENDED_KEY_USAGE , MBEDTLS_OID_SIZE(MBEDTLS_OID_EXTENDED_KEY_USAGE),
                                            0,
                                            server_outh, sizeof(server_outh));
   if (rc != 0) GOTO_END(-1);

   /* Set the key for the CSR */
   mbedtls_x509write_csr_set_key(&req, &pk);
   
   /* Create CSR in PEM format */
   rc = mbedtls_x509write_csr_pem(&req, (unsigned char*)CSRData, sizeof(CSRData), mbedtls_ctr_drbg_random, &ctr_drbg);

//static char CSRAN[256];    

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

   rc = mbedtls_pkcs5_pbkdf2_hmac_ext(MBEDTLS_MD_SHA256, 
                                  (uint8_t*)PRE_SHARED_KEY, strlen(PRE_SHARED_KEY), 
                                  NULL, 0,
                                  4096,
                                  sizeof(Key), Key);
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
/*  ClientTaskStartup                                                    */
/*                                                                       */
/*  This is the ELCA client startup task.                                */
/*  Will be used at startup when no key and certificates are available.  */
/*                                                                       */
/*  In    : task parameter                                               */
/*  Out   : none                                                         */
/*  Return: never                                                        */
/*************************************************************************/
static void ClientTaskStartup (void *p)
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

} /* ClientTaskStartup */

/*************************************************************************/
/*  ClientTaskUpdate                                                     */
/*                                                                       */
/*  This is the ELCA client update task.                                 */
/*  Will be used for an update of the certificates.                      */
/*                                                                       */
/*  In    : task parameter                                               */
/*  Out   : none                                                         */
/*  Return: never                                                        */
/*************************************************************************/
static void ClientTaskUpdate (void *p)
{
   int rc;

   nUpdateResult = 1;
   
   term_printf("ELCAinst: Client started...\r\n");

   /* Wait */
   OS_TimeDly(2000);
   
   /* Save new chained CRT  */
   rc = cert_Write_ChainCert(NewCRT, strlen(NewCRT));
   if (0 == rc)
   {
      nUpdateResult = 0;

      OS_TimeDly(5000);
   
      /* A certificate was installed, we can do a reset now */
      tal_CPUReboot();
   }
   else
   {
      nUpdateResult = -1;
   }
   
   nUpdateStartDone = 0;
         
   OS_TaskExit();
} /* ClientTaskUpdate */

/*************************************************************************/
/*  cgi_cert                                                             */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int cgi_cert (HTTPD_SESSION *hs)
{
   int         rc;
   static char b64_data[2048];
   size_t      len;
   json_t      JSON; 
   char      *pMode;
   char      *pCRT;
   
   IP_WEBS_CGISendHeader(hs);

   rc = IP_JSON_ParseHS(hs, &JSON, 8);
   if (-1 == rc) GOTO_END(ELCA_ERR);

   /* Check if the access is blocked for the moment */   
   rc = WebSidLoginBlocked();
   if (1 == rc) GOTO_END(ELCA_ERR_BLOCKED);

   /* Set default error, in case a wrong "command" was send */
   rc = ELCA_ERR;
   
   /* Get mode for determining the next steps */
   pMode = IP_JSON_GetString(&JSON, "mode");    

   /* Check for CSR request */
   if ((pMode != NULL) && (0 == strcmp(pMode, "reqcsr")))
   {
      /* Load our key */ 
      rc = KeyLoad(); 
      if(rc != ELCA_OK) GOTO_END(ELCA_ERR_KEY);
   
      /* Create CSR */
      rc = CSRCreate();
      if (ELCA_OK == rc)
      {
         s_puts("{", hs->s_stream);

         /* Output CSR */ 
         mbedtls_base64_encode((uint8_t*)b64_data, sizeof(b64_data), &len, (uint8_t*)CSRData, strlen(CSRData));
         s_puts("\"csr\":\"", hs->s_stream);
         for (int x = 0; x < (int)len; x++)
         {
            s_putchar(hs->s_stream, b64_data[x]);
         }
         s_puts("\",", hs->s_stream);

         s_printf(hs->s_stream, "\"cn\":\"%s\",", CSR_CN);
         s_printf(hs->s_stream, "\"an\":\"%s\",", CSR_AN);
         s_printf(hs->s_stream, "\"o\":\"%s\",", CSR_O);
         s_printf(hs->s_stream, "\"ou\":\"%s\",", CSR_OU);
         s_printf(hs->s_stream, "\"c\":\"%s\"", CSR_C);

         s_puts("}", hs->s_stream);
         s_flush(hs->s_stream);
      }
   }
   
   /* Check if a new chained CRT should be received */
   if ((pMode != NULL) && (0 == strcmp(pMode, "newcrt")))
   {
      pCRT = IP_JSON_GetString(&JSON, "crt");
      if (NULL == pCRT) GOTO_END(ELCA_ERR_CRT);
      if (0 == *pCRT) GOTO_END(ELCA_ERR_CRT);
      
      ConvertTextarea(pCRT);
      rc = CheckNewChainCRT(pCRT);
      if (ELCA_OK == rc)
      {
         s_puts("{", hs->s_stream);
         
         /* Server certificate */    
         s_printf(hs->s_stream, "\"srv_id_cn\":\"%s\",", NewCRTSrv.CN);
         s_printf(hs->s_stream, "\"srv_id_an\":\"%s\",", NewCRTSrv.AN);
         s_printf(hs->s_stream, "\"srv_id_o\":\"%s\",",  NewCRTSrv.O);
         s_printf(hs->s_stream, "\"srv_id_ou\":\"%s\",", NewCRTSrv.OU);
         s_printf(hs->s_stream, "\"srv_id_c\":\"%s\",",  NewCRTSrv.C);
         s_printf(hs->s_stream, "\"srv_id_nb\":\"%s\",", NewCRTSrv.vf);
         s_printf(hs->s_stream, "\"srv_id_na\":\"%s\",", NewCRTSrv.vt);
         s_printf(hs->s_stream, "\"srv_id_i\":\"%s\",",  NewCRTSrv.I);
         s_printf(hs->s_stream, "\"srv_id_s\":\"%s\",",  NewCRTSrv.S);

         /* Intermediate certificate */
         s_printf(hs->s_stream, "\"int_id_cn\":\"%s\",", NewCRTInt.CN);
         s_printf(hs->s_stream, "\"int_id_an\":\"%s\",", NewCRTInt.AN);
         s_printf(hs->s_stream, "\"int_id_o\":\"%s\",",  NewCRTInt.O);
         s_printf(hs->s_stream, "\"int_id_ou\":\"%s\",", NewCRTInt.OU);
         s_printf(hs->s_stream, "\"int_id_c\":\"%s\",",  NewCRTInt.C);
         s_printf(hs->s_stream, "\"int_id_nb\":\"%s\",", NewCRTInt.vf);
         s_printf(hs->s_stream, "\"int_id_na\":\"%s\",", NewCRTInt.vt);
         s_printf(hs->s_stream, "\"int_id_i\":\"%s\",",  NewCRTInt.I);
         s_printf(hs->s_stream, "\"int_id_s\":\"%s\"",   NewCRTInt.S);

         s_puts("}", hs->s_stream);
         s_flush(hs->s_stream);
      }
   }

   /* Check for final CRT update request */   
   if ((pMode != NULL) && (0 == strcmp(pMode, "update")))
   {
      if (0 == nUpdateStartDone)
      {
         nUpdateStartDone = 1;
         
         /* Hack, start Install task */   
         OS_TaskCreate(&TCBClient, ClientTaskUpdate, NULL, TASK_IP_ELCAC_PRIORITY,
                       ClientStack, sizeof(ClientStack), 
                       "ELCAupdate");
      }                       
      /* Send No error */
      rc = ELCA_OK;
      JSONSendError(hs, rc);
   }
   
   /* Check for the update result */
   if ((pMode != NULL) && (0 == strcmp(pMode, "result")))
   {
      rc = ELCA_OK;
      
      s_puts("{", hs->s_stream);

      s_printf(hs->s_stream, "\"result\":%d", nUpdateResult);

      s_puts("}", hs->s_stream);
      s_flush(hs->s_stream);
   }

end:  

   IP_JSON_Delete(&JSON);

   if (rc != ELCA_OK)
   {  
      JSONSendError(hs, rc);
   }   
   
   return(0);
} /* cgi_cert */


/*
 * SSI variable list
 */
static const SSI_EXT_LIST_ENTRY SSIList[] =
{
   {NULL, NULL}
};

/*
 * CGI variable list
 */
static const CGI_LIST_ENTRY CGIList[] =
{
   { "cgi-bin/cert.cgi", cgi_cert },

   {NULL, NULL}
};

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  IP_ELCAC_Init                                                        */
/*                                                                       */
/*  Initialize the ELCA functionality of the web server.                 */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_ELCAC_Init (void)
{
   IP_WEBS_SSIListAdd((SSI_EXT_LIST_ENTRY*)SSIList);
   IP_WEBS_CGIListAdd((CGI_LIST_ENTRY*)CGIList);
   
   /* Clear all data first */
   memset(KeyData,   0x00, sizeof(KeyData));
   memset(CSRData,   0x00, sizeof(CSRData));
   memset(CRTData,   0x00, sizeof(CRTData));
   memset(InterData, 0x00, sizeof(InterData));

} /* IP_ELCAC_Init */

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
   
      OS_TaskCreate(&TCBClient, ClientTaskStartup, NULL, TASK_IP_ELCAC_PRIORITY,
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

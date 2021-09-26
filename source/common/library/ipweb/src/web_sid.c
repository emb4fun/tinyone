/**************************************************************************
*  Copyright (c) 2019 by Michael Fischer (www.emb4fun.de).
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
*  09.03.2019  mifi  First Version.
**************************************************************************/
#define __WEB_SID_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/

#include <string.h>
#include <stdlib.h>
#include "tal.h"
#include "tcts.h"
#include "ipweb.h"
#include "xmem.h"

#if (_IP_WEB_SID_SUPPORT >= 1)

/*=======================================================================*/
/*  Extern                                                               */
/*=======================================================================*/

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

/*
 * The login is blocked after X incorrect entries
 */
#if !defined(IP_WEB_SID_LOGIN_ERROR_CNT_MAX)
#define _LOGIN_ERROR_CNT_MAX     3
#else
#define _LOGIN_ERROR_CNT_MAX     IP_WEB_SID_LOGIN_ERROR_CNT_MAX
#endif

/*
 * The login is blocked for X seconds
 */
#if !defined(IP_WEB_SID_LOGIN_TIMEOUT_SEC)
#define _LOGIN_TIMEOUT_SEC       60
#else
#define _LOGIN_TIMEOUT_SEC       IP_WEB_SID_LOGIN_TIMEOUT_SEC
#endif 

/*
 * The page is locked after X seconds of inactivity.
 */
#if !defined(IP_WEB_SID_TIMEOUT_SEC)
#define _SID_TIMEOUT_SEC         (3*60)
#else
#define _SID_TIMEOUT_SEC         (1*IP_WEB_SID_TIMEOUT_SEC)
#endif


#define SID_START                "sid="
#define SID_LEN                  32
#define SID_NONCE_LEN            16
#define SID_COOKIE_SIZE          90

#define SID_LIST_CNT             8

typedef struct _sid_
{
   int      nUserIndex;
   uint32_t dIPAddr;
   uint32_t dLastAccessTimeSec;
   int      nAccessGranted;
   uint32_t dPermission;
   char      StrSID[SID_LEN+1];           /* Add 1 for zero termination */    
   char      StrNonce[SID_NONCE_LEN+1];   /* Add 1 for zero termination */
} sid_t;

/*=======================================================================*/
/*  Definition of all global Data                                        */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

static sid_t  SIDList[SID_LIST_CNT];

static int      nLoginErrorCnt         = 0;
static int      nLoginBlocked          = 0;
static uint32_t dLoginBlockedStartTime = 0;

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  FindSIDEntry                                                         */
/*                                                                       */
/*  Find the session id for the given address, or a free entry.          */
/*                                                                       */
/*  In    : dIPAddr                                                      */
/*  Out   : none                                                         */
/*  Return: pSIDEntry                                                    */
/*************************************************************************/
static sid_t *FindSIDEntry (uint32_t dIPAddr, int *pIndex)
{
   sid_t   *pSIDEntry = NULL;
   int      nIndex;
   uint32_t dSecTime;
   
   /* 
    * Invalidate all entries with timeout first 
    */
   dSecTime = OS_TimeGetSeconds();
    
   for(nIndex = 0; nIndex < SID_LIST_CNT; nIndex++)
   {
      /* check if entry is in use */
      if (SIDList[nIndex].dIPAddr != 0)
      {
         /* Check timeout */
         if (OS_TEST_TIMEOUT(dSecTime, SIDList[nIndex].dLastAccessTimeSec, _SID_TIMEOUT_SEC))
         {
            /* Timeout => not valid anymore */
            SIDList[nIndex].dIPAddr        = 0;
            SIDList[nIndex].nAccessGranted = 0;
         }
      }    
   }
   
   /* 
    * Check if the ip address is used 
    */
   for(nIndex = 0; nIndex < SID_LIST_CNT; nIndex++)
   {
      if (dIPAddr == SIDList[nIndex].dIPAddr)
      {
         *pIndex   = nIndex;
         pSIDEntry = &SIDList[nIndex];
         break;
      }
   }
   
   /* 
    * Check for a free entry search 
    */
   if (NULL == pSIDEntry)
   {
      for(nIndex = 0; nIndex < SID_LIST_CNT; nIndex++)
      {
         if (0 == SIDList[nIndex].dIPAddr)
         {
            *pIndex   = nIndex;
            pSIDEntry = &SIDList[nIndex];
            break;
         }
      }
   }

   return(pSIDEntry);
} /* FindSIDEntry */

/*************************************************************************/
/*  CheckUserPassword                                                    */
/*                                                                       */
/*  Check if User/Password is valid.                                     */
/*                                                                       */
/*  In    : pSIDEntry, pUser, pPassword, pPermission                     */
/*  Out   : pPermission                                                  */
/*  Return: -1 == invalid / all other = valid                            */
/*************************************************************************/
static int CheckUserPassword (sid_t *pSIDEntry, char *pUser, char *pPassword, uint32_t *pPermission)
{
   int   nValid;
   
   (void)pSIDEntry;

   /* Check for valid user/password combination */
   nValid = WebUserCheckUserPassword(pUser, pPassword, pPermission);
   
   /* In case of an error increase the error counter */   
   if ((-1 == nValid) && (0 == nLoginBlocked) && (nLoginErrorCnt < _LOGIN_ERROR_CNT_MAX))
   {
      nLoginErrorCnt++;
      if (_LOGIN_ERROR_CNT_MAX == nLoginErrorCnt)
      {
         /* To many login error, block login for the next _LOGIN_TIMEOUT_SEC */
         nLoginBlocked = 1;
         dLoginBlockedStartTime = OS_TimeGetSeconds();
      }
   }
   
   /* For security, if login is blocked, invalidate the result again */
   if (1 == nLoginBlocked)
   {
      nValid = -1;
   }
   
   /* If no login error, clear error count */
   if (nValid != -1)
   {
      nLoginErrorCnt = 0;
   }
   
   return(nValid);
} /* CheckUserPassword */

/*************************************************************************/
/*  CreateSID                                                            */
/*                                                                       */
/*  Create a SID for he given IP address.                                */
/*                                                                       */
/*  In    : dIPAddr                                                      */
/*  Out   : none                                                         */
/*  Return: pSessionID                                                   */
/*************************************************************************/
static char *CreateSID (uint32_t dIPAddr)
{
   char         *pSessionID = NULL;
   sid_t        *pSIDEntry;
   int           nIndex;
   char           Hex[3];
   static uint8_t Nonce[(SID_LEN/2)];
   
   pSIDEntry = FindSIDEntry(dIPAddr, &nIndex);
   if (pSIDEntry != NULL)
   {   
      /*
       * Create SID
       */
      tal_CPURngHardwarePoll(Nonce, sizeof(Nonce));
      
      pSIDEntry->dIPAddr            = dIPAddr;
      pSIDEntry->dLastAccessTimeSec = OS_TimeGetSeconds();
      pSIDEntry->nAccessGranted     = 0;

      /* Save index in the first 2 digits */
      sprintf(pSIDEntry->StrSID, "%02X", nIndex);
      
      /* Save the rest of the Nonce */
      for(nIndex = 0; nIndex < ((SID_LEN/2)-1); nIndex++)
      {
         sprintf(Hex, "%02X", Nonce[nIndex]);
         strcat(pSIDEntry->StrSID, Hex);
      }
      
      pSessionID = pSIDEntry->StrSID;
   }
      
   return(pSessionID);
} /* CreateSID */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  WebSidInit                                                           */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void WebSidInit (void)
{
   memset(SIDList,  0x00, sizeof(SIDList));

} /* WebSidInit */

/*************************************************************************/
/*  WebSidParseCookie                                                    */
/*                                                                       */
/*  Parse the cookie to retrieve the SID.                                */
/*                                                                       */
/*  In    : pCookie                                                      */
/*  Out   : none                                                         */
/*  Return: pSessionID                                                   */
/*************************************************************************/
char *WebSidParseCookie (char *pCookie)
{
   char *pStart;
   char *pSessionID = NULL;
   
   if (pCookie != NULL)
   {
      pStart = strstr(pCookie, SID_START);
      if (pStart != NULL)
      {
         pStart += strlen(SID_START);
      
         pSessionID = xcalloc(XM_ID_WEB, 1, SID_LEN+1);         
         if (pSessionID != NULL)
         {
            memcpy(pSessionID, pStart, SID_LEN);
         }   
      }
   }      
   
   return(pSessionID);
} /* WebSidParseCookie */

/*************************************************************************/
/*  WebSidCreateCookie                                                   */
/*                                                                       */
/*  Create a cookie for the given session.                               */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: pCookie                                                      */
/*************************************************************************/
char *WebSidCreateCookie (HTTPD_SESSION *hs)
{
   char    *pCookie;
   char    *pSessionID;
   uint32_t dSrcIP;
   
   /* Get source IP address */   
   dSrcIP = hs->s_stream->strm_caddr.sin_addr.s_addr;
   
   pCookie = xmalloc(XM_ID_WEB, SID_COOKIE_SIZE);
   if (pCookie != NULL)
   {
      pSessionID = CreateSID(dSrcIP);
      if (pSessionID != NULL)
      {
         snprintf(pCookie, SID_COOKIE_SIZE, "%s%s; HttpOnly; SameSite=Lax; Path=/",
                  SID_START, pSessionID);
      }
      else
      {
         xfree(pCookie);
         pCookie = NULL;
      }                      
   }   

   return(pCookie);
} /* WebSidCreateCookie */

/*************************************************************************/
/*  WebSidCreateNonce                                                    */
/*                                                                       */
/*  Create a NONCE for the given session.                                */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: pNonce                                                       */
/*************************************************************************/
char *WebSidCreateNonce (HTTPD_SESSION *hs)
{
   char        *pNonce = NULL;
   int          nIndex;
   int          nSIDEntry;
   sid_t       *pSIDEntry;
   uint8_t       Nonce[(SID_NONCE_LEN/2)];   
   char          Hex[3];
   HTTP_REQUEST *req = &hs->s_req;

   /* Check first if the SID is valid */
   nSIDEntry = WebSidCheck(hs, req->req_sid, WEB_SID_HTTP); 
   if (nSIDEntry != -1)   
   {
      pSIDEntry = &SIDList[nSIDEntry];

      tal_CPURngHardwarePoll(Nonce, sizeof(Nonce));

      pSIDEntry->StrNonce[0] = 0;
      for(nIndex = 0; nIndex < (SID_NONCE_LEN/2); nIndex++)
      {
         sprintf(Hex, "%02X", Nonce[nIndex]);
         strcat(pSIDEntry->StrNonce, Hex);
      }
      pNonce = pSIDEntry->StrNonce;
   }

   return(pNonce);
} /* WebSidCreateNonce */

/*************************************************************************/
/*  WebSidCheck                                                          */
/*                                                                       */
/*  Check if the SessionID is valid.                                     */
/*                                                                       */
/*  Return -1 if the SID is invalid, otherwise return the position       */ 
/*  in the SIDList.                                                      */
/*                                                                       */
/*  In    : hs, pSessionID, nIsHttp                                      */
/*  Out   : none                                                         */
/*  Return: -1 == invalid / nSIDEntry                                    */
/*************************************************************************/
int WebSidCheck (HTTPD_SESSION *hs, char *pSessionID, int nIsHttp)
{
   int      nSIDEntry = -1;
   int      nIndex;
   sid_t   *pSIDEntry = NULL;
   uint32_t dIPAddr;
   char      Hex[3];
   
   /* Get source IP address */   
   dIPAddr = hs->s_stream->strm_caddr.sin_addr.s_addr;
   
   if (pSessionID != NULL)
   {
      Hex[0] = pSessionID[0];
      Hex[1] = pSessionID[1];
      Hex[2] = 0;
      nIndex = atoi(Hex);
      
      if (nIndex < SID_LIST_CNT)
      {
         pSIDEntry = &SIDList[nIndex];
         
         if( (dIPAddr == pSIDEntry->dIPAddr)                    &&
             (0 == memcmp(pSIDEntry->StrSID, pSessionID, SID_LEN)) )
         {      
            /* Check HTTP timeout */       
            if (OS_TEST_TIMEOUT(OS_TimeGetSeconds(), pSIDEntry->dLastAccessTimeSec, _SID_TIMEOUT_SEC))
            {
               /* Timeout => not valid anymore */
               nSIDEntry = -1;
               pSIDEntry->dIPAddr        = 0;
               pSIDEntry->nAccessGranted = 0;
            }
            else
            {
               /* Only HTTP calls retriggers => valid */
               if (WEB_SID_HTTP == nIsHttp)
               {
                  pSIDEntry->dLastAccessTimeSec = OS_TimeGetSeconds();
               }   
            
               nSIDEntry = nIndex;
            }
         }             
      }
   }
      
   return(nSIDEntry);
} /* WebSidCheck */

/*************************************************************************/
/*  WebSidCheckAccessGranted                                             */
/*                                                                       */
/*  Check if the access is granted.                                      */
/*  Return -1 if the SID is invalid, otherwise return the position       */ 
/*  in the SIDList.                                                      */
/*                                                                       */
/*  In    : hs, pSessionID, nIsHttp                                      */
/*  Out   : none                                                         */
/*  Return: 1 == granted / otherwise not granted                         */
/*************************************************************************/
int WebSidCheckAccessGranted (HTTPD_SESSION *hs, char *pSessionID, int nIsHttp)
{
   int      nGranted = 0;
   int      nIndex;
   sid_t   *pSIDEntry = NULL;
   uint32_t dIPAddr;
   char      Hex[3];
   
   (void)nIsHttp;
   
   /* Get source IP address */   
   dIPAddr = hs->s_stream->strm_caddr.sin_addr.s_addr;
   
   if (pSessionID != NULL)
   {
      Hex[0] = pSessionID[0];
      Hex[1] = pSessionID[1];
      Hex[2] = 0;
      nIndex = atoi(Hex);
      
      if (nIndex < SID_LIST_CNT)
      {
         pSIDEntry = &SIDList[nIndex];
         
         if( (dIPAddr == pSIDEntry->dIPAddr)                       &&
             (0 == memcmp(pSIDEntry->StrSID, pSessionID, SID_LEN)) &&
             (1 == pSIDEntry->nAccessGranted)                      )
         {     
            if (OS_TEST_TIMEOUT(OS_TimeGetSeconds(), pSIDEntry->dLastAccessTimeSec, _SID_TIMEOUT_SEC))
            {
               /* Timeout => not valid anymore */
               pSIDEntry->nAccessGranted = 0;
            }
            else
            {
               nGranted = 1;
            }
         }             
      }
   }
      
   return(nGranted);
} /* WebSidCheckAccessGranted */

/*************************************************************************/
/*  WebSidCheckUserPass                                                  */
/*                                                                       */
/*  Check if User/Password combination is valid.                         */
/*                                                                       */
/*  In    : hs, pUser, pPass                                             */
/*  Out   : none                                                         */
/*  Return: 0 = blocked / 1 = not blocked                                */
/*************************************************************************/
int WebSidCheckUserPass (HTTPD_SESSION *hs, char *pUser, char *pPass)
{
   int          nValid = 0;
   int          nUserPassValid;
   uint32_t     dPermission;
   int          nSIDEntry;
   sid_t       *pSIDEntry;   
   HTTP_REQUEST *req = &hs->s_req;
   
   /* Check first if the SID is valid */
   nSIDEntry = WebSidCheck(hs, req->req_sid, FALSE); 
   if (nSIDEntry != -1)
   {
      pSIDEntry = &SIDList[nSIDEntry];
   
      /* Check if User and Password are valid */
      nUserPassValid = CheckUserPassword(pSIDEntry, pUser, pPass, &dPermission);
      if (nUserPassValid != -1)
      {
         nValid = 1;
         pSIDEntry->nUserIndex     = nUserPassValid;
         pSIDEntry->nAccessGranted = 1;
         pSIDEntry->dPermission    = dPermission;
      }
      else
      {
         /* Not valid */
         pSIDEntry->nUserIndex     = 0;
         pSIDEntry->nAccessGranted = 0;
         pSIDEntry->dPermission    = 0;
      }
   }
   
   return(nValid);
} /* WebSidCheckUserPass */

/*************************************************************************/
/*  WebSidInvalidate                                                     */
/*                                                                       */
/*  Invalidate session id if available.                                  */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void WebSidInvalidate (HTTPD_SESSION *hs)
{
   char    *pSessionID = hs->s_req.req_sid;
   sid_t   *pSIDEntry;
   uint32_t dIPAddr;
   int      nIndex;
   char      Hex[3];

   /* Get source IP address */   
   dIPAddr = hs->s_stream->strm_caddr.sin_addr.s_addr;
   
   if (pSessionID != NULL)
   {
      Hex[0] = pSessionID[0];
      Hex[1] = pSessionID[1];
      Hex[2] = 0;
      nIndex = atoi(Hex);
      
      if (nIndex < SID_LIST_CNT)
      {
         pSIDEntry = &SIDList[nIndex];
         
         if( (dIPAddr == pSIDEntry->dIPAddr)                       &&
             (0 == memcmp(pSIDEntry->StrSID, pSessionID, SID_LEN)) )
         {
            pSIDEntry->dIPAddr        = 0;
            pSIDEntry->nAccessGranted = 0;
         }             
      }
   }
   
} /* WebSidInvalidate */

/*************************************************************************/
/*  WebSidGetPermission                                                  */
/*                                                                       */
/*  Return the permission for session.                                   */
/*                                                                       */
/*  In    : nSIDEntry                                                    */
/*  Out   : none                                                         */
/*  Return: dPermission                                                  */
/*************************************************************************/
uint32_t WebSidGetPermission (int nSIDEntry)
{
   uint32_t dPermission = 0;

   if (nSIDEntry < SID_LIST_CNT)
   {
      dPermission = SIDList[nSIDEntry].dPermission;
   }
   
   return(dPermission);
} /* WebSidGetPermission */

/*************************************************************************/
/*  WebSidGetUser                                                        */
/*                                                                       */
/*  Return the user for the session.                                     */
/*                                                                       */
/*  In    : nSIDEntry                                                    */
/*  Out   : none                                                         */
/*  Return: NULL = invalid / otherwise user pointer                      */
/*************************************************************************/
char *WebSidGetUser (int nSIDEntry)
{
   char *pUser = NULL;

   if (nSIDEntry < SID_LIST_CNT)
   {
      if (SIDList[nSIDEntry].nUserIndex != -1)
      {
         pUser = WebUserGetUser((uint8_t)SIDList[nSIDEntry].nUserIndex);
      }
   }
   
   return(pUser);
} /* WebSidGetUser */

/*************************************************************************/
/*  WebSidSetNewPass                                                     */
/*                                                                       */
/*  Set the new password if the old one match the session.               */
/*                                                                       */
/*  In    : hs, pPassUser, pPassNew                                      */
/*  Out   : none                                                         */
/*  Return: nValid < 0 = invalid / otherwise valid                       */
/*************************************************************************/
int WebSidSetNewPass (HTTPD_SESSION *hs, char *pPassUser, char *pPassNew)
{
   int    nValid = -1;
   int    nSIDEntry;
   sid_t *pSIDEntry;
         
   /* Check first if the SID is valid */
   nSIDEntry = WebSidCheck(hs, hs->s_req.req_sid, FALSE);
   if (nSIDEntry != -1)
   {
      pSIDEntry = &SIDList[nSIDEntry];
      (void)pSIDEntry;
      
      nValid = WebUserSetNewPass((char*)hs->s_req.req_sid_user, pPassUser, pPassNew);
      
      /*
       * nValid = -1: Incorrect Old Password entered
       * nValid = -2: The password does not meet the password policy requirements
       */
   }   
   
   return(nValid);
} /* WebSidSetNewPass */

/*************************************************************************/
/*  WebSidLoginInit                                                      */
/*                                                                       */
/*  Return the info if login must be initialize.                         */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 1 = must be init / 0 = not init needed                       */
/*************************************************************************/
int WebSidLoginInit (void)
{
   return( WebUserIsFirstUser() );
} /* WebSidLoginInit */

/*************************************************************************/
/*  WebSidLoginInitSet                                                   */
/*                                                                       */
/*  Set the new password for the admin.                                  */
/*                                                                       */
/*  In    : hs, pNewPass                                                 */
/*  Out   : none                                                         */
/*  Return: -1 = invalid / otherwise valid                               */
/*************************************************************************/
int WebSidLoginInitSet (HTTPD_SESSION *hs, char *pNewPass)
{
   int nValid = -1;
   int nSIDEntry;
   
   if (pNewPass != NULL)
   {
      /* Check first if the SID is valid */
      nSIDEntry = WebSidCheck(hs, hs->s_req.req_sid, FALSE);
      if (nSIDEntry != -1)
      {
         nValid = 0;
         WebUserFirstUserSet(pNewPass);
      }
   }      

   return(nValid);
} /* WebSidLoginInitSet */

/*************************************************************************/
/*  WebSidLoginBlocked                                                   */
/*                                                                       */
/*  Return the info if the login is blocked.                             */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 1 = blocked / 0 = not blocked                                */
/*************************************************************************/
int WebSidLoginBlocked (void)
{
   return(nLoginBlocked);
} /* WebSidLoginBlocked */

/*************************************************************************/
/*  WebSidLoginBlockedTime                                               */
/*                                                                       */
/*  Return the bloked timeout in seconds.                                */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: nBlockedTime                                                 */
/*************************************************************************/
uint32_t WebSidLoginBlockedTime (void)
{
   uint32_t dBlockedTime = 0;

   if (1 == nLoginBlocked)
   {
      if (OS_TEST_TIMEOUT(OS_TimeGetSeconds(), dLoginBlockedStartTime, _LOGIN_TIMEOUT_SEC))
      {
         nLoginBlocked  = 0;
         nLoginErrorCnt = 0;
      }
      else
      {
         dBlockedTime = _LOGIN_TIMEOUT_SEC - (OS_TimeGetSeconds() - dLoginBlockedStartTime);
      }
   }
   
   return(dBlockedTime);
} /* WebSidLoginBlockedTime */

/*************************************************************************/
/*  WebSidLogoutTime                                                     */
/*                                                                       */
/*  Return the logout time in seconds.                                   */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: nBlockedTime                                                 */
/*************************************************************************/
uint32_t WebSidLogoutTime (HTTPD_SESSION *hs)
{
   HTTP_REQUEST *req;
   int32_t      nTime = 0;
   sid_t       *pSIDEntry;
   int          nIndex;
   uint32_t     dIPAddr;
   uint32_t     dSecTime;
   char          Hex[3];
   
   if (hs != NULL)
   {
      req = &hs->s_req;

      /* Get source IP address */   
      dIPAddr = hs->s_stream->strm_caddr.sin_addr.s_addr;

      Hex[0] = req->req_sid[0];
      Hex[1] = req->req_sid[1];
      Hex[2] = 0;
      nIndex = atoi(Hex);

      if (nIndex < SID_LIST_CNT)
      {
         pSIDEntry = &SIDList[nIndex];
         
         if ((dIPAddr == pSIDEntry->dIPAddr) && (1 == pSIDEntry->nAccessGranted))
         {
            /* Check timeout */       
            dSecTime = OS_TimeGetSeconds();
            if (OS_TEST_TIMEOUT(dSecTime, pSIDEntry->dLastAccessTimeSec, _SID_TIMEOUT_SEC))
            {
               nTime = 0;
            }
            else
            {
               nTime = (int32_t)(_SID_TIMEOUT_SEC - (dSecTime - pSIDEntry->dLastAccessTimeSec));
               if (nTime < 0)
               {
                  nTime = 0;
               }
            }
         }
      }         
   }
   
   return((uint32_t)nTime);
} /* WebSidLogoutTime */

#endif /* (_IP_WEB_SID_SUPPORT >= 1) */

/*** EOF ***/

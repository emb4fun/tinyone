/**************************************************************************
*  Copyright (c) 2020 by Michael Fischer (www.emb4fun.de).
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
*  31.10.2020  mifi  First Version.
**************************************************************************/
#define __WEB_LOGIN_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/

#include <string.h>
#include "tal.h"
#include "ipstack.h"
#include "ipweb.h"
#include "xmem.h"

#include "mbedtls/base64.h"

#if (_IP_WEB_SID_SUPPORT >= 1)

/*lint -save -e801*/

/*=======================================================================*/
/*  Extern                                                               */
/*=======================================================================*/

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

#define LOGIN_OK              0  
#define LOGIN_ERR_USER_PASS   -1
#define LOGIN_ERR_BLOCKED     -2
/* The lines above must not changes */
#define LOGIN_ERR             -3
#define LOGIN_ERR_USER        -4
#define LOGIN_ERR_PASS_RULE   -5
#define LOGIN_ERR_RESET       -6
#define LOGIN_ERR_PASS_OTP    -7
#define LOGIN_ERR_PASS        -8

#define LOGIN_MODE_TOTP       1


#define GOTO_END(_a)  { rc = _a; goto end; }

/*=======================================================================*/
/*  Definition of all global Data                                        */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  JSONSendErrorAndCode                                                 */
/*                                                                       */
/*  In    : hs, nError, pMsg                                             */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void JSONSendErrorAndCode (HTTPD_SESSION *hs, int nError, char *pCode)
{
   s_puts("{", hs->s_stream);
   
   if (0 == nError)
   {
      s_printf(hs->s_stream, "\"err\":0,\"msg\":\"none\",\"code\":\"%s\"", pCode);
   }
   else
   {
      s_printf(hs->s_stream, "\"err\":%d,\"msg\":\"error\"", nError);
   }

   s_puts("}", hs->s_stream);
   s_flush(hs->s_stream);

} /* JSONSendErrorAndCode */

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
/*  sys_login_init                                                       */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_login_init (HTTPD_SESSION *hs)
{
   if (0 == WebSidLoginInit())
   {
      s_printf(hs->s_stream, "0");
   }
   else
   {
      s_printf(hs->s_stream, "1");
   }   

   s_flush(hs->s_stream);

   return(0);
} /* sys_login_init */

/*************************************************************************/
/*  sys_login_blocked                                                    */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_login_blocked (HTTPD_SESSION *hs)
{
   if (0 == WebSidLoginBlocked())
   {
      s_printf(hs->s_stream, "0");
   }
   else
   {
      s_printf(hs->s_stream, "1");
   }   

   s_flush(hs->s_stream);

   return(0);
} /* sys_login_blocked */

/*************************************************************************/
/*  sys_login_blocked_time                                               */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_login_blocked_time (HTTPD_SESSION *hs)
{
   s_printf(hs->s_stream, "%d", WebSidLoginBlockedTime());
   
   s_flush(hs->s_stream);

   return(0);
} /* sys_login_blocked_time */

/*************************************************************************/
/*  sys_login_user                                                       */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_login_user (HTTPD_SESSION *hs)
{
   if (hs->s_req.req_sid_user != NULL)
   {
      s_printf(hs->s_stream, "%s", hs->s_req.req_sid_user);
   }
   else
   {
      s_printf(hs->s_stream, "???");   /*lint !e585*/
   }
   
   s_flush(hs->s_stream);

   return(0);
} /* sys_login_user */

/*************************************************************************/
/*  sys_login_totp                                                       */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_login_totp (HTTPD_SESSION *hs)
{
   char Code[20];
   
   WebUserGetTOTPSecretBase32((char*)hs->s_req.req_sid_user, Code, sizeof(Code));
   
   s_printf(hs->s_stream, "\"%s\"", Code);
   
   s_flush(hs->s_stream);

   return(0);
} /* sys_login_totp */

/*************************************************************************/
/*  sys_login_totp_ena                                                   */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_login_totp_ena (HTTPD_SESSION *hs)
{
   if (0 == WebUserIsTOTP((char*)hs->s_req.req_sid_user))
   {
      s_printf(hs->s_stream, "0");
   }
   else
   {
      s_printf(hs->s_stream, "1");
   }   

   s_flush(hs->s_stream);

   return(0);
} /* sys_login_totp_ena */

/*************************************************************************/
/*  sys_login_totp_issuer                                                */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_login_totp_issuer (HTTPD_SESSION *hs)
{
#if !defined(IP_WEB_TOTP_ISSUER)
   s_printf(hs->s_stream, "\"TinyONE\"");
#else
   s_printf(hs->s_stream, "\"%s\"", IP_WEB_TOTP_ISSUER);
#endif   

   s_flush(hs->s_stream);

   return(0);
} /* sys_login_totp_issuer */

/*************************************************************************/
/*  sys_login_totp_label                                                */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_login_totp_label (HTTPD_SESSION *hs)
{
   char *Hostname;
   
   if (hs->s_req.req_sid_user != NULL)
   {
      s_printf(hs->s_stream, "\"%s@", hs->s_req.req_sid_user);
   }
   else
   {
      s_printf(hs->s_stream, "\"user@");
   }    
   
   Hostname = IP_IF_HostnameGet(0);
   if (Hostname != NULL)
   {
      s_printf(hs->s_stream, "%s\"", Hostname);
   }
   else
   {
      s_printf(hs->s_stream, "null\"");
   }

   s_flush(hs->s_stream);

   return(0);
} /* sys_login_totp_label */

/*************************************************************************/
/*  sys_nonce                                                            */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_nonce (HTTPD_SESSION *hs)
{
   char *pNonce;
   
   pNonce = WebSidCreateNonce(hs);
   if (pNonce != NULL)
   {
      s_printf(hs->s_stream, "%s", pNonce);
   }
   else
   {
      s_printf(hs->s_stream, "00000000");
   } 
   
   s_flush(hs->s_stream);

   return(0);
} /* sys_nonce */

/*************************************************************************/
/*  sys_logout_sec                                                       */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int sys_logout_sec (HTTPD_SESSION *hs)
{
   uint32_t dTime = WebSidLogoutTime(hs);
   
   s_printf(hs->s_stream, "%d", dTime);
   s_flush(hs->s_stream);

   return(0);
} /* sys_logout_sec */

/*************************************************************************/
/*  Login                                                                */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int Login (HTTPD_SESSION *hs)
{
   int      rc;
   size_t   olen;
   json_t  JSON;  
   char   *pMode; 
   char   *pUser;
   char   *pPass;
   char   *pCode;
   char   *pResetCode;
   char     RequestCode[20];
   uint8_t  UserMode = USER_MODE_NORMAL;

   RequestCode[0] = 0;

   IP_WEBS_CGISendHeader(hs);

   rc = IP_JSON_ParseHS(hs, &JSON, 16);
   if (-1 == rc) GOTO_END(LOGIN_ERR);

   /* Check if the access is blocked for the moment */   
   rc = WebSidLoginBlocked();
   if (1 == rc) GOTO_END(LOGIN_ERR_BLOCKED);
   
   /* The mode can be NULL or "reset" */
   pMode = IP_JSON_GetString(&JSON, "mode");


   /*
    * "login"
    */
   if (NULL == pMode)
   {
      pPass = IP_JSON_GetString(&JSON, "pass");
      if (NULL == pPass) GOTO_END(LOGIN_ERR);

      rc = mbedtls_base64_decode(pPass, strlen(pPass), &olen, pPass, strlen(pPass)); /*lint !e64*/
      if (rc != 0) GOTO_END(LOGIN_ERR);
      pPass[olen] = 0;

      /* Check for INIT mode */
      if (0 == WebSidLoginInit())
      {
         /* No INIT mode */
         pUser = IP_JSON_GetString(&JSON, "user");
         if (NULL == pUser) GOTO_END(LOGIN_ERR);

         /* Check if the user is valid, and get the mode */
         rc = WebUserIsValid(pUser, &UserMode);
         if (-1 == rc)
         {
            /* Invalid user */
            rc =  LOGIN_ERR_USER_PASS;

            /* Set SID error */
            WebSidErrorCntSet(1);
            if (1 == WebSidLoginBlocked())
            {
               rc = LOGIN_ERR_BLOCKED;
            }
         }
         else
         {
            /* Valid user, and valid UserMode */
            
            if (USER_MODE_NORMAL == UserMode)
            {
               /* Check user and password */
               if (0 == WebSidCheckUserPass(hs, pUser, pPass))
               {
                  /* Login not valid, check if it is blocked */
                  if (0 == WebSidLoginBlocked())
                  {
                     /* Not valid, but must not blocked */
                     rc = LOGIN_ERR_USER_PASS;
                  }
                  else
                  {
                     /* Error, must be blocked */
                     rc = LOGIN_ERR_BLOCKED;
            
                     /* A new NONCE must be created for a new login */
                     WebSidCreateNonce(hs);  
                  }
               }
               else
               {
                  /* User and password are ok */
                  rc = LOGIN_OK;
               }
            } /* end if (USER_MODE_NORMAL == UserMode) */
            
            if (USER_MODE_TOTP == UserMode)
            {
               /* This is the login for "normal" mode, but must switch for TOTP mode */
               rc = LOGIN_MODE_TOTP;
            }
                           
         } /* end if WebUserIsValid(pUser, &UserMode) */
      }
      else      
      {
         /* INIT mode */
      
         if (0 == WebSidLoginInitSet(hs, pPass))
         { 
            /* No error */
            rc = LOGIN_OK;
         
            /* A new NONCE must be created for a new login */
            WebSidCreateNonce(hs);  
         }
         else
         {
            /* Error */
            rc = LOGIN_ERR;
         }         
      }
   } /* end if (NULL == pMode) */      


   /*
    * "reset1"
    */
   if ((pMode != NULL) && (0 == strcmp(pMode, "reset1")))
   {
      pUser = IP_JSON_GetString(&JSON, "user");
      if (NULL == pUser) GOTO_END(LOGIN_ERR);
         
      rc = WebUserIsValid(pUser, NULL);
      if (0 == rc)
      {
         WebSidErrorCntSet(0);
         WebUserGetResetRequestCode(pUser, RequestCode, sizeof(RequestCode));
      }
      else
      {
         /* Invalid user */
         rc =  LOGIN_ERR_USER;

         /* Set SID error */
         WebSidErrorCntSet(1);
         if (1 == WebSidLoginBlocked())
         {
            rc = LOGIN_ERR_BLOCKED;
         }
      }
   } /* ((pMode != NULL) && (0 == strcmp(pMode == "reset1")) */


   /*
    * "reset2"
    */
   if ((pMode != NULL) && (0 == strcmp(pMode, "reset2")))
   {
      pUser = IP_JSON_GetString(&JSON, "user");
      if (NULL == pUser) GOTO_END(LOGIN_ERR);
      
      pPass = IP_JSON_GetString(&JSON, "pass");
      if (NULL == pPass) GOTO_END(LOGIN_ERR);
      
      rc = mbedtls_base64_decode(pPass, strlen(pPass), &olen, pPass, strlen(pPass)); /*lint !e64*/
      if (rc != 0) GOTO_END(LOGIN_ERR);
      pPass[olen] = 0;
      
      pResetCode = IP_JSON_GetString(&JSON, "code");
      if (NULL == pResetCode) GOTO_END(LOGIN_ERR);
         
      rc = WebUserPasswordReset(pUser, pPass, pResetCode); 
      if (0 == rc)
      {
         /* No error */
         WebSidErrorCntSet(0);
         WebSidInvalidate(hs);
      }
      else
      {
         /* Error */
         switch (rc)
         {
            case -1:  rc = LOGIN_ERR_USER;      break;
            case -2:  rc = LOGIN_ERR_PASS_RULE; break;
            case -3:  rc = LOGIN_ERR_RESET;     break;
            default:  rc = LOGIN_ERR;           break;
         }
         
         /* Check for reset error, this could be a start of a brute force attack */   
         if (LOGIN_ERR_RESET == rc)
         {
            /* Set SID error */
            WebSidErrorCntSet(1);
            if (1 == WebSidLoginBlocked())
            {
               rc = LOGIN_ERR_BLOCKED;
            }
         }
      }
   } /* ((pMode != NULL) && (0 == strcmp(pMode == "reset1")) */
   

   /*
    * "totp"
    */
   if ((pMode != NULL) && (0 == strcmp(pMode, "totp")))
   {
      pPass = IP_JSON_GetString(&JSON, "pass");
      if (NULL == pPass) GOTO_END(LOGIN_ERR);

      rc = mbedtls_base64_decode(pPass, strlen(pPass), &olen, pPass, strlen(pPass)); /*lint !e64*/
      if (rc != 0) GOTO_END(LOGIN_ERR);
      pPass[olen] = 0;

      pUser = IP_JSON_GetString(&JSON, "user");
      if (NULL == pUser) GOTO_END(LOGIN_ERR);
      
      pCode = IP_JSON_GetString(&JSON, "code");
      if (NULL == pCode) GOTO_END(LOGIN_ERR);
      
      /* Check user, password and code */
      if (0 == WebSidCheckUserPassTOTP(hs, pUser, pPass, (uint32_t)atol(pCode)))
      {
         /* Login not valid, check if it is blocked */
         if (0 == WebSidLoginBlocked())
         {
            /* Not valid, but must not blocked */
            rc = LOGIN_ERR_PASS_OTP;
         }
         else
         {
            /* Error, must be blocked */
            rc = LOGIN_ERR_BLOCKED;
      
            /* A new NONCE must be created for a new login */
            WebSidCreateNonce(hs);  
         }
      }
      else
      {
         /* User and password are ok */
         rc = LOGIN_OK;
      }
   } /* ((pMode != NULL) && (0 == strcmp(pMode == "totp")) */
   

end:  

   IP_JSON_Delete(&JSON);
   
   if (0 == RequestCode[0])
   {
      JSONSendError(hs, rc);
   }
   else
   {
      JSONSendErrorAndCode(hs, rc, RequestCode);
   }   
   
   return(0);
} /* Login */

/*************************************************************************/
/*  Logout                                                               */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int Logout (HTTPD_SESSION *hs)
{
   long      Avail;
   char    *pArg;
   char    *pVal;

   Avail = hs->s_req.req_length;
   while (Avail) 
   {
      pArg = HttpArgReadNext(hs, &Avail);
      if (pArg != NULL)
      {
         pVal = HttpArgValue(&hs->s_req);
         if (pVal)
         {
//            term_printf("%s: %s\r\n", pArg, pVal);
         }            
      }
   } 

   WebSidInvalidate(hs);

   HttpSendRedirectionCookie(hs, "sid=0; HttpOnly; Path=/", 303, "/login.htm", NULL);      
   
   return(0);
} /* Logout */


/*************************************************************************/
/*  ManageTOTP                                                           */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int ManageTOTP (HTTPD_SESSION *hs)
{
   int      rc;
   size_t   olen;
   json_t  JSON;  
   char   *pMode;
   char   *pCode; 
   char   *pPass;

   IP_WEBS_CGISendHeader(hs);

   rc = IP_JSON_ParseHS(hs, &JSON, 8);
   if (-1 == rc) GOTO_END(LOGIN_ERR);

   /* Check if the access is blocked for the moment */   
   rc = WebSidLoginBlocked();
   if (1 == rc) GOTO_END(LOGIN_ERR_BLOCKED);
   
   /* The mode can be "enable" or "disable" */
   pMode = IP_JSON_GetString(&JSON, "mode");

   /*
    * "enable"
    */
   if ((pMode != NULL) && (0 == strcmp(pMode, "enable")))
   {
      pCode = IP_JSON_GetString(&JSON, "code");
      if (NULL == pCode) GOTO_END(LOGIN_ERR);

      /* Enable TOTP if the code is valid */
      rc = WebUserTOTPEnable((char*)hs->s_req.req_sid_user, (uint32_t)atol(pCode));
      if (-1 == rc)
      {
         /* Invalid user */
         rc = LOGIN_ERR_USER_PASS;
      }
      else
      {
         /* Invalidate session, to force a new login with the new password */
         WebSidInvalidate(hs);
      
      } /* end if WebUserTOTPIsValid */
   }
   
   /*
    * "disable"
    */
   else if ((pMode != NULL) && (0 == strcmp(pMode, "disable")))
   {
      pPass = IP_JSON_GetString(&JSON, "pass");
      if (NULL == pPass) GOTO_END(LOGIN_ERR);

      rc = mbedtls_base64_decode(pPass, strlen(pPass), &olen, pPass, strlen(pPass)); /*lint !e64*/
      if (rc != 0) GOTO_END(LOGIN_ERR);
      pPass[olen] = 0;

      /* Disable TOTP if the pass is valid */
      rc = WebUserTOTPDisable((char*)hs->s_req.req_sid_user, pPass);
      if (-1 == rc)
      {
         /* Invalid pass */
         rc = LOGIN_ERR_PASS;
         
         /* Set SID error */
         WebSidErrorCntSet(1);
         if (1 == WebSidLoginBlocked())
         {
            rc = LOGIN_ERR_BLOCKED;
         }
      }
   }
   
   /*
    * Invalid mode
    */
   else
   {
      rc = LOGIN_ERR;
   }
   

end:  

   IP_JSON_Delete(&JSON);
  
   JSONSendError(hs, rc);
   
   return(0);
} /* ManageTOTP */

/*************************************************************************/
/*  ChangePass                                                           */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int ChangePass (HTTPD_SESSION *hs)
{
   int     rc;
   long    Avail;
   char  *pArg;
   char  *pVal;
   char  *pStr;
   char  *pOldPass = NULL;
   char  *pNewPass = NULL;
   char  *pRedir   = NULL;
   
   /* Check if the access is blocked for the moment */   
   rc = WebSidLoginBlocked();
   if (0 == rc)
   {
      /* Not blocked */ 

      Avail = hs->s_req.req_length;
      while (Avail) 
      {
         pArg = HttpArgReadNext(hs, &Avail);
         if (pArg != NULL)
         {
            pVal = HttpArgValue(&hs->s_req);
            if (pVal)
            {
//               term_printf("%s: %s\r\n", pArg, pVal);
            
               if      (strcmp(pArg, "old_pass") == 0)
               {
                  pOldPass = xstrdup(XM_ID_WEB, pVal);
               }
               else if (strcmp(pArg, "new_pass") == 0)
               {
                  pNewPass = xstrdup(XM_ID_WEB, pVal);
               }
               else if (strcmp(pArg, "redir") == 0)
               {
                  pRedir = xstrdup(XM_ID_WEB, pVal);
               }
            }            
         }
      } 

      if (pRedir != NULL)
      {
         /* New and New2 are equal, set New if Old is valid */
         rc = WebSidSetNewPass(hs, pOldPass, pNewPass);
         if (rc < 0)
         {
            /* Error, old password not valid */
            pStr = xmalloc(XM_ID_WEB, strlen(pRedir) + 16);
            if (pStr != NULL)
            {
               sprintf(pStr, "%s?err=%d", pRedir, rc);
               xfree(pRedir);
               pRedir = pStr;
            }
         }
         else
         {
            /* Invalidate session, to force a new login with the new password */
            WebSidInvalidate(hs);
         
            /* Change redir to "/index.htm" */
            pStr = xmalloc(XM_ID_WEB, 16);
            if (pStr != NULL)
            {
               sprintf(pStr, "/index.htm");
               xfree(pRedir);
               pRedir = pStr;
            }
         }
   
         HttpSendRedirection(hs, 303, pRedir, NULL);
      }

      xfree(pOldPass);
      xfree(pNewPass);
      xfree(pRedir);
   }
            
   return(0);
} /* ChangePass */


/*
 * SSI variable list
 */
static const SSI_EXT_LIST_ENTRY SSIList[] =
{
   { "sys_login_init",           sys_login_init         },
   { "sys_login_blocked",        sys_login_blocked      },
   { "sys_login_blocked_time",   sys_login_blocked_time },
   { "sys_login_user",           sys_login_user         },
   { "sys_login_totp",           sys_login_totp         },
   { "sys_login_totp_ena",       sys_login_totp_ena     },
   { "sys_login_totp_issuer",    sys_login_totp_issuer  },
   { "sys_login_totp_label",     sys_login_totp_label   },
   { "sys_nonce",                sys_nonce              },
   { "sys_logout_sec",           sys_logout_sec         },
   
   {NULL, NULL}
};

/*
 * CGI variable list
 */
static const CGI_LIST_ENTRY CGIList[] =
{
   { "cgi-bin/login.cgi",        Login      },
   { "cgi-bin/logout.cgi",       Logout     },
   { "cgi-bin/totp.cgi",         ManageTOTP },   
   { "cgi-bin/change_pass.cgi",  ChangePass },   

   {NULL, NULL}
};

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  IP_WEBS_LoginInit                                                    */
/*                                                                       */
/*  Initialize the Login functionality of the web server.                */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_WEBS_LoginInit (void)
{
   IP_WEBS_SSIListAdd((SSI_EXT_LIST_ENTRY*)SSIList);
   IP_WEBS_CGIListAdd((CGI_LIST_ENTRY*)CGIList);
   
   WebUserInit();

} /* IP_WEBS_LoginInit */

/*lint -restore*/

#endif /* (_IP_WEB_SID_SUPPORT >= 1) */

/*** EOF ***/

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
#include "ipweb.h"
#include "xmem.h"

#include "mbedtls/base64.h"

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
   char   *pUser;
   char   *pPass;

   IP_WEBS_CGISendHeader(hs);

   rc = IP_JSON_ParseHS(hs, &JSON, 8);
   if (-1 == rc) GOTO_END(LOGIN_ERR);

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

      /* Check user and password */
      if (0 == WebSidCheckUserPass(hs, pUser, pPass))
      {
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

end:  

   IP_JSON_Delete(&JSON);
   JSONSendError(hs, rc);
   
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
   { "sys_nonce",                sys_nonce              },
   
   {NULL, NULL}
};

/*
 * CGI variable list
 */
static const CGI_LIST_ENTRY CGIList[] =
{
   { "cgi-bin/login.cgi",        Login       },
   { "cgi-bin/logout.cgi",       Logout      },
   { "cgi-bin/change_pass.cgi",  ChangePass  },   


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

/*** EOF ***/

/**************************************************************************
*  Copyright (c) 2022 by Michael Fischer (www.emb4fun.de).
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
*  08.10.2022  mifi  First Version.
**************************************************************************/
#define __WEB_LOGIN_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/

#include <string.h>
#include "tal.h"
#include "ipweb.h"
#include "xmem.h"

#if (_IP_WEB_SID_SUPPORT == 0)

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
   { "sys_logout_sec",           sys_logout_sec         },
   
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
   
   WebUserInit();

} /* IP_WEBS_LoginInit */

/*lint -restore*/

#endif /* (_IP_WEB_SID_SUPPORT == 0) */

/*** EOF ***/

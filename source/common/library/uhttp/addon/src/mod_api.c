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
*  2. Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the 
*     documentation and/or other materials provided with the distribution.
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
*  06.08.2022  mifi  First Version.
**************************************************************************/
#define __MOD_API_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/
#include <stdio.h>
#include <string.h>

#include "uhttp_io.h"
#include "pro/uhttp/uhttpd.h"
#include "pro/uhttp/streamio.h"
#include "pro/uhttp/mediatypes.h"

#include "xmem.h"
#include "mod_api.h"
#include "ipweb.h"

#if (_IP_WEB_API_SUPPORT >= 1)

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all global Data                                        */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

static HTTP_API_FUNCTION *APIFunctionList = NULL;

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  Unauthorized (401)                                                   */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void Unauthorized (HTTPD_SESSION *hs)
{
   //hs->s_req.req_connection = HTTP_CONN_CLOSE;

   HttpSendHeaderTop(hs, 401);
   s_puts("Cache-Control: no-cache, private\r\n", hs->s_stream);
   HttpSendHeaderBottom(hs, "application", "json", -1, 0);   
   s_set_flags(hs->s_stream, S_FLG_CHUNKED);
   
   s_puts("{\r\n", hs->s_stream);
   s_puts("  \"success\" : 0,\r\n", hs->s_stream);
   s_puts("  \"err_code\" : 401,\r\n", hs->s_stream);
   s_puts("  \"err_msg\" : \"Unauthorized.\"\r\n", hs->s_stream);
   s_puts("}\r\n", hs->s_stream);
   
   s_flush(hs->s_stream);
   
} /* Unauthorized */

/*************************************************************************/
/*  Forbidden (403)                                                      */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void Forbidden (HTTPD_SESSION *hs)
{
   hs->s_req.req_connection = HTTP_CONN_CLOSE;

   HttpSendHeaderTop(hs, 403);
   s_puts("Cache-Control: no-cache, private\r\n", hs->s_stream);
   HttpSendHeaderBottom(hs, "application", "json", -1, 0);   
   s_set_flags(hs->s_stream, S_FLG_CHUNKED);
   
   s_puts("{\r\n", hs->s_stream);
   s_puts("  \"success\" : 0,\r\n", hs->s_stream);
   s_puts("  \"err_code\" : 403,\r\n", hs->s_stream);
   s_puts("  \"err_msg\" : \"A secure connection must be used.\"\r\n", hs->s_stream);
   s_puts("}\r\n", hs->s_stream);
   
   s_flush(hs->s_stream);
   
} /* Forbidden */

/*************************************************************************/
/*  ResourceNotFound (404)                                               */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void ResourceNotFound (HTTPD_SESSION *hs)
{
   HttpSendHeaderTop(hs, 404);
   s_puts("Cache-Control: no-cache, private\r\n", hs->s_stream);
   HttpSendHeaderBottom(hs, "application", "json", -1, 0);   
   s_set_flags(hs->s_stream, S_FLG_CHUNKED);

   s_puts("{\r\n", hs->s_stream);
   s_puts("  \"success\" : 0,\r\n", hs->s_stream);
   s_puts("  \"err_code\" : 404,\r\n", hs->s_stream);
   s_puts("  \"err_msg\" : \"Resource not found.\"\r\n", hs->s_stream);
   s_puts("}\r\n", hs->s_stream);
   
   s_flush(hs->s_stream);

} /* ResourceNotFound */

/*************************************************************************/
/*  HttpApiFunctionLookup                                                */
/*                                                                       */
/*  In    : uri                                                          */
/*  Out   : none                                                         */
/*  Return: api / NULL                                                   */
/*************************************************************************/
static HTTP_API_FUNCTION *HttpApiFunctionLookup (const char *uri)
{
   HTTP_API_FUNCTION *api = NULL;

   if (uri != NULL)
   {
      for (api = APIFunctionList; api != NULL; api = api->api_link)
      {
         if (0 == strcasecmp(api->api_uri, uri))
         {
            break;
         }     
      }
   }      
   
   return(api);
} /* HttpApiFunctionLookup */

/*************************************************************************/
/*  APICheckBearer                                                       */
/*                                                                       */
/*  In    : req_auth                                                     */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 == ERROR                                         */
/*************************************************************************/
int __attribute__((weak)) APICheckBearer (char *req_auth)
{
   (void)req_auth;

   /* Only an empty function */
   return(0);
} /* APICheckBearer */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  HttpRegisterApiFunction                                              */
/*                                                                       */
/*  In    : uri, handler                                                 */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 == ERROR                                         */
/*************************************************************************/
int HttpRegisterApiFunction (const char *uri, HTTP_API_HANDLER handler)
{
   int                rc = -1;
   HTTP_API_FUNCTION *api;
   
   if ((uri != NULL) && (handler != NULL))
   {
      api = xcalloc(XM_ID_WEB, 1, sizeof(HTTP_API_FUNCTION));
      if (api != NULL)
      {
         api->api_uri = xstrdup(XM_ID_WEB, uri);
         if (api->api_uri != NULL)
         {
            rc = 0;
            
            api->api_handler = handler;
            api->api_link    = APIFunctionList;
            APIFunctionList  = api;
         }
      }
   }
   
   return(rc);
} /* HttpRegisterApiFunction */

/*************************************************************************/
/*  HttpApiFunctionHandler                                               */
/*                                                                       */
/*  In    : hs, mt, filepath                                             */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 == ERROR                                         */
/*************************************************************************/
int HttpApiFunctionHandler (HTTPD_SESSION *hs, const MEDIA_TYPE_ENTRY *mt, const char *filepath)
{
   HTTP_API_FUNCTION *api;
   
   (void)mt;
   
   /*
    * Since an AccessToken is also transmitted with the API function,
    * only a secure connection should be used here for security reasons.
    */
   
   /* Check for a secure connection */
   if (1 == IP_WEBS_IsRunnungSSL())
   {
      /* Check the correct AccessToken */ 
      if (0 == APICheckBearer(hs->s_req.req_auth))
      {
         /* AccessToken was valid */
         api = HttpApiFunctionLookup(filepath);
         if (api) 
         {
            (*api->api_handler)(hs);
         }
         else
         {
            ResourceNotFound(hs);
         }   
      }
      else
      {
         Unauthorized(hs);
      }      
   }
   else
   {
      Forbidden(hs);
   }

   s_end(hs->s_stream);
   
   return(0);
} /* HttpVfsHandler */  

#endif /* (_IP_WEB_API_SUPPORT >= 1) */

/*** EOF ***/



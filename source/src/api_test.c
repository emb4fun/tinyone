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
*  07.08.2022  mifi  First Version.
**************************************************************************/
#define __API_TEST_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/
#include <stdio.h>
#include "tal.h"
#include "ipstack.h"
#include "ipweb.h"

#define JSMN_STATIC
#include "jsmn.h"

/*lint -save -e801*/

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

#define GOTO_END(_a)  { rc = _a; goto end; }

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  api_v1_get                                                           */
/*                                                                       */
/*  Output argument/value pair from a HTTP GET in json format.           */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int api_v1_get (HTTPD_SESSION *hs)
{
   int   nAvail = 0;
   char *pArg;
   char *pVal;
   
   /*
    * curl -X GET "http://192.168.1.200/api/v1/get?param=value" -H "accept: application/json" -H "Authorization: Bearer password" -v
    *
    * Ignore SSL certification warning, use -k
    * curl -X GET "https://192.168.1.200/api/v1/get?param=value" -H "accept: application/json" -H "Authorization: Bearer password" -v -k
    */

   IP_WEBS_APISendHeader(hs);
   
   /* Output loop */
   for (pArg = HttpArgParseFirst(&hs->s_req); pArg; pArg = HttpArgParseNext(&hs->s_req)) 
   {
      if (0 == nAvail)
      {
         /* First argument found */
         nAvail = 1;
         s_puts("{\r\n", hs->s_stream);
         s_puts("  \"success\" : 1,\r\n", hs->s_stream);
      }
      else
      {
         /* Next argument found */
         s_puts(",\r\n", hs->s_stream);
      }   
   
      pVal = HttpArgValue(&hs->s_req);
      if (pVal != NULL)
      {
         s_printf(hs->s_stream, "  \"%s\" : \"%s\"", pArg, pVal);
      }
      else
      {
         s_printf(hs->s_stream, "  \"%s\" : \"NULL\"", pArg);
      }
   }

   /* End of data */   
   if (1 == nAvail)
   {
      s_puts("\r\n", hs->s_stream);
      s_puts("}\r\n", hs->s_stream);
   }

   /* Check for error  */
   if (0 == nAvail)
   {
      s_puts("{\r\n", hs->s_stream);
      s_puts("  \"success\" : 0,\r\n", hs->s_stream);
      s_puts("  \"err_code\" : -1,\r\n", hs->s_stream);
      s_puts("  \"err_msg\" : \"No data available.\"\r\n", hs->s_stream);
      s_puts("}\r\n", hs->s_stream);
   }

   s_flush(hs->s_stream);

   return(0);
} /* api_v1_get */

/*************************************************************************/
/*  api_v1_post                                                          */
/*                                                                       */
/*  Output argument/value pair from a HTTP POST in json format.          */
/*  The data was send from the client to the server in json format too.  */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int api_v1_post (HTTPD_SESSION *hs)
{
   int      rc;
   json_t   JSON;   
   char   *pVal1;
   char   *pVal2;

   /*
    * curl -X POST "http://192.168.1.200/api/v1/post" -H "accept: application/json" -H "Content-Type: application/json" -H "Authorization: Bearer password" -d "{\"arg1\":\"val1\",\"arg2\":\"val2\"}" -v
    *
    * Ignore SSL certification warning, use -k
    * curl -X POST "https://192.168.1.200/api/v1/post" -H "accept: application/json" -H "Content-Type: application/json" -H "Authorization: Bearer password" -d "{\"arg1\":\"val1\",\"arg2\":\"val2\"}" -v -k
    */

   IP_WEBS_APISendHeader(hs);

   /* Parse data, prepare up to 32 token */
   rc = IP_JSON_ParseHS(hs, &JSON, 32);
   if (-1 == rc) GOTO_END(-1);
   
   /* Get data from token "arg1" */
   pVal1 = IP_JSON_GetString(&JSON, "arg1");
   if (NULL == pVal1) GOTO_END(-1);

   /* Get data from token "arg2" */
   pVal2 = IP_JSON_GetString(&JSON, "arg2");
   if (NULL == pVal2) GOTO_END(-1);
   
   /* arg1 and arg2 was available */
   rc = 0;
   
   s_puts("{\r\n", hs->s_stream);
   s_puts("  \"success\" : 1,\r\n", hs->s_stream);
   s_printf(hs->s_stream, "  \"arg1\" : \"%s\",\r\n", pVal1);
   s_printf(hs->s_stream, "  \"arg2\" : \"%s\"\r\n", pVal2);
   s_puts("}\r\n", hs->s_stream);

   
end: 

   /* Check for error  */
   if (rc != 0)
   {
      s_puts("{\r\n", hs->s_stream);
      s_puts("  \"success\" : 0,\r\n", hs->s_stream);
      s_puts("  \"err_code\" : -1,\r\n", hs->s_stream);
      s_puts("  \"err_msg\" : \"Data error.\"\r\n", hs->s_stream);
      s_puts("}\r\n", hs->s_stream);
   }

   s_flush(hs->s_stream);

   IP_JSON_Delete(&JSON);

   return(0);
} /* api_v1_post */


/*
 * API variable list
 */
static const API_LIST_ENTRY APIList[] =
{
   { "v1/get",  api_v1_get  },
   { "v1/post", api_v1_post },

   {NULL, NULL}
};

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  ATInit                                                               */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void ATInit (void)
{
   IP_WEBS_APIListAdd(APIList);

} /* ATInit */

/*************************************************************************/
/*  APICheckBearer                                                       */
/*                                                                       */
/*  In    : req_auth                                                     */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 == ERROR                                         */
/*************************************************************************/
int APICheckBearer (char *req_auth)
{
   int   nAccess = -1;
   char *pAccessToken;
   
   if (req_auth != NULL)
   {
      pAccessToken = strstr(req_auth, "Bearer ");
      if (pAccessToken != NULL)
      {
         pAccessToken += 7;
         if (0 == strcmp(pAccessToken, "password"))
         {
            nAccess = 0;
         }
      }
   }      
   
   return(nAccess);
} /* APICheckBearer */

/*lint -restore*/

/*** EOF ***/

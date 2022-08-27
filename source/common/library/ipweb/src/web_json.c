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
#define __WEB_JSON_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/

#include <string.h>
#include <stdlib.h>
#include "tal.h"
#include "tcts.h"
#include "ipweb.h"
#include "xmem.h"

#define JSMN_STATIC
#include "jsmn.h"

/*=======================================================================*/
/*  Extern                                                               */
/*=======================================================================*/

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all global Data                                        */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  IP_JSON_Parse                                                        */
/*                                                                       */
/*  In    : json, count, pData, Len                                      */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
int IP_JSON_Parse (json_t *pJSON, size_t Count, char *pData, size_t Len)
{
   int         rc = -1;
   jsmn_parser p;
   jsmntok_t *pTokens;
   char      *pStart;
   uint16_t   wLen;

   if (NULL == pJSON)
   {
      return(-1);
   }

   /* Clear first */   
   memset(pJSON, 0x00, sizeof(json_t));
   
   pTokens = (jsmntok_t*)xmalloc(XM_ID_WEB, sizeof(jsmntok_t) * Count);
   if (pTokens != NULL)
   {
      /* Store token list */
      pJSON->pTokens = pTokens;
   
      jsmn_init(&p);
      rc = jsmn_parse(&p, pData, Len, pTokens, Count);      
      
      /* Assume the top-level element is an object */
      if ((rc < 1) || (pTokens[0].type != JSMN_OBJECT))
      {
         /* Error */
         rc = -1;
         xfree(pTokens);
      }
      else
      {  
         pJSON->nNumTokens = rc;
         pJSON->pData      = pData;
         
         for (int i = 0; i < rc; i++)
         {
            if (JSMN_STRING == pTokens[i].type)
            {
               wLen   = (uint16_t)(pTokens[i].end - pTokens[i].start);
               pStart = pData + pTokens[i].start;
               pStart[wLen] = 0;
            }
         }
      }
   }      
   
   return(rc);
} /* IP_JSON_Parse */

/*************************************************************************/
/*  IP_JSON_ParseHS                                                      */
/*                                                                       */
/*  In    : hs, json, count                                              */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
int IP_JSON_ParseHS (HTTPD_SESSION *hs, json_t *pJSON, size_t Count)
{
   int          rc = -1;
   jsmn_parser  p;
   jsmntok_t   *pTokens;
   char        *pData;
   char        *pStart;
   uint16_t     wLen;
   
   if ((NULL == hs) || (NULL == pJSON))
   {
      return(-1);
   }

   /* Clear first */   
   memset(pJSON, 0x00, sizeof(json_t));
   
   /* Get max length info */
   wLen = (uint16_t)hs->s_req.req_length;

   /* Create token and data memory */
   pTokens = (jsmntok_t*)xmalloc(XM_ID_WEB, sizeof(jsmntok_t) * Count);
   pData   = (char*)xmalloc(XM_ID_WEB, wLen);
   
   /* Check for valid memory */
   if ((pTokens != NULL) && (pData != NULL))
   {
      /* Store token list */
      pJSON->pTokens = pTokens;
      
      /* Update buffer */
      rc = StreamUpdateBufferJson(hs->s_stream, hs->s_req.req_length, pData);
      if ((wLen != 0) && (rc != -1))
      {
         jsmn_init(&p);
         rc = jsmn_parse(&p, pData, wLen, pTokens, Count);      
      
         /* Assume the top-level element is an object */
         if ((rc < 1) || (pTokens[0].type != JSMN_OBJECT))
         {
            /* Error */
            rc = -1;
            xfree(pTokens);
            xfree(pData);
         }
         else
         {  
            pJSON->nNumTokens = rc;
            pJSON->pData      = pData;
         
            for (int i = 0; i < rc; i++)
            {
               if (JSMN_STRING == pTokens[i].type)
               {
                  wLen   = (uint16_t)(pTokens[i].end - pTokens[i].start);
                  pStart = pData + pTokens[i].start;
                  pStart[wLen] = 0;
               }
            }
         }
      }
   }
   else
   {
      xfree(pTokens);
      xfree(pData);
   }
   
   return(rc);
} /* IP_JSON_ParseHS */

/*************************************************************************/
/*  IP_JSON_Delete                                                       */
/*                                                                       */
/*  In    : pJSON                                                        */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
int IP_JSON_Delete (json_t *pJSON)
{   
   xfree(pJSON->pTokens);
   xfree(pJSON->pData);
   
   return(0);
} /* IP_JSON_Delete */

/*************************************************************************/
/*  IP_JSON_GetString                                                    */
/*                                                                       */
/*  In    : hs, pKey                                                     */
/*  Out   : none                                                         */
/*  Return: String / NULL                                                */
/*************************************************************************/
char *IP_JSON_GetString (json_t *pJSON, char *pKey)
{
   char      *pString = NULL;
   jsmntok_t *pTokens;
   int        nNumTokens; 

   /* Check parameter */
   if ((pJSON != NULL) && (pKey != NULL))
   {
      pTokens    = pJSON->pTokens;
      nNumTokens = pJSON->nNumTokens;
   
      for (int i = 0; i < nNumTokens; i++)
      {
         if (JSMN_STRING == pTokens[i].type)
         {
            if (0 == strcmp(pJSON->pData + pTokens[i].start, pKey))
            {
               if ((i < (nNumTokens-1)) && (JSMN_STRING == pTokens[i+1].type))
               {
                  pString = pJSON->pData + pTokens[i+1].start;
               }   
               break;
            }
         }
      }
   }      
   
   return(pString);
} /* IP_JSON_GetString */

/*** EOF ***/

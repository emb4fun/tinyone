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
*  06.08.2022  mifi  First Version.
**************************************************************************/
#define __WEB_API_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "tal.h"
#include "tcts.h"
#include "ipstack.h"
#include "ipweb.h"
#include "mod_api.h"

#if (_IP_WEB_API_SUPPORT >= 1)

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

#define MAX_API_LIST_ENTRY    32

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

static API_LIST_ENTRY *ListTable[MAX_API_LIST_ENTRY];

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  IP_WEBS_APIInit                                                      */
/*                                                                       */
/*  Initialize the API functionality of the web server.                  */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_WEBS_APIInit (void)
{
   memset(ListTable, 0x00, sizeof(ListTable));
   
} /* IP_WEBS_APIInit */

/*************************************************************************/
/*  IP_WEBS_APIListAdd                                                   */
/*                                                                       */
/*  Add an API function list to the API core.                            */
/*                                                                       */
/*  In    : pList                                                        */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_WEBS_APIListAdd (const API_LIST_ENTRY *pList)
{
   uint16_t Index;
   
   for(Index = 0; Index < MAX_API_LIST_ENTRY; Index++)
   {
      if (0 == ListTable[Index])
      {
         ListTable[Index] = (API_LIST_ENTRY*)pList;
         break;   
      }
   }

} /* IP_WEBS_APIListAdd */

/*************************************************************************/
/*  IP_WEBS_APIStart                                                     */
/*                                                                       */
/*  Start the API functionality of the web server.                       */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_WEBS_APIStart (void)
{
   uint16_t         ListIndex = 0;
   API_LIST_ENTRY *pList;

   /* Loop over the registered lists */   
   while ((ListIndex != MAX_API_LIST_ENTRY) && (ListTable[ListIndex] != NULL))
   {
      /* Get list */
      pList = ListTable[ListIndex];   

      /* Loop over the actual list */   
      while(pList->Var != NULL)
      {
         HttpRegisterApiFunction(pList->Var, pList->pFunc);
   
         /* Switch to next entry inside the list */
         pList++;
      }
      
      /* Switch to next list */  
      ListIndex++;
   }      
   
} /* IP_WEBS_APIStart */

/*************************************************************************/
/*  IP_WEBS_APISendHeader                                                */
/*                                                                       */
/*  Send an API web header.                                              */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_WEBS_APISendHeader (HTTPD_SESSION *hs)
{
   HttpSendHeaderTop(hs, 200);
   s_puts("Cache-Control: no-cache, private\r\n", hs->s_stream);
   HttpSendHeaderBottom(hs, "application", "json", -1, 0);   
   
   s_set_flags(hs->s_stream, S_FLG_CHUNKED);
   
} /* IP_WEBS_APISendHeader */

#endif /* (_IP_WEB_API_SUPPORT >= 1) */

/*** EOF ***/

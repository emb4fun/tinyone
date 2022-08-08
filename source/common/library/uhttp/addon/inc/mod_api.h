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
#if !defined(__MOD_API_H__)
#define __MOD_API_H__

/**************************************************************************
*  Includes
**************************************************************************/
#include <pro/uhttp/mediatypes.h>
#include <pro/uhttp/uhttpd.h>
#include "ipweb_conf.h"

#if !defined(IP_WEB_API_SUPPORT)
#define _IP_WEB_API_SUPPORT    0
#else
#define _IP_WEB_API_SUPPORT    IP_WEB_API_SUPPORT
#endif 

/**************************************************************************
*  Global Definitions
**************************************************************************/

/*
 * API function entry type.
 */
typedef struct _HTTP_API_FUNCTION HTTP_API_FUNCTION;

typedef int (*HTTP_API_HANDLER) (HTTPD_SESSION*);

/*
 * API function entry structure.
 */
struct _HTTP_API_FUNCTION 
{
   /* Chain link. */
   HTTP_API_FUNCTION *api_link;
   /* URI of the API function. */
   char *api_uri;
   /* Registered API function. */
   HTTP_API_HANDLER api_handler;
};


typedef struct _api_list_entry_
{
   const char *Var;
   int        (*pFunc)(HTTPD_SESSION *hs);
} API_LIST_ENTRY;

/**************************************************************************
*  Macro Definitions
**************************************************************************/

/**************************************************************************
*  Funtions Definitions
**************************************************************************/

int HttpRegisterApiFunction(const char *uri, HTTP_API_HANDLER handler);

int HttpApiFunctionHandler(HTTPD_SESSION *hs, const MEDIA_TYPE_ENTRY *mt, const char *filepath);

#endif /* !__MOD_API_H__ */

/*** EOF ***/

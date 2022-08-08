/**************************************************************************
*  Copyright (c) 2015 by Michael Fischer (www.emb4fun.de).
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
*  27.06.2015  mifi  First Version.
*  13.03.2016  mifi  Renamed to web.h.
**************************************************************************/
#if !defined(__IPWEB_H__)
#define __IPWEB_H__

/**************************************************************************
*  Includes
**************************************************************************/

#include "ipweb_conf.h"
#include "web_json.h"
#include "web_login.h"
#include "web_user.h"
#include "web_api.h"
#include "web_cgi.h"
#include "web_ssi.h"
#include "web_sid.h"

#include "pro/uhttp/mediatypes.h"
#include "pro/uhttp/modules/mod_auth_basic.h"
#include "pro/uhttp/modules/mod_cgi_func.h"
#include "pro/uhttp/modules/mod_redir.h"
#include "pro/uhttp/modules/mod_ssi.h"
#include "pro/uhttp/streamio.h"


/**************************************************************************
*  Global Definitions
**************************************************************************/

#define WEB_UPLOAD_BUFFER_SIZE   (2*1024*1024)

typedef struct _web_upload_
{
   uint8_t   Error;
   
   char    *pFileName;
   long     lFileSize;
   uint8_t *pBuffer;
   
   long     lBufferSize;
   uint8_t  bBufferIndex;
   
   char    *pType;
   char    *pRedirOK;
   char    *pRedirERR;
} web_upload_t;

/**************************************************************************
*  Macro Definitions
**************************************************************************/

/**************************************************************************
*  Functions Definitions
**************************************************************************/

int IP_WEBS_Init (void);

int IP_WEBS_Start (uint16_t wPort);
int IP_WEBS_SSLStart (uint16_t wPort);

int IP_WEBS_IsRunnung (void);
int IP_WEBS_IsRunnungSSL (void);

#endif /* !__IPWEB_H__ */

/*** EOF ***/

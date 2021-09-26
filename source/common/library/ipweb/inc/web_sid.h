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
#if !defined(__WEB_SID_H__)
#define __WEB_SID_H__

/**************************************************************************
*  Includes
**************************************************************************/

#include "ipweb_conf.h"
#include "pro/uhttp/uhttpd.h"

/**************************************************************************
*  Global Definitions
**************************************************************************/

#if !defined(IP_WEB_SID_SUPPORT) 
#define _IP_WEB_SID_SUPPORT   0
#else
#define _IP_WEB_SID_SUPPORT   IP_WEB_SID_SUPPORT
#endif  


#define WEB_SID_HTTP    1
#define WEB_SID_CGI     0

/**************************************************************************
*  Macro Definitions
**************************************************************************/

/**************************************************************************
*  Functions Definitions
**************************************************************************/

void     WebSidInit (void);
char    *WebSidParseCookie (char *pCookie);
char    *WebSidCreateCookie (HTTPD_SESSION *hs);
char    *WebSidCreateNonce (HTTPD_SESSION *hs);
int      WebSidCheck (HTTPD_SESSION *hs, char *pSessionID, int nIsHttp);
int      WebSidCheckAccessGranted (HTTPD_SESSION *hs, char *pSessionID, int nIsHttp);
int      WebSidCheckUserPass (HTTPD_SESSION *hs, char *pUser, char *pPass);
void     WebSidInvalidate (HTTPD_SESSION *hs);
uint32_t WebSidGetPermission (int nSIDEntry);
char    *WebSidGetUser (int nSIDEntry);
int      WebSidSetNewPass (HTTPD_SESSION *hs, char *pOldPass, char *pNewPass);

int      WebSidLoginInit (void);
int      WebSidLoginInitSet (HTTPD_SESSION *hs, char *pNewPass);

int      WebSidLoginBlocked (void);
uint32_t WebSidLoginBlockedTime (void);
uint32_t WebSidLogoutTime (HTTPD_SESSION *hs);

#endif /* !__WEB_SID_H__ */

/*** EOF ***/

/**************************************************************************
*  Copyright (c) 2019-2023 by Michael Fischer (www.emb4fun.de).
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
**************************************************************************/
#if !defined(__IPWEB_CONF_H__)
#define __IPWEB_CONF_H__

/**************************************************************************
*  Includes
**************************************************************************/

/**************************************************************************
*  Global Definitions
**************************************************************************/

#define IP_WEB_MAX_HTTP_TASKS             32
#define IP_WEB_TLS_MAX_HTTP_TASKS         32

#define IP_WEB_SID_SUPPORT                1
#define IP_WEB_SID_LOGIN_ERROR_CNT_MAX    3
#define IP_WEB_SID_LOGIN_TIMEOUT_SEC      60
#define IP_WEB_SID_TIMEOUT_SEC            (10*60)

/**************************************************************************
*  Macro Definitions
**************************************************************************/

/**************************************************************************
*  Funtions Definitions
**************************************************************************/
 
#endif /* !__IPWEB_CONF_H__ */

/*** EOF ***/

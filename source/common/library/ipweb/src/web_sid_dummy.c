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
#define __WEB_SID_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/

#include <string.h>
#include <stdlib.h>
#include "tal.h"
#include "tcts.h"
#include "ipweb.h"
#include "xmem.h"

#if (_IP_WEB_SID_SUPPORT == 0)

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

void WebSidInit (void)
{
}

char *WebSidParseCookie (char *pCookie)
{
   (void)pCookie;
   
   return(NULL);
}

char *WebSidCreateNonce (HTTPD_SESSION *hs)
{
   (void)hs;
   
   return(NULL);
}

int  WebSidCheckUserPass (HTTPD_SESSION *hs, char *pUser, char *pPass)
{
   (void)hs;
   (void)pUser;
   (void)pPass;
   
   return(1);   
}

void WebSidInvalidate (HTTPD_SESSION *hs)
{
   (void)hs;
}

int WebSidSetNewPass (HTTPD_SESSION *hs, char *pPassUser, char *pPassNewEncoded)
{
   (void)hs;
   (void)pPassUser;
   (void)pPassNewEncoded;
   
   return(0);
} 

int WebSidLoginBlocked (void)
{
   return(0);
}

uint32_t WebSidLoginBlockedTime (void)
{
   return(0);
}

int WebSidLoginInit (void)
{
   return(0);
}

int WebSidLoginInitSet (HTTPD_SESSION *hs, char *pNewPass)
{
   (void)hs;
   (void)pNewPass;
   
   return(0);
}

uint32_t WebSidLogoutTime (HTTPD_SESSION *hs)
{
   (void)hs;
   
   return(-1);
}

#endif /* (_IP_WEB_SID_SUPPORT == 0) */

/*** EOF ***/

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
*  14.11.2020  mifi  First Version.
**************************************************************************/
#if !defined(__WEB_USER_H__)
#define __WEB_USER_H__

/**************************************************************************
*  Includes
**************************************************************************/

#include "ipweb_conf.h"

/**************************************************************************
*  Global Definitions
**************************************************************************/

#define USER_MODE_NORMAL   0
#define USER_MODE_TOTP     1

/**************************************************************************
*  Macro Definitions
**************************************************************************/

/**************************************************************************
*  Functions Definitions
**************************************************************************/

void  WebUserInit (void);

int   WebUserIsFirstUser (void);
void  WebUserFirstUserSet (char *pPassword);

int   WebUserCheckUserPassword (char *pUser, char *pPassword, uint32_t *pPermission);
int   WebUserCheckUserPasswordTOTP (char *pUser, char *pPassword, uint32_t *pPermission, uint32_t dCode);

char *WebUserGetUser (uint8_t bIndex);
int   WebUserSetNewPass (char *pUser, char *pPassUser, char *pPassNew);

int   WebUserIsValid (char *pUser, uint8_t *pMode);

int   WebUserGetResetRequestCode (char *pUser, char *pBuffer, size_t Size);
int   WebUserGetResetCode (char *pRequestCode, size_t RequestSize, char *pReset, size_t ResetSize);
int   WebUserPasswordReset (char *pUser, char *pPassNew, char *pResetCode);

int   WebUserIsTOTP (char *pUser);
int   WebUserGetTOTPSecretBase32 (char *pUser, char *pBase32, size_t Base32Size);
int   WebUserTOTPEnable (char *pUser, uint32_t dValue);
int   WebUserTOTPDisable (char *pUser, char *pPassword);
int   WebUserCheckTOTP (char *pUser, uint32_t dValue);

#endif /* !__WEB_USER_H__ */

/*** EOF ***/

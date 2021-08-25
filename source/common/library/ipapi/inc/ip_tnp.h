/**************************************************************************
*  Copyright (c) 2014 by Michael Fischer (www.emb4fun.de).
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
*  08.02.2014  mifi  First Version.
*  01.06.2019  mifi  Added functionality, based on "NUTSetup":
*                    (https://www.emb4fun.de/archive/nutsetup/index.html)
*  05.10.2019  mifi  Rename SAC to TNP (Tiny Network Protocol)
*  06.06.2020  mifi  Rework to support 2 interfaces, and rename API.
**************************************************************************/
#if !defined(__IP_TNP_H__)
#define __IP_TNP_H__

/**************************************************************************
*  Includes
**************************************************************************/
#include <stdint.h>
#include "tnpsetup.h"

/**************************************************************************
*  Global Definitions
**************************************************************************/

typedef TNP_SETUP  IP_TNP_INFO;

/**************************************************************************
*  Macro Definitions
**************************************************************************/

/**************************************************************************
*  Functions Definitions
**************************************************************************/

void  IP_TNP_Start (void);
int   IP_TNP_IsReady (void);
int   IP_TNP_IsES (void);
int   IP_TNP_DeviceGet (uint32_t index, IP_TNP_INFO *pInfo);
int   IP_TNP_DeviceFind (char *pDevice, uint32_t *pAddr);
void  IP_TNP_SendRequest (uint8_t bIface);
void  IP_TNP_DHCPCallbackBound (uint8_t bIface);
void  IP_TNP_SetName (char *pName, uint8_t bLen);

#endif /* !__IP_TNP_H__ */

/*** EOF ***/

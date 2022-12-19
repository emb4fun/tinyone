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
*  06.06.2020  mifi  Rename API.
**************************************************************************/
#if !defined(__IP_DHCP_H__)
#define __IP_DHCP_H__

/**************************************************************************
*  Includes
**************************************************************************/
#include <stdint.h>

/**************************************************************************
*  Global Definitions
**************************************************************************/

typedef void (*ip_dhcp_callback_t)(void);

/**************************************************************************
*  Macro Definitions
**************************************************************************/

/**************************************************************************
*  Functions Definitions
**************************************************************************/

void     _IP_DHCP_CallbackBound (void *p);

void     IP_DHCP_Start (uint8_t iface);
void     IP_DHCP_Stop (uint8_t iface); 
int      IP_DHCP_IsStarted (uint8_t iface);

uint32_t IP_DHCP_ServerGet (uint8_t iface);
void     IP_DHCP_CallbackSet (uint8_t iface, ip_dhcp_callback_t callback);

uint32_t IP_DHCP_NTPServerGet (uint8_t iface);

void     IP_DHCP_TimeoutSet (uint8_t iface, uint32_t timeout_msec);

void     IP_DHCP_HostnameSet (uint8_t iface, char *hostname);
void     IP_DHCP_ClientIDSet (uint8_t iface, uint8_t *id, uint8_t len);

#endif /* !__IP_DHCP_H__ */

/*** EOF ***/

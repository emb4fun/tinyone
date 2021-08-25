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
*  31.05.2019  mifi  First Version.
**************************************************************************/
#if !defined(__ETC_H__)
#define __ETC_H__

/**************************************************************************
*  Includes
**************************************************************************/
#include <stdint.h>

/**************************************************************************
*  Global Definitions
**************************************************************************/

typedef enum _etc_ip_addr_
{
   ETC_IP_ADDR = 0,
   ETC_IP_MASK,
   ETC_IP_GW,
   ETC_IP_DNS,
   ETC_IP_DNS2,
   ETC_IP_NTP,
   ETC_IP_SYSLOG,
   
   ETC_IP_MAX
} ETC_IP_ADDRESS;

/**************************************************************************
*  Macro Definitions
**************************************************************************/

/**************************************************************************
*  Functions Definitions
**************************************************************************/

void     etc_Init (void);

char    *etc_ShortNameGet (void);
char    *etc_LongNameGet (void);
char    *etc_LocationGet (void);

int      etc_IPDhcpIsUsed (void);
int      etc_IPTnpIsES (void);
int      etc_IPmDNSIsUsed (void);

uint32_t etc_IPAddrGet (ETC_IP_ADDRESS eAddress);

int      etc_TimeZoneIDGet (void);
int      etc_TimeZoneOffsetGet (void);
int      etc_TimeZoneDstGet (void);
uint32_t etc_TimeNTPAddrGet (void);   
uint32_t etc_TimeNTPRefresh (void);

#endif /* !__ETC_H__ */

/*** EOF ***/

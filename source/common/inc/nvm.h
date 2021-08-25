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
*  20.04.2019  mifi  First Version.
**************************************************************************/
#if !defined(__NVM_H__)
#define __NVM_H__

/**************************************************************************
*  Includes
**************************************************************************/
#include <stdint.h>

/**************************************************************************
*  Global Definitions
**************************************************************************/

#define NVM_IP_USE_ENABLE  0x00000001
#define NVM_IP_USE_DHCP    0x00000002
#define NVM_IP_USE_TNPES   0x00000004
#define NVM_IP_USE_MDNS    0x00000008

#define NVM_IP_LOC_LEN     16

typedef struct _nvm_ip_
{
   uint32_t dUse;
   uint32_t dIPAddr;
   uint32_t dNETMask;
   uint32_t dGWAddr;
   uint32_t dDNSAddr;
   uint32_t dDNS2Addr;
   uint32_t dNTPAddr;
   uint32_t dSYSAddr;
   char      Location[NVM_IP_LOC_LEN];
} NVM_IP;

typedef struct _nvm_time_
{
   int32_t  nZoneID;
   int32_t  nZoneOffset;
   int32_t  nZoneDst;
   uint32_t dNTPAddr;
   uint32_t dNTPRefresh;   
} NVM_TIME;

typedef struct _nvm_dhcps_
{
   uint32_t dEnable;
   uint32_t dPoolStart;
   uint32_t dPoolSize;
   uint32_t dLease;
   uint32_t dIPMask;
   uint32_t dIPGate;
   uint32_t dIPDns;
   uint32_t dIPDns2;
   uint32_t dIPNtp;
   uint32_t dIPSysl;
} NVM_DHCPS;

/**************************************************************************
*  Macro Definitions
**************************************************************************/

/**************************************************************************
*  Functions Definitions
**************************************************************************/

/*
 * Misc
 */
void     nvm_Init (void);
void     nvm_Write (void);
void     nvm_Reset (void);


/*
 * IP
 */
void     nvm_IPGet (NVM_IP *pIP); 
void     nvm_IPSet (NVM_IP *pIP); 

uint32_t nvm_IPUseDHCPGet (void);
void     nvm_IPUseDHCPSet (uint32_t value);

uint32_t nvm_IPUseTNPESGet (void);
void     nvm_IPUseTNPESSet (uint32_t value);


/*
 * Time
 */
void     nvm_TimeGet (NVM_TIME *pTime); 
void     nvm_TimeSet (NVM_TIME *pTime); 


/*
 * DHCP Server
 */
void     nvm_DhcpServerGet (NVM_DHCPS *pDhcps); 
void     nvm_DhcpServerSet (NVM_DHCPS *pDhcps); 

uint32_t nvm_DhcpServerUseGet (void);
void     nvm_DhcpServerUseSet (uint32_t value);
  
#endif /* !__NVM_H__ */

/*** EOF ***/

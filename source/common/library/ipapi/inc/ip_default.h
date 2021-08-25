/**************************************************************************
*  Copyright (c) 2020 by Michael Fischer (www.emb4fun.de).
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
*  07.03.2020  mifi  First Version.
**************************************************************************/
#if !defined(__IP_DEFAULT_H__)
#define __IP_DEFAULT_H__

/**************************************************************************
*  Includes
**************************************************************************/
#include <stdint.h>
#include "ipstack_conf.h"

/**************************************************************************
*  Global Definitions
**************************************************************************/

/*
 * Default IP, NetMask, GW and MAC address.
 * Can be override by ipstack_conf.h  
 */   
#ifndef IP_DEFAULT_ADDRESS
#define IP_DEFAULT_ADDRESS          "192.168.1.200"
#endif

#ifndef IP_DEFAULT_NETMASK
#define IP_DEFAULT_NETMASK          "255.255.255.0"
#endif

#ifndef IP_DEFAULT_GATEWAY
#define IP_DEFAULT_GATEWAY          "0.0.0.0"
#endif

#ifndef IP_DEFAULT_DNS
#define IP_DEFAULT_DNS              "0.0.0.0"
#endif

#ifndef IP_DEFAULT_DNS2
#define IP_DEFAULT_DNS2             "0.0.0.0"
#endif

//#ifndef IP_DEFAULT_LAN_BROADCAST
//#define IP_DEFAULT_LAN_BROADCAST    "192.168.1.255"
//#endif

#ifndef IP_DEFAULT_MAC_ADDR  
#define IP_DEFAULT_MAC_ADDR         {0x01,0x02,0x03,0x04,0x05,0x06}
#endif

#ifndef IP_DEFAULT_OPT_DHCP
#define IP_DEFAULT_OPT_DHCP         1
#endif

#ifndef IP_DEFAULT_OPT_DHCPS
#define IP_DEFAULT_OPT_DHCPS        1
#endif

#ifndef IP_DEFAULT_OPT_DNS
#define IP_DEFAULT_OPT_DNS          1
#endif

#ifndef IP_DEFAULT_OPT_TNP
#define IP_DEFAULT_OPT_TNP          1
#endif

#ifndef IP_DEFAULT_OPT_SNTP
#define IP_DEFAULT_OPT_SNTP         1
#endif

#ifndef IP_DEFAULT_OPT_SNTPS
#define IP_DEFAULT_OPT_SNTPS        1
#endif

#ifndef IP_DEFAULT_OPT_MDNS
#define IP_DEFAULT_OPT_MDNS         1
#endif

#ifndef IP_DEFAULT_OPT_SYSL
#define IP_DEFAULT_OPT_SYSL         1
#endif

/**************************************************************************
*  Macro Definitions
**************************************************************************/

/**************************************************************************
*  Funtions Definitions
**************************************************************************/

#endif /* !__IP_DEFAULT_H__ */

/*** EOF ***/

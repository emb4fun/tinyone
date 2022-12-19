/**************************************************************************
*  Copyright (c) 2013 by Michael Fischer (www.emb4fun.de).
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
*  19.05.2013  mifi  First Version.
*  19.08.2013  mifi  Some rework for lwip.
*  06.06.2020  mifi  Rework to support 2 interfaces, and rename API.
**************************************************************************/
#if !defined(__IPSTACK_H__)
#define __IPSTACK_H__

/**************************************************************************
*  Includes
**************************************************************************/
#include <stdint.h>
#include "ipstack_conf.h"
#include "ip_default.h"
#include "ip_dhcp.h"
#include "ip_dhcpserver.h"
#include "ip_dns.h"
#include "ip_mdns.h"
#include "ip_tnp.h"
#include "ip_sntp.h"
#include "ip_sntpserver.h"
#include "ip_syslog.h"

#include "lwip\netdb.h"
#include "lwip\sockets.h"
#include "lwip\stats.h"

/**************************************************************************
*  Global Definitions
**************************************************************************/

#define INVALID_SOCKET  -1
#define SOCKET_ERROR    -1

typedef void (*ip_iface_link_callback_t)(int link);

#define IFACE_ANY       0xff

/**************************************************************************
*  Macro Definitions
**************************************************************************/

/**************************************************************************
*  Funtions Definitions
**************************************************************************/

/*
 * Support function
 */
void     IP_Init (void);

struct netif *IP_IF_NetIfGet (uint8_t iface);
 
uint32_t atoh(char *pAsciiAddr);
char    *htoa (uint32_t Addr, char *pBuf, int BufLen);


/*
 * Interface functions
 */
void     IP_IF_MACSet (uint8_t iface, uint8_t *mac);
void     IP_IF_AddrSet (uint8_t iface, uint32_t addr);
void     IP_IF_MaskSet (uint8_t iface, uint32_t addr);
void     IP_IF_GWSet (uint8_t iface, uint32_t addr);

void     IP_IF_MACGet (uint8_t iface, uint8_t *mac, uint8_t size);
uint32_t IP_IF_AddrGet (uint8_t iface);
uint32_t IP_IF_MaskGet (uint8_t iface);
uint32_t IP_IF_GWGet (uint8_t iface);
void     IP_IF_LinkSpeedDuplexGet (uint8_t iface, uint16_t *speed, uint8_t *duplex);

int      IP_IF_Start (uint8_t iface);
int      IP_IF_Stop (uint8_t iface);
int      IP_IF_IsReady (uint8_t iface);

void     IP_IF_OutputConfig (uint8_t iface);

void     IP_IF_StartupValuesGet (uint8_t iface, uint32_t *ipaddr, uint32_t *netmask, uint32_t *gw);


#endif /* !__IPSTACK_H__ */

/*** EOF ***/

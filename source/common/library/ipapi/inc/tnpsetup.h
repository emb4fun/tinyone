/**************************************************************************
*  Copyright (c) 2004 by Michael Fischer (www.emb4fun.de).
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
*  11.04.2004  mifi  First Version.
*  01.06.2019  mifi  Added functionality, based on "NUTSetup":
*                    (https://www.emb4fun.de/archive/nutsetup/index.html)
*  05.10.2019  mifi  Rename SAC to TNP (Tiny Network Protocol)
*  18.07.2020  mifi  Added support for TNP_SETUP_RESPONSE_ES
**************************************************************************/
#if !defined(__TNPSETUP_H__)
#define __TNPSETUP_H__

/**************************************************************************
*  Includes
**************************************************************************/
#ifdef _MSC_VER
#include <windows.h>
#else
#include <stdint.h>
#endif

/**************************************************************************
*  Global Definitions
**************************************************************************/

#ifdef _MSC_VER
typedef DWORD  uint32_t;
typedef WORD   uint16_t;
typedef BYTE   uint8_t;   
#endif


#define TNP_HEADER_MAGIC_1    0x54504E54
#define TNP_HEADER_MAGIC_2    0x4E54504E
#define TNP_HEADER_VERSION    1

#define TNP_UDP_PORT          54321
#define TNP_NAME_LEN          16  
#define TNP_LOCATION_LEN      16
#define TNP_MDNS_NAME_LEN     32  

#define TNP_MAX_NAME_LEN      (TNP_NAME_LEN+1)
#define TNP_MAX_LOCATION_LEN  (TNP_LOCATION_LEN+1)
#define TNP_MAX_MDNS_NAME_LEN (TNP_MDNS_NAME_LEN+1)


enum 
{
  TNP_SETUP_REQUEST = 0,
  TNP_SETUP_RESPONSE,
  TNP_SETUP_SET,
  TNP_SETUP_RESPONSE_ES
};

#ifdef _WINDOWS
#pragma pack(1)
typedef struct _tnp_setup_
#else
typedef struct __attribute__((__packed__)) _tnp_setup_
#endif
{
  uint32_t dMagic1;
  uint32_t dMagic2;
  uint16_t wSize;
  uint16_t wVersion;
  
  uint8_t  bReserve;
  uint8_t  bMode;
  uint8_t  bUseDHCP;
  uint8_t  bMACAddress[6];
  uint32_t dAddress;
  uint32_t dMask;
  uint32_t dGateway;
  uint32_t dFWVersion;
  char      Name[TNP_MAX_NAME_LEN];
  char      Location[TNP_MAX_LOCATION_LEN];
  char      MDNSName[TNP_MAX_MDNS_NAME_LEN];
} TNP_SETUP;

#ifdef _WINDOWS
#pragma pack()
#endif


/**************************************************************************
*  Macro Definitions
**************************************************************************/

/**************************************************************************
*  Functions Definitions
**************************************************************************/

#ifndef _WINDOWS
void TNPSetupStart (void);
#endif

#endif /* !__TNPSETUP_H__ */

/*** EOF ***/

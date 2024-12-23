/**************************************************************************
*  Copyright (c) 2019-2024 by Michael Fischer (www.emb4fun.de).
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
#define __NVM_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/
#include "tal.h"
#include "ff.h"
#include "minini.h"
#include "fsapi.h"
#include "ipstack.h"
#include "adler32.h"
#include "etc.h"
#include "nvm.h"

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

#define XFILE_CONFIG       "XFILE:/etc/config.txt"
#define NVM_DRIVE          "SD0:"
#define NVM_ROOT           "/etc"
#define NVM_CONFIG         NVM_DRIVE NVM_ROOT "/nvm.bin"

#define NVM_MAGIC_1        0x434F4E46
#define NVM_MAGIC_2        ((uint32_t)~NVM_MAGIC_1)
#define NVM_SIZE           sizeof(nvm_t)
#define NVM_VERSION        0x0001


typedef struct _nvm_header_
{
   uint32_t   dMagic1;
   uint32_t   dMagic2;
   uint16_t   wSize;
   uint16_t   wVersion;
   uint32_t   dCRC32;
} NVM_HEADER;

typedef struct _nvm_
{
   NVM_HEADER  Header;
   uint32_t   dStartData;
   NVM_IP      IP;
   NVM_TIME    Time;
   NVM_DHCPS   DHCPS; 
} nvm_t;

/*=======================================================================*/
/*  Definition of all global Data                                        */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

static nvm_t    NVM;
static NVM_IP   IP;
static NVM_TIME Time;

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  ResetNVM                                                             */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void ResetNVM (void)
{
   int fd;
   
   memset(&NVM, 0xFF, sizeof(NVM));
      
   /* Create directory if not available */
   f_mkdir(NVM_ROOT);
   
   /* Write data */
   fd = _open(NVM_CONFIG, _O_BINARY | _O_WRONLY | _O_CREATE_ALWAYS);
   if (fd != -1)
   {
      _write(fd, &NVM, NVM_SIZE);
      _close(fd);
   }

} /* ResetNVM */

/*************************************************************************/
/*  WriteNVM                                                             */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void WriteNVM (void)
{
   int       fd;
   uint32_t dSize;
   uint32_t dCRC32;
   
   /* Set "header" */   
   NVM.Header.dMagic1  = NVM_MAGIC_1;
   NVM.Header.dMagic2  = NVM_MAGIC_2;
   NVM.Header.wSize    = NVM_SIZE;
   NVM.Header.wVersion = NVM_VERSION;
   NVM.dStartData      = 0;
   
   /* Set CRC */   
   dSize             = NVM_SIZE - sizeof(NVM_HEADER);
   dCRC32            = adler32(ADLER_START_VALUE, (uint8_t*)&NVM.dStartData, dSize);
   NVM.Header.dCRC32 = dCRC32;

   /* Create directory if not available */
   f_mkdir(NVM_ROOT);
   
   /* Write data */
   fd = _open(NVM_CONFIG, _O_BINARY | _O_WRONLY | _O_CREATE_ALWAYS);
   if (fd != -1)
   {
      _write(fd, &NVM, NVM_SIZE);
      _close(fd);
   }
   
} /* WriteNVM */

/*************************************************************************/
/*  ReadNVM                                                              */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void ReadNVM (void)
{
   int       fd;
   int       ReadCount;
   uint32_t dSize;
   uint32_t dCRC32;

   /* Clear data first */
   dCRC32     = 0;
   ReadCount = 0;
   memset(&NVM, 0xFF, sizeof(NVM));

   /* Read configuration */
   fd = _open(NVM_CONFIG, _O_BINARY | _O_RDONLY);
   if (fd != -1)
   {
      ReadCount = _read(fd, &NVM, sizeof(NVM)); 
      _close(fd);
   }
   
   /* Check configuration */
   if( (NVM_SIZE    == ReadCount)           && /*lint !e737*/
       (NVM_MAGIC_1 == NVM.Header.dMagic1)  &&
       (NVM_MAGIC_2 == NVM.Header.dMagic2)  &&
       (NVM_SIZE    == NVM.Header.wSize)    &&
       (NVM_VERSION == NVM.Header.wVersion) )
   {
      dSize  = NVM_SIZE - sizeof(NVM_HEADER);
      dCRC32 = adler32(ADLER_START_VALUE, (uint8_t*)&NVM.dStartData, dSize);
   }       

   /* Check for error */   
   if (dCRC32 != NVM.Header.dCRC32)
   {
      /* Set default */
      memset(&NVM, 0x00, sizeof(NVM));
   
      /* 
       * Get default IP from the ETC system 
       */
      nvm_IPGet(&IP);
   
      IP.dUse  = NVM_IP_USE_ENABLE;
      IP.dUse |= ((1 == etc_IPDhcpIsUsed()) ? NVM_IP_USE_DHCP : 0);
      IP.dUse |= ((1 == etc_IPTnpIsES())    ? NVM_IP_USE_TNPES : 0);
      IP.dUse |= ((1 == etc_IPmDNSIsUsed()) ? NVM_IP_USE_MDNS : 0);
   
      IP.dIPAddr   = etc_IPAddrGet(ETC_IP_ADDR);
      IP.dNETMask  = etc_IPAddrGet(ETC_IP_MASK);
      IP.dGWAddr   = etc_IPAddrGet(ETC_IP_GW);
      IP.dDNSAddr  = etc_IPAddrGet(ETC_IP_DNS);
      IP.dDNS2Addr = etc_IPAddrGet(ETC_IP_DNS2);
      IP.dNTPAddr  = etc_IPAddrGet(ETC_IP_NTP);
      IP.dSYSAddr  = etc_IPAddrGet(ETC_IP_SYSLOG);
   
      strncpy(IP.Location,  etc_LocationGet(), NVM_IP_LOC_LEN);
   
      nvm_IPSet(&IP);  
   
   
      /*
       * Get the default Time from the ETC system
       */
      nvm_TimeGet(&Time);
   
      Time.nZoneID     = etc_TimeZoneIDGet();
      Time.nZoneOffset = etc_TimeZoneOffsetGet();
      Time.nZoneDst    = etc_TimeZoneDstGet();
      Time.dNTPAddr    = etc_TimeNTPAddrGet();
      Time.dNTPRefresh = etc_TimeNTPRefresh();

      nvm_TimeSet(&Time);          
   
      WriteNVM();
   }
   
} /* ReadNVM */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  nvm_Init                                                             */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void nvm_Init (void)
{
   /* Read configuration */   
   ReadNVM();

} /* nvm_Init */

/*************************************************************************/
/*  nvm_Write                                                            */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void nvm_Write (void)
{
   WriteNVM();   
} /* nvm_Write */

/*************************************************************************/
/*  nvm_Reset                                                            */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void nvm_Reset (void)
{
   ResetNVM();      
      
} /* nvm_Reset */

/*************************************************************************/
/*  nvm_IPGet                                                            */
/*                                                                       */
/*  In    : pIP                                                          */
/*  Out   : pIP                                                          */
/*  Return: none                                                         */
/*************************************************************************/
void nvm_IPGet (NVM_IP *pIP)
{
   if (pIP != NULL)
   {
      /* Copy data */
      memcpy(pIP, &NVM.IP, sizeof(NVM_IP));
   }
      
} /* nvm_IPGet */

/*************************************************************************/
/*  nvm_IPSet                                                            */
/*                                                                       */
/*  In    : pIP                                                          */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void nvm_IPSet (NVM_IP *pIP)
{
   if (pIP != NULL)
   {
      /* Copy data */
      pIP->dUse |= NVM_IP_USE_ENABLE;
      memcpy(&NVM.IP, pIP, sizeof(NVM_IP));
   }
      
} /* nvm_IPSet */

/*************************************************************************/
/*  nvm_IPUseDHCPGet                                                     */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 0 = DHCP not used / 1 = DHCP used                            */
/*************************************************************************/
uint32_t nvm_IPUseDHCPGet (void)
{
   return( (NVM.IP.dUse & NVM_IP_USE_DHCP) ? 1 : 0 );
} /* nvm_IPUseDHCPGet */

/*************************************************************************/
/*  nvm_IPUseDHCPSet                                                     */
/*                                                                       */
/*  In    : value                                                        */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void nvm_IPUseDHCPSet (uint32_t value)
{
   uint32_t dUse = NVM.IP.dUse;
   
   if (value != 0)
   {
      dUse |= NVM_IP_USE_DHCP;
   }
   else
   {
      dUse &= ~NVM_IP_USE_DHCP;
   }

   NVM.IP.dUse = dUse;
   
} /* nvm_IPUseDHCPSet */

/*************************************************************************/
/*  nvm_IPUseTNPESGet                                                    */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 0 = TNPES not used / 1 = TNPES used                          */
/*************************************************************************/
uint32_t nvm_IPUseTNPESGet (void)
{
   return( (NVM.IP.dUse & NVM_IP_USE_TNPES) ? 1 : 0 );
} /* nvm_IPUseTNPESGet */

/*************************************************************************/
/*  nvm_IPUseTNPESSet                                                    */
/*                                                                       */
/*  In    : value                                                        */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void nvm_IPUseTNPESSet (uint32_t value)
{
   uint32_t dUse = NVM.IP.dUse;
   
   if (value != 0)
   {
      dUse |= NVM_IP_USE_TNPES;
   }
   else
   {
      dUse &= ~NVM_IP_USE_TNPES;
   }

   NVM.IP.dUse = dUse;
   
} /* nvm_IPUseTNPESSet */

/*************************************************************************/
/*  nvm_TimeGet                                                          */
/*                                                                       */
/*  In    : pTime                                                        */
/*  Out   : pTime                                                        */
/*  Return: none                                                         */
/*************************************************************************/
void nvm_TimeGet (NVM_TIME *pTime)
{
   if (pTime != NULL)
   {
      if (0 == NVM.Time.dNTPRefresh)
      {
         NVM.Time.dNTPRefresh = SNTP_REFRESH_DEFAULT_MIN;
      }

      /* Copy data */
      memcpy(pTime, &NVM.Time, sizeof(NVM_TIME));
   }
      
} /* nvm_TimeGet */

/*************************************************************************/
/*  nvm_TimeSet                                                          */
/*                                                                       */
/*  In    : pTime                                                        */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void nvm_TimeSet (NVM_TIME *pTime)
{
   if (pTime != NULL)
   {
      /* Copy data */
      memcpy(&NVM.Time, pTime, sizeof(NVM_TIME));
   }
      
} /* nvm_TimeSet */

/*************************************************************************/
/*  nvm_DhcpServerGet                                                    */
/*                                                                       */
/*  In    : pDhcps                                                       */
/*  Out   : pDhcps                                                       */
/*  Return: none                                                         */
/*************************************************************************/
void nvm_DhcpServerGet (NVM_DHCPS *pDhcps)
{
   if (pDhcps != NULL)
   {
      /* Copy data */
      memcpy(pDhcps, &NVM.DHCPS, sizeof(NVM_DHCPS));
   }
      
} /* nvm_DhcpServerGet */

/*************************************************************************/
/*  nvm_DhcpServerSet                                                    */
/*                                                                       */
/*  In    : pDhcps                                                       */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void nvm_DhcpServerSet (NVM_DHCPS *pDhcps)
{
   if (pDhcps != NULL)
   {
      /* Copy data */
      memcpy(&NVM.DHCPS, pDhcps, sizeof(NVM_DHCPS));
   }
      
} /* nvm_DhcpServerSet */

/*************************************************************************/
/*  nvm_DhcpServerUseGet                                                 */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 0 = DHCP server  not used / 1 = DHCP server used             */
/*************************************************************************/
uint32_t nvm_DhcpServerUseGet (void)
{
   return(NVM.DHCPS.dEnable);
} /* nvm_DhcpServerUseGet */

/*************************************************************************/
/*  nvm_DhcpServerUseSet                                                 */
/*                                                                       */
/*  In    : value                                                        */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void nvm_DhcpServerUseSet (uint32_t value)
{
   if (value != 0)
   {
      NVM.DHCPS.dEnable = 1;
   }
   else
   {
      NVM.DHCPS.dEnable = 0;
   }

} /* nvm_DhcpServerUseSet */

/*** EOF ***/



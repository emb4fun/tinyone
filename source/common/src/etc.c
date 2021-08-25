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
*  05.10.2019  mifi  Rename SAC to TNP (Tiny Network Protocol)
**************************************************************************/
#define __ETC_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/
#include "tal.h"
#include "ff.h"
#include "minini.h"
#include "fsapi.h"
#include "ipstack.h"
#include "etc.h"
#include "nvm.h"

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

#ifndef ETC_DEFAULT_SYSTEM_NAME
#define ETC_DEFAULT_SYSTEM_NAME  "emb4fun Server"
#endif

#ifndef ETC_DEFAULT_LOCATION
#define ETC_DEFAULT_LOCATION     "***"
#endif

#ifndef ETC_DEFAULT_IP_DHCP
#define ETC_DEFAULT_IP_DHCP      0
#endif

#ifndef ETC_DEFAULT_IP_MDNS
#define ETC_DEFAULT_IP_MDNS      0
#endif

#ifndef ETC_DEFAULT_IP_TNP
#define ETC_DEFAULT_IP_TNP       0
#endif

#ifndef ETC_DEFAULT_IP_ADDR
#define ETC_DEFAULT_IP_ADDR      "192.168.1.200"
#endif

#ifndef ETC_DEFAULT_IP_MASK
#define ETC_DEFAULT_IP_MASK      "255.255.255.0"
#endif

#ifndef ETC_DEFAULT_IP_GW
#define ETC_DEFAULT_IP_GW        "0.0.0.0"
#endif

#ifndef ETC_DEFAULT_IP_NTP
#define ETC_DEFAULT_IP_NTP       "0.0.0.0"
#endif

#ifndef ETC_DEFAULT_IP_SYSLOG
#define ETC_DEFAULT_IP_SYSLOG    "0.0.0.0"
#endif

#ifndef ETC_DEFAULT_IP_DNS1
#define ETC_DEFAULT_IP_DNS1      "0.0.0.0"
#endif

#ifndef ETC_DEFAULT_IP_DNS2
#define ETC_DEFAULT_IP_DNS2      "0.0.0.0"
#endif

#define ETC_CONFIG_FILE          "XFILE:/etc/config.txt"

#define INFO_INI_FILE            "XFILE:/info.ini"

/*=======================================================================*/
/*  Definition of all global Data                                        */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

static int     nDHCPIsUsed;
static int     nTNPIsES;
static int     nMDNSIsUsed;

static uint32_t IPAddrTable[ETC_IP_MAX];
static char     ShortName[16];
static char     LongName[32];
static char     Location[16];

static int32_t  nTimeZoneID;
static int32_t  nTimeZoneOffset;
static int32_t  nTimeZoneDst;
static uint32_t dTimeNTPAddr;
static uint32_t dTimeNTPRefresh;   

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  etc_Init                                                             */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void etc_Init (void)
{
   char   String[32];
   NVM_IP IP;  
   
   /*
    * System
    */    
   ini_gets("system", "shortname", ETC_DEFAULT_SYSTEM_NAME, ShortName, sizeof(ShortName), ETC_CONFIG_FILE);
   ini_gets("system", "longname", ETC_DEFAULT_SYSTEM_NAME, LongName, sizeof(LongName), ETC_CONFIG_FILE);
   ini_gets("system", "location", ETC_DEFAULT_LOCATION, Location, sizeof(Location), ETC_CONFIG_FILE);


   /*
    * Network configuration
    */
   memset(IPAddrTable, 0x00, sizeof(IPAddrTable));
    
   nDHCPIsUsed = (int)ini_getl("network", "dhcp", ETC_DEFAULT_IP_DHCP, ETC_CONFIG_FILE);
   nTNPIsES    = (int)ini_getl("network", "tnpes", ETC_DEFAULT_IP_TNP,  ETC_CONFIG_FILE);
   nMDNSIsUsed = (int)ini_getl("network", "mdns", ETC_DEFAULT_IP_MDNS, ETC_CONFIG_FILE);

   ini_gets("network", "ip", ETC_DEFAULT_IP_ADDR, String, sizeof(String), ETC_CONFIG_FILE);
   IPAddrTable[ETC_IP_ADDR] = atoh(String);
   
   ini_gets("network", "mask", ETC_DEFAULT_IP_MASK, String, sizeof(String), ETC_CONFIG_FILE);
   IPAddrTable[ETC_IP_MASK] = atoh(String);
   
   ini_gets("network", "gw", ETC_DEFAULT_IP_GW, String, sizeof(String), ETC_CONFIG_FILE);
   IPAddrTable[ETC_IP_GW] = atoh(String);

   ini_gets("network", "dns", ETC_DEFAULT_IP_DNS1, String, sizeof(String), ETC_CONFIG_FILE);
   IPAddrTable[ETC_IP_DNS] = atoh(String);

   ini_gets("network", "dns2", ETC_DEFAULT_IP_DNS2, String, sizeof(String), ETC_CONFIG_FILE);
   IPAddrTable[ETC_IP_DNS2] = atoh(String);
   
   ini_gets("network", "ntp", ETC_DEFAULT_IP_NTP, String, sizeof(String), ETC_CONFIG_FILE);
   IPAddrTable[ETC_IP_NTP] = atoh(String);
   
   ini_gets("network", "sys", ETC_DEFAULT_IP_SYSLOG, String, sizeof(String), ETC_CONFIG_FILE);
   IPAddrTable[ETC_IP_SYSLOG] = atoh(String);


   /*
    * Time configuration
    */
   nTimeZoneID     = (int)ini_getl("time", "zoneid",     0, ETC_CONFIG_FILE);
   nTimeZoneOffset = (int)ini_getl("time", "zoneoffset", 0, ETC_CONFIG_FILE);
   nTimeZoneDst    = (int)ini_getl("time", "zonedst",    0, ETC_CONFIG_FILE);

   ini_gets("time", "ntpaddr", "0.0.0.0", String, sizeof(String), ETC_CONFIG_FILE);
   dTimeNTPAddr = atoh(String);
   
   dTimeNTPRefresh = (uint32_t)ini_getl("time", "ntprefresh",    0, ETC_CONFIG_FILE);

   /*
    * Initializethe NVM system
    */
   nvm_Init();

   /* 
    * Check if the NVM override the IP configuration 
    */
   nvm_IPGet(&IP);
   if (IP.dUse & NVM_IP_USE_ENABLE)
   {
      nDHCPIsUsed = (IP.dUse & NVM_IP_USE_DHCP) ? 1 : 0;
      nTNPIsES    = (IP.dUse & NVM_IP_USE_TNPES) ? 1 : 0;
      nMDNSIsUsed = (IP.dUse & NVM_IP_USE_MDNS) ? 1 : 0;    
      
      IPAddrTable[ETC_IP_ADDR]   = IP.dIPAddr;
      IPAddrTable[ETC_IP_MASK]   = IP.dNETMask;
      IPAddrTable[ETC_IP_GW]     = IP.dGWAddr;
      IPAddrTable[ETC_IP_DNS]    = IP.dDNSAddr;
      IPAddrTable[ETC_IP_DNS2]   = IP.dDNS2Addr;
      IPAddrTable[ETC_IP_NTP]    = IP.dNTPAddr;
      IPAddrTable[ETC_IP_SYSLOG] = IP.dSYSAddr;
      
      if (IP.Location[0] != 0)
      {
         memcpy(Location, IP.Location, NVM_IP_LOC_LEN);
      }         
   }

} /* etc_Init */

/*************************************************************************/
/*  etc_ShortNameGet                                                     */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: ShortName                                                    */
/*************************************************************************/
char *etc_ShortNameGet (void)
{
   //return(ShortName);
   return(PROJECT_NAME);
} /* etc_ShortNameGet */

/*************************************************************************/
/*  etc_LongNameGet                                                      */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: LongName                                                     */
/*************************************************************************/
char *etc_LongNameGet (void)
{
   //return(LongName);
   return(PROJECT_NAME_LONG);
} /* etc_LongNameGet */

/*************************************************************************/
/*  etc_LocationGet                                                      */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: SysLocation                                                  */
/*************************************************************************/
char *etc_LocationGet (void)
{
   return(Location);
} /* etc_LocationGet */

/*************************************************************************/
/*  etc_IPDhcpIsUsed                                                     */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 0 = DHCP not used / 1 = DHCP used                            */
/*************************************************************************/
int etc_IPDhcpIsUsed (void)
{
   return(nDHCPIsUsed);   
} /* etc_IPDhcpIsUsed */

/*************************************************************************/
/*  etc_IPTnpIsES                                                        */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 0 = TNP read only / 1 = enable setup                         */
/*************************************************************************/
int etc_IPTnpIsES (void)
{
   return(nTNPIsES);   
} /* etc_IPTnpIsES */

/*************************************************************************/
/*  etc_IPmDNSIsUsed                                                     */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 0 = mDNS not used / 1 = DHCP used                            */
/*************************************************************************/
int etc_IPmDNSIsUsed (void)
{
   return(nMDNSIsUsed);   
} /* etc_IPmDNSIsUsed */

/*************************************************************************/
/*  etc_IPAddrGet                                                        */
/*                                                                       */
/*  Return the requested IP address.                                     */
/*                                                                       */
/*  In    : eAddress                                                     */
/*  Out   : none                                                         */
/*  Return: Address                                                      */
/*************************************************************************/
uint32_t etc_IPAddrGet (ETC_IP_ADDRESS eAddress)
{
   uint32_t dAddress = 0;
   
   if (eAddress < ETC_IP_MAX)
   {
      dAddress = IPAddrTable[eAddress];
   }
   
   return(dAddress);
} /* etc_IPAddrGet */

/*************************************************************************/
/*  etc_TimeZoneIDGet                                                    */
/*                                                                       */
/*  Return the TimeZoneID.                                               */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: nTimeZoneID                                                  */
/*************************************************************************/
int etc_TimeZoneIDGet (void)
{
   return(nTimeZoneID);
} /* etc_TimeZoneIDGet */

/*************************************************************************/
/*  etc_TimeZoneOffsetGet                                                */
/*                                                                       */
/*  Return the TimeZoneOffset.                                           */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: nTimeZoneOffset                                              */
/*************************************************************************/
int etc_TimeZoneOffsetGet (void)
{
   return(nTimeZoneOffset);
} /* etc_TimeZoneOffsetGet */

/*************************************************************************/
/*  etc_TimeZoneDstGet                                                   */
/*                                                                       */
/*  Return the TimeZoneDst.                                              */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: nTimeZoneDst                                                 */
/*************************************************************************/
int etc_TimeZoneDstGet (void)
{
   return(nTimeZoneDst);
} /* etc_TimeZoneDstGet */

/*************************************************************************/
/*  etc_TimeNTPAddrGet                                                   */
/*                                                                       */
/*  Return the TimeNTPAddr.                                              */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: dTimeNTPAddr                                                 */
/*************************************************************************/
uint32_t etc_TimeNTPAddrGet (void)
{
   return(dTimeNTPAddr);
} /* etc_TimeNTPAddrGet */

/*************************************************************************/
/*  etc_TimeNTPRefresh                                                   */
/*                                                                       */
/*  Return the TimeNTPRefresh.                                           */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: dTimeNTPRefresh                                              */
/*************************************************************************/
uint32_t etc_TimeNTPRefresh (void)
{
   return(dTimeNTPRefresh);
} /* etc_TimeNTPRefresh */

/*** EOF ***/

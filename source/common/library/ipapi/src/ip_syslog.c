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
*  08.02.2014  mifi  First Version, reference is RFC3164.
*  07.02.2015  mifi  Added include of "tal.h".
*  06.06.2020  mifi  Rename API.
**************************************************************************/
#define __IP_SYSLOG_C__

/*=======================================================================*/
/*  Include                                                              */
/*=======================================================================*/
#include <stdio.h>
#include <stdarg.h>
#include <limits.h>
#include <time.h>
#include "tal.h"
#include "ipstack.h"

#if (IP_DEFAULT_OPT_SYSL >= 1)
/*=======================================================================*/
/*  All extern definitions                                               */
/*=======================================================================*/

extern int vsnprintf(char *s, size_t n, const char *fmt, __va_list arg);
extern int snprintf(char *s, size_t n, const char *fmt, ...);

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

#define SYSLOG_BUFFER_SIZE    1024
#define SYSLOG_PORT           514

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

static int        InitDone = 0;

static char       SyslogBuffer[SYSLOG_BUFFER_SIZE];
static uint32_t   SyslogServer = 0;
static int        SyslogSock   = SOCKET_ERROR;
static OS_SEMA    SyslogSema;


static char       Hostname[17] = {0};

static char NULL_TAG[] = "null";

static char NameOfMonth[12][4] =
{
   "Jan", "Feb", "Mar", "Apr", "May", "Jun", 
   "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  SendData                                                             */
/*                                                                       */
/*  Send the data to the syslog server.                                  */
/*                                                                       */
/*  In    : buffer, size                                                 */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void SendData (char *Buffer, int Size)
{
   struct sockaddr_in Dest;
   
   Dest.sin_family      = AF_INET;
   Dest.sin_port        = htons(SYSLOG_PORT);
   Dest.sin_addr.s_addr = SyslogServer;

   sendto(SyslogSock, Buffer, Size, 0, (struct sockaddr *)&Dest, sizeof(Dest));

} /* SendData */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  IP_SYSL_ServerSet                                                    */
/*                                                                       */
/*  Set the IP address of the syslog server in host order.               */
/*                                                                       */
/*  In    : iface, addr                                                  */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_SYSL_ServerSet (uint8_t iface, uint32_t addr)
{
   (void)iface;
   
   /* Store syslog server info */
   SyslogServer = htonl(addr);

   /* Check if the semaphore must be created */
   if (0 == InitDone)
   {
      InitDone = 1;

      OS_RES_CREATE(&SyslogSema);
   }
   
   /* Check if we have already a socket */
   if (SOCKET_ERROR == SyslogSock)
   {
      /* No socket, create it */
      SyslogSock = socket(AF_INET, SOCK_DGRAM, 0);
   }
   else
   {
      /* Socket available, close and create a new one */
      closesocket(SyslogSock);
      SyslogSock = socket(AF_INET, SOCK_DGRAM, 0);
   }
   
   /* Check for valid hostname */
   if (0 == Hostname[0])
   {
      /* No hostname was set use IP address instead */
      htoa(IP_IF_AddrGet(0), Hostname, sizeof(Hostname));
   }   
   
} /* IP_SYSL_ServerSet */

/*************************************************************************/
/*  IP_SYSL_ServerGet                                                    */
/*                                                                       */
/*  Get the IP address of the syslog server in host order.               */
/*                                                                       */
/*  In    : iface                                                        */
/*  Out   : none                                                         */
/*  Return: Address in host order                                        */
/*************************************************************************/
uint32_t IP_SYSL_ServerGet (uint8_t iface)
{
   (void)iface;
   
   return(ntohl(SyslogServer));
} /* IP_SYSL_ServerGet */

/*************************************************************************/
/*  IP_SYSL_HostnameSet                                                  */
/*                                                                       */
/*  Set the hostname information.                                        */
/*                                                                       */
/*  In    : iface, name                                                  */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_SYSL_HostnameSet (uint8_t iface, char *name)
{
   (void)iface;

   if (name != NULL)
   {
      snprintf(Hostname, sizeof(Hostname), "%s", name);
   }
} /* IP_SYSL_HostnameSet */

/*************************************************************************/
/*  IP_SYSL_Printf                                                       */
/*                                                                       */
/*  printf like output function over the syslog service.                 */
/*                                                                       */
/*  In    : iface, fmt, ...                                              */
/*  Out   : none                                                         */
/*  Return: returns the number of chars which was "print" / -1 otherwise */
/*************************************************************************/
int IP_SYSL_Printf (uint8_t iface, const char *fmt, ...)
{
   int     n = -1;
   int     i;
   int     pri;
   va_list ap;

   (void)iface;
   
   /* Check if syslog server is available */
   if (SyslogServer != 0)
   {
      OS_RES_LOCK(&SyslogSema);
   
      /* Build priority */
      pri = SYSL_PRIORITY(SYSL_SEV_INFO, SYSL_FAC_USER);
      i = sprintf(SyslogBuffer, "<%d>", pri);
   
      /* Build message */
      va_start(ap, fmt);
      n = vsnprintf(&SyslogBuffer[i], sizeof(SyslogBuffer)-i, fmt, ap) + i;
      va_end(ap);

      /* Remove \r and \n from the end of the string */      
      while (n > 0)
      {
         if ((SyslogBuffer[n-1] == '\r') ||
             (SyslogBuffer[n-1] == '\n') )
         {
            SyslogBuffer[n-1] = 0;
            n--;
         }
         else
         {
            break;
         }
      }
      
      SendData(SyslogBuffer, n);
      
      OS_RES_FREE(&SyslogSema);
   } /* end if (SyslogServer != 0) */      
   
   return(n);
} /* IP_SYSL_Printf */

/*************************************************************************/
/*  IP_SYSL_Output                                                       */
/*                                                                       */
/*  Syslog output function.                                              */
/*                                                                       */
/*  In    : iface, priority, tag, fmt, ...                               */
/*  Out   : none                                                         */
/*  Return: 0 on success / -1 otherwise                                  */
/*************************************************************************/
int IP_SYSL_Output (uint8_t iface, uint8_t priority, char *tag, char *fmt, ...)
{
   int       rc = -1;
   int       p, dt, h, t, m, n;
   struct tm tm;
   va_list   ap;
   uint32_t  Unixtime;

   (void)iface;
   
   /* Check if syslog server is available */
   if (SyslogServer != 0)
   {
      OS_RES_LOCK(&SyslogSema);
      
      /* Get UTC */
      Unixtime  = OS_UnixtimeGet();
      
      /* Adjust by timezone */
      Unixtime += OS_TimezoneSecGet();
      
      /* Get local time */
      gmtime_r((time_t*)&Unixtime, &tm);
      
      /* Build priority */
      p = snprintf(SyslogBuffer, sizeof(SyslogBuffer), "<%d>", priority);
      
      /* Build date and time */
      dt = snprintf(&SyslogBuffer[p], sizeof(SyslogBuffer)-p, 
                    "%s %2d %02d:%02d:%02d ", NameOfMonth[tm.tm_mon], tm.tm_mday,
                    tm.tm_hour, tm.tm_min, tm.tm_sec);

      /* Build hostname */
      h = snprintf(&SyslogBuffer[p+dt], sizeof(SyslogBuffer)-p-dt, "%s ", Hostname);
      
      /* Build tag */
      if (NULL == tag)
      {
         tag = NULL_TAG;
      }   
      t = snprintf(&SyslogBuffer[p+dt+h], sizeof(SyslogBuffer)-p-dt-h, "%s: ", tag);
      
      /* Build message */
      va_start(ap, fmt);
      m = vsnprintf(&SyslogBuffer[p+dt+h+t], sizeof(SyslogBuffer)-p-dt-h-t, fmt, ap);
      va_end(ap);
      
      n = p+dt+h+t+m;
      SendData(SyslogBuffer, n);

      OS_RES_FREE(&SyslogSema);
   } /* end if (SyslogServer != 0) */      
   
   return(rc);
} /* IP_SYSL_Output */

#else

uint32_t IP_SYSL_ServerGet (uint8_t iface)
{
   (void)iface;

   return(0);
} /* IP_SYSL_ServerGet */

#endif /* (IP_DEFAULT_OPT_SYSL >= 1) */
/*** EOF ***/

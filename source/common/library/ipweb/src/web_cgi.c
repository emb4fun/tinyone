/**************************************************************************
*  Copyright (c) 2015 by Michael Fischer (www.emb4fun.de).
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
*  27.06.2015  mifi  First Version.
*  13.03.2016  mifi  Renamed to web_cgi.c.
**************************************************************************/
#define __WEB_CGI_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "tal.h"
#include "tcts.h"
#include "ipstack.h"
#include "ipweb.h"
#include "web_sid.h"
#include "mod_cgi.h"
#include "nvm.h"
#include "fs.h"

#include "terminal.h"

#if !defined(IP_WEB_CGI_EXT_CST) 
#define _IP_WEB_CGI_EXT_CST   0
#else
#define _IP_WEB_CGI_EXT_CST   IP_WEB_CGI_EXT_CST
#endif

#if (_IP_WEB_CGI_EXT_CST >= 1)
#include "web_cgi_ext_cst.h"
#endif

static const CGI_LIST_ENTRY CGIList[]; /*lint !e85*/

/*=======================================================================*/
/*  All extern data                                                      */
/*=======================================================================*/

#if defined(__ARM_ARCH_4T__) || defined(__ARM_ARCH_7A__)

extern uint8_t __stack_start__[];
extern uint8_t __stack_end__[];

/* IRQ */
extern uint8_t __stack_irq_start__[];
extern uint8_t __stack_irq_end__[];

/* FIQ */
extern uint8_t __stack_fiq_start__[];
extern uint8_t __stack_fiq_end__[];

/* Supervisor */
extern uint8_t __stack_svc_start__[];
extern uint8_t __stack_svc_end__[];

/* Abort */
extern uint8_t __stack_abt_start__[];
extern uint8_t __stack_abt_end__[];

/* Undefined */
extern uint8_t __stack_und_start__[];
extern uint8_t __stack_und_end__[];

#endif /* defined(__ARM_ARCH_4T__) || defined(__ARM_ARCH_7A__) */

#if defined(__ARM_ARCH_7EM__) || defined(__ARM_ARCH_7M__)

extern uint8_t __stack_start__[];
extern uint8_t __stack_end__[];

extern uint8_t __stack_process_start__[];
extern uint8_t __stack_process_end__[];

#endif /* defined(__ARM_ARCH_7EM__) || defined(__ARM_ARCH_7M__) */

extern char   *LastUpdateName;
extern uint8_t LastUpdateError;

void Time64Init (void);

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

#define PERMISSION_ADMIN   0xFFFFFFFF

#define CHECK_JSON_COMMA(_a,_b)     \
{                                   \
   if (1 == _a)                     \
   {                                \
      _a = 0;                       \
   }                                \
   else                             \
   {                                \
      s_puts(",", (_b)->s_stream);  \
   }                                \
}      

#define MAX_CGI_LIST_ENTRY    32


/*=======================================================================*/
/*  Definition of all global Data                                        */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

static CGI_LIST_ENTRY *ListTable[MAX_CGI_LIST_ENTRY];

static uint8_t *UploadBuffer = NULL;

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

#define MAX_UPSIZE  1460

#if !defined(MIN)
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

int CheckForPost (HTTP_REQUEST * req, int type)
{
   if (req->req_method != HTTP_METHOD_POST || req->req_type == NULL) {
      /* Bad method, POST expected. */
      return -1;
   }
   if (type == 0 && strncasecmp(req->req_type, "application/x-www-form-urlencoded", 33) == 0) {
      return 0;
   }
   if (strncasecmp(req->req_type, "multipart/form-data", 19) == 0) {
      return 0;
   }
   /* Bad content. */
   return -1;
} /* CheckForPost */

char *GetMultipartBoundary (HTTP_REQUEST *req)
{
   char *rp = NULL;
   const char *bptr;
   int blen;

   /* Make sure this is a multipart post. */
   if (CheckForPost(req, 1) == 0) {
      /* Retrieve the boundary string. */
      bptr = HttpArgValueSub(req->req_type, "boundary", &blen);
      if (bptr) {
         /* Build a delimiter string. */
         rp = xmalloc(XM_ID_WEB, (size_t)(blen + 3));
         if (rp) {
            rp[0] = '-';
            rp[1] = '-';
            memcpy(rp + 2, bptr, (size_t)blen);
            rp[blen + 2] = '\0';
         }
      }
   }
   return rp;
} /* GetMultipartBoundary */

static int UploadFile (HTTPD_SESSION *hs, web_upload_t *Info)
{
   char *upname = NULL;
   long avail;
   char *line;
   char *delim;
   const char *sub_ptr;
   int sub_len;
   int got = 0;
   HTTP_STREAM *stream = hs->s_stream;
   HTTP_REQUEST *req = &hs->s_req;
   long filesize;

   /* Retrieve the boundary string. */
   delim = GetMultipartBoundary(req);
   if (delim == NULL) {
      return(-1);
   }

   avail = req->req_length;
   line = xmalloc(XM_ID_WEB, (size_t)(MIN(avail, MAX_UPSIZE) + 1));
   if (line == NULL) {
      /* No memory. */
      xfree(delim);
      return(-1);
   }

   /* If we have a delimiter, then process the boundary content. */
   while (avail > 0) {
      /* Parse the next boundary header. */
      if (HttpParseMultipartHeader(hs, delim, &avail)) {
         /* Broken connection. */
         break;
      }
      /* Ignore headers without content disposition line. */
      if (req->req_bnd_dispo) {
         /* Retrieve the name of the form item. */
         sub_ptr = HttpArgValueSub(req->req_bnd_dispo, "name", &sub_len);
         if (sub_ptr) {
            /* The item named 'image' contains the binary data of the file. */
            if (strncasecmp(sub_ptr, "image", (size_t)sub_len) == 0) {
               int eol = 0;

               /* Get the upload file name. */
               sub_ptr = HttpArgValueSub(req->req_bnd_dispo, "filename", &sub_len);
               if (sub_ptr && sub_len) {
                  upname = xmalloc(XM_ID_WEB, (size_t)(sub_len + 1));
                  if (upname) {
                     memcpy(upname, sub_ptr, (size_t)sub_len);
                     upname[sub_len] = 0;
                           
                     Info->pFileName = xstrdup(XM_ID_WEB, upname);
                     xfree(upname);    
                  }
               }
               
               /* Recieve the binary data. */
               filesize = 0;
               while (avail) {
                  /* Read until the next boundary line. */
                  got = StreamReadUntilString(stream, delim, line, MIN(avail, MAX_UPSIZE));
                  if (got <= 0) {
                     break;
                  }
                  avail -= got;
                  
                  /* Write data to the local file, if one had been opened. */
                  //term_printf(".");
                  if (eol) {
                     //term_printf("eol");
                     if ((filesize + 2) < Info->lBufferSize)
                     {
                       Info->pBuffer[filesize++] = '\r';
                       Info->pBuffer[filesize++] = '\n';
                     }
                     else
                     {
                       Info->Error = 1;
                     }  
                  }
                  eol = 0;
                  if (got >= 2 && line[got - 2] == '\r' && line[got - 1] == '\n') {
                     eol = 1;
                     got -= 2;
                  }
                       
                  /* Write data is memory is available */
                  if ((filesize + got) < Info->lBufferSize)
                  {
                     memcpy(&Info->pBuffer[filesize], line, (size_t)got);
                  }
                  else
                  {
                     Info->Error = 1;
                  }
                  filesize += got;
               }
               //term_printf("\r\n");
                   
               Info->lFileSize = filesize;
                   
               if (got < 0) {
                  /* Broken connection. */
                  break;
               }
            }
            else if (strncasecmp(sub_ptr, "buffer", (size_t)sub_len) == 0) {
               got = StreamReadUntilChars(hs->s_stream, "\n", "\r", line, MIN(avail, MAX_UPSIZE));
               if (got <= 0) {
                  break;
               }
               Info->bBufferIndex = (uint8_t)atoi(line);
            }
            else if (strncasecmp(sub_ptr, "type", (size_t)sub_len) == 0) {
               got = StreamReadUntilChars(hs->s_stream, "\n", "\r", line, MIN(avail, MAX_UPSIZE));
               if (got <= 0) {
                  break;
               }
               Info->pType = xstrdup(XM_ID_WEB, line);
            }
            else if (strncasecmp(sub_ptr, "redir_ok", (size_t)sub_len) == 0) {
               got = StreamReadUntilChars(hs->s_stream, "\n", "\r", line, MIN(avail, MAX_UPSIZE));
               if (got <= 0) {
                  break;
               }
               Info->pRedirOK = xstrdup(XM_ID_WEB, line);
            }
            else if (strncasecmp(sub_ptr, "redir_err", (size_t)sub_len) == 0) {
               got = StreamReadUntilChars(hs->s_stream, "\n", "\r", line, MIN(avail, MAX_UPSIZE));
               if (got <= 0) {
                  break;
               }
               Info->pRedirERR = xstrdup(XM_ID_WEB, line);
            }
         }
      }
   }
   
   if ((NULL == Info->pFileName) || (0 == Info->lFileSize))
   {
      Info->Error = 5;
   }
   
   xfree(delim);
   xfree(line);
   
   return(0);
} /* UploadFile */ 

/*************************************************************************/
/*  GetStackFreeCount                                                    */
/*                                                                       */
/*  In    : pStart, pEnd                                                 */
/*  Out   : none                                                         */
/*  Return: Free entries of the stack                                    */
/*************************************************************************/
static uint32_t GetStackFreeCount (uint8_t *pStart, uint8_t *pEnd)
{
   uint32_t dFreeCount = 0;
   
   while(pStart < pEnd)
   {
      if (*pStart == 0xCC)
      {
         dFreeCount++;   
      }
      else
      {
         break;
      }
      pStart++;
   }
   
   return(dFreeCount);
} /* GetStackFreeCount */

/*************************************************************************/
/*  NetSet                                                               */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int NetSet (HTTPD_SESSION *hs)
{
   long      Avail;
   char    *pArg;
   char    *pVal;
   char    *pRedir = NULL;
   uint32_t  IPAddr   = 0;
   uint32_t  NETMask  = 0; 
   uint32_t  GWAddr   = 0;
   uint32_t  DNSAddr  = 0; 
   uint32_t  DNSAddr2 = 0; 
   uint32_t  SyslAddr = 0;
   uint32_t  UseTNPES = 0;
   uint32_t  UseDHCP  = 0; 
   uint32_t dPermission = hs->s_req.req_sid_perm;
   NVM_IP    IP;

   Avail = hs->s_req.req_length;
   while (Avail) 
   {
      pArg = HttpArgReadNext(hs, &Avail);
      if (pArg != NULL)
      {
         pVal = HttpArgValue(&hs->s_req);
         if (pVal)
         {
            if      (strcmp(pArg, "ip") == 0)
            {
               if (strcmp(pVal, "auto") == 0)
               {
                  UseDHCP = 1;
               }
            }
            else if (strcmp(pArg, "ip_addr") == 0)
            {
               IPAddr = atoh(pVal);
            }
            else if (strcmp(pArg, "ip_mask") == 0)
            {
               NETMask = atoh(pVal);
            }
            else if (strcmp(pArg, "ip_gate") == 0)
            {
               GWAddr = atoh(pVal);
            }
            else if (strcmp(pArg, "ip_dns") == 0)
            {
               DNSAddr = atoh(pVal);
            }
            else if (strcmp(pArg, "ip_dns_sec") == 0)
            {
               DNSAddr2 = atoh(pVal);
            }
            else if (strcmp(pArg, "ip_syslog") == 0)
            {
               SyslAddr = atoh(pVal);
            }
            else if (strcmp(pArg, "disc_ena") == 0)
            {
               if (strcmp(pVal, "on") == 0)
               {
                  UseTNPES = 1;
               }
            }
            else if (strcmp(pArg, "redir") == 0)
            {
               pRedir = xstrdup(XM_ID_WEB, pVal);
            }
         }            
      }
   } 

   if (pRedir != NULL)
   {
      if (PERMISSION_ADMIN == dPermission)
      {
         nvm_IPUseTNPESSet(UseTNPES);
         nvm_IPUseDHCPSet(UseDHCP);
         
         nvm_IPGet(&IP);
         IP.dIPAddr  = IPAddr;
         IP.dNETMask = NETMask;
         IP.dGWAddr  = GWAddr;
         IP.dDNSAddr = DNSAddr;
         IP.dDNS2Addr = DNSAddr2;
         IP.dSYSAddr  = SyslAddr;
         nvm_IPSet(&IP);
            
         HttpSendRedirection(hs, 303, pRedir, NULL);

         /*
          * Update new settings and reboot
          */ 
         nvm_Write();
         OS_TimeDly(2000);
         tal_CPUReboot();
      }
      else
      {
         HttpSendRedirection(hs, 303, "/403.htm", NULL);
      }         
   }      

   xfree(pRedir);
   
   (void)DNSAddr2;
   (void)SyslAddr;

   return(0);
} /* NetSet */

/*************************************************************************/
/*  TimeGet                                                              */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int TimeGet (HTTPD_SESSION *hs)
{
   char     *pArg;
   long       Avail;
   int        First = 1;
   uint32_t  Unixtime;
   uint32_t  Time;
   struct tm TMUTC;
   struct tm TM;
   
   IP_WEBS_CGISendHeader(hs);

   /* Get time info */
   Unixtime = OS_UnixtimeGet();
   Time     = (uint32_t)(Unixtime + OS_TimezoneSecGet() + OS_TimezoneDstSecGet());   /*lint !e737*/
   gmtime_r((const time_t*)&Unixtime, &TMUTC);
   gmtime_r((const time_t*)&Time, &TM);

   Avail = hs->s_req.req_length;
   s_puts("{", hs->s_stream);
   
   while (Avail) 
   {
      pArg = HttpArgReadNext(hs, &Avail);
      if      (strcmp(pArg, "date") == 0) 
      {
         CHECK_JSON_COMMA(First, hs);
         s_printf(hs->s_stream, "\"date\":\"%02d.%02d.%04d\"",
                  TM.tm_mday, TM.tm_mon+1, TM.tm_year+1900);
      }
      else if (strcmp(pArg, "time") == 0)
      {
         CHECK_JSON_COMMA(First, hs);
         s_printf(hs->s_stream, "\"time\":\"%02d:%02d:%02d\"",
                  TM.tm_hour, TM.tm_min, TM.tm_sec);
      }
      else if (strcmp(pArg, "utcdate") == 0)
      {
         CHECK_JSON_COMMA(First, hs);
         s_printf(hs->s_stream, "\"utcdate\":\"%02d.%02d.%04d\"",
                  TMUTC.tm_mday, TMUTC.tm_mon+1, TMUTC.tm_year+1900);
      }
      else if (strcmp(pArg, "utctime") == 0)
      {
         CHECK_JSON_COMMA(First, hs);
         s_printf(hs->s_stream, "\"utctime\":\"%02d:%02d:%02d\"",
                  TMUTC.tm_hour, TMUTC.tm_min, TMUTC.tm_sec);
      }
      else if (strcmp(pArg, "unixtime") == 0)
      {
         uint64_t _time64 = Unixtime;
         CHECK_JSON_COMMA(First, hs);
         s_printf(hs->s_stream, "\"unixtime\":\"%lld\"", _time64);
      }
      else if (strcmp(pArg, "dst") == 0)
      {
         CHECK_JSON_COMMA(First, hs);
         if (0 == OS_TimezoneDstSecGet())
         {
            s_printf(hs->s_stream, "\"dst\":\"0\"");
         }
         else
         {
            s_printf(hs->s_stream, "\"dst\":\"1\"");
         }            
      }
      else if (strcmp(pArg, "ntpsync") == 0)
      {
         
         CHECK_JSON_COMMA(First, hs);
         s_printf(hs->s_stream, "\"ntpsync\":\"%ld\"", IP_SNTP_NextSyncGet());
      }
      
      else if (strcmp(pArg, "uptime") == 0)
      {
         uint32_t Uptime = OS_TimeGetSeconds();
         int      Day  = 0;
         int      Hour = 0;
         int      Min  = 0;
         int      Sec  = 0;
   
         Day     = (int)(Uptime / 86400); /*lint !e838*/
         Uptime %= 86400;
         Hour    = (int)(Uptime / 3600);  /*lint !e838*/
         Uptime %= 3600;
         Min     = (int)(Uptime / 60);    /*lint !e838*/
         Sec     = (int)(Uptime % 60);    /*lint !e838*/
         
         CHECK_JSON_COMMA(First, hs);
         s_printf(hs->s_stream, "\"uptime\":\"%dd %02dh %02dm %02ds\"",
                  Day, Hour, Min, Sec);
      }
      else
      {
         /* Unknown argument */
         CHECK_JSON_COMMA(First, hs);
         s_printf(hs->s_stream, "\"%s\":\"---\"", pArg);
      }
   }

   s_puts("}", hs->s_stream);
   s_flush(hs->s_stream);

   return(0);
} /* TimeGet */

/*************************************************************************/
/*  TimeSet                                                              */
/*                                                                       */
/*  Set local time                                                       */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int TimeSet (HTTPD_SESSION *hs)
{
   long      Avail;
   char    *pArg;
   char    *pVal;
   char    *pDate  = NULL;
   char    *pTime  = NULL;
   char    *pRedir = NULL;
   int       Day;
   int       Mon;
   int       Year;
   int       Hour;
   int       Min;
   int       Sec;
   struct tm TM;
   time_t    Localtime;
   time_t    Unixtime;
   uint32_t dPermission = hs->s_req.req_sid_perm;
   
   Avail = hs->s_req.req_length;
   while (Avail) 
   {
      pArg = HttpArgReadNext(hs, &Avail);
      if (pArg != NULL)
      {
         pVal = HttpArgValue(&hs->s_req);
         if (pVal)
         {
            if      (strcmp(pArg, "date") == 0)
            {
               pDate = xstrdup(XM_ID_WEB, pVal);
            }
            else if (strcmp(pArg, "time") == 0)
            {
               pTime = xstrdup(XM_ID_WEB, pVal);
            }
            else if (strcmp(pArg, "redir") == 0)
            {
               pRedir = xstrdup(XM_ID_WEB, pVal);
            }
         }            
      }
   }   

   /* Check if all required parameters are available */
   if ((pRedir != NULL) && (pDate != NULL) && (pTime != NULL))
   {
      if (PERMISSION_ADMIN == dPermission)
      {
         /*
          * The format must be
          *    date: dd.mm.yyyy
          *    time: hh.mm.ss
          */
         Day  = ((pDate[0] - '0') * 10)   + (pDate[1] - '0');
         Mon  = ((pDate[3] - '0') * 10)   + (pDate[4] - '0');
         Year = ((pDate[6] - '0') * 1000) + ((pDate[7] - '0') * 100) + ((pDate[8] - '0') * 10) + (pDate[9] - '0');
         Hour = ((pTime[0] - '0') * 10)   + (pTime[1] - '0');
         Min  = ((pTime[3] - '0') * 10)   + (pTime[4] - '0');
         Sec  = ((pTime[6] - '0') * 10)   + (pTime[7] - '0');
      
         /* Convert to local time */
         TM.tm_mday = Day;
         TM.tm_mon  = Mon - 1;
         TM.tm_year = Year - 1900;
         TM.tm_hour = Hour;
         TM.tm_min  = Min;
         TM.tm_sec  = Sec;
         Localtime  = mktime(&TM); 
      
         /* Convert Loctime to Unixtime */
         Unixtime = (Localtime - OS_TimezoneSecGet()) - OS_TimezoneDstSecGet();
         OS_UnixtimeSet((uint32_t)Unixtime);
         
         tal_BoardRTCSetTM(&TM);
          
         HttpSendRedirection(hs, 303, pRedir, NULL);
      }
      else
      {
         HttpSendRedirection(hs, 303, "/403.htm", NULL);
      }         
   }   
   
   xfree(pDate);
   xfree(pTime);
   xfree(pRedir);

   return(0);
} /* TimeSet */

/*************************************************************************/
/*  TimezoneSet                                                          */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int TimezoneSet (HTTPD_SESSION *hs)
{
   long       Avail;
   char     *pArg;
   char     *pVal;
   char     *pRedir = NULL;
   int16_t   TimezoneID = -1;
   int16_t   Offset = -1;
   int8_t    Dst = 0;
   uint32_t dPermission = hs->s_req.req_sid_perm;
   NVM_TIME  Time;
   
   Avail = hs->s_req.req_length;
   while (Avail) 
   {
      pArg = HttpArgReadNext(hs, &Avail);
      if (pArg != NULL)
      {
         pVal = HttpArgValue(&hs->s_req);
         if (pVal)
         {
            if      (strcmp(pArg, "timezone") == 0)
            {
               TimezoneID = (int16_t)atoi(pVal);
            }
            else if (strcmp(pArg, "offset") == 0)
            {
               Offset = (int16_t)atoi(pVal);
            }
            else if (strcmp(pArg, "dst") == 0)
            {
               Dst = 1;
            }
            else if (strcmp(pArg, "redir") == 0)
            {
               pRedir = xstrdup(XM_ID_WEB, pVal);
            }
         }            
      }
   }   
      
   /* Check for valid pRedir */
   if ((pRedir != NULL) && (TimezoneID != -1) && (Offset != -1))
   {
      if (PERMISSION_ADMIN == dPermission)
      {
         OS_TimezoneIDSet(TimezoneID);
         OS_TimezoneMinSet(Offset);
         OS_TimezoneDstSet(Dst);
         
         nvm_TimeGet(&Time);
         Time.nZoneID     = TimezoneID;
         Time.nZoneOffset = Offset;
         Time.nZoneDst    = Dst;
         nvm_TimeSet(&Time);
      
         nvm_Write();
      
         HttpSendRedirection(hs, 303, pRedir, NULL);
      }
      else
      {
         HttpSendRedirection(hs, 303, "/403.htm", NULL);
      }         
   }      

   xfree(pRedir);

   return(0);
} /* TimezoneSet */

/*************************************************************************/
/*  TimeNTPSet                                                           */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int TimeNTPSet (HTTPD_SESSION *hs)
{
   long      Avail;
   char    *pArg;
   char    *pVal;
   char    *pRedir = NULL;
   int8_t    Sync = 0;
   uint32_t  NTPAddr = 0;
   uint32_t  Refresh = 0;
   uint32_t  OldNTPAddr = 0;
   uint32_t  OldRefresh = 0;
   uint32_t dPermission = hs->s_req.req_sid_perm;
   NVM_TIME  Time;

   Avail = hs->s_req.req_length;
   while (Avail) 
   {
      pArg = HttpArgReadNext(hs, &Avail);
      if (pArg != NULL)
      {
         pVal = HttpArgValue(&hs->s_req);
         if (pVal)
         {
            if      (strcmp(pArg, "ntp_ip") == 0)
            {
               NTPAddr = atoh(pVal);
            }
            else if (strcmp(pArg, "ntp_refresh") == 0)
            {
               Refresh = (uint32_t)atol(pVal);
            }
            else if (strcmp(pArg, "sync") == 0)
            {
               Sync = 1;
            }
            else if (strcmp(pArg, "redir") == 0)
            {
               pRedir = xstrdup(XM_ID_WEB, pVal);
            }
         }            
      }
   } 
   
   if (pRedir != NULL)
   {
      if (PERMISSION_ADMIN == dPermission)
      {
         nvm_TimeGet(&Time);
         
         OldNTPAddr = Time.dNTPAddr;
         OldRefresh = Time.dNTPRefresh;
   
         IP_SNTP_ServerSet(NTPAddr);
         IP_SNTP_RefreshSet(Refresh);
         
         Time.dNTPAddr    = NTPAddr;
         Time.dNTPRefresh = Refresh;
         
         nvm_TimeSet(&Time);
         
         /* Check if we must save the new values */
         if ((NTPAddr != OldNTPAddr) || (Refresh != OldRefresh))
         {
            nvm_Write();
         }   
      
         if (1 == Sync)
         {
            IP_SNTP_TimeGet();
         }
      
         HttpSendRedirection(hs, 303, pRedir, NULL);
      }
      else
      {
         HttpSendRedirection(hs, 303, "/403.htm", NULL);
      }         
   }      

   xfree(pRedir);

   return(0);
} /* TimeNTPSet */

/*************************************************************************/
/*  TimeSyncSNTP                                                         */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int TimeSyncSNTP (HTTPD_SESSION *hs)
{
   long      Avail;
   char    *pArg;
   char    *pVal;
   char    *pRedir = NULL;

   Avail = hs->s_req.req_length;
   while (Avail) 
   {
      pArg = HttpArgReadNext(hs, &Avail);
      if (pArg != NULL)
      {
         pVal = HttpArgValue(&hs->s_req);
         if (pVal)
         {
            if (strcmp(pArg, "redir") == 0)
            {
               pRedir = xstrdup(XM_ID_WEB, pVal);
            }
         }            
      }
   } 
   
   if (pRedir != NULL)
   {
      IP_SNTP_TimeSync();
      HttpSendRedirection(hs, 303, pRedir, NULL);
   }      

   xfree(pRedir);

   return(0);
} /* TimeSyncSNTP */

/*************************************************************************/
/*  StatStack                                                            */
/*                                                                       */
/*  Output task stack statistics.                                        */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int StatStack (HTTPD_SESSION *hs)
{
   OS_TCB   *pTCB;
   uint8_t  *pStart;
   uint8_t  *pEnd;
   uint32_t  dSize;
   uint32_t  dFree;
   uint16_t  wTime1;
   uint16_t  wTime2;
   uint16_t  wDimCnt = 0;   
   
   IP_WEBS_CGISendHeader(hs);
   
   /* Get start of list */
   pTCB = OS_TaskGetList(); 
   while (pTCB != NULL)
   {
      /* Start of row */
      if (0 == (wDimCnt & 0x01))
      { 
         s_printf(hs->s_stream, "<tr>\r\n");
      }
      else
      {
         s_printf(hs->s_stream, "<tr class=\"dim\">\r\n");
      }   
      
      /* colum start */ 
      s_printf(hs->s_stream, "  <td>&nbsp;</td>\r\n");
   
      /* colum task name */
      if (pTCB->Name[0] != 0)
      {
         s_printf(hs->s_stream, "  <td>%s</td>\r\n", pTCB->Name);
      }
      else
      {
         s_printf(hs->s_stream, "  <td>%s</td>\r\n", "----------------");
      }

      pStart = pTCB->pStackStart;
      pEnd   = (uint8_t*)pStart + pTCB->wStackSize;
      dSize  = (uint32_t)pEnd - (uint32_t)pStart;
      dFree  = GetStackFreeCount(pStart, pEnd);
      wTime1 = (uint16_t)(pTCB->dStatUsage / 100);
      wTime2 = (uint16_t)(pTCB->dStatUsage % 100);
      
      /* colum prio, size, used, free and cpu load  */
      s_printf(hs->s_stream, "  <td>%d</td>\r\n",       pTCB->nPrio);
      s_printf(hs->s_stream, "  <td>%d</td>\r\n",       dSize);
      s_printf(hs->s_stream, "  <td>%d</td>\r\n",       (dSize - dFree));
      s_printf(hs->s_stream, "  <td>%d</td>\r\n",       dFree);
      s_printf(hs->s_stream, "  <td>%2d.%02d</td>\r\n", wTime1, wTime2);
      
      /* colum end */ 
      s_printf(hs->s_stream, "  <td>\r\n");
      
      /* row end */ 
      s_printf(hs->s_stream, "</td>");
      
      s_flush(hs->s_stream);
      
      wDimCnt++;
      pTCB = pTCB->pTaskNext;
   }   
         
   return(0);
} /* StatStack */

/*************************************************************************/
/*  StatRun                                                              */
/*                                                                       */
/*  Output runtime stack statistics.                                     */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int StatRun (HTTPD_SESSION *hs)
{
   uint32_t dSize;
   uint32_t dFree;
   
   IP_WEBS_CGISendHeader(hs);

#if defined(__ARM_ARCH_4T__) || defined(__ARM_ARCH_7A__)

   /* User/System Stack */
   dSize = (uint32_t)(__stack_end__ - __stack_start__);
   dFree = GetStackFreeCount(__stack_start__, __stack_end__);
   s_printf(hs->s_stream, "<tr>\r\n");
   s_printf(hs->s_stream, "  <td>&nbsp;</td>\r\n");
   s_printf(hs->s_stream, "  <td>User/System Stack</td>\r\n");
   s_printf(hs->s_stream, "  <td>%4d</td>\r\n", dSize);
   s_printf(hs->s_stream, "  <td>%4d</td>\r\n", (dSize - dFree));
   s_printf(hs->s_stream, "  <td>%4d</td>\r\n", dFree);
   s_printf(hs->s_stream, "  <td>&nbsp;</td>\r\n");
   s_printf(hs->s_stream, "</tr>\r\n");
   
   /* IRQ Stack */
   dSize = (uint32_t)(__stack_irq_end__ - __stack_irq_start__);
   dFree = GetStackFreeCount(__stack_irq_start__, __stack_irq_end__);
   s_printf(hs->s_stream, "<tr class=\"dim\">\r\n");
   s_printf(hs->s_stream, "  <td>&nbsp;</td>\r\n");
   s_printf(hs->s_stream, "  <td>IRQ Stack</td>\r\n");
   s_printf(hs->s_stream, "  <td>%4d</td>\r\n", dSize);
   s_printf(hs->s_stream, "  <td>%4d</td>\r\n", (dSize - dFree));
   s_printf(hs->s_stream, "  <td>%4d</td>\r\n", dFree);
   s_printf(hs->s_stream, "  <td>&nbsp;</td>\r\n");
   s_printf(hs->s_stream, "</tr>\r\n");

   /* FIQ Stack */
   dSize = (uint32_t)(__stack_fiq_end__ - __stack_fiq_start__);
   dFree = GetStackFreeCount(__stack_fiq_start__, __stack_fiq_end__);
   s_printf(hs->s_stream, "<tr>\r\n");
   s_printf(hs->s_stream, "  <td>&nbsp;</td>\r\n");
   s_printf(hs->s_stream, "  <td>FIQ Stack</td>\r\n");
   s_printf(hs->s_stream, "  <td>%4d</td>\r\n", dSize);
   s_printf(hs->s_stream, "  <td>%4d</td>\r\n", (dSize - dFree));
   s_printf(hs->s_stream, "  <td>%4d</td>\r\n", dFree);
   s_printf(hs->s_stream, "  <td>&nbsp;</td>\r\n");
   s_printf(hs->s_stream, "</tr>\r\n");

   /* Supervisior Stack */
   dSize = (uint32_t)(__stack_svc_end__ - __stack_svc_start__);
   dFree = GetStackFreeCount(__stack_svc_start__, __stack_svc_end__);
   s_printf(hs->s_stream, "<tr class=\"dim\">\r\n");
   s_printf(hs->s_stream, "  <td>&nbsp;</td>\r\n");
   s_printf(hs->s_stream, "  <td>Supervisior Stack</td>\r\n");
   s_printf(hs->s_stream, "  <td>%4d</td>\r\n", dSize);
   s_printf(hs->s_stream, "  <td>%4d</td>\r\n", (dSize - dFree));
   s_printf(hs->s_stream, "  <td>%4d</td>\r\n", dFree);
   s_printf(hs->s_stream, "  <td>&nbsp;</td>\r\n");
   s_printf(hs->s_stream, "</tr>\r\n");

   /* Abort Stack */
   dSize = (uint32_t)(__stack_abt_end__ - __stack_abt_start__);
   dFree = GetStackFreeCount(__stack_abt_start__, __stack_abt_end__);
   s_printf(hs->s_stream, "<tr>\r\n");
   s_printf(hs->s_stream, "  <td>&nbsp;</td>\r\n");
   s_printf(hs->s_stream, "  <td>Abort Stack</td>\r\n");
   s_printf(hs->s_stream, "  <td>%4d</td>\r\n", dSize);
   s_printf(hs->s_stream, "  <td>%4d</td>\r\n", (dSize - dFree));
   s_printf(hs->s_stream, "  <td>%4d</td>\r\n", dFree);
   s_printf(hs->s_stream, "  <td>&nbsp;</td>\r\n");
   s_printf(hs->s_stream, "</tr>\r\n");

   /* Undefined Stack */
   dSize = (uint32_t)(__stack_und_end__ - __stack_und_start__);
   dFree = GetStackFreeCount(__stack_und_start__, __stack_und_end__);
   s_printf(hs->s_stream, "<tr class=\"dim\">\r\n");
   s_printf(hs->s_stream, "  <td>&nbsp;</td>\r\n");
   s_printf(hs->s_stream, "  <td>Undefined Stack</td>\r\n");
   s_printf(hs->s_stream, "  <td>%4d</td>\r\n", dSize);
   s_printf(hs->s_stream, "  <td>%4d</td>\r\n", (dSize - dFree));
   s_printf(hs->s_stream, "  <td>%4d</td>\r\n", dFree);
   s_printf(hs->s_stream, "  <td>&nbsp;</td>\r\n");
   s_printf(hs->s_stream, "</tr>\r\n");
   
#endif /* defined(__ARM_ARCH_4T__) || defined(__ARM_ARCH_7A__) */

#if defined(__ARM_ARCH_7EM__) || defined(__ARM_ARCH_7M__)

   /* Main Stack */
   dSize = (uint32_t)(__stack_end__ - __stack_start__);
   dFree = GetStackFreeCount(__stack_start__, __stack_end__);
   s_printf(hs->s_stream, "<tr>\r\n");
   s_printf(hs->s_stream, "  <td>&nbsp;</td>\r\n");
   s_printf(hs->s_stream, "  <td>Main Stack</td>\r\n");
   s_printf(hs->s_stream, "  <td>%4d</td>\r\n", dSize);
   s_printf(hs->s_stream, "  <td>%4d</td>\r\n", (dSize - dFree));
   s_printf(hs->s_stream, "  <td>%4d</td>\r\n", dFree);
   s_printf(hs->s_stream, "  <td>&nbsp;</td>\r\n");
   s_printf(hs->s_stream, "</tr>\r\n");
   
   /* Process Stack */
   dSize = (uint32_t)(__stack_process_end__ - __stack_process_start__);
   dFree = GetStackFreeCount(__stack_process_start__, __stack_process_end__);
   s_printf(hs->s_stream, "<tr class=\"dim\">\r\n");
   s_printf(hs->s_stream, "  <td>&nbsp;</td>\r\n");
   s_printf(hs->s_stream, "  <td>Process Stack</td>\r\n");
   s_printf(hs->s_stream, "  <td>%4d</td>\r\n", dSize);
   s_printf(hs->s_stream, "  <td>%4d</td>\r\n", (dSize - dFree));
   s_printf(hs->s_stream, "  <td>%4d</td>\r\n", dFree);
   s_printf(hs->s_stream, "  <td>&nbsp;</td>\r\n");
   s_printf(hs->s_stream, "</tr>\r\n");

#endif /* defined(__ARM_ARCH_7EM__) || defined(__ARM_ARCH_7M__) */
   
   return(0);
} /* StatRun */

/*************************************************************************/
/*  StatMem                                                              */
/*                                                                       */
/*  Output runtime stack statistics.                                     */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int StatMem (HTTPD_SESSION *hs)
{
   char    *pName;
   uint32_t dSize;
   uint32_t dUsed;
   uint32_t dFree;
   uint32_t dPeak;
   uint16_t wNext;
   uint16_t wIndex = 0;   
   uint16_t wDimCnt = 0;   
   
   IP_WEBS_CGISendHeader(hs);

   pName = tal_MEMInfoGet(wIndex, &dSize, &dUsed, &dFree, &dPeak, &wNext);
   while (wNext != 0)
   {
      if (pName != NULL)
      {
         /* Start of row */
         if (0 == (wDimCnt & 0x01))
         { 
            s_printf(hs->s_stream, "<tr>\r\n");
         }
         else
         {
            s_printf(hs->s_stream, "<tr class=\"dim\">\r\n");
         }   
      
         /* colum start */ 
         s_printf(hs->s_stream, "  <td>&nbsp;</td>\r\n");
           
         s_printf(hs->s_stream, "  <td>%s</td>\r\n", pName);
         s_printf(hs->s_stream, "  <td style=\"text-align:right\">%d</td>\r\n", dSize);
         s_printf(hs->s_stream, "  <td style=\"text-align:right\">%d</td>\r\n", dUsed);
         s_printf(hs->s_stream, "  <td style=\"text-align:right\">%d</td>\r\n", dFree);
         s_printf(hs->s_stream, "  <td style=\"text-align:right\">%d</td>\r\n", dPeak);
      
         /* colum end */ 
         s_printf(hs->s_stream, "  <td>\r\n");
      
         /* row end */ 
         s_printf(hs->s_stream, "</td>");
      
         s_flush(hs->s_stream);
         
         wDimCnt++;
      }         
      
      wIndex++;

      pName = tal_MEMInfoGet(wIndex, &dSize, &dUsed, &dFree, &dPeak, &wNext);
   }   
   
   return(0);
} /* StatMem */

/*************************************************************************/
/*  CPULoad                                                              */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int CPULoad (HTTPD_SESSION *hs)
{
   IP_WEBS_CGISendHeader(hs);

   s_printf(hs->s_stream, "{\"cpuload\":\"%d\"}", OS_StatGetCPULoad());
   s_flush(hs->s_stream);

   return(0);
} /* CPULoad */

/*************************************************************************/
/*  Upload                                                               */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int Upload (HTTPD_SESSION *hs)
{
   web_upload_t Info;
   int         nErr = -1;
   
   if (NULL == UploadBuffer)
   {
      UploadBuffer = xmalloc(XM_ID_HEAP, WEB_UPLOAD_BUFFER_SIZE);   
      if (NULL == UploadBuffer) return(-1);
   }
      
   memset(&Info, 0, sizeof(web_upload_t));
   
   Info.pBuffer     = UploadBuffer;
   Info.lBufferSize = WEB_UPLOAD_BUFFER_SIZE;

   UploadFile(hs, &Info);

   if (0 == Info.Error)
   {
      nErr = fs_Upload(&Info);      
      
      xfree(LastUpdateName);
      if (Info.pFileName != NULL)
      {
         LastUpdateName  = xstrdup(XM_ID_WEB, Info.pFileName);
      }
      else
      {
         LastUpdateName  = xstrdup(XM_ID_WEB, "unknown");
      }
      LastUpdateError = (uint8_t)nErr;
   }
   else
   {
      LastUpdateError = (uint8_t)Info.Error;
   }
   
   if (0 == nErr)
   {
      if (Info.pRedirOK != NULL)
      {
         HttpSendRedirection(hs, 303, Info.pRedirOK, NULL);
      }         
   }
   else
   {
      if (Info.pRedirERR != NULL)
      {
         HttpSendRedirection(hs, 303, Info.pRedirERR, NULL);
      }         
   }

   xfree(Info.pFileName);
   xfree(Info.pType);
   xfree(Info.pRedirOK);
   xfree(Info.pRedirERR);
   
   xfree(UploadBuffer);
   UploadBuffer = NULL;

   return(0);
} /* Upload */

/*************************************************************************/
/*  Upgrade                                                              */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int Upgrade (HTTPD_SESSION *hs)
{
   int     nErr = -1;
   long     Avail;
   char   *pArg;
   char   *pVal;
   char   *pRedirOK  = NULL;
   char   *pRedirERR = NULL;
   uint8_t bIndex    = 0;
   uint8_t bType     = 0;

   Avail = hs->s_req.req_length;
   while (Avail) 
   {
      pArg = HttpArgReadNext(hs, &Avail);
      if (pArg != NULL)
      {
         pVal = HttpArgValue(&hs->s_req);
         if (pVal)
         {            
            if      (strcmp(pArg, "buffer") == 0)
            {  
               bIndex = (uint8_t)atoi(pVal);
            }
            else if (strcmp(pArg, "type") == 0)
            {
               if      (0 == strcmp(pVal, "web"))
               {
                  bType = 1;
               } 
               else if (0 == strcmp(pVal, "fw"))
               {
                  bType = 2;
               }
            }
            
            else if (strcmp(pArg, "redir_ok") == 0)
            {
               pRedirOK = xstrdup(XM_ID_WEB, pVal);
            }
            else if (strcmp(pArg, "redir_err") == 0)
            {
               pRedirERR = xstrdup(XM_ID_WEB, pVal);
            }
         }            
      }
   }   
   
   if ((pRedirOK != NULL) && (pRedirERR != NULL))
   {
      if ((1 == bIndex) || (2 == bIndex))
      {
         switch (bType)
         {
            case 1: nErr = fs_UpgradeWeb(bIndex); break;
            case 2: nErr = fs_UpgradeFw(bIndex);  break;
            default: /* Do nothing */             break;
         }
      }

      if (0 == nErr)
      {   
         HttpSendRedirection(hs, 303, pRedirOK, NULL);
            
         if (2 == bType)
         {         
            OS_TimeDly(2000);
            tal_CPUReboot();
         }            
      }
      else
      {
         LastUpdateError = 6;
         HttpSendRedirection(hs, 303, pRedirERR, NULL);
      }   
   }
   
   xfree(pRedirOK);
   xfree(pRedirERR);

   return(0);
} /* Upgrade */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  IP_WEBS_CGIInit                                                      */
/*                                                                       */
/*  Initialize the CGI functionality of the web server.                  */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_WEBS_CGIInit (void)
{
   memset(ListTable, 0x00, sizeof(ListTable));
   
   IP_WEBS_CGIListAdd((CGI_LIST_ENTRY*)CGIList);
   
   Time64Init();
   
} /* IP_WEBS_CGIInit */

/*************************************************************************/
/*  IP_WEBS_CGIListAdd                                                   */
/*                                                                       */
/*  Add a CGI function list to the CGI core.                             */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_WEBS_CGIListAdd (CGI_LIST_ENTRY *pList)
{
   uint16_t Index;
   
   for(Index = 0; Index < MAX_CGI_LIST_ENTRY; Index++)
   {
      if (0 == ListTable[Index])
      {
         ListTable[Index] = pList;
         break;   
      }
   }

} /* IP_WEBS_CGIListAdd */

/*************************************************************************/
/*  IP_WEBS_CGIStart                                                     */
/*                                                                       */
/*  Start the CGI functionality of the web server.                       */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_WEBS_CGIStart (void)
{
   uint16_t         ListIndex = 0;
   CGI_LIST_ENTRY *pList;

   /* Loop over the registered lists */   
   while ((ListIndex != MAX_CGI_LIST_ENTRY) && (ListTable[ListIndex] != NULL))
   {
      /* Get list */
      pList = ListTable[ListIndex];   

      /* Loop over the actual list */   
      while(pList->Var != NULL)
      {
         HttpRegisterCgiFunction(pList->Var,  pList->pFunc);
   
         /* Switch to next entry inside the list */
         pList++;
      }
      
      /* Switch to next list */  
      ListIndex++;
   }      
   
} /* IP_WEBS_CGIStart */

/*************************************************************************/
/*  IP_WEBS_CGISendHeader                                                */
/*                                                                       */
/*  Send a CGI web header.                                               */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_WEBS_CGISendHeader (HTTPD_SESSION *hs)
{
   HttpSendHeaderTop(hs, 200);
   s_puts("Cache-Control: no-cache, must-revalidate\r\n", hs->s_stream);
   s_puts("Expires: Sat, 26 Jul 1997 05:00:00 GMT\r\n", hs->s_stream);   
   HttpSendHeaderBottom(hs, NULL, NULL, -1, 0);   
   
   s_set_flags(hs->s_stream, S_FLG_CHUNKED);
   
} /* IP_WEBS_CGISendHeader */

/*************************************************************************/
/*  IP_WEBS_CGISendHeaderOctetStream                                     */
/*                                                                       */
/*  Send a CGI, octet-stream, web header.                                */
/*                                                                       */
/*  In    : hs                                                           */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IP_WEBS_CGISendHeaderOctetStream (HTTPD_SESSION *hs)
{
   HttpSendHeaderTop(hs, 200);
   s_puts("Cache-Control: no-cache, must-revalidate\r\n", hs->s_stream);
   s_puts("Expires: Sat, 26 Jul 1997 05:00:00 GMT\r\n", hs->s_stream);   
   HttpSendHeaderBottom(hs, "application", "octet-stream", -1, 0);   
   
   s_set_flags(hs->s_stream, S_FLG_CHUNKED);
   
} /* IP_WEBS_CGISendHeaderOctetStream */


/*
 * CGI variable list
 */
static const CGI_LIST_ENTRY CGIList[] =   /*lint !e31*/
{
   { "cgi-bin/net_set.cgi",         NetSet       },

   { "cgi-bin/time_get.cgi",        TimeGet      },
   { "cgi-bin/time_set.cgi",        TimeSet      },
   { "cgi-bin/time_zone_set.cgi",   TimezoneSet  },
   { "cgi-bin/time_ntp_set.cgi",    TimeNTPSet   },
   { "cgi-bin/time_sync_sntp.cgi",  TimeSyncSNTP },
   
   { "cgi-bin/stat_stack.cgi",      StatStack    },   
   { "cgi-bin/stat_run.cgi",        StatRun      },   
   { "cgi-bin/stat_mem.cgi",        StatMem      },   
   
   { "cgi-bin/cpuload.cgi",         CPULoad      },
   
   { "cgi-bin/upload.cgi",          Upload       },
   { "cgi-bin/upgrade.cgi",         Upgrade      },

#if (_IP_WEB_CGI_EXT_CST >= 1)
   WEB_CGI_EXT_CST
#endif   
      
   {NULL, NULL}
};

/*** EOF ***/

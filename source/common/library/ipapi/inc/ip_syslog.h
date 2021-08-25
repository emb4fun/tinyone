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
#if !defined(__IP_SYSLOG_H__)
#define __IP_SYSLOG_H__

/**************************************************************************
*  Includes
**************************************************************************/
#include <stdint.h>

/**************************************************************************
*  Global Definitions
**************************************************************************/

/**************************************************************************
*  Macro Definitions
**************************************************************************/

#define SL_PRINTF(_a)   IPSyslPrintf _a

/*
 * Syslog message facilities, see RFC3164
 */
#define SYSL_FAC_KERN      0     /* kernel messages */ 
#define SYSL_FAC_USER      1     /* user-level messages */ 
#define SYSL_FAC_MAIL      2     /* mail system */
#define SYSL_FAC_DAEMON    3     /* system daemons */
#define SYSL_FAC_AUTH      4     /* security/authorization messages */
#define SYSL_FAC_SYSLOG    5     /* messages generated internally by syslogd */
#define SYSL_FAC_LPR       6     /* line printer subsystem */
#define SYSL_FAC_NEWS      7     /* network news subsystem */
#define SYSL_FAC_UUCP      8     /* UUCP subsystem */
#define SYSL_FAC_CLOCK     9     /* clock daemon */
#define SYSL_FAC_AUTHPRIV  10    /* security/authorization messages */
#define SYSL_FAC_FTP       11    /* FTP daemon */
#define SYSL_FAC_NTP       12    /* NTP subsystem */
#define SYSL_FAC_LAUDIT    13    /* log audit */
#define SYSL_FAC_LALERT    14    /* log alert */
#define SYSL_FAC_CLOCK2    15    /* clock daemon */
#define SYSL_FAC_LOCAL0    16    /* local use 0 */
#define SYSL_FAC_LOCAL1    17    /* local use 1 */
#define SYSL_FAC_LOCAL2    18    /* local use 2 */
#define SYSL_FAC_LOCAL3    19    /* local use 3 */
#define SYSL_FAC_LOCAL4    20    /* local use 4 */
#define SYSL_FAC_LOCAL5    21    /* local use 5 */
#define SYSL_FAC_LOCAL6    22    /* local use 6 */
#define SYSL_FAC_LOCAL7    23    /* local use 7 */


/*
 * SYSLOG message severity see, see RFC3164
 */
#define SYSL_SEV_EMERG     0     /* Emergency: system is unusable */
#define SYSL_SEV_ALERT     1     /* Alert: action must be taken immediately */
#define SYSL_SEV_CRIT      2     /* Critical: critical conditions */
#define SYSL_SEV_ERR       3     /* Error: error conditions */
#define SYSL_SEV_WARNING   4     /* Warning: warning conditions */
#define SYSL_SEV_NOTICE    5     /* Notice: normal but significant condition */
#define SYSL_SEV_INFO      6     /* Informational: informational messages */
#define SYSL_SEV_DEBUG     7     /* Debug: debug-level messages */


#define SYSL_PRIORITY(_sev,_fac) ((_fac << 3) | _sev)

/**************************************************************************
*  Functions Definitions
**************************************************************************/

void     IP_SYSL_ServerSet (uint8_t iface, uint32_t addr);
uint32_t IP_SYSL_ServerGet (uint8_t iface);
void     IP_SYSL_HostnameSet (uint8_t iface, char *name);

int      IP_SYSL_Printf (uint8_t iface, const char *fmt, ...);
int      IP_SYSL_Output (uint8_t iface, uint8_t priority, char *tag, char *fmt, ...);

#endif /* !__IP_SYSLOG_H__ */

/*** EOF ***/

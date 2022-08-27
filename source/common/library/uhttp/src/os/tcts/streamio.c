/**************************************************************************
*  Copyright (c) 2015-2018 by Michael Fischer (www.emb4fun.de).
*  All rights reserved.
*
*  This module based on the win example from the Ethernut
*  (www.ethernut.de) project. Therefore:
*
*  Partial Copyright (C) 2012 by egnite GmbH.
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*  
*  1. Redistributions of source code must retain the above copyright 
*     notice, this list of conditions and the following disclaimer.
*  2. Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the 
*     documentation and/or other materials provided with the distribution.
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
*  08.02.2015  mifi  First Version.
**************************************************************************/
#define __STREAMIO_C__

/*=======================================================================*/
/*  Include                                                              */
/*=======================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <stdint.h>

#include "tal.h"
#include "ipstack.h"

#include "lwip\api.h"
#include "lwip\priv\sockets_priv.h"

#include "pro\uhttp\streamio.h"

/*=======================================================================*/
/*  All extern data                                                      */
/*=======================================================================*/

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all global Data                                        */
/*=======================================================================*/   

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

static ssl_write_t *ssl_write = NULL;
static ssl_read_t  *ssl_read  = NULL;

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

static int _recv (int s, void *mem, size_t len, int flags)
{
   int                got = -1;
   struct lwip_sock  *sock;
   void              *ssl;
   
   sock = lwip_socket_dbg_get_socket(s); 
   if (NULL == sock->conn->ssl)
   {
      got = recv(s, mem, len, flags);
   }
   else
   {
      ssl = sock->conn->ssl;
      if (ssl_read != NULL)
      {
         got  = ssl_read(ssl, mem, len);      
      }   
   } 
   
   return(got);
} /* _recv */

static int _send (int s, const void *dataptr, size_t size)
{
   int                rc = -1;
   struct lwip_sock  *sock;
   void              *ssl;
   
   sock = lwip_socket_dbg_get_socket(s); 
   if (NULL == sock->conn->ssl)
   {
      rc = send(s, dataptr, size, 0);  
   }
   else
   {
      ssl = sock->conn->ssl;
      if (ssl_write != NULL)
      {
         rc = ssl_write(ssl, dataptr, size);
      }
   }
   
   return(rc);
} /* _send */

static int _out (HTTP_STREAM *sp, const void *dataptr, size_t size)
{
   int      rc = 0;
   uint8_t *data = (uint8_t*)dataptr;
   int      free;
   int      copy;
   
   while (size > 0)
   {
      free = STREAM_OBUF_SIZE - sp->strm_olen;
      copy = (free >= size) ? size : free;
      
      memcpy(&sp->strm_obuf[sp->strm_olen], data, copy);
      sp->strm_olen += copy;
      data          += copy;
      size          -= copy;

      if (STREAM_OBUF_SIZE == sp->strm_olen)
      {
         rc = s_flush(sp);
      }
   }

   return(rc);
} /* _out */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

int StreamInit (void)
{
   return(0);
} /* StreamInit */


int StreamInitSsl (ssl_write_t *func_write, ssl_read_t *func_read)
{
    ssl_write = func_write;
    ssl_read  = func_read;

    return 0;
} /* StreamInitSsl */

int StreamUpdateBufferJson (HTTP_STREAM *sp, int siz, char *data)
{
   int  rc = -1;
   int  got;
   int  strm_siz;
   
   while (siz > 0)
   {
      /* Get size from the current stream buffer. */
      strm_siz = sp->strm_ilen - sp->strm_ipos;
      if (strm_siz != 0)
      {
         /* Read data from the actual buffer */
         memcpy(data, &sp->strm_ibuf[sp->strm_ipos], strm_siz);
         siz  -= strm_siz;
         data += strm_siz;
         
         if (siz > 0)
         {
            /* No more buffered data, re-fill the buffer. */
            got = _recv(sp->strm_csock, sp->strm_ibuf, 1460, 0);
            if (got <= 0)
            {
               /* Error */
               break;
            }
            else
            {
               /* Store data size of the stream */
               sp->strm_ilen = got;
               sp->strm_ipos = 0;
            }
         }
      }
      else
      {
         /* No more buffered data, re-fill the buffer. */
         got = _recv(sp->strm_csock, sp->strm_ibuf, 1460, 0);
         if (got <= 0)
         {
            /* Error */
            break;
         }
         else
         {
            /* Store data size of the stream */
            sp->strm_ilen = got;
            sp->strm_ipos = 0;
         }
      }
   } 
   
   /* Check if we got all data */
   if (0 == siz)
   {
      /* No error */
      rc = 0;
   }
   
   return(rc);
} /* StreamUpdateBufferJson */

int StreamReadUntilChars (HTTP_STREAM *sp, const char *delim, const char *ignore, char *buf, int siz)
{
   int  rc = 0;
   int  skip = 0;
   char ch;

   HTTP_ASSERT(sp != NULL);

   /* Do not read more characters than requested. */
   while (rc < siz)
   {
      /* Check the current stream buffer. */
      if (sp->strm_ipos == sp->strm_ilen)
      {
         /* No more buffered data, re-fill the buffer. */
         int got = _recv(sp->strm_csock, sp->strm_ibuf, 1460, 0);
         if (got <= 0)
         {
            /* Broken connection or timeout. */
            if (got < 0) 
            {
               rc = -1;
               skip = 0;
            }
            break;
         }
         
         sp->strm_ilen = got;
         sp->strm_ipos = 0;
      }
      
      ch = sp->strm_ibuf[sp->strm_ipos];
      sp->strm_ipos++;
      
      if (rc == 0 && ch == ' ')
      {
         /* Skip leading spaces. */
         skip++;
      } 
      else 
      {
         rc++;
         
         if (delim && strchr(delim, ch)) 
         {
            /* Delimiter found. */
            break;
         }
         
         if (buf && (ignore == NULL || strchr(ignore, ch) == NULL)) 
         {
            /* Add valid character to application buffer. */
            *buf++ = ch;
         }
      }
   }
   
   if (buf) 
   {
      *buf = '\0';
   }
   
   return(rc + skip);
} /* StreamReadUntilChars */


int StreamReadUntilString (HTTP_STREAM *sp, const char *delim, char *buf, int siz)
{
   int rc = 0;
   int n;
   int i;
   int delen = strlen(delim);

   HTTP_ASSERT(sp != NULL);

   /* Do not read more characters than requested. */
   while (rc < siz)
   {
      /* Check if the delimiter fits in the current stream buffer. */
      if (sp->strm_ipos >= sp->strm_ilen - delen)
      {
         int got;
         /* Not enough data to fit the delimiter, re-fill the buffer. */
         sp->strm_ilen -= sp->strm_ipos;
         memcpy(sp->strm_ibuf, sp->strm_ibuf + sp->strm_ipos, sp->strm_ilen);
         got = _recv(sp->strm_csock, sp->strm_ibuf + sp->strm_ilen, sizeof(sp->strm_ibuf) - sp->strm_ilen, 0);
         if (got <= 0)
         {
            /* Broken connection or timeout. */
            if (got < 0) 
            {
               rc = -1;
            }
            break;
         }
         sp->strm_ilen += got;
         sp->strm_ipos = 0;
      }
      
      for (i = sp->strm_ipos, n = 0; i < sp->strm_ilen && rc + n < siz; i++, n++)
      {
         if (*delim == sp->strm_ibuf[i])
         {
            if (i + delen >= sp->strm_ilen)
            {
               break;
            }
            if (memcmp(&sp->strm_ibuf[i], delim, delen) == 0)
            {
               break;
            }
         }
      }
      
      if (n)
      {
         memcpy(buf, sp->strm_ibuf + sp->strm_ipos, n);
         buf += n;
         rc += n;
         sp->strm_ipos += n;
      }
      else
      {
         break;
      }
   }
   
   return(rc);
} /* StreamReadUntilString */


int s_set_flags (HTTP_STREAM *sp, unsigned int flags)
{
#ifdef HTTP_CHUNKED_TRANSFER
   s_flush(sp);

   sp->strm_flags |= flags;

   return(0);
#else
   (void)sp;
   (void)flags; 
   
   return(-1);
#endif
} /* s_set_flags */


int s_write (const void *buf, size_t size, size_t count, HTTP_STREAM *sp)
{
   HTTP_ASSERT(sp != NULL);
   HTTP_ASSERT(buf != NULL);
   
   return( _out(sp, (const char *)buf, size * count) );
} /* s_write */


int s_puts (const char *str, HTTP_STREAM *sp)
{
   HTTP_ASSERT(sp != NULL);
   HTTP_ASSERT(str != NULL);

   return( _out(sp, str, strlen(str)) );
} /* s_puts */


int s_putchar (HTTP_STREAM *sp, const char c)
{
   HTTP_ASSERT(sp != NULL);

   return( _out(sp, &c, 1) );
} /* s_putchar */


int s_vputs (HTTP_STREAM *sp, ...)
{
   int rc = -1;
   int len;
   char *cp;
   char *buf;
   va_list ap;

   HTTP_ASSERT(sp != NULL);

   va_start(ap, sp);
   for (len = 0; (cp = va_arg(ap, char *)) != NULL; len += strlen(cp));
   va_end(ap);
   
   buf = xmalloc(XM_ID_WEB, len + 1);
   if (buf) 
   {
      va_start(ap, sp);
      for (*buf = '\0'; (cp = va_arg(ap, char *)) != NULL; strcat(buf, cp));
      va_end(ap);
      
      rc = _out(sp, buf, strlen(buf));
      
      xfree(buf);
   }
   return(rc);
} /* s_vputs */


int s_printf (HTTP_STREAM *sp, const char *fmt, ...)
{
   int rc = -1;
   char *buf;
   va_list ap;

   HTTP_ASSERT(sp != NULL);
   HTTP_ASSERT(fmt != NULL);

   buf = xmalloc(XM_ID_WEB, 1024);
   if (buf)
   {
      va_start(ap, fmt);
      rc = vsnprintf(buf, 1023, fmt, ap);
      va_end(ap);
      
      rc = _out(sp, buf, rc);
      
      xfree(buf);
   }
   return(rc);
} /* s_printf */


int s_flush (HTTP_STREAM *sp)
{
   int rc = -1;

   if (sp->strm_olen != 0)
   {
#ifdef HTTP_CHUNKED_TRANSFER
      if (sp->strm_flags & S_FLG_CHUNKED)
      {
         static const char *crlf = "\r\n";
         char cs[11];
      
         itoa(sp->strm_olen, cs, 16);
         strcat(cs, crlf);
         rc  = _send(sp->strm_csock, cs, strlen(cs));
         rc |= _send(sp->strm_csock, sp->strm_obuf, sp->strm_olen);
         rc |= _send(sp->strm_csock, crlf, 2);
      }   
      else
#endif      
      {      
         rc = _send(sp->strm_csock, sp->strm_obuf, sp->strm_olen);
      }         
      
      sp->strm_olen = 0;
   }
   
   return(rc);
} /* s_flush */


void s_end (HTTP_STREAM *sp)
{
   s_flush(sp);

#ifdef HTTP_CHUNKED_TRANSFER
   if (sp->strm_flags & S_FLG_CHUNKED)
   {
      _send(sp->strm_csock, "0\r\n\r\n", 5);
   }
   
   sp->strm_flags &= ~S_FLG_CHUNKED;
#else
   (void)sp;
   (void)flags; 
#endif
} /* s_end */

/*** EOF ***/

/*
 * Copyright (C) 2012 by egnite GmbH
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * For additional information see http://www.ethernut.de/
 */

/*!
 * $Id$
 */

#include "tcts.h"
#include "xmem.h"

#include <cfg/http.h>
#if !defined(HTTPD_EXCLUDE_DATE)
#include <pro/rfctime.h>
#endif
#include <pro/uhttp/uhttpd.h>

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <memdebug.h>

typedef struct _HTTP_RESPONSE_STATUS HTTP_RESPONSE_STATUS;

struct _HTTP_RESPONSE_STATUS {
    int rs_code;
    const char *rs_text;
};

static const HTTP_RESPONSE_STATUS response_list[] = {
    { 100, "Continue" },
    { 101, "Switching Protocols" },
    { 102, "Processing" },
    { 103, "Checkpoint" },
    { 200, "OK" },
    { 201, "Created" },
    { 202, "Accepted" },
#if HTTP_VERSION >= 0x11
    { 203, "Non-Authoritative Information" },
#endif
    { 204, "No Content" },
    { 205, "Reset Content" },
    { 206, "Partial Content" },
    { 207, "Multi-Status" },
    { 208, "Already Reported" },
    { 226, "IM Used" },
    { 300, "Multiple Choices" },
    { 301, "Moved Permanently" },
    { 302, "Moved Temporarily" },
#if HTTP_VERSION >= 0x11
    { 303, "See Other" },
#endif
    { 304, "Not Modified" },
#if HTTP_VERSION >= 0x11
    { 305, "Use Proxy" },
#endif
    { 306, "Switch Proxy" },
#if HTTP_VERSION >= 0x11
    { 307, "Temporary Redirect" },
#endif
    { 308, "Resume Incomplete" },
    { 400, "Bad Request" },
    { 401, "Unauthorized" },
    { 402, "Payment Required" },
    { 403, "Forbidden" },
    { 404, "Not Found" },
    { 405, "Not Allowed" },
    { 406, "Not Acceptable" },
    { 407, "Proxy Authentication Required" },
    { 408, "Request Time-out" },
    { 409, "Conflict" },
    { 410, "Gone" },
    { 411, "Length Required" },
    { 412, "Precondition Failed" },
    { 413, "Request Entity Too Large" },
    { 414, "Request-URI Too Large" },
    { 415, "Unsupported Media Type" },
    { 416, "Requested Range Not Satisfiable" },
    { 417, "Expectation Failed" },
    { 418, "I'm a teapot" },
    { 422, "Unprocessable Entity" },
    { 423, "Locked" },
    { 424, "Failed Dependency" },
    { 425, "Unordered Collection" },
    { 426, "Upgrade Required" },
    { 428, "Precondition Required" },
    { 429, "Too Many Requests" },
    { 431, "Request Header Fields Too Large" },
    { 444, "No Response" },
    { 449, "Retry With" },
    { 499, "Client Closed Request" },
    { 500, "Internal Server Error" },
    { 501, "Method Not Implemented" },
    { 502, "Bad Gateway" },
    { 503, "Service Temporarily Unavailable" },
    { 504, "Gateway Time-out" },
    { 505, "HTTP Version Not Supported" },
    { 506, "Variant Also Negotiates" },
    { 507, "Insufficient Storage" },
    { 508, "Loop Detected" },
    { 509, "Bandwidth Limit Exceeded" },
    { 510, "Not Extended" },
    { 511, "Network Authentication Required" },
    { 598, "Network read timeout error" },
    { 599, "Network connect timeout error" }
};

#define HTTP_NUM_RESPONSES   (sizeof(response_list) / sizeof(HTTP_RESPONSE_STATUS))

const char *HttpResponseText(int code)
{
    const char *rp = NULL;
    int i;

    for (i = 0; i < (int)HTTP_NUM_RESPONSES; i++) {
        if (code == response_list[i].rs_code) {
            break;
        }
    }
    if (i < (int)HTTP_NUM_RESPONSES && code == response_list[i].rs_code) {
        rp = response_list[i].rs_text;
    }
    if (rp == NULL) {
        static const char *err_txt = "Error";
        static const char *ok_txt = "OK";

        if (code < 400) {
            rp = ok_txt;
        } else {
            rp = err_txt;
        }
    }
    return rp;
}

void HttpSendStreamHeaderTop(HTTP_STREAM *stream, int status)
{
    static const char fmt_P[] = "HTTP/%d.%d %d %s\r\nServer: uHTTP\r\n";

    s_printf(stream, fmt_P, HTTP_MAJOR_VERSION, HTTP_MINOR_VERSION, status, HttpResponseText(status));

#if !defined(HTTPD_EXCLUDE_DATE)
    {
        time_t now = OS_UnixtimeGet();
        s_vputs(stream, ct_Date, ": ", Rfc1123TimeString(gmtime(&now)), " GMT\r\n", NULL);
    }
#endif
}

void HttpSendHeaderTop(HTTPD_SESSION *hs, int status)
{
    HttpSendStreamHeaderTop(hs->s_stream, status);
}

void HttpSendStreamHeaderSse(HTTP_STREAM *stream)
{
    static const char fmt[] = "HTTP/%d.%d %d %s\r\nServer: uHTTP\r\n";

    s_printf(stream, fmt, HTTP_MAJOR_VERSION, HTTP_MINOR_VERSION, 200, HttpResponseText(200));
    s_puts("Content-Type: text/event-stream\r\n", stream);
    s_puts("Expires: 1 JAN 2018 00:00:00 GMT\r\n", stream);
    s_puts("Last-Modified: 1 JAN 2018 00:00:00 GMT\r\n", stream);
    s_puts("Cache-Control: no-cache\r\n", stream);
    s_puts("\r\n", stream);
   
    s_flush(stream);
}

void HttpSendHeaderSse(HTTPD_SESSION *hs)
{
    HttpSendStreamHeaderSse(hs->s_stream);
}


#if !defined(HTTPD_EXCLUDE_DATE)

void HttpSendStreamHeaderDate(HTTP_STREAM *stream, time_t mtime)
{
    if (mtime) {
        s_vputs(stream, ct_Last_Modified, ": ", Rfc1123TimeString(gmtime(&mtime)), " GMT\r\n", NULL);
    }
    }

void HttpSendHeaderDate(HTTPD_SESSION *hs, time_t mtime)
{
    HttpSendStreamHeaderDate(hs->s_stream, mtime);
}

#endif

void HttpSendStreamHeaderBottom(HTTP_STREAM *stream, const char *type, const char *subtype, int conn, long bytes)
{
    (void)conn;
    if (type && subtype) {
        s_vputs(stream, ct_Content_Type, ": ", type, "/", subtype, "\r\n", NULL);
    } else {
        s_puts("Content-Type: Unknown\r\n", stream);
    }
    
    if (bytes >= 0) {
        s_printf(stream, "%s: %ld\r\n", ct_Content_Length, bytes);
    }
    
#ifdef HTTP_CHUNKED_TRANSFER
    else {
        s_puts("Transfer-Encoding: chunked\r\n", stream);
    }
#endif

#if 1
#if HTTP_KEEP_ALIVE_REQ
    if (conn == HTTP_CONN_KEEP_ALIVE) {
        s_puts("Connection: keep-alive\r\n", stream);
    } else {
        s_puts("Connection: close\r\n", stream);
    }
#else    
    s_puts("Connection: close\r\n", stream);
#endif
#endif

    s_puts("\r\n", stream);
}

void HttpSendHeaderBottom(HTTPD_SESSION *hs, const char *type, const char *subtype, long bytes, int isgzip)
{
#if HTTP_KEEP_ALIVE_REQ && !defined(HTTP_CHUNKED_TRANSFER)
    if (bytes < 0) {
        hs->s_req.req_connection = HTTP_CONN_CLOSE;
    }
#endif

    if (isgzip != 0) {
        s_puts("Content-Encoding: gzip\r\n", hs->s_stream);
    }

    HttpSendStreamHeaderBottom(hs->s_stream, type, subtype, hs->s_req.req_connection, bytes);
}

void HttpSendStreamError(HTTP_STREAM *stream, int status, const char *realm)
{
    static const char _body[]    = "<HTML><HEAD><TITLE>%d %s</TITLE></HEAD><BODY>%d %s</BODY></HTML>\r\n";
    static const char _body404[] = "<HTML><HEAD><TITLE>%d %s</TITLE></HEAD><BODY>%d %s<br/><br/><a href=\"/\">Home</a></BODY></HTML>\r\n";
    static const char _body503[] = "<HTML><HEAD><TITLE>%d %s</TITLE></HEAD><BODY><h1>Service Unavailable</h1><hr>%d %s</BODY></HTML>\r\n";
    const char *text = HttpResponseText(status);
    const char *body = _body;
    
    if (404 == status) {
        body = _body404;
    }

    if (503 == status) {
        body = _body503;
    }

    HttpSendStreamHeaderTop(stream, status);
    if (realm) {
        static const char auth_fmt_P[] = "WWW-Authenticate: Basic realm=\"%s\"\r\n";
        s_printf(stream, auth_fmt_P, realm);
    }
    
    HttpSendStreamHeaderBottom(stream, "text", "html", HTTP_CONN_CLOSE, strlen(body) - 1 + 2 * (1 + strlen(text) - 2));
    s_printf(stream, body, status, text, status, text);
    s_flush(stream);
}

void HttpSendError(HTTPD_SESSION *hs, int status)
{
    char *realm = NULL;

#if HTTP_KEEP_ALIVE_REQ
    if (status >= 400) {
        hs->s_req.req_connection = HTTP_CONN_CLOSE;
    }
#endif
    if (status == 401) {
        realm = hs->s_req.req_realm;
    }
    HttpSendStreamError(hs->s_stream, status, realm);
}

/*!
 * \brief Transmit a redirection page.
 */
int HttpSendRedirection(HTTPD_SESSION *hs, int code, ...)
{
    va_list ap;
    int len;
    char *cp;
    char *loc;

    va_start(ap, code);
    for (len = 0; (cp = va_arg(ap, char *)) != NULL; len += strlen(cp));
    va_end(ap);
    loc = xmalloc(XM_ID_WEB, len + 1);
    if (loc) {
        static const char body[] =
                "<html><body><a href=\"%s\">Continue</a></body></html>\r\n";
        HTTP_STREAM *sp = hs->s_stream;

        HttpSendHeaderTop(hs, code);

        va_start(ap, code);
        for (*loc = '\0'; (cp = va_arg(ap, char *)) != NULL; strcat(loc, cp));
        va_end(ap);

        /* Close the connection after the redirect */
        hs->s_req.req_connection = HTTP_CONN_CLOSE;

        s_vputs(sp, ct_Location, ": ", loc, "\r\n", NULL);
        HttpSendHeaderBottom(hs, "text", "html", sizeof(body) - 1 + strlen(loc) - 2, 0);

        s_printf(sp, body, loc);
        s_flush(sp);
        xfree(loc);
    }
    return 0;
}

int HttpSendRedirectionCookie(HTTPD_SESSION *hs, char *cookie, int code, ...)
{
    va_list ap;
    int len;
    char *cp;
    char *loc;

    va_start(ap, code);
    for (len = 0; (cp = va_arg(ap, char *)) != NULL; len += strlen(cp));
    va_end(ap);
    loc = xmalloc(XM_ID_WEB, len + 1);
    if (loc) {
        static const char body[] =
                "<html><body><a href=\"%s\">Continue</a></body></html>\r\n";
        HTTP_STREAM *sp = hs->s_stream;

        HttpSendHeaderTop(hs, code);

        va_start(ap, code);
        for (*loc = '\0'; (cp = va_arg(ap, char *)) != NULL; strcat(loc, cp));
        va_end(ap);

        /* Close the connection after the redirect */
        hs->s_req.req_connection = HTTP_CONN_CLOSE;

        s_vputs(sp, ct_Location, ": ", loc, "\r\n", NULL);
        
        if (cookie != NULL)
        {
            s_printf(sp, "Set-Cookie: %s\r\n", cookie); 
        }
        
        HttpSendHeaderBottom(hs, "text", "html", sizeof(body) - 1 + strlen(loc) - 2, 0);

        s_printf(sp, body, loc);
        s_flush(sp);
        xfree(loc);
    }
    return 0;
}


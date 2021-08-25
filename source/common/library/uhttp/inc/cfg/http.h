#ifndef _INCLUDE_CFG_HTTP_H_
#define _INCLUDE_CFG_HTTP_H_

/*
 * Do not edit! Automatically generated on Sat Jun 29 12:50:40 2013
 */

#define HTTP_PLATFORM_STREAMS

#define HTTP_CHUNKED_TRANSFER

#define HTTPD_EXCLUDE_DATE

//#define HTTP_SUPPORT_VFS

#ifndef HTTP_MAJOR_VERSION
#define HTTP_MAJOR_VERSION    1
#endif

#ifndef HTTP_MINOR_VERSION
#define HTTP_MINOR_VERSION    1
#endif

#ifndef HTTP_DEFAULT_ROOT
#define HTTP_DEFAULT_ROOT     "UROM:"
#endif

#ifndef HTTP_MAX_REQUEST_SIZE
#define HTTP_MAX_REQUEST_SIZE 256
#endif

#ifndef HTTP_FILE_CHUNK_SIZE
#define HTTP_FILE_CHUNK_SIZE  512
#endif


#ifdef HTTP_KEEP_ALIVE_REQ
#undef HTTP_KEEP_ALIVE_REQ
#endif
#define HTTP_KEEP_ALIVE_REQ   LWIP_TCP_KEEPALIVE

#endif
/*** EOF ***/


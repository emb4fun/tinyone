/**************************************************************************
*  Copyright (c) 2021-2024 by Michael Fischer (www.emb4fun.de).
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
**************************************************************************/
#if !defined(__ELCA_RPC_H__)
#define __ELCA_RPC_H__

/**************************************************************************
*  Includes
**************************************************************************/

#ifdef _MSC_VER
#include <windows.h>
#include "stdint.h"
#else
#include <stdint.h>
#endif

#ifdef _WINDOWS
#pragma pack(1)
#define PACKED(_a)   _a
#endif

#if !defined(PACKED)
#define PACKED(_a)    __attribute__((__packed__)) _a
#endif   

/**************************************************************************
*  Global Definitions
**************************************************************************/

/*
 * ELCA server port
 */
#define ELCA_SERVER_PORT          54322

/*
 * ELCA error codes
 */
#define ELCA_RPC_OK               0
#define ELCA_RPC_ERROR            -1
#define ELCA_RPC_ERR_LOCKED       -2 
#define ELCA_RPC_ERR_LEN          -3
#define ELCA_RPC_ERR_FUNC         -4
#define ELCA_RPC_ERR_ROOT_SIZE    -5
#define ELCA_RPC_ERR_INTER_SIZE   -6
#define ELCA_RPC_ERR_NO_SERVER    -100  

/*
 * ELCA header infos
 */
#define ELCA_RPC_HEADER_MAGIC_1   0x41434C45  // "ELCARPC"
#define ELCA_RPC_HEADER_MAGIC_2   0x00435052
#define ELCA_RPC_SIZEVER          ((((uint32_t)sizeof(elca_header_t)) << 16) | ELCA_RPC_VERSION)
#define ELCA_RPC_HEADER_SIZE      sizeof(elca_header_t)

#define ELCA_RPC_VERSION          1

#define ELCA_RPC_RAND_SIZE        16
#define ELCA_RPC_CSR_SIZE         1024
#define ELCA_RPC_CRT_SIZE         1024


/*
 * RPC data structure 
 */

/*************************************************************************/

/*
 * CRT, create CRT by the given CSR
 */
typedef struct
{
   uint8_t  Random[ELCA_RPC_RAND_SIZE];
   uint16_t CSRLen;
   uint8_t  CSR[ELCA_RPC_CSR_SIZE];
} PACKED(elca_call_get_crt_t);
#define ELCA_CALL_GET_CRT_SIZE      sizeof(elca_call_get_crt_t)

typedef struct
{
   uint8_t  Random[ELCA_RPC_RAND_SIZE];
   uint16_t CRTLen;
   uint8_t  CRT[ELCA_RPC_CRT_SIZE];
} PACKED(elca_reply_get_crt_t);
#define ELCA_REPLY_GET_CRT_SIZE     sizeof(elca_reply_get_crt_t)

/*-----------------------------------------------------------------------*/

/*
 * GetRoot
 */

#define ELCA_CALL_GET_ROOT_SIZE     0

typedef struct
{
   uint8_t  CRT[ELCA_RPC_CRT_SIZE];
} PACKED(elca_reply_get_root_t);
#define ELCA_REPLY_GET_ROOT_SIZE    sizeof(elca_reply_get_root_t)

/*-----------------------------------------------------------------------*/

/*
 * GetInter
 */

#define ELCA_CALL_GET_INTER_SIZE    0

typedef struct
{
   uint8_t  CRT[ELCA_RPC_CRT_SIZE];
} PACKED(elca_reply_get_inter_t);
#define ELCA_REPLY_GET_INTER_SIZE   sizeof(elca_reply_get_inter_t)


/*************************************************************************/

typedef union
{
   elca_call_get_crt_t     cGetCRT;      
   elca_reply_get_crt_t    rGetCRT;

   elca_reply_get_root_t   rGetRoot;
   
   elca_reply_get_inter_t  rGetInter;
   
} PACKED(elca_data_t);

/*************************************************************************/

/*
 * RPC functions
 */

typedef enum
{
   ELCA_MSG_GET_CRT = 0,
   ELCA_MSG_GET_ROOT,
   ELCA_MSG_GET_INTER,
   
   /**************************/
   ELCA_MSG_END = 0xFFFFFFFF
} elca_msg_func;

/*************************************************************************/

typedef struct _elca_header_
{
   uint32_t      Magic1; 
   uint32_t      Magic2; 
   uint32_t      SizeVer;
   elca_msg_func Func;
   uint32_t      XID;
   uint32_t      Len;
   int32_t       Result;
} PACKED(elca_header_t);

typedef struct _elca_msg_
{
   elca_header_t   Header;
   elca_data_t     Data;
} PACKED(elca_msg_t); 


#ifdef _WINDOWS
#pragma pack()
#endif

/**************************************************************************
*  Macro Definitions
**************************************************************************/

/**************************************************************************
*  Functions Definitions
**************************************************************************/

#endif /* !__ELCA_RPC_H__ */

/*** EOF ***/

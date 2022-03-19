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
*  04.10.2015  mifi  First Version
*  08.12.2015  mifi  Expand xbin header size from 64 to 256 bytes.
*  07.01.2016  mifi  Change header version from 2 to 3.
**************************************************************************/
#if !defined(__XBIN_H__)
#define __XBIN_H__

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
typedef BYTE   uint8_t;   
#endif

#define XBIN_HEADER_MAGIC_1   0x6E696278
#define XBIN_HEADER_MAGIC_2   0x6E696278
#define XBIN_HEADER_SIZEVER   0x01000003

#define XBIN_SIZE_OF_CRC32    sizeof(uint32_t)   

#define XBIN_DATA_NAME_SIZE   20

typedef struct _xbin_header_
{
   uint32_t dMagic1;
   uint32_t dMagic2;
   uint32_t dSizeVersion;
   uint32_t dProductID;
   uint32_t dDataTotalSize;
   uint32_t dDataSectorSize;
   uint32_t dDataCreationTime;
   uint32_t dDataCRC32;
   uint8_t   DataName[XBIN_DATA_NAME_SIZE];
   uint32_t dDataVersion;
   uint32_t dReserve[49];
   uint32_t dHeaderCRC32;
} XBIN_HEADER;

/**************************************************************************
*  Macro Definitions
**************************************************************************/

/**************************************************************************
*  Funtions Definitions
**************************************************************************/

#endif /* !__XBIN_H__ */

/*** EOF ***/

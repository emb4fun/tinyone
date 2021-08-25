/**************************************************************************
*  Copyright (c) 2017 by Michael Fischer (www.emb4fun.de).
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
*  08.04.2017  mifi  First Version.
**************************************************************************/
#if !defined(__FSAPI_H__)
#define __FSAPI_H__

/**************************************************************************
*  Includes
**************************************************************************/
#include <stdint.h>
#include <time.h>
#include "fcntl.h"

/**************************************************************************
*  Global Definitions
**************************************************************************/

struct _stat 
{
   uint32_t st_size;
   time_t   st_mtime;
};

typedef struct _fs_fs_ FS_FS;

struct _fs_fs_ 
{
   char DriveName[9];
   
   int  (*Open)  (const char *name, int mode);
   int  (*Close) (int fd);
   int  (*Read)  (int fd, void *buffer, uint32_t count);
   int  (*Write) (int fd, const void *buffer, uint32_t count);
   int  (*Seek)  (int fd, long offset, int origin);
   int  (*Stat)  (int fd, struct _stat *pStat);
   long (*Filelength)(int fd);
   int  (*Ioctl) (int fd, int cmd, void *data);
   
   FS_FS  *pNext;
};

/**************************************************************************
*  Macro Definitions
**************************************************************************/

/**************************************************************************
*  Functions Definitions
**************************************************************************/

void FSInit (void);
int  FSRegister (FS_FS *pFS);

int  _open (const char *name, int mode);
int  _close (int fd);
int  _read (int fd, void *buffer, size_t count);
int  _write (int fd, const void *buffer, size_t count);
int  _seek (int fd, long offset, int origin);
int  _fstat (int fh, struct _stat *s);
long _filelength (int fd);
int  _ioctl (int fd, int cmd, void *buffer);

#endif /* !__FSAPI_H__ */

/*** EOF ***/

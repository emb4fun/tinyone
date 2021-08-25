/**************************************************************************
*  Copyright (c) 2016 by Michael Fischer (www.emb4fun.de).
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
*  30.10.2016  mifi  First Version, ROMFS is based on XFILE.
**************************************************************************/
#define __FS_ROMFS_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tal.h"
#include "fsapi.h"
#include "xfile.h"
#include "adler32.h"

extern const char _romfs_data[];

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

/* File Control Block */
typedef struct _FCB_ 
{
   uint8_t  *pData;
   uint32_t   Size;
   uint32_t   Pos;
} FCB;

/*=======================================================================*/
/*  Definition of all global Data                                        */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

static time_t             FileTime    = 0;
static XFILE_FAT_ENTRY  *pDir         = NULL;
static uint8_t          *pImageBuffer = NULL;

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  xfile_open                                                           */
/*                                                                       */
/*  Open a file and return the "file handle".                            */
/*                                                                       */
/*  In    : fs, name, mode                                               */
/*  Out   : none                                                         */
/*  Return: file handle                                                  */
/*************************************************************************/
static int xfile_open (const char *name, int mode)
{
   int     FileHandle = -1;
   FCB   *pFCB = NULL;
   char  *pName;
   int     i;
   
   (void)mode;
   
   if ((pImageBuffer != NULL) && (name != NULL))
   {
      if (*name == '/') 
      {
         name++;
      }
   
      /*
       * In general a file will be opened who is available too.
       * Therefore the descriptors can be allocated before.
       */
      pFCB = xmalloc(XM_ID_FS, sizeof(FCB));
      
      /* Check for valid descriptors */
      if (pFCB != NULL)
      {
         i = 0;
         while(pDir[i].dFilename != 0)
         {
            pName = (char*)(pImageBuffer + pDir[i].dFilename);
            if (0 == strcmp(pName, name))
            {
               pFCB->pData = (uint8_t*)(pImageBuffer + pDir[i].dData);    
               pFCB->Size  = pDir[i].dFilelength;
               pFCB->Pos   = 0;
            
               /* File is available, "create" descriptors */
               FileHandle = (int)pFCB;
               break;
            }
         
            i++;
         }

         if (-1 == FileHandle)
         {
            /* File not found */
            xfree(pFCB);
         } 
      }
   }
      
   return(FileHandle);  /*lint !e593*/
} /* xfile_open */

/*************************************************************************/
/*  xfile_close                                                          */
/*                                                                       */
/*  Close the file by the given "file handle".                           */
/*                                                                       */
/*  In    : fd                                                           */
/*  Out   : none                                                         */
/*  Return: Result                                                       */
/*************************************************************************/
static int xfile_close (int fd)
{
   FCB *pFCB = (FCB*)fd;

   if (pFCB != NULL)
   {
      xfree(pFCB);
   }      
   
   return(0);
} /* xfile_close */

/*************************************************************************/
/*  xfile_read                                                           */
/*                                                                       */
/*  Read data from the file.                                             */
/*                                                                       */
/*  In    : fd, buffer, size                                             */
/*  Out   : none                                                         */
/*  Return: Number of bytes read                                         */
/*************************************************************************/
static int xfile_read (int fd, void *buffer, size_t size)
{
   FCB *pFCB = (FCB*)fd;
   
   if ((pImageBuffer != NULL) && (fd != 0) && (buffer != NULL) && (size > 0))
   {
      if ((unsigned int) size > pFCB->Size - pFCB->Pos)
      {
         size = (size_t)(pFCB->Size - pFCB->Pos);
      }
         
      if (size) 
      {
         memcpy(buffer, (pFCB->pData + pFCB->Pos), size);
         pFCB->Pos += size;
      }
   }
   else
   {
      size = (size_t)-1;
   }
   
   return((int)size);
} /* xfile_read */

/*************************************************************************/
/*  xfile_seek                                                           */
/*                                                                       */
/*  Move the file pointer to a new position.                             */
/*                                                                       */
/*  In    : fd, offset, origin                                           */
/*  Out   : none                                                         */
/*  Return: New position from the start of file.                         */
/*************************************************************************/
static int xfile_seek (int fd, long offset, int origin)
{
   long   pos = -1;
   FCB  *pFCB = (FCB*)fd;

   if ((pImageBuffer != NULL) && (fd != 0))
   {
      switch (origin) 
      {
         case SEEK_CUR:
         {
            pos = (long)pFCB->Pos + offset;
            break;
         }   
           
         case SEEK_END:
         {
            pos = (long)pFCB->Size + offset;
            break;
         }   
         
         default:
         {
            pos = (long)pFCB->Pos;
            /* Do nothing */
            break;
         }
      }
      
      /* Check for valid range */
      if ((pos >= 0) && (pos < (long)pFCB->Size))
      {
         pFCB->Pos = (uint32_t)pos;
      }
      else
      {
         pos = -1;
      }
   }   

   return(pos);
} /* xfile_seek */

/*************************************************************************/
/*  xfile_filelength                                                     */
/*                                                                       */
/*  Return the length of a file.                                         */
/*                                                                       */
/*  In    : fd                                                           */
/*  Out   : none                                                         */
/*  Return: File size                                                    */
/*************************************************************************/
static long xfile_filelength (int fd)
{
   long   size = 0;
   FCB  *pFCB = (FCB*)fd;

   if ((pImageBuffer != NULL) && (fd != 0))
   {
      size = (long)pFCB->Size;
   }
   
   return(size);
} /* xfile_filelength */

/*************************************************************************/
/*  xfile_fstat                                                          */
/*                                                                       */
/*  Get information about the file.                                      */
/*                                                                       */
/*  In    : fd, stat                                                     */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = Error                                          */
/*************************************************************************/
static int xfile_fstat (int fd, struct _stat *stat)
{
   int    rc = -1;
   FCB  *pFCB = (FCB*)fd;
   
   if ((pImageBuffer != NULL) && (fd != 0) && (stat != NULL))
   {
      stat->st_size  = pFCB->Size;
      stat->st_mtime = FileTime; 
      
      rc = 0;
   }      
   
   return(rc);
} /* xfile_fstat */

/*************************************************************************/
/*  xfile_ioctl                                                          */
/*                                                                       */
/*  In    : fd, cmd, data                                                */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = Error                                          */
/*************************************************************************/
static int xfile_ioctl (int fd, int cmd, void *data)
{
   int rc = -1;
   
   (void)fd;
   (void)cmd;
   (void)data;
   
   return(rc);
} /* _ioctl */


static FS_FS FileSystem = 
{
   "ROMFS",
   xfile_open,
   xfile_close,
   xfile_read,
   NULL,          /* Write */
   xfile_seek,
   xfile_fstat,
   xfile_filelength,
   xfile_ioctl,
   
   NULL           /* pNext */
};

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  romfs_Init                                                           */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void romfs_Init (void)
{
   XFILE_HEADER *pHeader;
   uint32_t       CRC32;

   /* Register the file system. */
   FSRegister(&FileSystem);
   
   /*
    * Mount filesystem
    */
   pHeader      = (XFILE_HEADER*)_romfs_data;  /*lint !e826*/
   pDir         = NULL;
   pImageBuffer = NULL;
   
   if ( (XFILE_HEADER_MAGIC_1 == pHeader->dMagic1)      &&
        (XFILE_HEADER_MAGIC_2 == pHeader->dMagic2)      &&
        (XFILE_HEADER_SIZEVER == pHeader->dSizeVersion) )
   {
      /* Check CRC32 of the header */
      CRC32 = adler32(ADLER_START_VALUE, (uint8_t*)pHeader, sizeof(XFILE_HEADER) - XFILE_SIZE_OF_CRC32);
      if (CRC32 == pHeader->dHeaderCRC32)
      {
         pDir         = (XFILE_FAT_ENTRY*)(_romfs_data + sizeof(XFILE_HEADER)); /*lint !e826*/
         FileTime     = (time_t)pHeader->dDataCreationTime;
         pImageBuffer = (uint8_t*)pDir;
      
         /* Check xfile data CRC32 */
         CRC32 = adler32(ADLER_START_VALUE, (uint8_t*)pDir, pHeader->dDataTotalSize);
         if (CRC32 != pHeader->dDataCRC32)
         {
            /* Error */
            pImageBuffer = NULL;
         }
      }
   }       
   
} /* romfs_Init */

/*** EOF ***/



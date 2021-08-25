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
*  04.01.2015  mifi  First Version.
*  13.03.2016  mifi  Renamed to fs_fatfs.c
**************************************************************************/
#define __FS_FATFS_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tal.h"
#include "fsapi.h"
#include "fs_fatfs.h"
#include "ff.h"
#include "diskio.h"

#include "ff.h"
#include "pro\uhttp\modules\mod_redir.h"

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

#define LOGICAL_DRIVE         "0:/"

#define SD0FS_TASL_DELAY_MS   500
#define CARD_INSERT_DELAY_MS  100

/*=======================================================================*/
/*  Definition of all global Data                                        */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

/* 
 * Some TASK variables like stack and task control block.
 */
static OS_STACK (SD0FSStack, TASK_SD0FS_STK_SIZE);
static OS_TCB TCBSD0FS;

/*
 * File system object
 */
static FATFS FSObject;

/*
 * Mount / UnMount callbacks
 */
static fatfs_mount_callback_t MountCallback   = NULL;
static fatfs_mount_callback_t UnMountCallback = NULL;

static int nInitDone = 0;

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  fatfs_open                                                           */
/*                                                                       */
/*  Open a file and return the "file handle".                            */
/*                                                                       */
/*  In    : fs, name, mode                                               */
/*  Out   : none                                                         */
/*  Return: file handle                                                  */
/*************************************************************************/
static int fatfs_open (const char *name, int mode)
{
   FRESULT     Result;
   FIL        *FileDesc;
   int         FileHandle = -1;
   FILINFO     Info;
   struct tm   TM;
   WORD        Value;
   BYTE        ffmode;

   ffmode  = 0;
   ffmode |= (mode & _O_RDONLY)        ? FA_READ  : 0;
   ffmode |= (mode & _O_WRONLY)        ? FA_WRITE : 0;
   ffmode |= (mode & _O_APPEND)        ? FA_OPEN_APPEND : 0;
   ffmode |= (mode & _O_CREATE_ALWAYS) ? FA_CREATE_ALWAYS : 0;
    
   /*
    * In general a file will be opened who is available too.
    * Therefore the descriptors can be allocated before.
    */
   FileDesc = xmalloc(XM_ID_FS, sizeof(FIL));
   
   /* Check for valid descriptors */
   if (FileDesc != NULL)
   {
      Result = f_open(FileDesc, name, ffmode);
      if(FR_OK == Result)
      {
         FileHandle = (int)FileDesc;

         /* Get file info */
         memset(&Info, 0x00, sizeof(Info));
         Result = f_stat(name, &Info);
         if(FR_OK == Result)
         {
            memset(&TM, 0x00, sizeof(TM));
            
            Value      = Info.fdate;
            TM.tm_mday = Value & 0x1F;
            Value      = Value >> 5;
            
            TM.tm_mon  = (Value & 0x0F) - 1;
            Value      = Value >> 4;
            
            TM.tm_year = (1980 + Value) - 1900;
            
            
            Value      = Info.ftime;
            TM.tm_sec  = (Value & 0x1F) * 2; 
            Value      = Value >> 5;

            TM.tm_min  = Value & 0x3F;
            Value      = Value >> 6;
            
            TM.tm_hour = Value; 
            
            FileDesc->time = (DWORD)mktime(&TM);
         }
         else
         {
            FileDesc->time = 0;
         }
      }
      else
      {
         /* Error, file could not be found */
         xfree(FileDesc);
      }
   }
      
   return(FileHandle);
} /* fatfs_open */

/*************************************************************************/
/*  fatfs_close                                                          */
/*                                                                       */
/*  Close the file by the given "file handle".                           */
/*                                                                       */
/*  In    : fd                                                           */
/*  Out   : none                                                         */
/*  Return: Result                                                       */
/*************************************************************************/
static int fatfs_close (int fd)
{
   FIL *FileDesc = (FIL*)fd;

   if (FileDesc != NULL)
   {
      f_close(FileDesc);
      xfree(FileDesc);
   }
   
   return(0);
} /* fatfs_close */

/*************************************************************************/
/*  fatfs_read                                                           */
/*                                                                       */
/*  Read data from the file.                                             */
/*                                                                       */
/*  In    : fd, buffer, size                                             */
/*  Out   : none                                                         */
/*  Return: Number of bytes read                                         */
/*************************************************************************/
static int fatfs_read (int fd, void *buffer, size_t size)
{
   FRESULT  Result;
   FIL     *FileDesc = (FIL*)fd;
   uint32_t BytesRead;

   if (FileDesc != NULL)
   {
      Result = f_read(FileDesc, buffer, size, (UINT*)&BytesRead);
      if (FR_OK == Result)
      {
         size = (size_t)BytesRead;
      }
      else
      {
         size = (size_t)-1;
      }
   }
   else
   {
      size = (size_t)-1;
   }
   
   return((int)size);
} /* fatfs_read */

/*************************************************************************/
/*  fatfs_write                                                          */
/*                                                                       */
/*  Write data to the file.                                              */
/*                                                                       */
/*  In    : fd, buffer, size                                             */
/*  Out   : none                                                         */
/*  Return: Number of bytes written                                      */
/*************************************************************************/
static int fatfs_write (int fd, const void *buffer, size_t size)
{
   FRESULT  Result;
   FIL     *FileDesc = (FIL*)fd;
   uint32_t BytesWritten;

   if (FileDesc != NULL)
   {
      Result = f_write(FileDesc, buffer, size, (UINT*)&BytesWritten);
      if (FR_OK == Result)
      {
         size = (size_t)BytesWritten;
      }
      else
      {
         size = (size_t)-1;
      }
   }
   else
   {
      size = (size_t)-1;
   }
   
   return((int)size);
} /* fatfs_write */

/*************************************************************************/
/*  fatfs_seek                                                           */
/*                                                                       */
/*  Move the file pointer to a new position.                             */
/*                                                                       */
/*  In    : fd, offset, origin                                           */
/*  Out   : none                                                         */
/*  Return: New position from the start of file.                         */
/*************************************************************************/
static int fatfs_seek (int fd, long offset, int origin)
{
   long  pos = -1;
   FIL  *FileDesc = (FIL*)fd;

   if (FileDesc != NULL)
   {
      if (SEEK_SET == origin)
      {
         pos  = 0;
         pos += offset;
         f_lseek(FileDesc, (DWORD)pos); 
      }

      if (SEEK_CUR == origin)
      {
         pos  = (long)f_tell(FileDesc);
         pos += offset;
         f_lseek(FileDesc, (DWORD)pos); 
      }         
   }
   
   return(pos);
} /* fatfs_seek */

/*************************************************************************/
/*  fatfs_filelength                                                     */
/*                                                                       */
/*  Return the length of a file.                                         */
/*                                                                       */
/*  In    : fd                                                           */
/*  Out   : none                                                         */
/*  Return: File size                                                    */
/*************************************************************************/
static long fatfs_filelength (int fd)
{
   long  size = 0;
   FIL  *FileDesc = (FIL*)fd;

   if (FileDesc != NULL)
   {
      size = (long)f_size(FileDesc);
   }
   
   return(size);
} /* fatfs_filelength */

/*************************************************************************/
/*  fatfs_fstat                                                          */
/*                                                                       */
/*  Get information about the file.                                      */
/*                                                                       */
/*  In    : fd, stat                                                     */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = Error                                          */
/*************************************************************************/
static int fatfs_fstat (int fd, struct _stat *stat)
{
   int   Res = -1;
   FIL  *FileDesc = (FIL*)fd;

   if (FileDesc != NULL)
   {
      stat->st_size  = (uint32_t)f_size(FileDesc);
      stat->st_mtime = (time_t)FileDesc->time;
      
      if (stat->st_mtime != 0)
      {
         Res = 0;
      }
   }
   
   return(Res);
} /* fatfs_fstat */


static FS_FS FileSystem = 
{
   "SD0",
   fatfs_open,
   fatfs_close,
   fatfs_read,
   fatfs_write,
   fatfs_seek,
   fatfs_fstat,
   fatfs_filelength,
   NULL,          /* Ioctl */
   
   NULL           /* pNext */
};


/*************************************************************************/
/*  MemoryCardRemoved                                                    */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void MemoryCardRemoved (void)
{
   FRESULT Res;
   
   Res = f_mount(NULL, "", 0);
   
   disk_removed(0);
   
   if (FR_OK == Res)
   {
      TAL_DEBUG(TAL_DBG_FS, "SD0: Memory card removed, unmount memory card, OK");
   }
   else
   {
      TAL_DEBUG(TAL_DBG_FS, "SD0: Memory card removed, unmount memory card, Error");
   }
   
   if (UnMountCallback != NULL)
   {
      UnMountCallback();
   }
   
} /* MemoryCardRemoved */

/*************************************************************************/
/*  MemoryCardInserted                                                   */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void MemoryCardInserted (void)
{
   FRESULT Res;

   Res = f_mount(&FSObject, LOGICAL_DRIVE, 0);
   
   if (FR_OK == Res)
   {
      TAL_DEBUG(TAL_DBG_FS, "SD0: Memory card inserted, mount memory card, OK");
   }
   else
   {
      TAL_DEBUG(TAL_DBG_FS, "SD0: Memory card inserted, mount memory card, Error");
   }
   
   if (MountCallback != NULL)
   {
      MountCallback();
   }
} /* MemoryCardInserted */

/*************************************************************************/
/*  SD0FSTask                                                            */
/*                                                                       */
/*  This is the filesystem task for SD0.                                 */
/*                                                                       */
/*  In    : arg                                                          */
/*  Out   : none                                                         */
/*  Return: never                                                        */
/*************************************************************************/
static void SD0FSTask (void *arg)
{
   DSTATUS  DiskStatus;
   DSTATUS  DiskStatusOld;
   
   (void)arg;
   
   /* Register the file system. */
   FSRegister(&FileSystem);

   /* Init diskio interface */
   disk_initialize(0);
   
   /* Get drive status */
   DiskStatus    = disk_status(0) & STA_NODISK; 
   DiskStatusOld = DiskStatus;
   
   /* Check if a memory card is available */
   if (0 == (DiskStatus & STA_NODISK))
   {
      OS_TimeDly(CARD_INSERT_DELAY_MS);
      MemoryCardInserted();
   }
   else
   {
      TAL_DEBUG(TAL_DBG_FS, "SD0: No memory card available");
      MemoryCardRemoved();   
   }

   /* Initialisation is done */
   nInitDone = 1;

   while(1)
   {
      OS_TimeDly(SD0FS_TASL_DELAY_MS);
      
      /* Check if a memory card is available */
      DiskStatus  = disk_status(0) & STA_NODISK;
      if (DiskStatus != DiskStatusOld)      
      {
         if (0 == (DiskStatus & STA_NODISK))
         {
            OS_TimeDly(CARD_INSERT_DELAY_MS);
            disk_initialize(0);
            MemoryCardInserted();
         } 
         
         if (STA_NODISK == (DiskStatus & STA_NODISK))
         {
            MemoryCardRemoved();         
         } 
      }
      DiskStatusOld = DiskStatus;
      
   } /* end while(1) */

} /* SD0FSTask */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  fatfs_Init                                                           */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void fatfs_Init (void)
{
   static uint8_t InitDone = 0;
   
   if (0 == InitDone)
   {
      InitDone = 1;

      OS_TaskCreate(&TCBSD0FS, SD0FSTask, NULL, TASK_SD0FS_PRIORITY,
                    SD0FSStack, sizeof(SD0FSStack), 
                    "SD0-FS");
   }                   

} /* fatfs_Init */

/*************************************************************************/
/*  fatfs_Ready                                                          */
/*                                                                       */
/*  Check if the initialisation was done.                                */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 0 = not read / 1 = ready                                     */
/*************************************************************************/
int fatfs_Ready (void)
{
   return(nInitDone);
} /* fatfs_Ready */

/*************************************************************************/
/*  fatfs_DiskStatus                                                     */
/*                                                                       */
/*  Return the disk status.                                              */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 0 = no disk / 1 = disk available                             */
/*************************************************************************/
int fatfs_DiskStatus (void)
{
   int     nAvailable = 0;
   DSTATUS  DiskStatus;

   DiskStatus = disk_status(0) & STA_NODISK; 
   
   /* Check if a memory card is available */
   if (0 == (DiskStatus & STA_NODISK))
   {
      nAvailable = 1;
   }

   return(nAvailable);
} /* fatfs_DiskStatus */

/*************************************************************************/
/*  fatfs_SetMountCallback                                               */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void fatfs_SetMountCallback (fatfs_mount_callback_t Callback)
{
   MountCallback = Callback;
} /* fatfs_SetMountCallback */

/*************************************************************************/
/*  fatfs_SetUnMountCallback                                             */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void fatfs_SetUnMountCallback (fatfs_mount_callback_t Callback)
{
   UnMountCallback = Callback;
} /* fatfs_SetUnMountCallback */

/*** EOF ***/



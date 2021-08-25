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
#define __FSAPI_C__

/*=======================================================================*/
/*  Include                                                              */
/*=======================================================================*/

#include <stdint.h>
#include <string.h>
#include "tal.h"
#include "fsapi.h"
#include "fcntl.h"

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

/*
 * Maximum open files at the same time
 */
#define MAX_OPEN_FILES  32

typedef struct _fs_file_desc_
{
   FS_FS *pFS;      /* File System */
   void  *pHandle;  /* File handle */ 
} FS_FILE_DESC;

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

static FS_FS       *pFileSystemList = NULL;
static FS_FILE_DESC  FileDescList[MAX_OPEN_FILES];
static OS_SEMA       Sema;

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  FindFSByDriveName                                                    */
/*                                                                       */
/*  Find file system by drive name.                                      */
/*                                                                       */
/*  In    : pDrive                                                       */
/*  Out   : none                                                         */
/*  Return: NULL / pDev                                                  */
/*************************************************************************/
static FS_FS *FindFSByDriveName (const char *pName)
{
   FS_FS *pFS = NULL;
   
   if (pName != NULL)
   {
      for (pFS = pFileSystemList; pFS != NULL; pFS = pFS->pNext)
      {
         if (0 == strcmp(pFS->DriveName, pName))
         {
            break;
         }
      }
   }                        

   return(pFS);
} /* FindFSByDriveName */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  FSInit                                                               */
/*                                                                       */
/*  Initialize the file system support.                                  */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void FSInit (void)
{
   memset(FileDescList, 0x00, sizeof(FileDescList));
   
   OS_RES_CREATE(&Sema);
   
} /* FSInit */

/*************************************************************************/
/*  FSRegister                                                           */
/*                                                                       */
/*  Register a file system.                                              */
/*                                                                       */
/*  In    : pDev                                                         */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = Error                                          */
/*************************************************************************/
int FSRegister (FS_FS *pFS)
{
   int Result = -1;
   
   if (0 == FindFSByDriveName(pFS->DriveName)) 
   {
      pFS->pNext      = pFileSystemList;
      pFileSystemList = pFS;
      Result = 0;
   }
   
   return(Result);
} /* FSRegister */

/*************************************************************************/
/*  _open                                                                */
/*                                                                       */
/*  Open a file and return the file handle.                              */
/*                                                                       */
/*  In    : name, mode                                                   */
/*  Out   : none                                                         */
/*  Return: -1 = Error / file handle                                     */
/*************************************************************************/
int _open (const char *name, int mode)
{
   int     Result = -1;
   int     Handle;
   int     FileDesc;   
   char    Drive[9];
   int     DriveLen = 0;
   char  *pFilename = NULL;
   FS_FS *pFS = NULL;
   
   OS_RES_LOCK(&Sema);
   
   /* Find drive */
   while (DriveLen < (int)sizeof(Drive) -1)
   {
      if (*name != ':')
      {
         Drive[DriveLen] = *name;
         DriveLen++;
         name++;
      }
      else
      {
         Drive[DriveLen] = 0;
         name++;
         pFilename = (char*)name;
         break;
      }
   }

   /* Check if a filename is available too */ 
   if (pFilename != NULL)
   {
      /* Get device */
      pFS = FindFSByDriveName(Drive);
      if (pFS != NULL)
      {
         /* Open file */
         if (pFS->Open != NULL)
         {
            Handle = pFS->Open(pFilename, mode);
            if (Handle != -1)
            {
               /* Search a free file descriptor */
               for (FileDesc = 0; FileDesc < MAX_OPEN_FILES; FileDesc++)
               {
                  if (NULL == FileDescList[FileDesc].pFS)
                  {
                     break;
                  }
               }
               
               /* Check if entry is available */
               if (FileDesc < MAX_OPEN_FILES)
               {
                  FileDescList[FileDesc].pFS     = pFS;
                  FileDescList[FileDesc].pHandle = (void*)Handle;
                  
                  Result = FileDesc;
               }
               else
               {
                  /* Ups, no descriptor available, file must be closed */
                  if (pFS->Close != NULL)
                  {
                     pFS->Close(Handle);   
                  }
               }
            }
         }
      }
   }
   
   OS_RES_FREE(&Sema);
   
   return(Result);
} /* _open */ 

/*************************************************************************/
/*  _close                                                               */
/*                                                                       */
/*  Close the file by the given file handle.                             */
/*                                                                       */
/*  In    : fd                                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = Error                                          */
/*************************************************************************/
int _close (int fd)
{
   int     Result = -1;
   int     Handle;
   FS_FS *pFS;
   
   OS_RES_LOCK(&Sema);

   if (fd < MAX_OPEN_FILES)
   {
      pFS    = FileDescList[fd].pFS;
      Handle = (int)(FileDescList[fd].pHandle);
      if ((pFS->Close != NULL) && (Handle != 0))
      {
         Result = pFS->Close(Handle);
         FileDescList[fd].pFS     = NULL;
         FileDescList[fd].pHandle = NULL;
      }
   }

   OS_RES_FREE(&Sema);

   return(Result);   
} /* _close */

/*************************************************************************/
/*  _read                                                                */
/*                                                                       */
/*  Read data from a file.                                               */
/*                                                                       */
/*  In    : fd, buffer, count                                            */
/*  Out   : none                                                         */
/*  Return: Number of bytes read                                         */
/*************************************************************************/
int _read (int fd, void *buffer, uint32_t count)
{
   int     BytesRead = -1;
   int     Handle;
   FS_FS *pFS;
   
   OS_RES_LOCK(&Sema);

   if (fd < MAX_OPEN_FILES)
   {
      pFS    = FileDescList[fd].pFS;
      Handle = (int)(FileDescList[fd].pHandle);
      if ((pFS->Read != NULL) && (Handle != 0))
      {
         BytesRead = pFS->Read(Handle, buffer, count);
      }
   }      
   
   OS_RES_FREE(&Sema);

   return(BytesRead);
} /* _read */

/*************************************************************************/
/*  _write                                                               */
/*                                                                       */
/*  Write data to a file.                                                */
/*                                                                       */
/*  In    : fd, buffer, count                                            */
/*  Out   : none                                                         */
/*  Return: Number of bytes written                                      */
/*************************************************************************/
int _write (int fd, const void *buffer, size_t count)
{
   int     BytesWritten = -1;
   int     Handle;
   FS_FS *pFS;
   
   OS_RES_LOCK(&Sema);

   if (fd < MAX_OPEN_FILES)
   {
      pFS    = FileDescList[fd].pFS;
      Handle = (int)(FileDescList[fd].pHandle);
      if ((pFS->Write != NULL) && (Handle != 0))
      {
         BytesWritten = pFS->Write(Handle, buffer, count);
      }
   }      
   
   OS_RES_FREE(&Sema);

   return(BytesWritten);
} /* _write */

/*************************************************************************/
/*  _seek                                                                */
/*                                                                       */
/*  Moves the file read/write pointer.                                   */
/*                                                                       */
/*  In    : fd, offset, origin                                           */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = Error                                          */
/*************************************************************************/
int _seek (int fd, long offset, int origin)
{
   int     Result = -1;
   int     Handle;
   FS_FS *pFS;

   OS_RES_LOCK(&Sema);

   if (fd < MAX_OPEN_FILES)
   {
      pFS    = FileDescList[fd].pFS;
      Handle = (int)(FileDescList[fd].pHandle);
      if ((pFS->Seek != NULL) && (Handle != 0))
      {
         Result = pFS->Seek(Handle, offset, origin);
      }
   }      

   OS_RES_FREE(&Sema);
   
   return(Result);
} /* _seek */

/*************************************************************************/
/*  _fstat                                                               */
/*                                                                       */
/*  Get information about the file.                                      */
/*                                                                       */
/*  In    : fd, stat                                                     */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = Error                                          */
/*************************************************************************/
int _fstat (int fd, struct _stat *stat)
{
   int     Result = -1;
   int     Handle;
   FS_FS *pFS;

   OS_RES_LOCK(&Sema);

   if (fd < MAX_OPEN_FILES)
   {
      pFS    = FileDescList[fd].pFS;
      Handle = (int)(FileDescList[fd].pHandle);
      if ((pFS->Stat != NULL) && (Handle != 0))
      {
         Result = pFS->Stat(Handle, stat);
      }
   }      

   OS_RES_FREE(&Sema);
   
   return(Result);
} /* fstat */

/*************************************************************************/
/*  _filelength                                                          */
/*                                                                       */
/*  Return the length of a file.                                         */
/*                                                                       */
/*  In    : fd                                                           */
/*  Out   : none                                                         */
/*  Return: File size                                                    */
/*************************************************************************/
long _filelength (int fd)
{
   long    Size = 0;
   int     Handle;
   FS_FS *pFS;

   OS_RES_LOCK(&Sema);

   if (fd < MAX_OPEN_FILES)
   {
      pFS    = FileDescList[fd].pFS;
      Handle = (int)(FileDescList[fd].pHandle);
      if ((pFS->Filelength != NULL) && (Handle != 0))
      {
         Size = pFS->Filelength(Handle);
      }
   }         

   OS_RES_FREE(&Sema);

   return(Size);
} /* _filelength */

/*** EOF ***/

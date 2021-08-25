/**************************************************************************
*  Copyright (c) 2019 by Michael Fischer (www.emb4fun.de).
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
*  31.05.2019  mifi  First Version.
*  15.08.2020  mifi  Added support for compressed web image data.
**************************************************************************/
#define __FS_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/

#include <stdint.h>
#include <stdlib.h>

#include "tal.h"
#include "fs.h"
#include "ff.h"

#include "fcntl.h"
#include "fsapi.h"
#include "fs_fatfs.h"
#include "fs_xfile.h"
#include "fs_romfs.h"
#include "ipweb.h"
#include "xfile.h"
#include "xbin.h"

/*=======================================================================*/
/*  Definition of all extern Data                                        */
/*=======================================================================*/

int xbin_Check (uint8_t *pImage, uint32_t ImageSize);
int xbin_Copy (uint8_t bIndex);

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

#if !defined(MIN)
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#define ROMFS_HTTP_ROOT_PATH  "ROMFS:/htdocs/"

/*=======================================================================*/
/*  Definition of all global Data                                        */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

static uint8_t  *pWebImageBuffer = NULL;
static uint32_t   WebImageSize = 0;

static char       Web1Name[XFILE_HEADER_NAME_SIZE] = {0};
static char       Web2Name[XFILE_HEADER_NAME_SIZE] = {0};
static uint32_t  dWeb1Version = 0;
static uint32_t  dWeb2Version = 0;

static char       Fw1Name[XFILE_HEADER_NAME_SIZE] = {0};
static char       Fw2Name[XFILE_HEADER_NAME_SIZE] = {0};
static uint32_t  dFw1Version = 0;
static uint32_t  dFw2Version = 0;

static XFILE_HEADER Xfile;
static XBIN_HEADER  Xbin;

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  ReadBuffer                                                           */
/*                                                                       */
/*  Read chunks and yield instead of complete read.                      */
/*                                                                       */
/*  In    : fd, pBuffer, Size                                            */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void ReadBuffer (int fd, uint8_t *pBuffer, uint32_t Size)
{
   uint32_t Index = 0;
   uint32_t BytesRead;

   do 
   {
      OS_TaskYield();
   
      if (Size > 4096)
      {
         BytesRead = (uint32_t)_read(fd, &pBuffer[Index], 4096);
      }
      else
      {
         BytesRead = (uint32_t)_read(fd, &pBuffer[Index], Size);
      }
      
      Index += BytesRead;
      Size  -= BytesRead;
   
   } while (Size != 0);
   
} /* ReadBuffer */

/*************************************************************************/
/*  Mount                                                                */
/*                                                                       */
/*  In    : fd                                                           */
/*  Out   : none                                                         */
/*  Return: 0 == OK / error case                                         */
/*************************************************************************/
static int Mount (int fd)
{
   int      nErr = -1;
   uint8_t *pBuffer;
   
   WebImageSize = (uint32_t)_filelength(fd);
   pWebImageBuffer = (uint8_t*)xmalloc(XM_ID_HEAP, WebImageSize);
   if (pWebImageBuffer != NULL)
   {
      ReadBuffer(fd, pWebImageBuffer, WebImageSize);
      
      pBuffer = xfile_Mount(pWebImageBuffer, WebImageSize);
      if (pBuffer != NULL)
      {
         nErr = 0;
         
         /* Check if the "new buffer" is the correct one */
         if (pBuffer != pWebImageBuffer)
         {
            xfree(pWebImageBuffer);    /* Delete old buffer */
            pWebImageBuffer = pBuffer; /* Use new buffer */
         }
      }            
   }
   
   return(nErr);
} /* Mount */ 

/*************************************************************************/
/*  UnMount                                                              */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void UnMount (void)
{
  xfile_UnMount();
  xfree(pWebImageBuffer);
  WebImageSize    = 0;
  pWebImageBuffer = NULL;
  
} /* UnMount */

/*************************************************************************/
/*  SDMountCallback                                                      */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void SDMountCallback (void)
{
   int       fd;
   uint8_t *pBuffer;
   
#if 1 /* New functionality */      
   uint8_t bIndex = 1;
   
   /*
    * Read index
    */
   fd = _open("SD0:/etc/web.bin", _O_BINARY | _O_RDONLY);
   if (fd != -1)
   {
      _read(fd, &bIndex, 1);
      _close(fd);
   }
   
   /* Open file */
   fd = -1;
   if (1 == bIndex) fd = _open("SD0:/etc/web1.bin", _O_BINARY | _O_RDONLY);
   if (2 == bIndex) fd = _open("SD0:/etc/web2.bin", _O_BINARY | _O_RDONLY);
#else
   fd = _open("SD0:/htdocs/xfile.bin", _O_BINARY | _O_RDONLY);
#endif   
   
   if (fd != -1)
   {
      /*
       * Web image was found
       */
      WebImageSize = (uint32_t)_filelength(fd);
      pWebImageBuffer = (uint8_t*)xmalloc(XM_ID_HEAP, WebImageSize);
      if (pWebImageBuffer != NULL)
      {
         /* Read "xfile" filesystem */
         ReadBuffer(fd, pWebImageBuffer, WebImageSize);
         
         pBuffer = xfile_Mount(pWebImageBuffer, WebImageSize);
         if (pBuffer != NULL)
         {
            /* Check if the "new buffer" is the correct one */
            if (pBuffer != pWebImageBuffer)
            {
               xfree(pWebImageBuffer);    /* Delete old buffer */
               pWebImageBuffer = pBuffer; /* Use new buffer */
            }
         
            /*
             * "xfile" filesystem available.
             */
            HttpRegisterRootPath("XFILE:"); 
            HttpRegisterRedir("", "/index.htm", 307);
            TAL_DEBUG(TAL_DBG_FS, "FS: Use XFILE filesystem");
         }
         else
         {
            /* 
             * Clear invalid "xfile" filesystem 
             */
            xfree(pWebImageBuffer);
            
            WebImageSize    = 0;
            pWebImageBuffer = NULL;

            /* Could not mount XFILE, use ROMFS instead */
            HttpRegisterRootPath(ROMFS_HTTP_ROOT_PATH); 
            HttpRegisterRedir("", "/nocardfs.htm", 307);
            TAL_DEBUG(TAL_DBG_FS, "FS: Use ROMFS filesystem");
         }
      } /* end if (pWebImageBuffer != NULL) */
      
      _close(fd);
      
   } /* end fd = _open("webimage", _O_BINARY | _O_RDONLY); */         
      
} /* SDMountCallback */

/*************************************************************************/
/*  SDUnMountCallback                                                    */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void SDUnMountCallback (void)
{
   /*
    * UnMount "xfile" filesystem if needed.
    */
   if (pWebImageBuffer != NULL)
   {
      xfile_UnMount();
      
      xfree(pWebImageBuffer);
      WebImageSize    = 0;
      pWebImageBuffer = NULL;
   }

   HttpRegisterRootPath(ROMFS_HTTP_ROOT_PATH); 
   HttpRegisterRedir("", "/nocard.htm", 307);
   
   Web1Name[0]  = 0;
   Web2Name[0]  = 0;
   dWeb1Version = 0;
   dWeb2Version = 0;

} /* SDUnMountCallback */

/*************************************************************************/
/*  WebUpload                                                            */
/*                                                                       */
/*  Handle the upload from the web interface.                            */
/*                                                                       */
/*  In    : pUpload                                                      */
/*  Out   : none                                                         */
/*  Return: 0 = OK / error case                                          */
/*************************************************************************/
static int WebUpload (web_upload_t *pUpload)
{
   int      nErr;
   int       fd;   
   uint32_t dWriteCount;
   uint8_t *pBuffer = pUpload->pBuffer;
   uint32_t dSize   = (uint32_t)pUpload->lFileSize;
   uint32_t dWriteSize;
   XFILE_HEADER *pXfile;
      
   /* Check file first */ 
   nErr = xfile_Check(pBuffer, dSize);    
   if (0 == nErr)
   {
      pXfile = (XFILE_HEADER*)pUpload->pBuffer; /*lint !e826*/
      
      /* Setup header info and open file */
      if (1 == pUpload->bBufferIndex)
      {
         memcpy(Web1Name, pXfile->DataName, XFILE_HEADER_NAME_SIZE);
         dWeb1Version = pXfile->dDataVersion;
         
         fd = _open("SD0:/etc/web1.bin", _O_BINARY | _O_WRONLY | _O_CREATE_ALWAYS);
      }
      else
      {
         memcpy(Web2Name, pXfile->DataName, XFILE_HEADER_NAME_SIZE);
         dWeb2Version = pXfile->dDataVersion;
      
         fd = _open("SD0:/etc/web2.bin", _O_BINARY | _O_WRONLY | _O_CREATE_ALWAYS);
      }   

      /* Write file */
      if (fd != -1)
      {
         nErr = 0;
         while(dSize > 0)
         {
            dWriteSize = MIN(dSize, 4096);
      
            dWriteCount = (uint32_t)_write(fd, pBuffer, dWriteSize);
            if (dWriteCount != dWriteSize)
            {
               nErr = XFILE_ERROR_SD;
               break;
            }
         
            pBuffer += dWriteSize;
            dSize   -= dWriteSize; 
            
            OS_TaskYield(); 
         }         
         _close(fd);
      
      }
      else
      {
         nErr = XFILE_ERROR_SD; 
      }
   } /* end if "WebCheck(" */     
   
   return(nErr);
} /* WebUpload */

/*************************************************************************/
/*  FwUpload                                                             */
/*                                                                       */
/*  Handle the upload from the web interface.                            */
/*                                                                       */
/*  In    : pUpload                                                      */
/*  Out   : none                                                         */
/*  Return: 0 = OK / error case                                          */
/*************************************************************************/
static int FwUpload (web_upload_t *pUpload)
{
   int      nErr;
   int       fd;   
   uint32_t dWriteCount;
   uint8_t *pBuffer = pUpload->pBuffer;
   uint32_t dSize   = (uint32_t)pUpload->lFileSize;
   uint32_t dWriteSize;
   XBIN_HEADER *pXbin;
      
   /* Check file first */ 
   nErr = xbin_Check(pBuffer, dSize);    
   if (0 == nErr)
   {
      pXbin = (XBIN_HEADER*)pUpload->pBuffer; /*lint !e826*/
      
      /* Setup header info and open file */
      if (1 == pUpload->bBufferIndex)
      {
         memcpy(Fw1Name, pXbin->DataName, XBIN_DATA_NAME_SIZE);
         dFw1Version = pXbin->dDataVersion;
         
         fd = _open("SD0:/etc/fw1.bin", _O_BINARY | _O_WRONLY | _O_CREATE_ALWAYS);
      }
      else
      {
         memcpy(Fw2Name, pXbin->DataName, XBIN_DATA_NAME_SIZE);
         dFw2Version = pXbin->dDataVersion;
      
         fd = _open("SD0:/etc/fw2.bin", _O_BINARY | _O_WRONLY | _O_CREATE_ALWAYS);
      }   

      /* Write file */
      if (fd != -1)
      {
         nErr = 0;
         while(dSize > 0)
         {
            dWriteSize = MIN(dSize, 4096);
      
            dWriteCount = (uint32_t)_write(fd, pBuffer, dWriteSize);
            if (dWriteCount != dWriteSize)
            {
               nErr = XFILE_ERROR_SD;
               break;
            }
         
            pBuffer += dWriteSize;
            dSize   -= dWriteSize; 
            
            OS_TaskYield(); 
         }         
         _close(fd);
      
      }
      else
      {
         nErr = XFILE_ERROR_SD; 
      }
   } /* end if "WebCheck(" */     
   
   return(nErr);
} /* FwUpload */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  fs_Init                                                              */
/*                                                                       */
/*  Intitialize the file system.                                         */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void fs_Init (void)
{
   /*
    * Initialize the file systems support
    */
   FSInit();

   /*
    * Register the xfile and romfs filesystem
    */   
   xfile_Init();
   romfs_Init();
   
   /* 
    * Register and start the SD-Card filesystem
    */
   fatfs_Init();
   
   /*
    * Wait for SD-Card initialisation done
    */
   while( 0 == fatfs_Ready() )
   {
      OS_TimeDly(100);
   }

   /*
    * ROMFS is the primary filesystem for the webserver, for the moment
    */
   HttpRegisterRootPath(ROMFS_HTTP_ROOT_PATH); 
   HttpRegisterRedir("", "/nocard.htm", 307);
   
   /* 
    * Check if a SD card is available
    */   
   if (1 == fatfs_DiskStatus())
   {
      SDMountCallback();
   }
   
   /* 
    * Register the SD-Card callback functions
    */
   fatfs_SetMountCallback(SDMountCallback);
   fatfs_SetUnMountCallback(SDUnMountCallback);

} /* fs_Init */

/*************************************************************************/
/*  fs_Upload                                                            */
/*                                                                       */
/*  Handle the upload from the web interface.                            */
/*                                                                       */
/*  In    : pUpload                                                      */
/*  Out   : none                                                         */
/*  Return: 0 = OK / error case                                          */
/*************************************************************************/
int fs_Upload (web_upload_t *pUpload)
{
   int nErr = -1;
   
   term_printf("Upload\r\n");
   term_printf("   Name : %s\r\n", pUpload->pFileName); 
   term_printf("   Size : %d\r\n", pUpload->lFileSize); 
   term_printf("   Index: %d\r\n", pUpload->bBufferIndex); 

   if (0 == strcmp(pUpload->pType, "web"))
   {
      nErr = WebUpload(pUpload);
      if (0 == nErr)
      {
         term_printf("Web upload OK\r\n");
      }
      else
      {
         term_printf("Web upload ERROR\r\n");
      }
      term_printf("\r\n");
   }

   if (0 == strcmp(pUpload->pType, "fw"))
   {
      nErr = FwUpload(pUpload);
      if (0 == nErr)
      {
         term_printf("Fw upload OK\r\n");
      }
      else
      {
         term_printf("Fw upload ERROR\r\n");
      }
      term_printf("\r\n");
   }

   return(nErr);
} /* fs_Upload */

/*************************************************************************/
/*  fs_UpgradeWeb                                                        */
/*                                                                       */
/*  Handle the upgrade from the web interface.                           */
/*                                                                       */
/*  In    : bIndex                                                       */
/*  Out   : none                                                         */
/*  Return: 0 = OK / error case                                          */
/*************************************************************************/
int fs_UpgradeWeb (uint8_t bIndex)
{
   int  nErr = -1;
   int   fd  = -1;

   /* Check parameter */
   if ((bIndex != 1) && (bIndex != 2)) return(-1);
      
   /* Open file */
   if (1 == bIndex) fd = _open("SD0:/etc/web1.bin", _O_BINARY | _O_RDONLY);
   if (2 == bIndex) fd = _open("SD0:/etc/web2.bin", _O_BINARY | _O_RDONLY);
   
   if (fd != -1)
   {
      UnMount();
      Mount(fd);
      _close(fd);

      /* Write info */
      fd = _open("SD0:/etc/web.bin", _O_BINARY | _O_WRONLY | _O_CREATE_ALWAYS);
      if (fd != -1)
      {
         _write(fd, &bIndex, 1);
         _close(fd);
      }
      
      nErr = 0;
   } 
   
   if (0 == nErr)
   {
      term_printf("Web upgrade OK\r\n");
   }  
   else
   {
      term_printf("Web upgrade ERROR\r\n");
   }
          
   return(nErr);
} /* fs_UpgradeWeb */

/*************************************************************************/
/*  fs_UpgradeFw                                                         */
/*                                                                       */
/*  Handle the upgrade from the firmware.                                */
/*                                                                       */
/*  In    : bIndex                                                       */
/*  Out   : none                                                         */
/*  Return: 0 = OK / error case                                          */
/*************************************************************************/
int fs_UpgradeFw (uint8_t bIndex)
{
   int  nErr = -1;
   int   fd  = -1;

   /* Check parameter */
   if ((bIndex != 1) && (bIndex != 2)) return(-1);
      
   /* Open file */
   if (1 == bIndex) fd = _open("SD0:/etc/fw1.bin", _O_BINARY | _O_RDONLY);
   if (2 == bIndex) fd = _open("SD0:/etc/fw2.bin", _O_BINARY | _O_RDONLY);
   
   if (fd != -1)
   {
      _close(fd);

      /* Write info */
      fd = _open("SD0:/etc/fw.bin", _O_BINARY | _O_WRONLY | _O_CREATE_ALWAYS);
      if (fd != -1)
      {
         _write(fd, &bIndex, 1);
         _close(fd);
      }
      
      /* Copy firmware image */      
      nErr = xbin_Copy(bIndex);
   } 
   
   if (0 == nErr)
   {
      term_printf("Fw upgrade OK\r\n");
   }  
   else
   {
      term_printf("Fw upgrade ERROR\r\n");
   }
          
   return(nErr);
} /* fs_UpgradeFw */

/*************************************************************************/
/*  fs_WebGetName                                                        */
/*                                                                       */
/*  Return the name of the selected buffer.                              */
/*                                                                       */
/*  In    : bIndex                                                       */
/*  Out   : none                                                         */
/*  Return: pName                                                        */
/*************************************************************************/
char *fs_WebGetName (uint8_t bIndex)
{
   char *pName;
   int    fd       = -1;   
   int    MustRead = 0;
   int   nReadCount;
   
   if ((bIndex != 1) && (bIndex != 2))
   {
      return("error");
   }
   
   if (1 == bIndex) pName = Web1Name;
   if (2 == bIndex) pName = Web2Name;
   if (0 == *pName) MustRead = 1;   /*lint !e644*/

   /* Check if must read */
   if (1 == MustRead)
   {
      if (1 == bIndex) fd = _open("SD0:/etc/web1.bin", _O_BINARY | _O_RDONLY);
      if (2 == bIndex) fd = _open("SD0:/etc/web2.bin", _O_BINARY | _O_RDONLY);

      /* Read header */
      if (fd != -1)
      {
         nReadCount = _read(fd, &Xfile, sizeof(Xfile));
         _close(fd);
      
         if (nReadCount == (int)sizeof(Xfile))
         {
            memcpy(pName, Xfile.DataName, XFILE_HEADER_NAME_SIZE);
         }
      }
      else
      {
         memcpy(pName, "empty", 5+1);
      }
   }      
   
   return(pName);
} /* fs_WebGetName */

/*************************************************************************/
/*  fs_WebGetVersion                                                     */
/*                                                                       */
/*  Return the version of the selected buffer.                           */
/*                                                                       */
/*  In    : bIndex                                                       */
/*  Out   : none                                                         */
/*  Return: dVersion                                                     */
/*************************************************************************/
uint32_t fs_WebGetVersion (uint8_t bIndex)
{
   uint32_t *pVersion;
   int       fd       = -1;   
   int       MustRead = 0;
   int      nReadCount;
   
   if ((bIndex != 1) && (bIndex != 2))
   {
      return(0);
   }
   
   if (1 == bIndex) pVersion = &dWeb1Version;
   if (2 == bIndex) pVersion = &dWeb2Version;
   if (0 == *pVersion) MustRead = 1;   /*lint !e644*/

   /* Check if must read */
   if (1 == MustRead)
   {
      if (1 == bIndex) fd = _open("SD0:/etc/web1.bin", _O_BINARY | _O_RDONLY);
      if (2 == bIndex) fd = _open("SD0:/etc/web2.bin", _O_BINARY | _O_RDONLY);

      /* Read header */
      if (fd != -1)
      {
         nReadCount = _read(fd, &Xfile, sizeof(Xfile));
         _close(fd);
      
         if (nReadCount == (int)sizeof(Xfile))
         {
            *pVersion = Xfile.dDataVersion;
         }
      }
      else
      {
         *pVersion = 0;
      }
   }      
   
   return(*pVersion);
} /* fs_WebGetVersion */

/*************************************************************************/
/*  fs_FwGetName                                                         */
/*                                                                       */
/*  Return the name of the selected buffer.                              */
/*                                                                       */
/*  In    : bIndex                                                       */
/*  Out   : none                                                         */
/*  Return: pName                                                        */
/*************************************************************************/
char *fs_FwGetName (uint8_t bIndex)
{
   char *pName;
   int    fd       = -1;   
   int    MustRead = 0;
   int   nReadCount;
   
   if ((bIndex != 1) && (bIndex != 2))
   {
      return("error");
   }
   
   if (1 == bIndex) pName = Fw1Name;
   if (2 == bIndex) pName = Fw2Name;
   if (0 == *pName) MustRead = 1;   /*lint !e644*/

   /* Check if must read */
   if (1 == MustRead)
   {
      if (1 == bIndex) fd = _open("SD0:/etc/fw1.bin", _O_BINARY | _O_RDONLY);
      if (2 == bIndex) fd = _open("SD0:/etc/fw2.bin", _O_BINARY | _O_RDONLY);

      /* Read header */
      if (fd != -1)
      {
         nReadCount = _read(fd, &Xbin, sizeof(Xbin));
         _close(fd);
      
         if (nReadCount == (int)sizeof(Xbin))
         {
            memcpy(pName, Xbin.DataName, XBIN_DATA_NAME_SIZE);
         }
      }
      else
      {
         memcpy(pName, "empty", 5+1);
      }
   }      
   
   return(pName);
} /* fs_FwGetName */

/*************************************************************************/
/*  fs_FwGetVersion                                                      */
/*                                                                       */
/*  Return the version of the selected buffer.                           */
/*                                                                       */
/*  In    : bIndex                                                       */
/*  Out   : none                                                         */
/*  Return: dVersion                                                     */
/*************************************************************************/
uint32_t fs_FwGetVersion (uint8_t bIndex)
{
   uint32_t *pVersion;
   int       fd       = -1;   
   int       MustRead = 0;
   int      nReadCount;
   
   if ((bIndex != 1) && (bIndex != 2))
   {
      return(0);
   }
   
   if (1 == bIndex) pVersion = &dFw1Version;
   if (2 == bIndex) pVersion = &dFw2Version;
   if (0 == *pVersion) MustRead = 1;   /*lint !e644*/

   /* Check if must read */
   if (1 == MustRead)
   {
      if (1 == bIndex) fd = _open("SD0:/etc/fw1.bin", _O_BINARY | _O_RDONLY);
      if (2 == bIndex) fd = _open("SD0:/etc/fw2.bin", _O_BINARY | _O_RDONLY);

      /* Read header */
      if (fd != -1)
      {
         nReadCount = _read(fd, &Xbin, sizeof(Xbin));
         _close(fd);
      
         if (nReadCount == (int)sizeof(Xbin))
         {
            *pVersion = Xbin.dDataVersion;
         }
      }
      else
      {
         *pVersion = 0;
      }
   }      
   
   return(*pVersion);
} /* fs_FwGetVersion */

/*** EOF ***/



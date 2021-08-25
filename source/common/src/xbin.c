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
*  13.10.2019  mifi  First Version.
**************************************************************************/
#define __FS_XBIN_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/
#include <stdio.h>
#include <stdlib.h>

#include "tal.h"
#include "fs.h"
#include "fcntl.h"
#include "fsapi.h"

#include "xbin.h"
#include "adler32.h"

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

#define XBIN_ERROR_NO_FW   1  /* No valid fw file */
#define XBIN_ERROR_CRC     2  /* CRC or defective file */ 
#define XBIN_ERROR_SIG     3  /* Invalid signature */
#define XBIN_ERROR_SD      4  /* SD card error */

/*=======================================================================*/
/*  Definition of all global Data                                        */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

static XBIN_HEADER  Xbin;

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  xbin_Check                                                           */
/*                                                                       */
/*  Check if this is a valid XBIN file.                                  */
/*                                                                       */
/*  In    : pImage, ImageSize                                            */
/*  Out   : none                                                         */
/*  Return: 0 == OK / error cause                                        */
/*************************************************************************/
int xbin_Check (uint8_t *pImage, uint32_t ImageSize)
{
   int          nErr = XBIN_ERROR_NO_FW;
   XBIN_HEADER *pHeader;
   uint32_t      CRC32;
   
   (void)ImageSize;

   pHeader = (XBIN_HEADER*)pImage;  /*lint !e826*/

   if ( (XBIN_HEADER_MAGIC_1 == pHeader->dMagic1)      &&
        (XBIN_HEADER_MAGIC_2 == pHeader->dMagic2)      &&
        (XBIN_HEADER_SIZEVER == pHeader->dSizeVersion) )
   {
      /* Check CRC32 of the header */
      CRC32 = adler32(ADLER_START_VALUE, (uint8_t*)pHeader, sizeof(XBIN_HEADER) - XBIN_SIZE_OF_CRC32);
      if (CRC32 == pHeader->dHeaderCRC32)
      {
         /* Check xfile data CRC32 */
         CRC32 = adler32(ADLER_START_VALUE, (uint8_t*)&pImage[sizeof(XBIN_HEADER)], pHeader->dDataTotalSize);
         if (CRC32 == pHeader->dDataCRC32)
         {
            nErr = 0;
         }
         else
         {
            nErr = XBIN_ERROR_CRC;
         }
      }
      else
      {
         nErr = XBIN_ERROR_CRC;
      }
   }
   
   return(nErr);
} /* xbin_Check */

/*************************************************************************/
/*  xbin_Copy                                                            */
/*                                                                       */
/*  Copy firmware image.                                                 */
/*                                                                       */
/*  In    : bIndex                                                       */
/*  Out   : none                                                         */
/*  Return: 0 == OK / error cause                                        */
/*************************************************************************/
int xbin_Copy (uint8_t bIndex)
{
   int          nErr = -1;
   int           fd  = -1;
   XBIN_HEADER *pHeader = &Xbin;
   uint32_t      CRC32;
   uint8_t     *pBuffer = NULL;
   
   /* Check parameter */
   if ((bIndex != 1) && (bIndex != 2)) return(-1);
      
   /* 
    * Open file to read 
    */
   if (1 == bIndex) fd = _open("SD0:/etc/fw1.bin", _O_BINARY | _O_RDONLY);
   if (2 == bIndex) fd = _open("SD0:/etc/fw2.bin", _O_BINARY | _O_RDONLY);
   
   if (fd != -1)
   {
      _read(fd, &Xbin, sizeof(Xbin));

      if ( (XBIN_HEADER_MAGIC_1 == pHeader->dMagic1)      &&
           (XBIN_HEADER_MAGIC_2 == pHeader->dMagic2)      &&
           (XBIN_HEADER_SIZEVER == pHeader->dSizeVersion) )
      {
         /* Check CRC32 of the header */
         CRC32 = adler32(ADLER_START_VALUE, (uint8_t*)pHeader, sizeof(XBIN_HEADER) - XBIN_SIZE_OF_CRC32);
         if (CRC32 == pHeader->dHeaderCRC32)
         {
            pBuffer = xmalloc(XM_ID_HEAP, Xbin.dDataTotalSize);
            if (pBuffer != NULL)
            {
               _read(fd, pBuffer, Xbin.dDataTotalSize);   
               CRC32 = adler32(ADLER_START_VALUE, pBuffer, Xbin.dDataTotalSize);
               if (CRC32 == Xbin.dDataCRC32)
               {
                  nErr = 0;
               }
            }
         }
      }         
      _close(fd);
   }
   
   if (0 == nErr)
   {
      /*
       * Write new file
       */
      nErr = -1; 
      fd = _open("SD0:/firmware.bin", _O_BINARY | _O_WRONLY | _O_CREATE_ALWAYS);
      if (fd != -1)
      {
         _write(fd, &Xbin, sizeof(XBIN_HEADER));
         _write(fd, pBuffer, Xbin.dDataTotalSize);
         _close(fd);
      }
   
      /*
       * Check new file
       */       
      fd = _open("SD0:/firmware.bin", _O_BINARY | _O_RDONLY);
      if (fd != -1)
      {
         _read(fd, &Xbin, sizeof(Xbin));

         if ( (XBIN_HEADER_MAGIC_1 == pHeader->dMagic1)      &&
              (XBIN_HEADER_MAGIC_2 == pHeader->dMagic2)      &&
              (XBIN_HEADER_SIZEVER == pHeader->dSizeVersion) )
         {
            /* Check CRC32 of the header */
            CRC32 = adler32(ADLER_START_VALUE, (uint8_t*)pHeader, sizeof(XBIN_HEADER) - XBIN_SIZE_OF_CRC32);
            if (CRC32 == pHeader->dHeaderCRC32)
            {
               _read(fd, pBuffer, Xbin.dDataTotalSize);   
               CRC32 = adler32(ADLER_START_VALUE, pBuffer, Xbin.dDataTotalSize);
               if (CRC32 == Xbin.dDataCRC32)
               {
                  nErr = 0;
               }
            }
         }         
         _close(fd);
      }
   }      
   
   if (pBuffer != NULL)
   {
      xfree(pBuffer);
   }
   
   return(nErr);
} /* xbin_Copy */

/*** EOF ***/



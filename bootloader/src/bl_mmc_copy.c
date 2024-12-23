/**************************************************************************
*  Copyright (c) 2017-2024 by Michael Fischer (www.emb4fun.de).
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
*  12.02.2017  mifi  First Version.
*  14.07.2019  mifi  Use BOOT_NAME instead of define in code.
**************************************************************************/
#define __BL_MMC_COPY_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/
#include <string.h>
#include <stdint.h>

#include "ff.h"
#include "bl.h"
#include "uartStdio.h"
#include "adler32.h"
#include "xbin.h"

extern unsigned int entryPoint;

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

#define DATA_BUF_SIZE   512

/*=======================================================================*/
/*  Definition of all global Data                                        */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

static XBIN_HEADER  Xbin;

static char g_cTmpBuf[DATA_BUF_SIZE] __attribute__ ((aligned (32)));

static FIL hFile;

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  HSMMCSDImageCopy                                                     */
/*                                                                       */
/*  This function copies the application image to the DDR by reading the */
/*  header info.                                                         */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 0 = Error / 1 = OK                                           */
/*************************************************************************/
unsigned int HSMMCSDImageCopy (void)
{
    FRESULT fresult;
    unsigned int uiBytesRead = 0;
    unsigned char *destAddr;
    char *fname = BOOT_NAME;
    
    XBIN_HEADER *pHeader = &Xbin;
    uint32_t      CRC32;

    /*
    ** Open the file for reading.
    */
    fresult = f_open(&hFile, fname, FA_READ);

    /*
    ** If there was some problem opening the file, then return an error.
    */
    if(fresult != FR_OK)
    {
        UARTPuts("\r\nUnable to open application file\r\n", -1);
        return 0;
    }
    else
    {
        UARTPuts("\r\nCopying application image from MMC/SD card to RAM\r\n", -1);
        
        destAddr = (unsigned char*)0x80000000;
        entryPoint = 0x80000000;
    }

    /* 
     * Read anc check XBIN header first
     */
    fresult = f_read(&hFile, &Xbin, sizeof(XBIN_HEADER), &uiBytesRead); 
    if (FR_OK == fresult)
    {
        fresult = FR_INT_ERR;
        
        /* Check XBIN header */ 
        if ( (XBIN_HEADER_MAGIC_1 == pHeader->dMagic1)      &&
             (XBIN_HEADER_MAGIC_2 == pHeader->dMagic2)      &&
             (XBIN_HEADER_SIZEVER == pHeader->dSizeVersion) )
        {
            /* Check CRC32 of the header */
            CRC32 = adler32(ADLER_START_VALUE, (uint8_t*)pHeader, sizeof(XBIN_HEADER) - XBIN_SIZE_OF_CRC32);
            if (CRC32 == pHeader->dHeaderCRC32)
            {
                fresult = FR_OK;
            }
        }    
    }
    
    if (fresult != FR_OK)
    {
        entryPoint = 0;
    
        UARTPuts("\r\n Error CRC header\r\n", -1);
        f_close(&hFile);
        return(0);
    }

    /*
    ** Enter a loop to repeatedly read data from the file and display it, until
    ** the end of the file is reached.
    */
    do
    {
        /*
        ** Read a block of data from the file.  Read as much as can fit in the
        ** temporary buffer, including a space for the trailing null.
        */
        fresult = f_read(&hFile, g_cTmpBuf, sizeof(g_cTmpBuf) - 1,
                         &uiBytesRead);

        /*
        ** If there was an error reading, then print a newline and return the
        ** error to the user.
        */
        if(fresult != FR_OK)
        {
            UARTPuts("\r\n Error reading application file\r\n", -1);
            return 0;
        }

        if(uiBytesRead >= sizeof(g_cTmpBuf))
        {
            return 0;
        }

        /*
        ** Null terminate the last block that was read to make it a null
        ** terminated string that can be used with printf.
        */
        g_cTmpBuf[uiBytesRead] = 0;

        /*
        ** Read the last chunk of the file that was received.
        */
        memcpy(destAddr, g_cTmpBuf, (sizeof(g_cTmpBuf) - 1));
        destAddr += (sizeof(g_cTmpBuf) - 1);
        /*
        ** Continue reading until less than the full number of bytes are read.
        ** That means the end of the buffer was reached.
        */
    }
    while(uiBytesRead == sizeof(g_cTmpBuf) - 1);

    /*
    ** Close the file.
    */
    fresult = f_close(&hFile);
    
    (void)fresult;
    
    /*
     * Check image
     */
    CRC32 = adler32(ADLER_START_VALUE, (uint8_t*)0x80000000, pHeader->dDataTotalSize);
    if (CRC32 != pHeader->dDataCRC32)
    {
        entryPoint = 0;
        UARTPuts("\r\n Error CRC image\r\n", -1);
        return(0);
    
    }

    /*
    ** Return success.
    */
    return 1;
} /* HSMMCSDImageCopy */

/*** EOF ***/



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
*  11.04.2017  mifi  First Version.
**************************************************************************/
#define __MINGLUE_C__

/*=======================================================================*/
/*  Include                                                              */
/*=======================================================================*/

#include <stdint.h>
#include "tal.h"
#include "fsapi.h"
#include "minini.h"

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  ini_openread                                                         */
/*                                                                       */
/*  Open the file and return the file handle.                            */
/*                                                                       */
/*  In    : Filename, File                                               */
/*  Out   : fp                                                           */
/*  Return: 0 = ERROR / 1 = OK                                           */
/*************************************************************************/
int ini_openread (const char *Filename, INI_FILETYPE *File)
{
   int ok = 0;
   int fd;
   
   fd = _open(Filename, 0);
   if (fd != -1)
   {
      ok    = 1; 
      *File = fd;   
   }
   
   return(ok);
} /* ini_openread */

/*************************************************************************/
/*  ini_close                                                            */
/*                                                                       */
/*  Close the file by the given file handle.                             */
/*                                                                       */
/*  In    : File                                                         */
/*  Out   : none                                                         */
/*  Return: 0 = ERROR / 1 = OK                                           */
/*************************************************************************/
int ini_close (INI_FILETYPE *File)
{
   int ok = 0;
   
   if (0 == _close(*File))
   {
      ok = 1;
   }
   
   return(ok);
} /* ini_close */

/*************************************************************************/
/*  ini_read                                                             */
/*                                                                       */
/*  Read a string from a file, up to 0x0A or end of file.                */
/*                                                                       */
/*  In    : Buffer, Size, File                                           */
/*  Out   : none                                                         */
/*  Return: Number of bytes read                                         */
/*************************************************************************/
int ini_read (char *Buffer, uint32_t Size, INI_FILETYPE *File)
{
   int     BytesRead = 0;
   int     Result;
   uint8_t Data;
   
   /* Check for vaild parameter */
   if ((Buffer != NULL) && (Size != 0) && (File != NULL))
   {
      Size     -= 1; /* Sub one for the 0 terminator */
      BytesRead = 0;
      while (Size)
      {
         Result = _read(*File, &Data, 1);
         if (0 == Result)     /* Check for end of file */
         {
            break;   
         }
         else
         {
            if (0x0A == Data) /* Check for 0x0A */
            {
               break;
            }
            else
            {
               Buffer[BytesRead] = (char)Data;
               BytesRead++;
               Size--;
            }
         }
      }  
      Buffer[BytesRead] = 0;
   }

   return(BytesRead);
} /* ini_read */

/*** EOF ***/

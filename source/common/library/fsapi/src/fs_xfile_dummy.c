/**************************************************************************
*  Copyright (c) 2023 by Michael Fischer (www.emb4fun.de).
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
*  01.07.2023  mifi  First Version.
**************************************************************************/
#define __FS_XFILE_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tal.h"

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all global Data                                        */
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
/*  xfile_Init                                                           */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void xfile_Init (void)
{
} /* xfile_Init */

/*************************************************************************/
/*  xfile_Check                                                          */
/*                                                                       */
/*  Check if this is a valid XFILE.                                      */
/*                                                                       */
/*  In    : pImage, ImageSize                                            */
/*  Out   : none                                                         */
/*  Return: 0 == OK / error cause                                        */
/*************************************************************************/
int xfile_Check (uint8_t *pImage, uint32_t ImageSize)
{
   (void)pImage;
   (void)ImageSize;

   return(-1);
} /* xfile_Check */

/*************************************************************************/
/*  xfile_Mount                                                          */
/*                                                                       */
/*  Mount the "xfile" buffer.                                            */
/*                                                                       */
/*  In    : pImage, Size                                                 */
/*  Out   : none                                                         */
/*  Return: pBuffer on success, NULL otherwise.                          */
/*************************************************************************/
uint8_t *xfile_Mount (uint8_t *pImage, uint32_t ImageSize)
{
   (void)pImage;
   (void)ImageSize;

   return(NULL);
} /* xfile_Mount */

/*************************************************************************/
/*  xfile_UnMount                                                        */
/*                                                                       */
/*  UnMount the "xfile" filesystem.                                      */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void xfile_UnMount (void)
{
} /* xfile_UnMount */

/*************************************************************************/
/*  xfile_GetName                                                        */
/*                                                                       */
/*  Return the name of the xfile.                                        */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
char *xfile_GetName (void)
{
   char *pName = "unknown";

   return(pName);
} /* xfile_GetName */

/*************************************************************************/
/*  xfile_GetVersion                                                     */
/*                                                                       */
/*  Return the version of the xfile.                                     */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
uint32_t xfile_GetVersion (void)
{
   return(0);
} /* xfile_GetVersion */

/*** EOF ***/



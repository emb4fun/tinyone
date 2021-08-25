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
*  25.01.2015  mifi  First Version.
**************************************************************************/
#define __MOD_VFS_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <fcntl.h>
#include <uhttp_io.h>
#include <pro/uhttp/uhttpd.h>
#include <pro/uhttp/streamio.h>
#include <pro/uhttp/mediatypes.h>

#include "mod_vfs.h"

#if defined(HTTP_SUPPORT_VFS)

extern const VFS_LIST_ENTRY VFSList[];

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
/*  HttpVfsHandler                                                       */
/*                                                                       */
/*  In    : hs, mt, filepath                                             */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 == ERROR                                         */
/*************************************************************************/
int HttpVfsHandler (HTTPD_SESSION *hs, const MEDIA_TYPE_ENTRY *mt, const char *filepath)
{
   int              res = -1;
   long             avail;
   VFS_LIST_ENTRY *pList = (VFS_LIST_ENTRY*)VFSList;
   char            *arg;   
   char            *val;
   
   (void)mt;
   
   while (pList->Var != NULL)
   {
      /* Check for the virtual file */
      if (0 == strncasecmp(pList->Var, filepath, strlen(filepath)))
      {
         /* Get content length */
         avail = hs->s_req.req_length;
         if (avail)  /* Check for POST Method */
         {
            /*
             * POST Method was used
             */
             
            /* Get argument */
            arg = HttpArgReadNext(hs, &avail);
            if (arg)
            {
               /* Get value */
               val = HttpArgValue(&hs->s_req);
               if ((val != NULL) && (pList->pFunc != NULL))
               {
                  /* Call virtual file function */
                  res = pList->pFunc(hs, arg, val);
                  break;
               }
            }
         }
         else
         {
            /*
             * GET Method was used
             */
            if (pList->pFunc != NULL)
            {
               res = pList->pFunc(hs, NULL, NULL);
               break;
            }
         }
      }
      pList++;
   }
   
   /* In case of an ERROR, send 404 */   
   if (-1 == res)
   {
      HttpSendError(hs, 404);
   }   
   
   return(0);
} /* HttpVfsHandler */  

#endif /* defined(HTTP_SUPPORT_VFS) */

/*** EOF ***/



/**************************************************************************
*  Copyright (c) 2020-2022 by Michael Fischer (www.emb4fun.de).
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
**************************************************************************/
#define __XMEMPOOL_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include "tal.h"

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

#define FS_MEMORY_SIZE     (  64 * 1024)
#define LWIP_MEMORY_SIZE   ( 256 * 1024)
#define WEB_MEMORY_SIZE    ( 256 * 1024)
#define TLS_MEMORY_SIZE    (1024 * 1024)

/*=======================================================================*/
/*  Definition of all global Data                                        */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all extern Data                                        */
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
/*  xmem_Init()                                                          */
/*                                                                       */
/*  Initialize the memory pool.                                          */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void xmem_Init (void)
{
   uint8_t *pBuffer;

   /*lint -save -e423 -e429 */

   pBuffer = xmalloc(XM_ID_HEAP, FS_MEMORY_SIZE);
   tal_MEMAdd(XM_ID_FS, "FS", pBuffer, FS_MEMORY_SIZE);

   pBuffer = xmalloc(XM_ID_HEAP, LWIP_MEMORY_SIZE);
   tal_MEMAdd(XM_ID_IP, "lwIP", pBuffer, LWIP_MEMORY_SIZE);
   
   pBuffer = xmalloc(XM_ID_HEAP, WEB_MEMORY_SIZE);
   tal_MEMAdd(XM_ID_WEB, "Web", pBuffer, WEB_MEMORY_SIZE);
   
   pBuffer = xmalloc(XM_ID_HEAP, TLS_MEMORY_SIZE);
   tal_MEMAdd(XM_ID_TLS, "mbedTLS", pBuffer, TLS_MEMORY_SIZE);
   
   /*lint -restore */

} /* xmem_Init */

/*************************************************************************/
/*  mbedtls_calloc                                                       */
/*                                                                       */
/*  Allocate memory from the mbedTLS pool of the given size, and         */
/*  initialize them to zero. Returns NULL if memory cannot be allocated. */
/*                                                                       */
/*  In    : nobj, size                                                   */
/*  Out   : none                                                         */
/*  Return: p / NULL                                                     */
/*************************************************************************/
void *mbedtls_calloc (size_t n, size_t size)
{
   void *p;

   p = xcalloc(XM_ID_TLS, n, size);

   return(p);
} /* mbedtls_calloc */

/*************************************************************************/
/*  mbedtls_free                                                         */
/*                                                                       */
/*  Frees the allocated memory of the mbedTLS pool.                      */
/*                                                                       */
/*  In    : p                                                            */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void mbedtls_free (void *p)
{
  xfree(p);
} /* mbedtls_free */

/*************************************************************************/
/*  lwip_malloc                                                          */
/*                                                                       */
/*  Allocate memory from the lwIP pool of the given size.                */
/*  Returns NULL if memory cannot be allocated.                          */
/*                                                                       */
/*  In    : size                                                         */
/*  Out   : none                                                         */
/*  Return: p / NULL                                                     */
/*************************************************************************/
void *lwip_malloc (size_t size)
{
   void *p;

   p = xmalloc(XM_ID_IP, size);

   return(p);
} /* lwip_malloc */

/*************************************************************************/
/*  lwip_calloc                                                          */
/*                                                                       */
/*  Allocate memory from the lwIP pool of the given size, and            */
/*  initialize them to zero. Returns NULL if memory cannot be allocated. */
/*                                                                       */
/*  In    : nobj, size                                                   */
/*  Out   : none                                                         */
/*  Return: p / NULL                                                     */
/*************************************************************************/
void *lwip_calloc (size_t n, size_t size)
{
   void *p;

   p = xcalloc(XM_ID_TLS, n, size);

   return(p);
} /* lwip_calloc */

/*************************************************************************/
/*  lwip_free                                                            */
/*                                                                       */
/*  Frees the allocated memory of the lwIP pool.                         */
/*                                                                       */
/*  In    : p                                                            */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void lwip_free (void *p)
{
  xfree(p);
} /* lwip_free */

/*** EOF ***/

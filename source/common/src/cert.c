/**************************************************************************
*  Copyright (c) 2020 by Michael Fischer (www.emb4fun.de).
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
*  05.03.2020  mifi  First Version.
**************************************************************************/
#define __CERT_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/
#include <string.h>
#include "fsapi.h"
#include "cert.h"
#include "ff.h"
#include "elca_client.h"

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all global Data                                        */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

static char DeviceKey[2048];
static char ChainCert[2048];

static size_t DeviceKeySize = 0;
static size_t ChainCertSize = 0;

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  cert_Init                                                            */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void cert_Init (void)
{
   int    fd;
   size_t Length;
   int    ReadCount;   
   
   memset(DeviceKey, 0x00, sizeof(DeviceKey));
   memset(ChainCert, 0x00, sizeof(ChainCert));
   
   /* Read device key  */
   fd = _open("SD0:/certs/device.key", _O_BINARY | _O_RDONLY);
   if (fd != -1)
   {
      Length = (size_t)_filelength(fd);
      if (Length < sizeof(DeviceKey))
      {
         ReadCount = _read(fd, DeviceKey, Length); 
         DeviceKey[ReadCount++] = 0;
         DeviceKeySize = (size_t)ReadCount;
      }
      _close(fd);
   }
   
   /* Read chain cert  */
   fd = _open("SD0:/certs/chain.crt", _O_BINARY | _O_RDONLY);
   if (fd != -1)
   {
      Length = (size_t)_filelength(fd);
      if (Length < sizeof(ChainCert))
      {
         ReadCount = _read(fd, ChainCert, Length); 
         ChainCert[ReadCount++] = 0;
         ChainCertSize = (size_t)ReadCount;
      }
      _close(fd);
   }
      
} /* cert_Init */

/*************************************************************************/
/*  cert_Check                                                           */
/*                                                                       */
/*  Check if the data for the cert system are available.                 */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 0 == OK / error cause                                        */
/*************************************************************************/
int cert_Check (void)
{
   int rc = 0;
   int fd;

   /* Check device key */
   fd = _open("SD0:/certs/device.key", _O_BINARY | _O_RDONLY);
   if (fd != -1)
   {
      _close(fd);
   }
   else
   {
      rc = -1;
   }

   /* Check chain cert */
   fd = _open("SD0:/certs/chain.crt", _O_BINARY | _O_RDONLY);
   if (fd != -1)
   {
      _close(fd);
   }
   else
   {
      rc = -1;
   }
   
   return(rc);
} /* cert_Check */

/*************************************************************************/
/*  cert_ELCACallback                                                    */
/*                                                                       */
/*  This is the ELCA callback.                                           */
/*                                                                       */
/*  In    : nError                                                       */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void cert_ELCACallback (int nError)
{
   char *pData;
   size_t Len;
   int    fd;
   
   if (0 == nError)
   {
      /* Create directory if not available */
      f_mkdir("/certs");

      /*
       * Write device key 
       */
      pData = IP_ELCAC_KeyGet();
      Len   = strlen(pData);

      fd = _open("SD0:/certs/device.key", _O_BINARY | _O_WRONLY | _O_CREATE_ALWAYS);
      if (fd != -1)
      {
         _write(fd, pData, Len);
         _close(fd);
      }

      /*
       * Write chain cert
       */
      pData = IP_ELCAC_DevCRTGet();
      Len   = strlen(pData);
      fd = _open("SD0:/certs/chain.crt", _O_BINARY | _O_WRONLY | _O_CREATE_ALWAYS);
      if (fd != -1)
      {
         _write(fd, pData, Len);

         pData = IP_ELCAC_InterCRTGet();
         Len   = strlen(pData);
         _write(fd, pData, Len);
         
         _close(fd);
      }
      
   }
   
} /* cert_ELCACallback */

/*************************************************************************/
/*  cert_Get_DeviceKey                                                   */
/*                                                                       */
/*  Get the device key data.                                             */
/*                                                                       */
/*  In    : buf, buflen                                                  */
/*  Out   : none                                                         */
/*  Return: 0 == OK / error cause                                        */
/*************************************************************************/
int cert_Get_DeviceKey (char **buf, size_t *buflen)
{
   int nErr = -1;
   
   if ((buf != NULL) && (buflen != NULL))
   {
      nErr    = 0;
      *buf    = DeviceKey;
      *buflen = DeviceKeySize;
   }
   
   return(nErr);
} /* cert_Get_DeviceKey */

/*************************************************************************/
/*  cert_Get_ChainCert                                                   */
/*                                                                       */
/*  Get the chain cert data, device + intermediate.                      */
/*                                                                       */
/*  In    : buf, buflen                                                  */
/*  Out   : none                                                         */
/*  Return: 0 == OK / error cause                                        */
/*************************************************************************/
int cert_Get_ChainCert (char **buf, size_t *buflen)
{
   int nErr = -1;
   
   if ((buf != NULL) && (buflen != NULL))
   {
      nErr    = 0;
      *buf    = ChainCert;
      *buflen = ChainCertSize;
   }
   
   return(nErr);
} /* cert_Get_ChainCert */

/*************************************************************************/
/*  cert_Write_ChainCert                                                 */
/*                                                                       */
/*  Write the chain cert data, device + intermediate.                    */
/*                                                                       */
/*  In    : buf, buflen                                                  */
/*  Out   : none                                                         */
/*  Return: 0 == OK / error cause                                        */
/*************************************************************************/
int cert_Write_ChainCert (char *buf, size_t buflen)
{
   int rc = -1;  
   int fd;

   fd = _open("SD0:/certs/chain.crt", _O_BINARY | _O_WRONLY | _O_CREATE_ALWAYS);
   if (fd != -1)
   {
      _write(fd, buf, buflen);
      _close(fd);
      
      rc = 0;
   }

   return(rc);
} /* cert_Write_ChainCert */

/*** EOF ***/

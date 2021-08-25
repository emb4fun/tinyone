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
*  14.11.2020  mifi  First Version.
**************************************************************************/
#define __WEB_USER_C__

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/

#include <string.h>
#include "tal.h"
#include "ipweb.h"
#include "fsapi.h"
#include "adler32.h"

#include "mbedtls/sha256.h"

/*=======================================================================*/
/*  Extern                                                               */
/*=======================================================================*/

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

#define sha2_context    mbedtls_sha256_context
#define sha2_start(_a)  mbedtls_sha256_starts_ret(_a, 0)
#define sha2_update     mbedtls_sha256_update_ret
#define sha2_finish     mbedtls_sha256_finish_ret


#define USER_MAGIC      0x52455355  /* USER */
#define USER_SIZEVER    ((((uint32_t)sizeof(user_db_t)) << 16) | 0x0001)
#define USER_CNT        8


#define RESET_SIZE      8
#define SALT_SIZE       8
#define USER_SIZE       31
#define HASH_SIZE       32

typedef struct _user_
{
   uint8_t   Reset[RESET_SIZE];
   uint8_t   Salt[SALT_SIZE];
   char      User[USER_SIZE+1];
   uint8_t   PassHash[HASH_SIZE];
   uint32_t dPermission;
} user_t;

typedef struct __attribute__((__packed__)) _user_db_
{
   uint32_t dMagic;
   uint32_t dSizeVersion;
   user_t    User[USER_CNT];
   uint32_t dCRC32;   
} user_db_t;

#define USER_ADMIN   0

/*
 * Policy mask
 */
#define SIZE_OK         0x01
#define UPPERCASE_OK    0x02
#define LOWERCASE_OK    0x04
#define NUMBER_OK       0x08
#define SYMBOL_OK       0x10

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

//static user_t UserList[USER_CNT];
static user_db_t UserDB;

static int nMustInitDB;

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  CheckPasswordRules                                                   */
/*                                                                       */
/*  In    : pPass                                                        */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int CheckPasswordRules (char *pPass)
{
   int      rc = -1;
   uint8_t bFlags = 0;

   #define PASS_MASK_OK 0x1F
   
   if (strlen(pPass) >= 8)
   {
      bFlags |= SIZE_OK;
      
      while (*pPass != 0)
      {
         /* Check uppercase */
         if      ((*pPass >= 'A') && (*pPass <= 'Z'))
         {
            bFlags |= UPPERCASE_OK;
         }
         /* Check lowercase */
         else if ((*pPass >= 'a') && (*pPass <= 'z'))
         {
            bFlags |= LOWERCASE_OK;
         }
         /* Check numbers */
         else if ((*pPass >= '0') && (*pPass <= '9'))
         {
            bFlags |= NUMBER_OK;
         }
         
         /* " !\"#$%&'()*+,-./:;<=>?@[\]^_`{|}~" */
         /* https://owasp.org/www-community/password-special-characters */
         
         /* Check symbols " !"#$%&'()*+,-./" */
         else if ((*pPass >= 0x20) && (*pPass <= 0x2F))
         {
            bFlags |= SYMBOL_OK;
         }
         /* Check symbols ":;<=>?@" */
         else if ((*pPass >= 0x3A) && (*pPass <= 0x40))
         {
            bFlags |= SYMBOL_OK;
         }
         /* Check symbols "[\]^_`" */
         else if ((*pPass >= 0x5B) && (*pPass <= 0x60))
         {
            bFlags |= SYMBOL_OK;
         }
         /* Check symbols "{|}~" */
         else if ((*pPass >= 0x7B) && (*pPass <= 0x7E))
         {
            bFlags |= SYMBOL_OK;
         }
         
         pPass++;
      }
   }
      
   /* Check password policy requirements */
   if (PASS_MASK_OK == bFlags)
   {
      rc = 0;
   }
   
   return(rc);
} /* CheckPasswordRules */

/*************************************************************************/
/*  StrToLower                                                           */
/*                                                                       */
/*  In    : pStr                                                         */
/*  Out   : pStr                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void StrToLower (char *pStr)
{
   if (pStr != NULL)
   {
      while (*pStr != 0)
      {
         *pStr = (char)tolower(*pStr);
         pStr++;
      }
   }
} /* StrToLower */

/*************************************************************************/
/*  CreateReset                                                          */
/*                                                                       */
/*  In    : pReset                                                       */
/*  Out   : pReset                                                       */
/*  Return: none                                                         */
/*************************************************************************/
static void CreateReset (uint8_t *pReset)
{
   tal_CPURngHardwarePoll(pReset, RESET_SIZE);

} /* CreateReset */

/*************************************************************************/
/*  CreateSalt                                                           */
/*                                                                       */
/*  In    : pSalt                                                        */
/*  Out   : pSalt                                                        */
/*  Return: none                                                         */
/*************************************************************************/
static void CreateSalt (uint8_t *pSalt)
{
   tal_CPURngHardwarePoll(pSalt, SALT_SIZE);

} /* CreateSalt */

/*************************************************************************/
/*  CreateHashBySalt                                                     */
/*                                                                       */
/*  In    : pSalt, pPass, PassLen, pHash                                 */
/*  Out   : pHash                                                        */
/*  Return: none                                                         */
/*************************************************************************/
static void CreateHashBySalt (uint8_t *pSalt, char *pPass, size_t PassLen, uint8_t *pHash)
{
   static sha2_context HASHctx;

   sha2_start(&HASHctx);
   sha2_update(&HASHctx, (uint8_t*)pSalt, SALT_SIZE);
   sha2_update(&HASHctx, (uint8_t*)pPass, PassLen);
   sha2_finish(&HASHctx, pHash);

} /* CreateHashBySalt */

/*************************************************************************/
/*  UserDBWrite                                                          */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int UserDBWrite (void)
{
   int rc = -1;
   int fd;
   
   UserDB.dCRC32 = adler32(ADLER_START_VALUE, (uint8_t*)&UserDB, sizeof(UserDB) - sizeof(uint32_t));

   fd = _open("SD0:/etc/user.bin", _O_BINARY | _O_WRONLY | _O_CREATE_ALWAYS);
   if (fd != -1)
   {
      rc = _write(fd, &UserDB, sizeof(UserDB));
      _close(fd);
   
      /* Check write */
      if (rc == (int)sizeof(UserDB))
      {
         rc = 0;
      }
   }
   
   return(rc);
} /* UserDBWrite */

/*************************************************************************/
/*  UserDBRead                                                           */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int UserDBRead (void)
{
   int       rc = -1;
   int       fd;
   uint32_t dCRC32;
   
   fd = _open("SD0:/etc/user.bin", _O_BINARY | _O_RDONLY);
   if (fd != -1)
   {
      rc = _read(fd, &UserDB, sizeof(UserDB));
      _close(fd);
   
      /* Check read */
      if (rc == (int)sizeof(UserDB))
      {
         /* Check CRC */
         dCRC32 = adler32(ADLER_START_VALUE, (uint8_t*)&UserDB, sizeof(UserDB) - sizeof(uint32_t));
         if (dCRC32 == UserDB.dCRC32)
         {
            rc = 0;
         }
      }
      else
      {
         rc = -1;
      }
   }
   
   return(rc);
} /* UserDBRead */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  WebUserInit                                                          */
/*                                                                       */
/*  Initialize the User functionality of the web server.                 */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void WebUserInit (void)
{
   int rc;

   /* Read UserDB */   
   rc = UserDBRead();
   if (-1 == rc)
   {
      nMustInitDB = 1;
   
      /* Init UserDB list */
      memset(&UserDB, 0x00, sizeof(UserDB));
   
      UserDB.dMagic       = USER_MAGIC;  
      UserDB.dSizeVersion = USER_SIZEVER;

      /*
       * Setup admin
       */
      CreateReset(UserDB.User[USER_ADMIN].Reset);
      CreateSalt(UserDB.User[USER_ADMIN].Salt);
      sprintf(UserDB.User[USER_ADMIN].User, "admin");
      UserDB.User[USER_ADMIN].dPermission = 0xFFFFFFFF;
      
#if 0
      /*
       * Setup guest
       */
      CreateReset(UserDB.User[1].Reset);
      CreateSalt(UserDB.User[1].Salt);
      sprintf(UserDB.User[1].User, "guest");
      CreateHashBySalt(UserDB.User[1].Salt, "guest", 5, UserDB.User[1].PassHash);
      UserDB.User[1].dPermission = 0;

      /*
       * Setup user1
       */
      CreateReset(UserDB.User[2].Reset);
      CreateSalt(UserDB.User[2].Salt);
      sprintf(UserDB.User[2].User, "user1");
      CreateHashBySalt(UserDB.User[2].Salt, "user1", 5, UserDB.User[2].PassHash);
      UserDB.User[2].dPermission = 0x00000001;
   
      /*
       * Setup user2
       */
      CreateReset(UserDB.User[3].Reset);
      CreateSalt(UserDB.User[3].Salt);
      sprintf(UserDB.User[3].User, "user2");
      CreateHashBySalt(UserDB.User[3].Salt, "user2", 5, UserDB.User[3].PassHash);
      UserDB.User[3].dPermission = 0x00000002;
#endif      
      
   }
   else
   {
      nMustInitDB = 0;
   }
   
} /* WebUserInit */

/*************************************************************************/
/*  WebUserIsFirstUser                                                   */
/*                                                                       */
/*  Return the info if the "User" must be initialize.                    */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 1 = must be init / 0 = not init needed                       */
/*************************************************************************/
int WebUserIsFirstUser (void)
{
   return(nMustInitDB);
} /* WebUserIsFirstUser */

/*************************************************************************/
/*  WebUserFirstUserSet                                                  */
/*                                                                       */
/*  Set the initial password for the admin user.                         */
/*                                                                       */
/*  In    : pPassword                                                    */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void WebUserFirstUserSet (char *pPassword)
{
   if ((pPassword != NULL) && (1 == nMustInitDB))
   {
      nMustInitDB = 0;
      CreateHashBySalt(UserDB.User[USER_ADMIN].Salt, 
                       pPassword, strlen(pPassword),
                       UserDB.User[USER_ADMIN].PassHash);
      
      /* Update UserDB */
      UserDBWrite();
   }      
   
} /* WebUserFirstUserSet */

/*************************************************************************/
/*  WebUserCheckUserPassword                                             */
/*                                                                       */
/*  Check if User/Password is valid.                                     */
/*                                                                       */
/*  In    : pUser, pPassword, pPermission                                */
/*  Out   : pPermission                                                  */
/*  Return: -1 == invalid / user index                                   */
/*************************************************************************/
int WebUserCheckUserPassword (char *pUser, char *pPassword, uint32_t *pPermission)
{
   int           nValid = -1;
   int           nIndex;
   static uint8_t Hash[HASH_SIZE];
   
   if ( (pUser != NULL) && (pPassword != NULL) && (pPermission != NULL))
   {   
      *pPermission = 0;
      
      /* tolower pUser */
      StrToLower(pUser);
   
      /* Check for valid user/password combination */
      for(nIndex = 0; nIndex < USER_CNT; nIndex++)
      {
         /* Check if user match with our "database" */
         if (0 == strcmp(UserDB.User[nIndex].User, pUser))
         {
            /* Create hash from password */
            CreateHashBySalt(UserDB.User[nIndex].Salt, pPassword, strlen(pPassword), Hash);
      
            if (0 == memcmp(UserDB.User[nIndex].PassHash, Hash, HASH_SIZE))
            {
               nValid       = nIndex;
               *pPermission = UserDB.User[nIndex].dPermission;
               break;
            }
            else
            {
               /* Wrong password */
               break;
            }
         }
      }
   }      
   
   return(nValid);   
} /* WebUserCheckUserPassword */

/*************************************************************************/
/*  WebUserGetUser                                                       */
/*                                                                       */
/*  Return the User for the given index.                                 */
/*                                                                       */
/*  In    : bIndex                                                       */
/*  Out   : none                                                         */
/*  Return: NULL = invalid / otherwise user pointer                      */
/*************************************************************************/
char *WebUserGetUser (uint8_t bIndex)
{
   char *pUser = NULL;

   if (bIndex < USER_CNT)
   {
      pUser = xmalloc(XM_ID_WEB, HASH_SIZE);
      if (pUser != NULL)
      {
         pUser = xstrdup(XM_ID_WEB, UserDB.User[bIndex].User);
      } 
   }

   return(pUser);
} /* WebUserGetUser */

/*************************************************************************/
/*  WebUserSetNewPass                                                    */
/*                                                                       */
/*  Set the new password if the old one match.                           */
/*                                                                       */
/*  In    : pUser, pPassUser, pPassNew                                   */
/*  Out   : none                                                         */
/*  Return: nValid >= 0 valid / otherwise invalid                        */
/*************************************************************************/
int WebUserSetNewPass (char *pUser, char *pPassUser, char *pPassNew)
{
   int           nValid = -1;
   int           nIndex;
   static uint8_t Hash[HASH_SIZE];
   
   if ((pUser != NULL) && (pPassUser != NULL) && (pPassNew != NULL))
   {
      /* Check for valid user/password combination */
      for(nIndex = 0; nIndex < USER_CNT; nIndex++)
      {
         /* Check if user match with our "database" */
         if (0 == strcmp(UserDB.User[nIndex].User, pUser))
         {
            /* Create hash from password */
            CreateHashBySalt(UserDB.User[nIndex].Salt, pPassUser, strlen(pPassUser), Hash);
      
            if (0 == memcmp(UserDB.User[nIndex].PassHash, Hash, HASH_SIZE))
            {
               /* Check password rules */
               if (-1 == CheckPasswordRules(pPassNew))
               {
                  nValid = -2;
                  break;
               }
            
               nValid = nIndex;
               
               /* Password is valid, create the new one */
               CreateReset(UserDB.User[nIndex].Reset);
               CreateSalt(UserDB.User[nIndex].Salt);
               CreateHashBySalt(UserDB.User[nIndex].Salt, pPassNew, strlen(pPassNew), UserDB.User[nIndex].PassHash);
               
               /* Update UserDB */
               UserDBWrite();
               break;
            }
            else
            {
               /* Wrong password */
               break;
            }
         }
      }
   }
   
   /*
    * -1 = Incorrect Old Password entered
    * -2 = The password does not meet the password policy requirements
    */
   
   return(nValid);
} /* WebUserSetNewPass */

/*** EOF ***/

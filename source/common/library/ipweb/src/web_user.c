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
#include "base32.h"

#include "mbedtls/md.h"
#include "mbedtls/sha1.h"
#include "mbedtls/sha256.h"

/*=======================================================================*/
/*  Extern                                                               */
/*=======================================================================*/

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

#define SHA1_HASH_SIZE  20

#define sha1_context    mbedtls_sha1_context
#define sha1_start(_a)  mbedtls_sha1_starts_ret(_a)
#define sha1_update     mbedtls_sha1_update_ret
#define sha1_finish     mbedtls_sha1_finish_ret


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
#define TOTP_SIZE       10

typedef struct _user_
{
   uint8_t   ResetNonce[RESET_SIZE];
   uint8_t   Salt[SALT_SIZE];
   uint8_t   TOTP[TOTP_SIZE];
   char      User[USER_SIZE+1];
   uint8_t   PassHash[HASH_SIZE];
   uint8_t   Mode;
   uint32_t dPermission;
} user_t;

//typedef struct __attribute__((__packed__)) _user_db_
typedef struct _user_db_
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
/*  CheckDBUser                                                          */
/*                                                                       */
/*  Check if the given user exist in the DB.                             */
/*                                                                       */
/*  In    : pUser                                                        */
/*  Out   : none                                                         */
/*  Return: NULL = ERROR / pDBUser                                       */
/*************************************************************************/
static user_t *CheckDBUser (char *pUser)
{
   user_t *pDBUser = NULL;
   int     nIndex;

   if (pUser != NULL)
   {      
      /* Check if the given user exist */
      for(nIndex = 0; nIndex < USER_CNT; nIndex++)
      {
         /* Check if user match with our "database" */
         if (0 == strcasecmp(UserDB.User[nIndex].User, pUser))
         {
            pDBUser = &UserDB.User[nIndex];
            break;
         }
      }
   }
   
   return(pDBUser);
} /* CheckUser */

/*************************************************************************/
/*  CalcTOTPValue                                                        */
/*                                                                       */
/*  Based on RFC4226 (HOTP: An HMAC-Based One-Time Password Algorithm)   */
/*                                                                       */
/*  In    : pKey, KeySize, dUnixtime                                     */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static uint32_t CalcTOTPValue (uint8_t *pKey, size_t KeySize, uint32_t dUnixtime)
{
   uint32_t            dValue;
   uint64_t             Counter; /* 8-byte counter value, the moving factor */
   mbedtls_md_context_t ctx;
   mbedtls_md_type_t    md_type = MBEDTLS_MD_SHA1;
   uint8_t              hash[SHA1_HASH_SIZE];
   uint8_t              offset;
   uint32_t             bin_code;
   
   /*
    * Calculate counter first, for a window of 30 seconds
    */
   Counter = (dUnixtime / 30);

   /* 
    * Counter must in big-endian format.
    * This took some time to figure out, Google can help :)
    * Take a look here: https://github.com/fmount/c_otp/blob/master/src/rfc4226.c
    */
   Counter = (uint64_t)htonl((uint32_t)Counter) << 32;
   
   /*
    * Step 1: Generate an HMAC-SHA-1 value Let HS = HMAC-SHA-1(K,C)
    * HS is a 20-byte string
    */
   mbedtls_md_init(&ctx);   
   mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 1);   
	
   mbedtls_md_hmac_starts(&ctx, pKey, KeySize);   
   mbedtls_md_hmac_update(&ctx, (const unsigned char *)&Counter, sizeof(Counter));   
   mbedtls_md_hmac_finish(&ctx, hash); 

   mbedtls_md_free(&ctx);   

   /*
    * Step 2: Generate a 4-byte string (Dynamic Truncation)
    */
   offset   = hash[19] & 0x0f;
   bin_code = ((hash[offset]   & 0x7f) << 24) |
              ((hash[offset+1] & 0xff) << 16) |
              ((hash[offset+2] & 0xff) <<  8) |
               (hash[offset+3] & 0xff);

   /*
    * Step 3: Compute an HOTP value
    */ 
   dValue = bin_code % 1000000;              
   
   return(dValue);
} /* CalcTOTPValue */

/*************************************************************************/
/*  CalcTOTPSecretBase32                                                 */
/*                                                                       */
/*  Based on RFC4226 (HOTP: An HMAC-Based One-Time Password Algorithm)   */
/*                                                                       */
/*  In    : pKey, KeySize, pBase32, Base32Size                           */
/*  Out   : none                                                         */
/*  Return: -1 = error / count of base32 characters                      */
/*************************************************************************/
static int CalcTOTPSecretBase32 (uint8_t *pKey, size_t KeySize, char *pBase32, size_t Base32Size)
{
   int rc = -1;
   
   /*
    * KeySize should be a multiple of 5
    * Base32Size will be (KeySize/5) * 8.
    */
   if (0 == (KeySize % 5))
   {
     rc = base32_encode(pKey, (int)KeySize, (uint8_t*)pBase32, (int)Base32Size);
   
     if (((rc / 8) * 5) != (int)KeySize)
     {
        rc = -1;
     }
   }     
   
   return(rc);
} /* CalcTOTPSecretBase32 */  

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
/*  CreateTOTP                                                           */
/*                                                                       */
/*  In    : pTOTP                                                        */
/*  Out   : pTOTP                                                        */
/*  Return: none                                                         */
/*************************************************************************/
static void CreateTOTP (uint8_t *pTOTP)
{
   tal_CPURngHardwarePoll(pTOTP, TOTP_SIZE);

} /* CreateTOTP */

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
/*  CreateResetCodeByRequest                                             */
/*                                                                       */
/*  Create the reset code for the given request nonce.                   */
/*                                                                       */
/*  In    : pRequest, RequestSize, pResetCode, ResetSize                 */
/*  Out   : pResetCode                                                   */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
static int CreateResetCodeByRequest (char *pRequest, size_t RequestSize, char *pResetCode, size_t ResetSize, uint8_t *pResetNonce)
{
   int rc = -1;
   
   /* 
    * Do some magic with the nonce to create the reset code. 
    *
    * The format of the request code will look like:   XXXX-XXXX-XXXX-XXXX
    * And the format of the reset code will lokk like: XXXXXXXX
    *
    * Input:  Request code
    * Output: Reset code
    */
   if( (pRequest   != NULL) && (RequestSize != 0)  &&
       (pResetCode != NULL) && (ResetSize   >= 17) )
   {
      if (pResetNonce != NULL)
      {
         rc = 0;
      
         /*
          * The magic code is your secret. Here I am just 
          * using the pResetNonce in reverse order as an example.
          * Normally, the request code should be used to generate the
          * reset code. Maybe with the help of a hash?
          */
         snprintf(pResetCode, ResetSize, "%02X%02X%02X%02X%02X%02X%02X%02X",
                  pResetNonce[7], pResetNonce[6], pResetNonce[5], pResetNonce[4],
                  pResetNonce[3], pResetNonce[2], pResetNonce[1], pResetNonce[0]);
      }                  
   }
   
   return(rc);
} /* CreateResetCodeByRequest */

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
      CreateReset(UserDB.User[USER_ADMIN].ResetNonce);
      CreateSalt(UserDB.User[USER_ADMIN].Salt);
      sprintf(UserDB.User[USER_ADMIN].User, "admin");
      UserDB.User[USER_ADMIN].Mode = USER_MODE_NORMAL;
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
   /* Check parameters */
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

   /* Check parameters */
   if ( (pUser != NULL) && (pPassword != NULL) && (pPermission != NULL) )
   {   
      *pPermission = 0;
      
      /* Check for valid user/password combination */
      for(nIndex = 0; nIndex < USER_CNT; nIndex++)
      {
         /* Check if user match with our "database" */
         if (0 == strcasecmp(UserDB.User[nIndex].User, pUser))
         {
            /* Check If TOTP is disabled, must be disabled */
            if (0 == (USER_MODE_TOTP & UserDB.User[nIndex].Mode))
            {
               /* Create hash from password */
               CreateHashBySalt(UserDB.User[nIndex].Salt, pPassword, strlen(pPassword), Hash);
      
               /* Check "password" */   
               if (0 == memcmp(UserDB.User[nIndex].PassHash, Hash, HASH_SIZE))
               {
                  nValid       = nIndex;
                  *pPermission = UserDB.User[nIndex].dPermission;
               }
               else
               {
                  /* Wrong password */
                  nValid = -1;
               }
            }
            else
            {
               /* Error, TOTP is enabled, must used WebUserCheckUserPasswordTOTP instead */
               nValid = -1;
            }
            break;
         } /* end if (0 == strcasecmp(UserDB.User[nIndex].User, pUser)) */
      } /* end for(nIndex = 0; nIndex < USER_CNT; nIndex++) */
   } /* end if ( (pUser != NULL) && (pPassword != NULL) && (pPermission != NULL) ) */      
   
   return(nValid);   
} /* WebUserCheckUserPassword */

/*************************************************************************/
/*  WebUserCheckUserPasswordTOTP                                         */
/*                                                                       */
/*  Check if User/Password/Code is valid.                                */
/*                                                                       */
/*  In    : pUser, pPassword, pPermission                                */
/*  Out   : pPermission                                                  */
/*  Return: -1 == invalid / user index                                   */
/*************************************************************************/
int WebUserCheckUserPasswordTOTP (char *pUser, char *pPassword, uint32_t *pPermission, uint32_t dCode)
{
   int           nValid = -1;
   int           nIndex;
   uint32_t      dValue;
   static uint8_t Hash[HASH_SIZE];

   /* Check parameters */
   if ( (pUser != NULL) && (pPassword != NULL) && (pPermission != NULL) )
   {   
      *pPermission = 0;
      
      /* Check for valid user/password combination */
      for(nIndex = 0; nIndex < USER_CNT; nIndex++)
      {
         /* Check if user match with our "database" */
         if (0 == strcasecmp(UserDB.User[nIndex].User, pUser))
         {
            /* Check If TOTP is enabled. must be enabled */
            if (USER_MODE_TOTP & UserDB.User[nIndex].Mode)
            {
               /* Create hash from password */
               CreateHashBySalt(UserDB.User[nIndex].Salt, pPassword, strlen(pPassword), Hash);

               /* Check user/password */      
               if (0 == memcmp(UserDB.User[nIndex].PassHash, Hash, HASH_SIZE))
               {
                  /* User and password are valid, check TOTP now */
               
                  /* Check actual time */               
                  dValue = CalcTOTPValue(UserDB.User[nIndex].TOTP, TOTP_SIZE, OS_UnixtimeGet());
                  if (dValue == dCode)
                  {
                     /* Code is valid */
                     nValid       = nIndex;
                     *pPermission = UserDB.User[nIndex].dPermission;
                  }
                  else
                  {
                     /* Ups not valid, check 30 seconds before */
                     dValue = CalcTOTPValue(UserDB.User[nIndex].TOTP, TOTP_SIZE, OS_UnixtimeGet()-30);
                     if (dValue == dCode)
                     {
                        /* Code is valid */
                        nValid       = nIndex;
                        *pPermission = UserDB.User[nIndex].dPermission;
                     }
                  }
               }
               else
               {
                  /* Error, wrong password */
                  nValid = -1;
               }
            }
            else
            {
               /* Error, TOTP is disabled, must used WebUserCheckUserPassword instead */
               nValid = -1;
            }
            break;
         } /* end if (0 == strcasecmp(UserDB.User[nIndex].User, pUser)) */
      } /* end for(nIndex = 0; nIndex < USER_CNT; nIndex++) */
   } /* end if ( (pUser != NULL) && (pPassword != NULL) && (pPermission != NULL) ) */
   
   return(nValid);   
} /* WebUserCheckUserPasswordTOTP */

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

   /* Check parameters */
   if (bIndex < USER_CNT)
   {
      pUser = xstrdup(XM_ID_WEB, UserDB.User[bIndex].User);
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

   /* Check parameters */
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
               CreateReset(UserDB.User[nIndex].ResetNonce);
               CreateSalt(UserDB.User[nIndex].Salt);
               CreateTOTP(UserDB.User[nIndex].TOTP);
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

/*************************************************************************/
/*  WebUserIsValid                                                       */
/*                                                                       */
/*  Check if the given user exist.                                       */
/*                                                                       */
/*  In    : pUser, pMode                                                 */
/*  Out   : pMode                                                        */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
int WebUserIsValid (char *pUser, uint8_t *pMode)
{
   int      rc = -1;
   user_t *pDBUser = NULL;

   /* Check parameters */
   if (pUser != NULL)
   {      
      /* Check if the given user exist */
      pDBUser = CheckDBUser(pUser);
      if (pDBUser != NULL)
      {
         rc = 0;
         if (pMode != NULL)
         {
            *pMode = pDBUser->Mode;
         }
      }
   }
   
   return(rc);
} /* WebUserIsValid */

/*************************************************************************/
/*  WebUserGetResetRequestCode                                           */
/*                                                                       */
/*  Return the password reset request code for the given user.           */
/*                                                                       */
/*  In    : pUser, pBuffer, Size                                         */
/*  Out   : pBuffer                                                      */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
int WebUserGetResetRequestCode (char *pUser, char *pBuffer, size_t Size)
{
   int      rc = -1;
   user_t *pDBUser = NULL;
   
   /*
    * The format of the request code will look like: XXXX-XXXX-XXXX-XXXX
    */  

   /* Check parameters */
   if ((pUser != NULL) && (pBuffer != NULL) && (Size >= 20))
   {
      /* Check if the given user exist */
      pDBUser = CheckDBUser(pUser);
      if (pDBUser != NULL)
      {
         rc = 0;  /* No error, user exist */
         
         /* Use the ResetNonce of the user to create the reset request code */
         snprintf(pBuffer, Size, "%02X%02X-%02X%02X-%02X%02X-%02X%02X",
                  pDBUser->ResetNonce[0], pDBUser->ResetNonce[1],
                  pDBUser->ResetNonce[2], pDBUser->ResetNonce[3],
                  pDBUser->ResetNonce[4], pDBUser->ResetNonce[5],
                  pDBUser->ResetNonce[6], pDBUser->ResetNonce[7]);
      }
   }
   
   return(rc);
} /* WebUserGetResetRequestCode */

/*************************************************************************/
/*  WebUserGetResetCode                                                  */
/*                                                                       */
/*  Create the reset code for the given request string.                  */
/*                                                                       */
/*  Note: This function will be used for the automat which create the    */
/*        password reset code.                                           */
/*                                                                       */
/*  In    : pRequestCode, RequestSize, pReset, ResetSize                 */
/*  Out   : pReset                                                       */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
int WebUserGetResetCode (char *pRequestCode, size_t RequestSize, char *pReset, size_t ResetSize)
{
   int rc = -1;
   
   /*
    * The format of the request code will look like:   XXXX-XXXX-XXXX-XXXX
    * And the format of the reset code will lokk like: XXXXXXXX
    *
    * Input:  Request code
    * Output: Reset code
    */  
   
   if( (pRequestCode != NULL) && (RequestSize !=  0) && 
       (pReset       != NULL) && (ResetSize   >= 17) )
   {
      rc = CreateResetCodeByRequest(pRequestCode, RequestSize, pReset, ResetSize, NULL);
   }       
   
   return(rc);
} /* WebUserGetResetCode */

/*************************************************************************/
/*  WebUserPasswordReset                                                 */
/*                                                                       */
/*  Reset the password for the given user, if the reset code is valid.   */
/*                                                                       */
/*  In    : pUser, pPassNew, pResetCode                                  */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
int WebUserPasswordReset (char *pUser, char *pPassNew, char *pResetCode)
{
   int         rc = -1; /* Error, user */
   user_t    *pDBUser = NULL;
   static char UserResetCode[20];

   /* Check parameters */
   if ((pUser != NULL) && (pResetCode != NULL))
   {
      if (16 == strlen(pResetCode))
      {
         /* Check if the given user exist */
         pDBUser = CheckDBUser(pUser);
         if (pDBUser != NULL)
         {
            /* Found valid user, check password rule */
            if (-1 == CheckPasswordRules(pPassNew))
            {
               rc = -2; /* Error, wrong password rule */
            }
            else
            {
               /* Use the ResetNonce of the user to create the reset request code */
               snprintf(UserResetCode, sizeof(UserResetCode), "%02X%02X-%02X%02X-%02X%02X-%02X%02X",
                        pDBUser->ResetNonce[0], pDBUser->ResetNonce[1],
                        pDBUser->ResetNonce[2], pDBUser->ResetNonce[3],
                        pDBUser->ResetNonce[4], pDBUser->ResetNonce[5],
                        pDBUser->ResetNonce[6], pDBUser->ResetNonce[7]);
                  
               /* Create the reset code for the user */
               CreateResetCodeByRequest(UserResetCode, strlen(UserResetCode), 
                                        UserResetCode, strlen(UserResetCode), 
                                        pDBUser->ResetNonce);

               /* Now we can compare the expected user reset code with the given reset code */
               if (0 == strcasecmp(UserResetCode, pResetCode))
               {
                  /* Valid reset code */
                  rc = 0;

                  /* Reset code is valid, use the new password */
                  CreateReset(pDBUser->ResetNonce);
                  CreateSalt(pDBUser->Salt);
                  CreateHashBySalt(pDBUser->Salt, pPassNew, strlen(pPassNew), pDBUser->PassHash);
               
                  memset(pDBUser->TOTP, 0x00, TOTP_SIZE);
                  pDBUser->Mode = USER_MODE_NORMAL;
            
                  /* Update UserDB */
                  UserDBWrite();
               }
               else
               {
                  rc = -3; /* Error, wrong reset code */
               }
            }                  
         } /* end if (pDBUser != NULL) */
      }
      else
      {
         rc = -3; /* Error, wrong reset code */
      } /* end if (16 == strlen(pResetCode)) */
   } /* end if ((pUser != NULL) && (pResetCode != NULL)) */
   
   return(rc);
} /* WebUserPasswordReset */

/*************************************************************************/
/*  WebUserIsTOTP                                                        */
/*                                                                       */
/*  Return the info if TOTP is enabled or not.                           */
/*                                                                       */
/*  In    : pUser, pBase32, Base32Size                                   */
/*  Out   : none                                                         */
/*  Return: 1 = enabled / 0 = not                                        */
/*************************************************************************/
int WebUserIsTOTP (char *pUser)
{
   int     nEnabled = 0;
   user_t *pDBUser = NULL;

   /* Check parameters */
   if (pUser != NULL)
   {      
      /* Check if the given user exist */
      pDBUser = CheckDBUser(pUser);
      if (pDBUser != NULL)
      {
         if (USER_MODE_TOTP & pDBUser->Mode)
         {
            nEnabled = 1;
         }
      }
   }
   
   return(nEnabled);
} /* WebUserIsTOTP */

/*************************************************************************/
/*  WebUserGetTOTPSecretBase32                                           */
/*                                                                       */
/*  Based on RFC4226 (HOTP: An HMAC-Based One-Time Password Algorithm)   */
/*                                                                       */
/*  In    : pUser, pBase32, Base32Size                                   */
/*  Out   : pBase32                                                      */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
int WebUserGetTOTPSecretBase32 (char *pUser, char *pBase32, size_t Base32Size)
{
   int      rc = -1;
   user_t *pDBUser = NULL;

   /* Check parameters */
   if ((pUser != NULL) && (pBase32 != NULL) && (Base32Size >= 17))
   {      
      /* Check if the given user exist */
      pDBUser = CheckDBUser(pUser);
      if (pDBUser != NULL)
      {
         /* Check If TOTP is not enabled */
         if (0 == (pDBUser->Mode & USER_MODE_TOTP))
         {
            /* Not enabled, create TOTP secret */
            CreateTOTP(pDBUser->TOTP);
            CalcTOTPSecretBase32(pDBUser->TOTP, TOTP_SIZE, pBase32, Base32Size);  
         }
         else
         {
            /* TOTP is already enabled */
            snprintf(pBase32, Base32Size, "ISALREADYENABLED");
         }
      }
   }

   return(rc);
} /* WebUserGetTOTPSecretBase32 */

/*************************************************************************/
/*  WebUserTOTPEnable                                                    */
/*                                                                       */
/*  Enable the TOTP for the given user.                                  */
/*                                                                       */
/*  In    : pUser, dValueIn                                              */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
int WebUserTOTPEnable (char *pUser, uint32_t dValueIn)
{
   int       rc = -1;
   user_t  *pDBUser = NULL;
   uint32_t dValue;

   /* Check parameters */
   if (pUser != NULL)
   {      
      /* Check if the given user exist */
      pDBUser = CheckDBUser(pUser);
      if (pDBUser != NULL)
      {
         /* Check If TOTP is not enabled */
         if (0 == (pDBUser->Mode & USER_MODE_TOTP))
         {
            dValue = CalcTOTPValue(pDBUser->TOTP, TOTP_SIZE, OS_UnixtimeGet());
            if (dValue == dValueIn)
            {
               /* Enable, code is valid */
               rc = 0;
               
               /* Update user info and DB */
               pDBUser->Mode = USER_MODE_TOTP;
               UserDBWrite();
            }
         }
         else
         {
            /* Error, TOTP is already enabled */
            rc = -1;
         }
      }
   }

   return(rc);
} /* WebUserTOTPEnable */

/*************************************************************************/
/*  WebUserTOTPDisable                                                   */
/*                                                                       */
/*  Disable the TOTP for the given user.                                 */
/*                                                                       */
/*  In    : pUser, pPass                                                 */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
int WebUserTOTPDisable (char *pUser, char *pPassword)
{
   int            rc = -1;
   user_t       *pDBUser = NULL;
   static uint8_t Hash[HASH_SIZE];

   /* Check parameters */
   if ((pUser != NULL) && (pPassword != NULL))
   {      
      /* Check if the given user exist */
      pDBUser = CheckDBUser(pUser);
      if (pDBUser != NULL)
      {
         /* Check If TOTP is enabled */
         if (pDBUser->Mode & USER_MODE_TOTP)
         {
            /* Create hash from password */
            CreateHashBySalt(pDBUser->Salt, pPassword, strlen(pPassword), Hash);

            /* Check "password" */   
            if (0 == memcmp(pDBUser->PassHash, Hash, HASH_SIZE))
            {
               /* Disable, password is valid */
               rc = 0;
               
               /* Update user info and DB */
               memset(pDBUser->TOTP, 0x00, TOTP_SIZE);
               pDBUser->Mode &= ~USER_MODE_TOTP;
               UserDBWrite();
            }
         }
         else
         {
            /* Error, TOTP is disabled */
            rc = -1;
         }
      }
   }

   return(rc);
} /* WebUserTOTPDisable */

/*************************************************************************/
/*  WebUserCheckTOTP                                                     */
/*                                                                       */
/*  Check if User/TOTPis valid.                                          */
/*                                                                       */
/*  In    : pUser, dValueIn                                              */
/*  Out   : none                                                         */
/*  Return: 0 = OK / -1 = ERROR                                          */
/*************************************************************************/
int WebUserCheckTOTP (char *pUser, uint32_t dValueIn)
{
   int       rc = -1;
   user_t  *pDBUser = NULL;
   uint32_t dValue;

   /* Check parameters */
   if (pUser != NULL)
   {      
      /* Check if the given user exist */
      pDBUser = CheckDBUser(pUser);
      if (pDBUser != NULL)
      {
         /* Check If TOTP is enabled */
         if (USER_MODE_TOTP & pDBUser->Mode)
         {
            dValue = CalcTOTPValue(pDBUser->TOTP, TOTP_SIZE, OS_UnixtimeGet());
            if (dValue == dValueIn)
            {
               /* Enable code is valid */
               rc = 0;
            }
         }
         else
         {
            /* Error, TOTP is not enabled */
            rc = -1;
         }
      }
   }

   return(rc);
} /* WebUserCheckTOTP */

/*** EOF ***/

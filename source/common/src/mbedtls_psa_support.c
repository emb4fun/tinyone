/**************************************************************************
*
*  mbedTLS/PSA crypto support.
*
*  This file contains the adaptation of the mbedTLS/PSA crypto library
*  to the TCTS RTOS and the TAL of this project:
*  - The threading ALT support (mutex and condition variable callbacks)
*  - The entropy hook mbedtls_platform_get_entropy() for the PSA random
*    generator (MBEDTLS_PSA_DRIVER_GET_ENTROPY)
*
*  History:
*
*  14.11.2026  mifi  First Version.
**************************************************************************/

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/
#include "mbedtls_psa_support.h"

#include "tcts.h"
#include "tal.h"

#include "mbedtls/threading.h"
#include "mbedtls/platform.h"

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  MbedMutexInit                                                        */
/*                                                                       */
/*  mbedTLS threading ALT support, create a new mutex.                  */
/*                                                                       */
/*  In    : pMutex                                                       */
/*  Out   : none                                                         */
/*  Return: 0 = OK / error                                               */
/*************************************************************************/
static int MbedMutexInit (mbedtls_platform_mutex_t *pMutex)
{
   /* A TCTS mutex is created in a valid state and can not fail */
   OS_MutexCreate(pMutex);

   return(0);
} /* MbedMutexInit */

/*************************************************************************/
/*  MbedMutexFree                                                        */
/*                                                                       */
/*  mbedTLS threading ALT support, free a mutex.                         */
/*                                                                       */
/*  In    : pMutex                                                       */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void MbedMutexFree (mbedtls_platform_mutex_t *pMutex)
{
   /* Nothing to do, a TCTS mutex needs no free */
   (void)pMutex;
} /* MbedMutexFree */

/*************************************************************************/
/*  MbedMutexLock                                                        */
/*                                                                       */
/*  mbedTLS threading ALT support, lock a mutex.                         */
/*                                                                       */
/*  In    : pMutex                                                       */
/*  Out   : none                                                         */
/*  Return: 0 = OK / error                                               */
/*************************************************************************/
static int MbedMutexLock (mbedtls_platform_mutex_t *pMutex)
{
   int rc;

   rc = MBEDTLS_ERR_THREADING_MUTEX_ERROR;
   if (OS_RC_OK == OS_MutexWait(pMutex, OS_WAIT_INFINITE))
   {
      rc = 0;
   }

   return(rc);
} /* MbedMutexLock */

/*************************************************************************/
/*  MbedMutexUnlock                                                      */
/*                                                                       */
/*  mbedTLS threading ALT support, unlock a mutex.                       */
/*                                                                       */
/*  In    : pMutex                                                       */
/*  Out   : none                                                         */
/*  Return: 0 = OK / error                                               */
/*************************************************************************/
static int MbedMutexUnlock (mbedtls_platform_mutex_t *pMutex)
{
   OS_MutexSignal(pMutex);

   return(0);
} /* MbedMutexUnlock */

/*************************************************************************/
/*  MbedCondInit                                                          */
/*                                                                       */
/*  mbedTLS threading ALT support, create a new condition variable.      */
/*                                                                       */
/*  In    : pCond                                                        */
/*  Out   : none                                                         */
/*  Return: 0 = OK / error                                               */
/*************************************************************************/
static int MbedCondInit (mbedtls_platform_condition_variable_t *pCond)
{
   /* Binary semaphore, no signal pending at creation time */
   OS_SemaCreate(pCond, 0, 1);

   return(0);
} /* MbedCondInit */

/*************************************************************************/
/*  MbedCondFree                                                          */
/*                                                                       */
/*  mbedTLS threading ALT support, free a condition variable.            */
/*                                                                       */
/*  In    : pCond                                                        */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void MbedCondFree (mbedtls_platform_condition_variable_t *pCond)
{
   /* Nothing to do, a TCTS semaphore needs no free */
   (void)pCond;
} /* MbedCondFree */

/*************************************************************************/
/*  MbedCondSignal                                                        */
/*                                                                       */
/*  mbedTLS threading ALT support, signal a condition variable.         */
/*                                                                       */
/*  In    : pCond                                                        */
/*  Out   : none                                                         */
/*  Return: 0 = OK / error                                               */
/*************************************************************************/
static int MbedCondSignal (mbedtls_platform_condition_variable_t *pCond)
{
   int rc = -1;

   if (OS_RC_OK == OS_SemaSignal(pCond))
   {
      rc = 0;
   }

   return(rc);
} /* MbedCondSignal */

/*************************************************************************/
/*  MbedCondBroadcast                                                     */
/*                                                                       */
/*  mbedTLS threading ALT support, broadcast a condition variable.       */
/*                                                                       */
/*  In    : pCond                                                        */
/*  Out   : none                                                         */
/*  Return: 0 = OK / error                                               */
/*************************************************************************/
static int MbedCondBroadcast (mbedtls_platform_condition_variable_t *pCond)
{
   /*
    * Note: A TCTS semaphore wakes up one waiting task per signal only.
    * For the use cases of mbedTLS here, at most one task is waiting
    * at a condition variable at the same time.
    */
   int rc = -1;

   if (OS_RC_OK == OS_SemaSignal(pCond))
   {
      rc = 0;
   }

   return(rc);
} /* MbedCondBroadcast */

/*************************************************************************/
/*  MbedCondWait                                                          */
/*                                                                       */
/*  mbedTLS threading ALT support, wait for a condition variable.        */
/*                                                                       */
/*  In    : pCond, pMutex                                                */
/*  Out   : none                                                         */
/*  Return: 0 = OK / error                                               */
/*************************************************************************/
static int MbedCondWait (mbedtls_platform_condition_variable_t *pCond,
                         mbedtls_platform_mutex_t *pMutex)
{
   int rc = -1;

   /* Unlock the mutex before the wait and lock it again afterwards */
   OS_MutexSignal(pMutex);
   if (OS_RC_OK == OS_SemaWait(pCond, OS_WAIT_INFINITE))
   {
      if (OS_RC_OK == OS_MutexWait(pMutex, OS_WAIT_INFINITE))
      {
         rc = 0;
      }
   }

   return(rc);
} /* MbedCondWait */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  mbedtls_psa_support_init                                              */
/*                                                                       */
/*  Initialize the mbedTLS/PSA crypto support (threading ALT support).   */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void mbedtls_psa_support_init (void)
{
   /*
    * Setup the mbedTLS threading support, this must be done before
    * any other mbedTLS or PSA crypto function is used.
    */
   mbedtls_threading_set_alt(MbedMutexInit, MbedMutexFree,
                             MbedMutexLock, MbedMutexUnlock,
                             MbedCondInit, MbedCondFree,
                             MbedCondSignal, MbedCondBroadcast,
                             MbedCondWait);
} /* mbedtls_psa_support_init */

/*************************************************************************/
/*  mbedtls_platform_get_entropy                                          */
/*                                                                       */
/*  This function is called by the PSA crypto library (mbedTLS 4.x)      */
/*  to get entropy from the hardware random number generator. It         */
/*  replaces mbedtls_hardware_poll() of the previous mbedTLS versions,  */
/*  see MBEDTLS_PSA_DRIVER_GET_ENTROPY in prj_psa_crypto_conf.h.         */
/*                                                                       */
/*  In    : flags, estimate_bits, output, output_size                    */
/*  Out   : estimate_bits, output                                        */
/*  Return: 0 = OK / -1 = ERROR                                           */
/*************************************************************************/
int mbedtls_platform_get_entropy (psa_driver_get_entropy_flags_t flags,
                                 size_t *estimate_bits,
                                 unsigned char *output, size_t output_size)
{
   int rc = 0;

   (void)flags;

   if (output == NULL)
   {
      rc = -1;
   }
   else
   {
      tal_CPURngHardwarePoll(output, output_size);
      *estimate_bits = (8 * output_size); /* The RNG delivers full entropy */
   }

   return(rc);
} /* mbedtls_platform_get_entropy */

/*** EOF ***/
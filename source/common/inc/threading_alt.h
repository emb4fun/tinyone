/**************************************************************************
*
*  mbedTLS MBEDTLS_THREADING_ALT configuration.
*
*  Map the mbedTLS mutex type "mbedtls_platform_mutex_t" to the
*  mutex type of the TCTS RTOS. This file will be included by
*  mbedtls/threading.h as soon MBEDTLS_THREADING_ALT is enabled,
*  see mbedtls_conf.h.
*
*  Ensure that the TCTS mutex functions (OS_MutexCreate, OS_MutexSignal
*  and OS_MutexWait) are used for the implementation, see tcts.h.
*
*  History:
*
*  14.11.2026  mifi  First Version.
**************************************************************************/
#if !defined(__THREADING_ALT_H__)
#define __THREADING_ALT_H__

/**************************************************************************
*  Includes
**************************************************************************/
#include "tcts.h"

/**************************************************************************
*  Global Definitions
**************************************************************************/

/*
 * Use the TCTS mutex as the mbedTLS mutex.
 */
typedef OS_MUTEX mbedtls_platform_mutex_t;

/*
 * Use the TCTS semaphore as the mbedTLS condition variable.
 */
typedef OS_SEMA mbedtls_platform_condition_variable_t;

#endif /* !__THREADING_ALT_H__ */

/*** EOF ***/
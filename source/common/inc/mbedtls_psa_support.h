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
#if !defined(__MBEDTLS_PSA_SUPPORT_H__)
#define __MBEDTLS_PSA_SUPPORT_H__

/**************************************************************************
*  Includes
**************************************************************************/
#include "psa/crypto_driver_random.h"

/**************************************************************************
*  Global function prototypes
**************************************************************************/

/*
 * Initialize the mbedTLS/PSA crypto support (threading ALT support).
 * This function must be called at the beginning of the start task,
 * before any other mbedTLS or PSA crypto function is used.
 */
void mbedtls_psa_support_init (void);

/*
 * Entropy hook for the PSA random generator, this function is called
 * by the crypto library.
 * Note: The function is also declared extern in the crypto configuration
 * file, see mbedtls_crypto_conf.
 */
int mbedtls_platform_get_entropy (psa_driver_get_entropy_flags_t flags,
                                 size_t *estimate_bits,
                                 unsigned char *output, size_t output_size);

#endif /* !__MBEDTLS_PSA_SUPPORT_H__ */

/*** EOF ***/
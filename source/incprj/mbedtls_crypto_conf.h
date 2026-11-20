/**************************************************************************
*
*  mbedTLS PSA crypto configuration.
*
*  This file contains the explicit PSA_WANT_* list for the PSA crypto
 *  library (TF-PSA-Crypto). Since mbedTLS 4.x it is selected via the
 *  TF_PSA_CRYPTO_CONFIG_FILE define in the SEGGER project file.
*
*  The list is derived from the usage of this project:
*  - TOTP HMAC-SHA1 and SHA-256 password hash, see web_user.c
*  - AES-256-CBC CSR encryption and PBKDF2-HMAC-SHA256 AES key
*    derivation, see elca_client.c
*  - ECC P-256 key generate/export/sign for the ELCA client
*  - TLS 1.2 and 1.3, see MBEDTLS_SSL_CIPHERSUITES in
*    prj_mbedtls_conf.h: AES-GCM/CCM, ChaCha20-Poly1305,
*    SHA-256/384 and ECDHE-ECDSA
*
*  Note: Keep in mind that only algorithms and key types can be
*  enabled here for which the corresponding mbedTLS sources are
*  part of the SEGGER project file, see prj/tiny-one.emProject.
*
*  History:
*
*  11.12.2026  mifi  First Version.
**************************************************************************/
#if !defined(__PRJ_PSA_CRYPTO_CONF_H__)
#define __PRJ_PSA_CRYPTO_CONF_H__

/**************************************************************************
*  Hash algorithms
**************************************************************************/
#define PSA_WANT_ALG_SHA_1                      1  /* TOTP (HMAC-SHA1) */
#define PSA_WANT_ALG_SHA_256                    1  /* Hashes, CSR, TLS */
#define PSA_WANT_ALG_SHA_384                    1  /* TLS AES_256_GCM_SHA384 */
#define PSA_WANT_ALG_SHA_512                    1  /* X.509 verification */

/**************************************************************************
*  MAC algorithms
**************************************************************************/
#define PSA_WANT_ALG_HMAC                       1  /* TOTP, PBKDF2 */
#define PSA_WANT_KEY_TYPE_HMAC                  1

/**************************************************************************
*  Block cipher and AEAD algorithms
**************************************************************************/
#define PSA_WANT_KEY_TYPE_AES                   1
#define PSA_WANT_ALG_CBC_NO_PADDING             1  /* ELCA CSR encryption */
#define PSA_WANT_ALG_ECB_NO_PADDING             1
#define PSA_WANT_ALG_GCM                        1  /* TLS AES-GCM suites */
#define PSA_WANT_ALG_CCM                        1  /* TLS AES-CCM suites */
#define PSA_WANT_ALG_CCM_STAR_NO_TAG            1
#define PSA_WANT_KEY_TYPE_CHACHA20              1
#define PSA_WANT_ALG_STREAM_CIPHER              1
#define PSA_WANT_ALG_CHACHA20_POLY1305          1  /* TLS ChaCha20 suites */

/**************************************************************************
*  Elliptic curve cryptography
**************************************************************************/
#define PSA_WANT_ECC_SECP_R1_256                1  /* Device key, TLS */
#define PSA_WANT_ECC_SECP_R1_384                1  /* TLS */
#define PSA_WANT_KEY_TYPE_ECC_PUBLIC_KEY        1
#define PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_BASIC    1
#define PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_IMPORT   1
#define PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_EXPORT   1
#define PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_GENERATE 1
#define PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_DERIVE   1
#define PSA_WANT_ALG_ECDSA                      1  /* Key and cert signature */
#define PSA_WANT_ALG_DETERMINISTIC_ECDSA        1
#define PSA_WANT_ALG_ECDH                       1  /* TLS ECDHE */

/**************************************************************************
*  Key derivation
**************************************************************************/
#define PSA_WANT_KEY_TYPE_DERIVE                1  /* TLS key schedule */
#define PSA_WANT_KEY_TYPE_RAW_DATA              1
#define PSA_WANT_ALG_TLS12_PRF                  1  /* TLS 1.2 */
#define PSA_WANT_ALG_TLS12_PSK_TO_MS            1
#define PSA_WANT_ALG_TLS12_ECJPAKE_TO_PMS       1
#define PSA_WANT_ALG_HKDF                       1  /* TLS 1.3 */
#define PSA_WANT_ALG_HKDF_EXTRACT               1
#define PSA_WANT_ALG_HKDF_EXPAND                1
#define PSA_WANT_ALG_PBKDF2_HMAC                1  /* ELCA AES key derivation */

/**************************************************************************
*  mbedTLS crypto module options
*
*  Note: Since mbedTLS 4.x these modules are part of the crypto library
*  (TF-PSA-Crypto) and must be configured here, see
*  mbedtls_config_check_user.h ("must be configured in psa/crypto_config.h").
*  MBEDTLS_PSA_CRYPTO_C is the master switch of the crypto library, it
*  derives MBEDTLS_PSA_CRYPTO_CLIENT and MBEDTLS_ENTROPY_C, see
*  crypto_adjust_config_support.h.
**************************************************************************/
#define MBEDTLS_PSA_CRYPTO_C
#define MBEDTLS_PSA_KEY_STORE_DYNAMIC
#define MBEDTLS_PSA_DRIVER_GET_ENTROPY   /* The app provides mbedtls_platform_get_entropy(), see mbedtls_psa_support.c */
#define MBEDTLS_CTR_DRBG_C
#define MBEDTLS_HMAC_DRBG_C
#define MBEDTLS_MD_C
#define MBEDTLS_ASN1_PARSE_C
#define MBEDTLS_ASN1_WRITE_C
#define MBEDTLS_BASE64_C
#define MBEDTLS_PEM_PARSE_C
#define MBEDTLS_PEM_WRITE_C
#define MBEDTLS_PK_C
#define MBEDTLS_PK_PARSE_C
#define MBEDTLS_PK_PARSE_EC_COMPRESSED
#define MBEDTLS_PK_PARSE_EC_EXTENDED
#define MBEDTLS_PK_WRITE_C
#define MBEDTLS_PKCS5_C
#define MBEDTLS_ECP_NIST_OPTIM

/**************************************************************************
*  mbedTLS platform and threading options
*
*  Note: Since mbedTLS 4.x these options belong to the crypto library
*  (TF-PSA-Crypto, see tf-psa-crypto/platform/ and .../include/mbedtls/
*  threading.h) and must be configured here, see
*  mbedtls_config_check_user.h ("must be configured in psa/crypto_config.h").
**************************************************************************/
#define MBEDTLS_PLATFORM_C
#define MBEDTLS_PLATFORM_MEMORY
#define MBEDTLS_PLATFORM_CALLOC_MACRO  mbedtls_calloc
#define MBEDTLS_PLATFORM_FREE_MACRO    mbedtls_free

/*
 * The memory pool is implemented by the application, see xmempool.c.
 * With MBEDTLS_PLATFORM_MEMORY and both macros above, mbedtls/platform.h
 * only maps the function names to the macros without any declaration.
 * Therefore the functions must be declared here. The declarations are
 * visible in all library and application files, because this file is
 * included by tf-psa-crypto/build_info.h as first configuration file.
 */
#include <stddef.h>
extern void *mbedtls_calloc(size_t n, size_t size);
extern void  mbedtls_free(void *ptr);
#define MBEDTLS_DEPRECATED_WARNING
#define MBEDTLS_AES_ROM_TABLES
#define MBEDTLS_THREADING_ALT
#define MBEDTLS_THREADING_C

#endif /* !__PRJ_PSA_CRYPTO_CONF_H__ */

/*** EOF ***/

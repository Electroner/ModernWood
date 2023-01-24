# Install script for directory: B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/ModernWood")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "B:/Programas/Espressif/tools/xtensa-esp32s3-elf/esp-2022r1-11.2.0/xtensa-esp32s3-elf/bin/xtensa-esp32s3-elf-objdump.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mbedtls" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ FILES
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/aes.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/aria.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/asn1.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/asn1write.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/base64.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/bignum.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/build_info.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/camellia.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/ccm.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/chacha20.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/chachapoly.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/check_config.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/cipher.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/cmac.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/compat-2.x.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/config_psa.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/constant_time.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/ctr_drbg.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/debug.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/des.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/dhm.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/ecdh.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/ecdsa.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/ecjpake.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/ecp.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/entropy.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/error.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/gcm.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/hkdf.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/hmac_drbg.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/mbedtls_config.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/md.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/md5.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/memory_buffer_alloc.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/net_sockets.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/nist_kw.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/oid.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/pem.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/pk.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/pkcs12.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/pkcs5.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/platform.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/platform_time.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/platform_util.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/poly1305.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/private_access.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/psa_util.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/ripemd160.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/rsa.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/sha1.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/sha256.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/sha512.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/ssl.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/ssl_cache.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/ssl_ciphersuites.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/ssl_cookie.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/ssl_ticket.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/threading.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/timing.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/version.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/x509.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/x509_crl.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/x509_crt.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/mbedtls/x509_csr.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/psa" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ FILES
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/psa/crypto.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/psa/crypto_builtin_composites.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/psa/crypto_builtin_primitives.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/psa/crypto_compat.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/psa/crypto_config.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/psa/crypto_driver_common.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_composites.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_primitives.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/psa/crypto_extra.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/psa/crypto_platform.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/psa/crypto_se_driver.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/psa/crypto_sizes.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/psa/crypto_struct.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/psa/crypto_types.h"
    "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/mbedtls/mbedtls/include/psa/crypto_values.h"
    )
endif()


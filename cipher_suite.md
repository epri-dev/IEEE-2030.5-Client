OpenSSL Cipher Suite
====================

The 2030.5 protocol requires the cipher suite TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8
for secure communication between client and server, however accounting for
clients and servers that may not yet support elliptic curve cryptography, the
2030.5 protocol gives provisions for optionally supporting RSA-based cipher
suites where the client and exchange RSA certificates outside of the
manufacturing PKI.

By default the EPRI library supports only TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8,
however the library used for TLS (OpenSSL) supports RSA based cipher suites as
well. To configure OpenSSL to support other cipher suites, the EPRI client
library needs to be modifed:

1. Edit openssl.c
-----------------

Change the constant `CIPHER_LIST` to include other OpenSSL supported cipher
suites. By default `CIPHER_LIST` is defined as:

    #define CIPHER_LIST "ECDHE-ECDSA-AES128-CCM8"

You can add other cipher suites by including them in `CIPHER_LIST` seperated by
commas and ordered according to preference. For a complete listing of cipher
suites supported by OpenSSL, run the command:

    openssl ciphers

This command will list the cipher suites according to their OpenSSL name.

2. Run `build.sh` to recompile
------------------------------

When you recompile, the cipher list will be updated and passed to OpenSSL when
the application is run.



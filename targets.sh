#!/bin/bash
#
# targets.sh
#
# build targets, used by build.sh

tls_lib="openssl" # choose tls library ( openssl wolfssl )

openssl_libs=( -lssl -lcrypto -lpthread -ldl )
#flags+=( -Wno-format -Wno-unused-result -Wunused-variable -Wreturn-type -Wunused-but-set-variable -Wformat -Wformat-security )

declare -n tls_libs=${tls_lib}_libs # wolfssl_libs or openssl_libs
if [[ $tls_lib == "openssl" ]]; then
    se_core_flags=( -DOPENSSL_TLS )
    command -v openssl > /dev/null
    if [ $? -eq 1 ]; then
	echo "The openssl command is not found, make sure that OpenSSL version 1.1.0 installed."
	exit 0
    fi
    version=`openssl version -a`
    if [[ ! ($version =~ "OpenSSL 1.1.0") ]]; then
	echo "The EPRI 2030.5 client requires OpenSSL version 1.1.0."
	echo "openssl version -a"
	echo $version
	exit 0
    fi
else
    se_core_flags=( -DWOLFSSL_TLS )
fi
se_core_flags+=( -fPIC )

clean_build () {
    rm -r build
}

se_objects=( se_core.o )
se_libs=( ${tls_libs[@]} )
se_targets=( client_test csip_test )
targets=( schema_gen se )

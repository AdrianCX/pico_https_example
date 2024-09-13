#!/bin/bash -ex

# This generates a new self signed certificate for testing
openssl ecparam -name prime256v1 -genkey -noout -outform DER -out ./key.der
openssl req -new -x509 -key ./key.der -outform DER -out ./cert.der -days 360

if [ -z "$1" ]; then
    TARGET_FOLDER=$(pwd)
else
    TARGET_FOLDER="$1"
fi

echo "#ifndef _CERT_H" > "$TARGET_FOLDER/cert.h"
echo "#define _CERT_H" >> "$TARGET_FOLDER/cert.h"
xxd -i ./cert.der >> "$TARGET_FOLDER/cert.h"
echo "#endif" >> "$TARGET_FOLDER/cert.h"

echo "#ifndef _KEY_H" > "$TARGET_FOLDER/key.h"
echo "#define _KEY_H" >> "$TARGET_FOLDER/key.h"
xxd -i ./key.der >> "$TARGET_FOLDER/key.h"
echo "#endif" >> "$TARGET_FOLDER/key.h"

rm -f ./cert.der ./key.der

# You can convert existing certificates from pem to der via line below then use xxd: (ec or rsa based on what you have)
# openssl ec -in ./key.pem -outform DER -out ./key.der

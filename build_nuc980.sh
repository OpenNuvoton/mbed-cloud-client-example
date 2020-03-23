#!/bin/sh

set +x 
export TOOLCHAIN_DIR=/usr/local/arm_linux_4.8/usr/
python pal-platform/pal-platform.py deploy --target=ARM_NUC980_mbedtls generate

# Patch
cp -af nuvoton/* .

cd __ARM_NUC980_mbedtls
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=./../pal-platform/Toolchain/GCC-NUC980/GCC-NUC980.cmake -DEXTERNAL_DEFINE_FILE=./../define.txt
make mbedCloudClientExample.elf

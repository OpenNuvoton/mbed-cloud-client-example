#!/bin/sh

set +x 

pip2 install virtualenv

mkdir -p ~/virtualenvs

virtualenv ~/virtualenvs/nk-980iot

~/virtualenvs/nk-980iot/bin/activate

mbed deploy

exit 0

python pal-platform/pal-platform.py deploy --target=ARM_NUC980_mbedtls generate

cd __ARM_NUC980_mbedtls

export TOOLCHAIN_DIR=/usr/local/arm_linux_4.8/usr/

cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=./../pal-platform/Toolchain/GCC-NUC980/GCC-NUC980.cmake -DEXTERNAL_DEFINE_FILE=./../define.txt

make mbedCloudClientExample.elf

cp -af Debug/mbedCloudClientExample.elf.elf ../../nuc980bsp/rootfs/mnt/mbed-cloud/mbedCloudClientExample.elf
cd ../../nuc980bsp/linux-4.4.x
make uImage
mv ../image/980uimage ../image/980uimage.bin

#python pal-platform/pal-platform.py fullbuild --target ARM_NUC980_mbedtls --toolchain GCC-NUC980 --external ./../define-nuc980.txt --name mbedCloudClientExample.elf
#python pal-platform/pal-platform.py fullbuild --target ARM_NUC980_mbedtls --toolchain GCC-NUC980 --external ./../define.txt --name mbedCloudClientExample.elf

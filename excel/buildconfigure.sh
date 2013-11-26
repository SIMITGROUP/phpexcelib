#!/bin/bash
MACOSX_DEPLOYMENT_TARGET=10.9
CFLAGS="-arch x86_64 -g -Os -pipe -no-cpp-precomp"
CCFLAGS=" -arch x86_64 -g -Os -pipe"
CXXFLAGS=" -arch x86_64 -g -Os -pipe"
LDFLAGS=" -arch x86_64 -bind_at_load"
export CFLAGS CXXFLAGS LDFLAGS CCFLAGS MACOSX_DEPLOYMENT_TARGET
echo $CFLAGS
./configure --with-php-config=/Applications/XAMPP/xamppfiles/bin/php-config
make clean

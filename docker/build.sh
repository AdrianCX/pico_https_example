#!/bin/bash -ex

if [ ! -d "/source/build/picotool" ]; then
    mkdir -p /source/build/picotool
fi

if [ ! -d "/source/build/docker" ]; then
    mkdir -p /source/build/docker
fi

if [ ! -d "/source/build/test" ]; then
    mkdir -p /source/build/test
fi

cd /source/build/picotool
cmake -DPICO_SDK_PATH=/source/pico_https/pico-sdk/ /source/pico_https/picotool/
make
make install

cd /source/build/docker
cmake -DCMAKE_BUILD_TYPE=Release VERBOSE=1 /source/
make

cd /source/build/test
cmake -DCMAKE_BUILD_TYPE=Release VERBOSE=1 /source/pico_https/test/
make
ctest --verbose
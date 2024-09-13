#!/bin/bash -ex

pushd $(dirname "$0")

#if [ -z "$(docker images -q local_build_pico_https 2> /dev/null)" ]; then
    pushd docker
    docker build -t local_build_pico_https .
    popd
#fi

docker run -v "$(pwd)":/source/ -it local_build_pico_https
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y cmake gcc-arm-none-eabi libnewlib-arm-none-eabi xxd python3 g++ bash

COPY ./build.sh /build.sh

ENTRYPOINT ["/bin/bash", "-c", "/build.sh"]


FROM ubuntu:18.04

# COPY bldc from https://github.com/vedderb/bldc
COPY bldc /root/

WORKDIR /root
CMD make

# Always need to "apt-get update" after editing the sources or before package install
RUN apt-get update 

RUN apt-get install -y software-properties-common
RUN DEBIAN_FRONTEND=noninteractive add-apt-repository ppa:team-gcc-arm-embedded/ppa
RUN apt-get update 

# Convenience packages for working within the container's shell
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y \
        build-essential vim aptitude less 

RUN DEBIAN_FRONTEND=noninteractive apt-get install -y gcc-arm-embedded


FROM ubuntu:18.04
#FROM linuxcontainers/debian-slim:latest

# Always need to "apt-get update" after editing the sources or before package install
RUN apt-get update 
RUN apt-get install -y software-properties-common git wget

RUN DEBIAN_FRONTEND=noninteractive add-apt-repository ppa:team-gcc-arm-embedded/ppa
RUN apt-get update 

RUN DEBIAN_FRONTEND=noninteractive apt-get install -y build-essential vim nano aptitude less python
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y gcc-arm-embedded 

COPY get_vesc.sh /

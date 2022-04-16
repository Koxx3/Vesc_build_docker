FROM ubuntu:18.04

# Always need to "apt-get update" after editing the sources or before package install
RUN DEBIAN_FRONTEND=noninteractive add-apt-repository ppa:team-gcc-arm-embedded/ppa
RUN apt-get update 
RUN apt-get install -y software-properties-common git wget
RUN apt-get install -y build-essential vim nano aptitude less python
RUN apt-get install -y gcc-arm-embedded 

COPY get_vesc.sh /

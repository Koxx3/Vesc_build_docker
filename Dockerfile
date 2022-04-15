FROM ubuntu:18.04
#FROM linuxcontainers/debian-slim:latest

# Always need to "apt-get update" after editing the sources or before package install
RUN apt-get update 
RUN apt-get install -y software-properties-common git
RUN apt-get install -y software-properties-common unzip wget

RUN DEBIAN_FRONTEND=noninteractive add-apt-repository ppa:team-gcc-arm-embedded/ppa
RUN apt-get update 

RUN DEBIAN_FRONTEND=noninteractive apt-get install -y build-essential vim aptitude less 
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y gcc-arm-embedded

RUN git clone  --verbose  https://github.com/vedderb/bldc /bldc
#COPY bldc from https://github.com/vedderb/bldc

RUN wget https://github.com/vedderb/bldc/archive/refs/heads/master.zip
RUN unzip /master.zip
RUN pwd
RUN ls -altr

RUN cd /bldc-master && make arm_sdk_install

WORKDIR /bldc-master
CMD make fw_100_250 && mv /bldc/build/BLDC_4_ChibiOS.bin /bldc/

FROM ubuntu:18.04
#FROM linuxcontainers/debian-slim:latest

# Always need to "apt-get update" after editing the sources or before package install
RUN apt-get update 
RUN apt-get install -y software-properties-common git

RUN DEBIAN_FRONTEND=noninteractive add-apt-repository ppa:team-gcc-arm-embedded/ppa
RUN apt-get update 

RUN DEBIAN_FRONTEND=noninteractive apt-get install -y build-essential vim nano aptitude less gcc-arm-embedded
#RUN DEBIAN_FRONTEND=noninteractive apt-get install -y gcc-arm-embedded 

RUN git clone https://github.com/vedderb/bldc /bldc-master

RUN git clone https://github.com/JohnSpintend/bldc /bldc-spintend

RUN git clone https://github.com/vedderb/bldc-bootloader /bldc-bootloader

RUN cp /bldc-spintend/hwconf/hw_ubox* /bldc-master/hwconf

RUN cd /bldc-master && make arm_sdk_install

WORKDIR /bldc-master
#CMD make fw_100_250 && mv /bldc/build/BLDC_4_ChibiOS.bin /bldc/

FROM ubuntu:18.04

RUN apt-get update 
RUN apt-get install -y software-properties-common git wget
RUN add-apt-repository ppa:team-gcc-arm-embedded/ppa
RUN apt-get update 

RUN apt-get install -y software-properties-common git wget
RUN apt-get install -y build-essential vim nano aptitude less python
RUN apt-get install -y gcc-arm-embedded 

COPY get_vesc.sh /

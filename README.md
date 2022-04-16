# Docker

## Install docker on Linux
```
sh ./install_docker.sh
``` 

## Install docker on Windows

https://docs.docker.com/desktop/windows/install/

# Build image

## Build VESC on Linux
```
./build.sh
``` 

# Run image

## Build VESC on Linux
```
 ./run.sh
``` 

## Once image is built & launched / in image bash

```
./get_vesc.sh
cd bldc-master && make uboxv2_100_100
cd bldc-503 && make
``` 
File is available in "build" folder

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
sh ./build.sh
``` 

## Build VESC on Windows
```
sh ./build.bat
``` 

# Run image

## Build VESC on Linux
```
sh ./run.sh
``` 

## Build VESC on Windows
```
sh ./run.bat
``` 

## Once image is built & launched / in image bash

```
./get_vesc.sh
cd bldc-master && make uboxv2_100_100
``` 
File is available in "build" folder

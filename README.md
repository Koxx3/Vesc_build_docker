# Docker

## Install docker on Linux
```
sh ./install_docker.sh'
``` 

## Install docker on Windows

https://docs.docker.com/desktop/windows/install/

# Build

## Build VESC on Linux
```
sh ./build.sh'
``` 

## Build VESC on Windows
```
sh ./build.bat
``` 

## Once image is build / in image shell

```
make 100_250 && mv /bldc-master/build/100_250/100_250.bin /bldc/
``` 
File is available in "bldc" folder

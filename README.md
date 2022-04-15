# Docker

## Install docker on Linux
```
sh ./install_docker.sh
``` 

## Install docker on Windows

https://docs.docker.com/desktop/windows/install/

# Build

## Build VESC on Linux
```
sh ./build.sh
``` 

## Build VESC on Windows
```
sh ./build.bat
``` 

## Once image is built & launched / in image bash

```
make uboxv2_100_100 && find . -name *.bin | xargs -n 1 -I {} cp {} /bldc/
``` 
File is available in "bldc" folder

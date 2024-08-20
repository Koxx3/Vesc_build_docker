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

## For vesc HW selection in 5.03 FW
``` 
nano /bldc-503/conf_general.h 
``` 
 
## Once image is built & launched / in image bash

```
./get_vesc.sh
cd /bldc-master && make uboxv2_100_100
cd /bldc-503 && make
``` 
File is available in "build" folder

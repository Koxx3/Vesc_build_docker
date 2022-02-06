# David_vesc

## Install docker
'curl -fsSL https://get.docker.com -o get-docker.sh
sh ./get-docker.sh'
 
 
## Build GigaVESC
git clone https://github.com/Koxx3/David_vesc.git
cd David_vesc
docker build -t vesc_fw_503 .
docker run --rm -it -v $(pwd)/bldc:/bldc vesc_fw_cont

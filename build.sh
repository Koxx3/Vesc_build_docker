docker build -t vesc_fw .
#docker run --rm -it -v $(pwd)/bldc:/bldc vesc_fw /bin/bash -c "make fw_$1 && mv /bldc-master/build/$1/$1.bin /bldc/"
docker run --rm -it -v $(pwd)/bldc:/bldc vesc_fw bash
#mv $(pwd)/bldc/build/BLDC_4_ChibiOS.bin $(pwd)/firmware.bin


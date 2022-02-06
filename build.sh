docker build -t vesc_fw_503 .
docker run --rm -it -v $(pwd)/bldc:/bldc vesc_fw_cont
mv $(pwd)/bldc/build/BLDC_4_ChibiOS.bin $(pwd)/GigaVesc.bin


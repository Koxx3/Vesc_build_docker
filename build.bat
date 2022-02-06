docker build -t vesc_fw_503
docker run --name "build_gigavesc" --rm -it -v ${pwd}\bldc:/bldc vesc_fw_503
mv bldc/build/BLDC_4_ChibiOS.bin GigaVesc.bin

pause


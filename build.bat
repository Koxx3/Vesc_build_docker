docker build -t vesc_fw
docker run --name "build_gigavesc" --rm -it -v ${pwd}\bldc:/bldc vesc_fw bash

pause


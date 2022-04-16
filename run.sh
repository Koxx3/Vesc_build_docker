docker run --name vesc_fw --rm -it \
-v $(pwd)/bldc:/bldc \
-v $(pwd)/bldc-master:/bldc-master \
 vesc_fw bash


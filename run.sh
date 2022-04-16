docker run --name vesc_fw --rm -it \
-v $(pwd)/bldc-503:/bldc-503 \
-v $(pwd)/bldc-master:/bldc-master \
 vesc_fw bash


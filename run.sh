docker run --name vesc_fw --rm -it \
-v $(pwd)/bldc-503:/bldc-503 \
-v $(pwd)/bldc-605:/bldc-605 \
-v $(pwd)/other_hw:/other_hw \
 vesc_fw bash


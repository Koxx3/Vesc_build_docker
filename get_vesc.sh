git clone -b release_6_05 https://github.com/vedderb/bldc /bldc-605
git clone -b release_5_03 https://github.com/vedderb/bldc /bldc-503
# git clone https://github.com/JohnSpintend/bldc /bldc-spintend
# git clone https://github.com/vedderb/bldc-bootloader /bldc-bootloader
# cp /bldc-spintend/hwconf/hw_ubox* /bldc-master/hwconf
# cp /bldc-spintend/hwconf/hw_ubox* /bldc-503/hwconf

# cp /other_hw/hw_micro* /bldc-master/hwconf
# cp /other_hw/hw_micro* /bldc-503/hwconf

# sed -i '1s/^/#define UBOX_V2_100V_250D\n/' /bldc-503/hwconf/hw_uboxv2_100_100.h
# sed -i '1s/^/#define UBOX_V2_100V_250D\n/' /bldc-master/hwconf/hw_uboxv2_100_100.h

cd /bldc-605
make arm_sdk_install

git clone https://github.com/vedderb/bldc /bldc-master
git clone https://github.com/JohnSpintend/bldc /bldc-spintend
git clone https://github.com/vedderb/bldc-bootloader /bldc-bootloader
cp /bldc-spintend/hwconf/hw_ubox* /bldc-master/hwconf
sed -i '1s/^/#define UBOX_V2_100V_250D\n/' /bldc-master/hwconf/hw_uboxv2_100_100.h



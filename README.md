# u-boot-2016.07
for mc210

make distclean
make mc210_defconfig
make CROSS_COMPILE=arm-buildroot-linux-gnueabihf- V=1
make CROSS_COMPILE=arm-buildroot-linux-gnueabihf- spl/u-boot-spl.bin

fdisk -l
dd bs=512 seek=1 if=/dev/zero of=/dev/sdc count=2047
dd iflag=dsync oflag=dsync if=spl/mc210-spl.bin of=/dev/sdc seek=1
dd iflag=dsync oflag=dsync if=u-boot.bin of=/dev/sdc seek=49

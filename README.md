# u-boot-2016.07
for mc210

make distclean
make mc210_defconfig
make CROSS_COMPILE=arm-buildroot-linux-gnueabihf- V=1

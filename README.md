# u-boot-2016.07
for mc210/tq210

make distclean
make mc210_defconfig/tq210_defconfig
make

sudo fdisk -l
sudo dd bs=512 seek=1 if=/dev/zero of=/dev/sde count=2047
sudo dd bs=512 seek=1 iflag=dsync oflag=dsync if=u-boot-with-spl.bin of=/dev/sde

mmc erase 0x21 0x10

# linux-4.4
for mc210

make ARCH=arm mc210_defconfig
make ARCH=arm CROSS_COMPILE=arm-buildroot-linux-gnueabihf- menuconfig
make ARCH=arm CROSS_COMPILE=arm-buildroot-linux-gnueabihf- zImage
make ARCH=arm CROSS_COMPILE=arm-buildroot-linux-gnueabihf- LOADADDR=0x20008000 uImage
make ARCH=arm CROSS_COMPILE=arm-buildroot-linux-gnueabihf- s5pv210-mc210.dtb

tftpboot 20008000 uImage; tftpboot 20508000 s5pv210-mc210.dtb; bootm 20008000 - 20508000
fatload mmc 0:1 20008000 uImage
fatload mmc 0:1 20508000 s5pv210-mc210.dtb
bootm 20008000 - 20508000

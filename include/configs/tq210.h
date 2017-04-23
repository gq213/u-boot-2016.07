/*
 * Copyright (C) 2009 Samsung Electronics
 * Minkyu Kang <mk7.kang@samsung.com>
 * Kyungmin Park <kyungmin.park@samsung.com>
 *
 * Configuation settings for the SAMSUNG Universal (s5pc100) board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H


#include <linux/sizes.h>
#include <asm/arch/cpu.h>		/* get chip and board defs */
/* u-boot-2016.07/include/config_defaults.h */
/*#define DEBUG*/


/*
 * High Level Configuration Options
 */
#define CONFIG_SAMSUNG		1	/* in a SAMSUNG core */
#define CONFIG_S5P		1	/* which is in a S5P Family */
#define CONFIG_S5PC110		1	/* which is in a S5PC110 */
#define CONFIG_MACH_TQ210	1	/* working with tq210 */


/*
 * Low Level (hardware related)
 */
#define CONFIG_SYS_CACHELINE_SIZE	64	/* Cache Line Size of the CPU */

/* input clock of PLL: has 24MHz input clock at S5PC110 */
#define CONFIG_SYS_CLK_FREQ_C110	24000000

/* DRAM Base */
#define CONFIG_SYS_SDRAM_BASE		0x20000000

/* tq210 has 2 banks of DRAM, but swap the bank */
#define CONFIG_NR_DRAM_BANKS	2
#define PHYS_SDRAM_1		0x20000000	/* DDR2 Bank #0 */
#define PHYS_SDRAM_1_SIZE	SZ_512M		/* 512 MB in Bank #0 */
#define PHYS_SDRAM_2		0x40000000	/* DDR2 Bank #1 */
#define PHYS_SDRAM_2_SIZE	SZ_512M		/* 512 MB in Bank #1 */

/* This option should be defined if the board does not have parallel flash. */
#define CONFIG_SYS_NO_FLASH	1


/*
 * Board initialization settings
 */
#define CONFIG_SYS_LONGHELP		/* undef to save memory */
#define CONFIG_SYS_CBSIZE	256	/* Console I/O Buffer Size */
#define CONFIG_SYS_PBSIZE	384	/* Print Buffer Size */
#define CONFIG_SYS_MAXARGS	16	/* max number of command args */
#define CONFIG_SYS_BARGSIZE	CONFIG_SYS_CBSIZE	/* Boot Argument Buffer Size */

/* stdin, stdout and stderr are in evironment */
#define CONFIG_SYS_CONSOLE_IS_IN_ENV

#define CONFIG_CMDLINE_EDITING
#define CONFIG_AUTO_COMPLETE

#define CONFIG_ARCH_CPU_INIT
#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

#define CONFIG_MISC_COMMON
#define CONFIG_MISC_INIT_R
#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG


/*
 * Default Environment
 */
#define CONFIG_BOOTARGS		"noinitrd console=ttySAC0 root=/dev/mmcblk0p2 rw " \
				"rootfstype=ext4 rootwait init=/linuxrc"
#define CONFIG_BOOTCOMMAND	"fatload mmc 0:1 20008000 uImage; bootm 20008000"
#define CONFIG_BAUDRATE		115200
#define CONFIG_ENV_VARS_UBOOT_CONFIG
#define CONFIG_EXTRA_ENV_SETTINGS \
	"bootk=" \
		"tftpboot 20008000 uImage; " \
		"bootm 20008000\0" \
	"nfsboot=" \
		"set bootargs noinitrd console=tty0 console=ttySAC0 root=/dev/nfs rw nfsroot=192.168.169.201:/home/work/nfs/rootfs,v3,nolock,tcp ip=dhcp init=/linuxrc coherent_pool=2M; " \
		"run bootk\0"

#define CONFIG_ENV_SIZE		SZ_8K

/* FLASH and environment organization */
#define CONFIG_ENV_IS_IN_MMC
#define CONFIG_SYS_MMC_ENV_DEV		0
#define CONFIG_ENV_OFFSET		(512 + SZ_16K)	/*16 K reserved for BL1*/


/*
 * Run settings
 */
#define CONFIG_PWM			1		/* PWM */

/* Size of malloc() pool before and after relocation */
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + SZ_4M)

#define CONFIG_SYS_TEXT_BASE		0x24800000

#define CONFIG_SYS_LOAD_ADDR		0x24000000	/* 8m */
#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_LOAD_ADDR - 0x1000000)	/* 48m */

#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_CMDLINE_TAG
#define CONFIG_INITRD_TAG


/* MMC */
#define CONFIG_GENERIC_MMC
#define CONFIG_MMC
#define CONFIG_SDHCI
#define CONFIG_S5P_SDHCI
#define SDHCI_MAX_HOSTS		4


/*
 * Partition and FileSystem
 */
#define CONFIG_DOS_PARTITION

#define CONFIG_FS_FAT
#define CONFIG_FAT_WRITE
#define CONFIG_SUPPORT_VFAT

#define CONFIG_FS_EXT4
#define CONFIG_EXT4_WRITE


/*
 * NetWork
 */
#define CONFIG_DRIVER_DM9000
#define CONFIG_DM9000_BASE	0x88000000
#define DM9000_IO		CONFIG_DM9000_BASE
#define DM9000_DATA		(CONFIG_DM9000_BASE + 0x4)
#define CONFIG_DM9000_NO_SROM
/*#define CONFIG_DM9000_DEBUG*/

#define CONFIG_NETMASK		255.255.255.0
#define CONFIG_IPADDR		192.168.169.202
#define CONFIG_SERVERIP		192.168.169.201
#define CONFIG_GATEWAYIP	192.168.169.1


/*
 * SPL Settings
 */
#define CONFIG_SPL_LDSCRIPT		"arch/arm/mach-s5pc1xx/u-boot-spl.lds"
#define CONFIG_SPL_MAX_FOOTPRINT	(16 * 1024)
#define CONFIG_VAR_SIZE_SPL

#define CONFIG_SPL_TEXT_BASE 		0xD0020010
#define CONFIG_SPL_STACK		0xD0037D80

#define CONFIG_POWER_HOLD

#endif	/* __CONFIG_H */

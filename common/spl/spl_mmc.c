/*
 * (C) Copyright 2010
 * Texas Instruments, <www.ti.com>
 *
 * Aneesh V <aneesh@ti.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <dm.h>
#include <spl.h>
#include <linux/compiler.h>
#include <errno.h>
#include <asm/u-boot.h>
#include <errno.h>
#include <mmc.h>
#include <image.h>

DECLARE_GLOBAL_DATA_PTR;

typedef unsigned int(*copy_sd_mmc_to_mem)
	(unsigned int channel, unsigned int start_block, unsigned short block_size, unsigned int *dst, unsigned int with_init);

int spl_mmc_load_image(void)
{
	unsigned int sd_ch;
	unsigned int ret;

	sd_ch = *(volatile unsigned int *)(0xD0037488);
	copy_sd_mmc_to_mem copy_bl2 = (copy_sd_mmc_to_mem)(*(unsigned int *)(0xD0037F98));

	uart_puts("copy bl2 to dram...\r\n");

	if (sd_ch == 0xEB000000) {
		ret = copy_bl2(0, CONFIG_BL2_SD_BLOCK_OFFSET, CONFIG_BL2_SD_BLOCK_SIZE, (unsigned int *)CONFIG_SYS_TEXT_BASE, 0);
	} else if (sd_ch == 0xEB200000) {
		ret = copy_bl2(2, CONFIG_BL2_SD_BLOCK_OFFSET, CONFIG_BL2_SD_BLOCK_SIZE, (unsigned int *)CONFIG_SYS_TEXT_BASE, 0);
	} else {
		uart_puts("sd_ch error!\r\n");
		return -1;
	}

	if (ret == 0) {
		uart_puts("copy error!\r\n");
		return -1;
	}

	uart_puts("copy ok.\r\n");

	return 0;
}

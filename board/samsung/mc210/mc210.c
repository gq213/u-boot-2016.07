/*
 *  Copyright (C) 2008-2009 Samsung Electronics
 *  Minkyu Kang <mk7.kang@samsung.com>
 *  Kyungmin Park <kyungmin.park@samsung.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <samsung/misc.h>
#include <asm/arch/gpio.h>
#include <asm/arch/mmc.h>

DECLARE_GLOBAL_DATA_PTR;

u32 get_board_rev(void)
{
	return 0;
}

int board_init(void)
{
	/* Set Initial global variables */
	gd->bd->bi_arch_number = MACH_TYPE_MC210;
	gd->bd->bi_boot_params = PHYS_SDRAM_1 + 0x100;

	return 0;
}

int dram_init(void)
{
	gd->ram_size = PHYS_SDRAM_1_SIZE + PHYS_SDRAM_2_SIZE;

	return 0;
}

void dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;
	gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
	gd->bd->bi_dram[1].size = PHYS_SDRAM_2_SIZE;
}

#ifdef CONFIG_DISPLAY_BOARDINFO
int checkboard(void)
{
	puts("Board:\tmc210\n");
	return 0;
}
#endif


#ifdef CONFIG_MISC_INIT_R
int misc_init_r(void)
{
#ifdef CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG
	set_board_info();
#endif
	return 0;
}
#endif


#ifdef CONFIG_GENERIC_MMC
int board_mmc_init(bd_t *bis)
{
	int i, ret;

	/*
	 * MMC2 GPIO
	 * GPG2[0]	SD_2_CLK
	 * GPG2[1]	SD_2_CMD
	 * GPG2[2]	SD_2_CDn	-> Not used
	 * GPG2[3:6]	SD_2_DATA[0:3]
	 */
	for (i = S5PC110_GPIO_G20; i < S5PC110_GPIO_G27; i++) {
		if (i == S5PC110_GPIO_G22)
			continue;

		/* GPG2[0:6] special function 2 */
		gpio_cfg_pin(i, 0x2);
		/* GPG2[0:6] pull disable */
		gpio_set_pull(i, S5P_GPIO_PULL_NONE);
		/* GPG2[0:6] drv 4x */
		gpio_set_drv(i, S5P_GPIO_DRV_4X);
	}

	ret = s5p_mmc_init(2, 4);
	if (ret)
		error("MMC: Failed to init SD card (MMC:2).\n");

	return ret;
}
#endif

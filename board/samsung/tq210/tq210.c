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
#include <asm/arch/sromc.h>
#include <netdev.h>
#include <asm/gpio.h>

DECLARE_GLOBAL_DATA_PTR;

u32 get_board_rev(void)
{
	return 0;
}

#ifdef CONFIG_DRIVER_DM9000
#define CONFIG_ENV_SROM_BANK	1
static void dm9000_pre_init(void)
{
	u32 smc_bw_conf, smc_bc_conf;

	/* Ethernet needs bus width of 16 bits */
	smc_bw_conf = SMC_DATA16_WIDTH(CONFIG_ENV_SROM_BANK)
			| SMC_BYTE_ADDR_MODE(CONFIG_ENV_SROM_BANK);
	smc_bc_conf = SMC_BC_TACS(0) | SMC_BC_TCOS(0) | SMC_BC_TACC(7)
			| SMC_BC_TCOH(0) | SMC_BC_TAH(0)
			| SMC_BC_TACP(0) | SMC_BC_PMC(0);

	/* Select and configure the SROMC bank */
	s5p_config_sromc(CONFIG_ENV_SROM_BANK, smc_bw_conf, smc_bc_conf);
}
#endif

int board_init(void)
{
	/* Set Initial global variables */
	gd->bd->bi_arch_number = MACH_TYPE_TQ210;
	gd->bd->bi_boot_params = PHYS_SDRAM_1 + 0x100;

#ifdef CONFIG_DRIVER_DM9000
	dm9000_pre_init();
#endif

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
	puts("Board:\ttq210\n");
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
	 * MMC0 GPIO
	 * GPG0[0]	SD_0_CLK
	 * GPG0[1]	SD_0_CMD
	 * GPG0[2]	SD_0_CDn	-> Not used
	 * GPG0[3:6]	SD_0_DATA[0:3]
	 */
	for (i = S5PC110_GPIO_G00; i < S5PC110_GPIO_G07; i++) {
		if (i == S5PC110_GPIO_G02)
			continue;

		/* GPG0[0:6] special function 2 */
		gpio_cfg_pin(i, 0x2);
		/* GPG0[0:6] pull disable */
		gpio_set_pull(i, S5P_GPIO_PULL_NONE);
		/* GPG0[0:6] drv 4x */
		gpio_set_drv(i, S5P_GPIO_DRV_4X);
	}

	ret = s5p_mmc_init(0, 4);
	if (ret)
		error("MMC: Failed to init SD card (MMC:0).\n");

	return ret;
}
#endif

#ifdef CONFIG_CMD_NET
int board_eth_init(bd_t *bis)
{
#ifdef CONFIG_DRIVER_DM9000
	return dm9000_initialize(bis);
#else
	return -1;
#endif
}
#endif

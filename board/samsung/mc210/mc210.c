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
			| SMC_BYTE_ADDR_MODE(CONFIG_ENV_SROM_BANK)
			| SMC_WAIT_ENABLE(CONFIG_ENV_SROM_BANK)
			| SMC_BYTE_ENABLE(CONFIG_ENV_SROM_BANK);
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
	gd->bd->bi_arch_number = MACH_TYPE_MC210;
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
	int i, ret, ret_sd = 0;

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

	/*
	 * SD card (T_FLASH) detect and init
	 * T_FLASH_DETECT: GPG3[2] input mode
	 */
	gpio_request(S5PC110_GPIO_G32, "t_flash_detect");
	gpio_cfg_pin(S5PC110_GPIO_G32, S5P_GPIO_INPUT);
	gpio_set_pull(S5PC110_GPIO_G32, S5P_GPIO_PULL_UP);

	if (!gpio_get_value(S5PC110_GPIO_G32)) {
		for (i = S5PC110_GPIO_G30; i < S5PC110_GPIO_G37; i++) {
			if (i == S5PC110_GPIO_G32)
				continue;

			/* GPG3[0:6] special function 2 */
			gpio_cfg_pin(i, 0x2);
			/* GPG3[0:6] pull disable */
			gpio_set_pull(i, S5P_GPIO_PULL_NONE);
			/* GPG3[0:6] drv 4x */
			gpio_set_drv(i, S5P_GPIO_DRV_4X);
		}

		ret_sd = s5p_mmc_init(3, 4);
		if (ret_sd)
			error("MMC: Failed to init SD card (MMC:3).\n");
	}

	return ret & ret_sd;
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

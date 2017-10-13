#include <common.h>


extern void uart_puts(char *s);
typedef unsigned int(*copy_sd_mmc_to_mem)
	(unsigned int channel, unsigned int start_block, unsigned short block_size, unsigned int *dst, unsigned int with_init);
typedef void __noreturn (*image_entry_noargs_t)(void);


void copy_bl2_to_ram(void)
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
		while(1);
	}

	if (ret == 0) {
		uart_puts("copy error!\r\n");
		while(1);
	} else
		uart_puts("copy ok.\r\n");

	image_entry_noargs_t image_entry = (image_entry_noargs_t)(CONFIG_SYS_TEXT_BASE);

	uart_puts("uboot...\r\n");
	image_entry();
}

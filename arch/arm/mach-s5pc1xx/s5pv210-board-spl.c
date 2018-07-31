/*
 * (C) Copyright 2015 Google, Inc
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <asm/arch/uart.h>
#include <asm/arch/gpio.h>

DECLARE_GLOBAL_DATA_PTR;

void uart_putc(char c)
{
#ifdef CONFIG_MACH_MC210
	struct s5p_uart *const uart = (struct s5p_uart *)samsung_get_base_uart2();
#else
	struct s5p_uart *const uart = (struct s5p_uart *)samsung_get_base_uart();
#endif

	while ((readl(&uart->utrstat) & 0x4) != 0x4);
	writeb(c, &uart->utxh);
}

void uart_puts(char *s)
{
	while (*s != '\0') {
		uart_putc(*s++);
	}
}

void debug_reg(u32 value)
{
	u32 m;
	u32 i;

	uart_puts("0x");
	for(i=0; i<8; i++)
	{
		m = (value>>((7-i)*4)) & 0x0000000f;

		if(m>9)
			m = m - 0xa + 'A'; 
		else
			m = m + '0';

		uart_putc(m);
	}	
	uart_puts("\r\n");
}

static void uart_init(void)
{
#ifdef CONFIG_MACH_MC210
	struct s5p_gpio_bank *const gpio = (struct s5p_gpio_bank *)samsung_get_base_gpio() + sizeof(struct s5p_gpio_bank);
	struct s5p_uart *const uart = (struct s5p_uart *)samsung_get_base_uart2();
#else
	struct s5p_gpio_bank *const gpio = (struct s5p_gpio_bank *)samsung_get_base_gpio();
	struct s5p_uart *const uart = (struct s5p_uart *)samsung_get_base_uart();
#endif

	writel(0x22, &gpio->con);

	writel(0, &uart->ufcon);
	writel(0, &uart->umcon);

	writel(3, &uart->ulcon);
	writel(0x3C5, &uart->ucon);

	writel(34, &uart->ubrdiv);
	writew(0xDDDD, &uart->rest.slot);

	uart_puts("OK\r\n");
}

#ifdef CONFIG_SPL_BOARD_INIT
void spl_board_init(void)
{
	uart_init();
}
#endif

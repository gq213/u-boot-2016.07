#include <common.h>
#include <asm/arch/clock.h>
#include <asm/arch/dmc.h>
#include <asm/arch/uart.h>
#include <asm/arch/gpio.h>


void clock_init(void)
{
	struct s5pc110_clock *const clock = (struct s5pc110_clock *)samsung_get_base_clock();

	writel((3  << 8) | (125 << 16) | (1 << 0) | (1 << 31), &clock->apll_con);	/* FOUT_APLL = 1000MHz */  
	writel((12 << 8) | (667 << 16) | (1 << 0) | (1 << 31), &clock->mpll_con);	/* FOUT_MPLL = 667MHz */  
	writel((3  << 8) | (48  << 16) | (2 << 0) | (1 << 31), &clock->epll_con);	/* FOUT_EPLL = 96MHz */  
	writel((6  << 8) | (108 << 16) | (3 << 0) | (1 << 31), &clock->vpll_con);	/* FOUT_VPLL = 54MHz */  
	while (!(readl(&clock->apll_con) & (1 << 29)));
	while (!(readl(&clock->vpll_con) & (1 << 29)));
	while (!(readl(&clock->mpll_con) & (1 << 29)));
	while (!(readl(&clock->epll_con) & (1 << 29)));

	/* select pll out */
	writel((1 << 0) | (1 << 4) | (1 << 8) | (1 << 12), &clock->src0);

	/*
	@ APLL_RATIO = 0,      freq(ARMCLK)    = MOUT_MSYS / (APLL_RATIO + 1)      = 1000MHz
	@ A2M_RATIO = 4,       freq(SCLKA2M)   = SCLKAPLL / (A2M_RATIO + 1)        = 200MHz
	@ HCLK_MSYS_RATIO = 4, freq(HCLK_MSYS) = ARMCLK / (HCLK_MSYS_RATIO + 1)    = 200MHz
	@ PCLK_MSYS_RATIO = 1, freq(PCLK_MSYS) = HCLK_MSYS / (PCLK_MSYS_RATIO + 1) = 100MHz
	@ HCLK_DSYS_RATIO = 3, freq(HCLK_DSYS) = MOUT_DSYS / (HCLK_DSYS_RATIO + 1) = 166MHz
	@ PCLK_DSYS_RATIO = 1, freq(PCLK_DSYS) = HCLK_DSYS / (PCLK_DSYS_RATIO + 1) = 83MHz
	@ HCLK_PSYS_RATIO = 4, freq(HCLK_PSYS) = MOUT_PSYS / (HCLK_PSYS_RATIO + 1) = 133MHz
	@ PCLK_PSYS_RATIO = 1, freq(PCLK_PSYS) = HCLK_PSYS / (PCLK_PSYS_RATIO + 1) = 66MHz
	*/
        writel((0 << 0) | (4 << 4) | (4 << 8) | (1 << 12) | (3 << 16) | (1 << 20) | (4 << 24) | (1 << 28), &clock->div0);  
}

void ddr_init(void)
{
	struct s5pv210_dmc *const dmc0 = (struct s5pv210_dmc *)samsung_get_base_dmc0();
	struct s5pv210_dmc *const dmc1 = (struct s5pv210_dmc *)samsung_get_base_dmc1();

	/* DMC0 initialization at single Type*/
	writel(0x00101000, &dmc0->phycontrol0);
	writel(0x00000086, &dmc0->phycontrol1);
	writel(0x00101002, &dmc0->phycontrol0);
	writel(0x00101003, &dmc0->phycontrol0);
	while ((readl(&dmc0->phystatus) & 0x7) != 0x7);
	writel(((readl(&dmc0->phystatus) & 0x3fc0) << 18) | 0x00101003, &dmc0->phycontrol0);
	/* setting DDR2 */
	writel(0x0FFF2010, &dmc0->concontrol);
	writel(0x00212400, &dmc0->memcontrol);
	writel(0x20E01323, &dmc0->memconfig0);
	writel(0x40F01323, &dmc0->memconfig1);
	writel(0xFF000000, &dmc0->prechconfig);
	writel(0x00000618, &dmc0->timingref);
	writel(0x28233287, &dmc0->timingrow);
	writel(0x23240304, &dmc0->timingdata);
	writel(0x09C80232, &dmc0->timingpower);
	writel(0x07000000, &dmc0->directcmd);
	writel(0x01000000, &dmc0->directcmd);
	writel(0x00020000, &dmc0->directcmd);
	writel(0x00030000, &dmc0->directcmd);
	writel(0x00010400, &dmc0->directcmd);
	writel(0x00000542, &dmc0->directcmd);
	writel(0x01000000, &dmc0->directcmd);
	writel(0x05000000, &dmc0->directcmd);
	writel(0x05000000, &dmc0->directcmd);
	writel(0x00000442, &dmc0->directcmd);
	writel(0x00010780, &dmc0->directcmd);
	writel(0x00010400, &dmc0->directcmd);
	writel(0x07100000, &dmc0->directcmd);
	writel(0x01100000, &dmc0->directcmd);
	writel(0x00120000, &dmc0->directcmd);
	writel(0x00130000, &dmc0->directcmd);
	writel(0x00110400, &dmc0->directcmd);
	writel(0x00100542, &dmc0->directcmd);
	writel(0x01100000, &dmc0->directcmd);
	writel(0x05100000, &dmc0->directcmd);
	writel(0x05100000, &dmc0->directcmd);
	writel(0x00100442, &dmc0->directcmd);
	writel(0x00110780, &dmc0->directcmd);
	writel(0x00110400, &dmc0->directcmd);
	writel(0x0FF02030, &dmc0->concontrol);
	writel(0xFFFF00FF, &dmc0->pwrdnconfig);
	writel(0x00202400, &dmc0->memcontrol);

	/* DMC1 initialization */
	writel(0x00101000, &dmc1->phycontrol0);
	writel(0x00000086, &dmc1->phycontrol1);
	writel(0x00101002, &dmc1->phycontrol0);
	writel(0x00101003, &dmc1->phycontrol0);
	while ((readl(&dmc1->phystatus) & 0x7) != 0x7);
	writel(((readl(&dmc1->phystatus) & 0x3fc0) << 18) | 0x00101003, &dmc1->phycontrol0);
	/* setting fot DDR2 */
	writel(0x0FFF2010, &dmc1->concontrol);
	writel(0x00202400, &dmc1->memcontrol);
	writel(0x40C01323, &dmc1->memconfig0);
	writel(0x00E01323, &dmc1->memconfig1);
	writel(0xFF000000, &dmc1->prechconfig);
	writel(0x00000618, &dmc1->timingref);
	writel(0x28233289, &dmc1->timingrow);
	writel(0x23240304, &dmc1->timingdata);
	writel(0x08280232, &dmc1->timingpower);
	writel(0x07000000, &dmc1->directcmd);
	writel(0x01000000, &dmc1->directcmd);
	writel(0x00020000, &dmc1->directcmd);
	writel(0x00030000, &dmc1->directcmd);
	writel(0x00010400, &dmc1->directcmd);
	writel(0x00000542, &dmc1->directcmd);
	writel(0x01000000, &dmc1->directcmd);
	writel(0x05000000, &dmc1->directcmd);
	writel(0x05000000, &dmc1->directcmd);
	writel(0x00000442, &dmc1->directcmd);
	writel(0x00010780, &dmc1->directcmd);
	writel(0x00010400, &dmc1->directcmd);
	writel(0x07100000, &dmc1->directcmd);
	writel(0x01100000, &dmc1->directcmd);
	writel(0x00120000, &dmc1->directcmd);
	writel(0x00130000, &dmc1->directcmd);
	writel(0x00110440, &dmc1->directcmd);
	writel(0x00100542, &dmc1->directcmd);
	writel(0x01100000, &dmc1->directcmd);
	writel(0x05100000, &dmc1->directcmd);
	writel(0x05100000, &dmc1->directcmd);
	writel(0x00100442, &dmc1->directcmd);
	writel(0x00110780, &dmc1->directcmd);
	writel(0x00110400, &dmc1->directcmd);
	writel(0x0FF02030, &dmc1->concontrol);
	writel(0xFFFF00FF, &dmc1->pwrdnconfig);
	writel(0x00202400, &dmc1->memcontrol);
}

void uart_putc(char c)
{
	struct s5p_uart *const uart = (struct s5p_uart *)samsung_get_base_uart2();

	while ((readl(&uart->utrstat) & 0x4) != 0x4);
	writeb(c, &uart->utxh);
}

void uart_puts(char *s)
{
	while (*s != '\0') {
		uart_putc(*s++);
	}
}

void uart_init(void)
{
	struct s5p_gpio_bank *const gpio = (struct s5p_gpio_bank *)samsung_get_base_gpio() + sizeof(struct s5p_gpio_bank);
	struct s5p_uart *const uart = (struct s5p_uart *)samsung_get_base_uart2();

	writel(0x22, &gpio->con);

	writel(0, &uart->ufcon);
	writel(0, &uart->umcon);

	writel(3, &uart->ulcon);
	writel(0x3C5, &uart->ucon);

	writel(34, &uart->ubrdiv);
	writew(0xDDDD, &uart->rest.slot);

	uart_puts("OK\r\n");
}

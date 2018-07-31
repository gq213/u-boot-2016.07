#define ELFIN_CLOCK_POWER_BASE		0xE0100000
#define APLL_LOCK_OFFSET			0x00
#define APLL_CON0_OFFSET			0x100
#define APLL_CON1_OFFSET			0x104
#define MPLL_CON_OFFSET				0x108
#define VPLL_CON_OFFSET				0x120
#define CLK_SRC0_OFFSET				0x200
#define CLK_DIV0_OFFSET				0x300
#define CLK_DIV6_OFFSET				0x318

#define APLL_LOCKTIME_VAL					0x2cf

#define AFC_ON								0x00000000
#define AFC_OFF								0x10000010

#define APLL_MDIV							0x7d
#define APLL_PDIV							0x3
#define APLL_SDIV							0x1

#define MPLL_MDIV							0x29b
#define MPLL_PDIV							0xc
#define MPLL_SDIV							0x1

#define VPLL_MDIV							0x6c
#define VPLL_PDIV							0x6
#define VPLL_SDIV							0x3

#define set_pll(mdiv, pdiv, sdiv)	(1<<31 | mdiv<<16 | pdiv<<8 | sdiv)

#define APLL_VAL							set_pll(APLL_MDIV,APLL_PDIV,APLL_SDIV)
#define MPLL_VAL							set_pll(MPLL_MDIV,MPLL_PDIV,MPLL_SDIV)
#define VPLL_VAL							set_pll(VPLL_MDIV,VPLL_PDIV,VPLL_SDIV)

/* CLK_DIV0 */
#define APLL_RATIO							0
#define A2M_RATIO							4
#define HCLK_MSYS_RATIO						8
#define PCLK_MSYS_RATIO						12
#define HCLK_DSYS_RATIO						16
#define PCLK_DSYS_RATIO						20
#define HCLK_PSYS_RATIO						24
#define PCLK_PSYS_RATIO						28

#define CLK_DIV0_MASK						0x7fffffff

#define CLK_DIV0_VAL						((0<<APLL_RATIO)|(4<<A2M_RATIO)|(4<<HCLK_MSYS_RATIO)|(1<<PCLK_MSYS_RATIO)\
												|(3<<HCLK_DSYS_RATIO)|(1<<PCLK_DSYS_RATIO)|(4<<HCLK_PSYS_RATIO)|(1<<PCLK_PSYS_RATIO))


	.globl system_clock_init
system_clock_init:

	ldr	r0, =ELFIN_CLOCK_POWER_BASE	@0xe0100000

	/* Set Mux to FIN */
	ldr	r1, =0x0
	str	r1, [r0, #CLK_SRC0_OFFSET]

	ldr	r1,	=APLL_LOCKTIME_VAL
	str	r1,	[r0, #APLL_LOCK_OFFSET]

	/* Disable PLL */
#if defined(CONFIG_CHECK_MPLL_LOCK)
retryloop:
#endif
	ldr	r1, =0x0
	str	r1, [r0, #APLL_CON0_OFFSET]
	ldr	r1, =0x0
	str	r1, [r0, #MPLL_CON_OFFSET]

	ldr	r1, =0x0
	str	r1, [r0, #MPLL_CON_OFFSET]

	ldr   	r1, [r0, #CLK_DIV0_OFFSET]
	ldr	r2, =CLK_DIV0_MASK
	bic	r1, r1, r2

	ldr	r2, =CLK_DIV0_VAL
	orr	r1, r1, r2
	str	r1, [r0, #CLK_DIV0_OFFSET]

	ldr	r1, =APLL_VAL
	str	r1, [r0, #APLL_CON0_OFFSET]

	ldr	r1, =MPLL_VAL
	str	r1, [r0, #MPLL_CON_OFFSET]

	ldr	r1, =VPLL_VAL
	str	r1, [r0, #VPLL_CON_OFFSET]

	ldr	r1, =AFC_ON
	str	r1, [r0, #APLL_CON1_OFFSET]

	mov	r1, #0x10000
1:	subs	r1, r1, #1
	bne	1b

#if defined(CONFIG_CHECK_MPLL_LOCK)
	/* MPLL software workaround */
	ldr	r1, [r0, #MPLL_CON_OFFSET]
	orr     r1, r1, #(1<<28)
	str	r1, [r0, #MPLL_CON_OFFSET]

	mov	r1, #0x100
1:	subs	r1, r1, #1
	bne	1b

	ldr	r1, [r0, #MPLL_CON_OFFSET]
	and	r1, r1, #(1<<29)
	cmp	r1, #(1<<29)
	bne 	retryloop

	/* H/W lock detect disable */
	ldr	r1, [r0, #MPLL_CON_OFFSET]
	bic     r1, r1, #(1<<28)
	str	r1, [r0, #MPLL_CON_OFFSET]
#endif

	ldr	r1, [r0, #CLK_SRC0_OFFSET]
	ldr	r2, =0x10001111
	orr	r1, r1, r2
	str	r1, [r0, #CLK_SRC0_OFFSET]

	/* CLK_DIV6 */
	ldr	r1, [r0, #CLK_DIV6_OFFSET]
	bic	r1, r1, #(0x7<<12)	@; ONENAND_RATIO: 0
	str	r1, [r0, #CLK_DIV6_OFFSET]

	mov	pc, lr

#
# (C) Copyright 2000-2003
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# (C) Copyright 2008
# Guennadi Liakhovetki, DENX Software Engineering, <lg@denx.de>
#
# SPDX-License-Identifier:	GPL-2.0+
#

ifndef CONFIG_SPL_BUILD
obj-y	= cache.o
obj-y	+= reset.o

obj-y	+= clock.o
else
obj-y	= s5pv210-board-spl.o
obj-y	+= lowlevel_init.o
obj-y	+= s5pv210_clock.o
obj-y	+= s5pv210_ddr.o
obj-y	+= s5pv210_tzpc.o
endif

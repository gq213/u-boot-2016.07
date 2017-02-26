#ifndef __DMC_H__
#define __DMC_H__

#ifndef __ASSEMBLY__
struct s5pv210_dmc {
	unsigned int concontrol;
	unsigned int memcontrol;
	unsigned int memconfig0;
	unsigned int memconfig1;
	unsigned int directcmd;
	unsigned int prechconfig;
	unsigned int phycontrol0;
	unsigned int phycontrol1;
	unsigned char res1[0x8];
	unsigned int pwrdnconfig;
	unsigned char res2[0x4];
	unsigned int timingref;
	unsigned int timingrow;
	unsigned int timingdata;
	unsigned int timingpower;
	unsigned int phystatus;
	unsigned int chip0status;
	unsigned int chip1status;
	unsigned int arefstatus;
	unsigned int mrstatus;
	unsigned int phytest0;
	unsigned int phytest1;
};
#endif
#endif

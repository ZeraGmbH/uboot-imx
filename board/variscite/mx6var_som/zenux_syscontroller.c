#ifndef CONFIG_SPL_BUILD
#include "zenux_syscontroller.h"
#include <common.h>
#include <i2c.h>

#define SYS_I2C_NUM 0x00
#define SYS_I2C_ADR 0x22

const u16 cmdIdGetCtrlVersion = 0x0003;

bool probeSysController(void)
{
	i2c_set_bus_num(SYS_I2C_NUM);
	return 0 == i2c_probe(SYS_I2C_ADR);
}

#endif

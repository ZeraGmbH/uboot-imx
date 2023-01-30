#ifndef CONFIG_SPL_BUILD
#include "zenux_syscontroller.h"
#include <common.h>
#include <i2c.h>

#define SYS_I2C_NUM 0x00
#define SYS_I2C_ADR 0x22

bool probeSysController(void)
{
	bool ret;
	int oldbus = i2c_get_bus_num();

	i2c_set_bus_num(SYS_I2C_NUM);
	ret = (0 == i2c_probe(SYS_I2C_ADR));

	i2c_set_bus_num(oldbus);
	return ret;
}

#endif

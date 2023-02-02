#ifndef CONFIG_SPL_BUILD
#include "zenux_syscontroller.h"
#include "zenux_mcontroller_io.h"
#include <common.h>
#include <i2c.h>

#define SYS_I2C_NUM 0x00
#define SYS_I2C_ADR 0x22

const u16 cmdIdGetCtrlVersion = 0x0003;

static struct udevice *getI2cDevice();

bool probeSysController(void)
{
    i2c_set_bus_num(SYS_I2C_NUM);
    return 0 == i2c_probe(SYS_I2C_ADR);
}

bool readCTRLVersion(char* receivedVersion)
{
    struct udevice *i2cDevice = getI2cDevice();
    u16 lenReturned = readCmd(i2cDevice, cmdIdGetCtrlVersion, (u8*)receivedVersion);
    receivedVersion[lenReturned] = 0;
    return lenReturned > 0;
}

static struct udevice *getI2cDevice()
{
    struct udevice *i2cDevice;
    i2c_get_chip_for_busnum(SYS_I2C_NUM, SYS_I2C_ADR, 1, i2cDevice);
    return i2cDevice;
}

#endif

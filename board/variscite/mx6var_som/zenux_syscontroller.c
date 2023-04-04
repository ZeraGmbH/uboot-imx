#ifndef CONFIG_SPL_BUILD
#include "zenux_syscontroller.h"
#include "zenux_mcontroller_io.h"
#include <common.h>
#include <i2c.h>

#define SYS_I2C_NUM 0x00
#define SYS_I2C_ADR 0x22

const u16 cmdIdGetCtrlVersion = 0x0003;
const u16 cmdIDGetInstrumentClass = 0x0020;
const u16 cmdIdGetDisplayType = 0x0023;

static struct udevice *getI2cDevice();

bool probeSysController(void)
{
    i2c_set_bus_num(SYS_I2C_NUM);
    return 0 == i2c_probe(SYS_I2C_ADR);
}

bool checkForStringWithContent(char* receivedData, u16 lenReturned)
{
    if(lenReturned > 1) {
        receivedData[lenReturned-1] = 0; // last is checksum
        return true;
    }
    return false;
}

bool readCTRLVersion(char* receivedVersion)
{
    i2c_set_bus_num(SYS_I2C_NUM);
    u16 lenReturned = readCmd(SYS_I2C_ADR, cmdIdGetCtrlVersion, (u8*)receivedVersion);
    return checkForStringWithContent(receivedVersion, lenReturned);
}

bool readInstrumentClass(char* receivedClass)
{
    i2c_set_bus_num(SYS_I2C_NUM);
    u16 lenReturned = readCmd(SYS_I2C_ADR, cmdIDGetInstrumentClass, (u8*)receivedClass);
    return checkForStringWithContent(receivedClass, lenReturned);
}

bool readDisplayType(u8* receivedType)
{
    i2c_set_bus_num(SYS_I2C_NUM);
    char* receivedData[MAX_READ_LEN_ZHARD];
    u16 lenReturned = readCmd(SYS_I2C_ADR, cmdIdGetDisplayType, (u8*)receivedData);
    if(lenReturned-1 == 1) {
        *receivedType = receivedData[0];
        return true;
    }
    return false;
}

#endif

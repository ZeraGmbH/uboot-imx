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

bool probeSysController(void)
{
    int ret;
    i2c_set_bus_num(SYS_I2C_NUM);
    ret = i2c_probe(SYS_I2C_ADR);
    udelay(50);
    return 0 == ret;
}

bool checkForStringWithContent(char* receivedData, u16 lenReturned)
{
    if(lenReturned > 1) {
        receivedData[lenReturned-1] = 0; // last is checksum
        return true;
    }
    return false;
}

static bool readString(char* stringBuffer, const u16 cmdId)
{
    memset (stringBuffer, 0, MAX_READ_LEN_ZHARD);
    i2c_set_bus_num(SYS_I2C_NUM);
    u16 lenReturned = readCmd(SYS_I2C_ADR, cmdId, (u8*)stringBuffer);
    return checkForStringWithContent(stringBuffer, lenReturned);
}

bool readCTRLVersion(char* receivedVersion)
{
    return readString(receivedVersion, cmdIdGetCtrlVersion);
}

bool readInstrumentClass(char* receivedClass)
{
    return readString(receivedClass, cmdIDGetInstrumentClass);
}

bool readDisplayType(u8* receivedType)
{
    i2c_set_bus_num(SYS_I2C_NUM);
    u8 receivedData[MAX_READ_LEN_ZHARD];
    u16 lenReturned = readCmd(SYS_I2C_ADR, cmdIdGetDisplayType, receivedData);
    if(lenReturned-1 == 1) {
        *receivedType = receivedData[0];
        return true;
    }
    return false;
}

#endif

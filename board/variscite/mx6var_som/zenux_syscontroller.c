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
const u16 cmdIdLcdStartWatchdog = 0x0043;

/* Background:
   Systemcontroller is always on and switches off I2C pull up voltage in
   power off state. On power on the systemcontroller switches on I2C pull
   up voltage. At the time of writing this can cause systemcontroller's I2C
   unit to NAK first communication request. Future versions of
   systemcontroller will fix this by enabling I2C once pullup voltage is on.
   So maybe then we can remove probing but it can remain - there is no harm
   expected.
   Update: Systemcontroller was fixed: I2c unit is off while system powerered
   off.
 */
bool probeSysController(void)
{
    int ret;
    i2c_set_bus_num(SYS_I2C_NUM);
    ret = i2c_probe(SYS_I2C_ADR);
    udelay(50);   // avoid sporadic NACK from uC
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

bool enableLcdWatchdog(void)
{
    i2c_set_bus_num(SYS_I2C_NUM);
    u8 paramData = 1;
    return writeCmd(SYS_I2C_ADR, cmdIdLcdStartWatchdog, &paramData, 1);
}

bool disableLcdWatchdog(void)
{
    i2c_set_bus_num(SYS_I2C_NUM);
    u8 paramData = 0;
    return writeCmd(SYS_I2C_ADR, cmdIdLcdStartWatchdog, &paramData, 1);
}

static int do_enableLcdWatchdog(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    return enableLcdWatchdog() ? CMD_RET_SUCCESS : CMD_RET_FAILURE;
}

U_BOOT_CMD(
	enableLcdWatchdog, 1, 1, do_enableLcdWatchdog,
	"enable LCD watchdog",
	"- systemcontroller LCD watchdog on"
);

static int do_disableLcdWatchdog(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    return disableLcdWatchdog() ? CMD_RET_SUCCESS : CMD_RET_FAILURE;
}

U_BOOT_CMD(
	disableLcdWatchdog, 1, 1, do_disableLcdWatchdog,
	"disable LCD watchdog",
	"- systemcontroller LCD watchdog off"
);

#endif

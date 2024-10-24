#ifndef __ZENUX_MCONTROLLER_IO_H
#define __ZENUX_MCONTROLLER_IO_H

#include <asm/types.h>
#include <stdbool.h>
#include <common.h>
#include <dm/device.h>
#include <i2c.h>

#ifndef CONFIG_SPL_BUILD

#define MAX_READ_LEN_ZHARD 128

u16 readCmd(uint i2cAddr, u16 cmdId, u8 *readBuff);
u16 readCmdWithRepeatOnError(uint i2cAddr, u16 cmdId, u8 *readBuff);
bool writeCmd(uint i2cAddr, u16 cmdId, u8 *cmdParam, u8 paramLen);
bool writeCmdWithRepeatOnError(uint i2cAddr, u16 cmdId, u8 *cmdParam, u8 paramLen);

#endif

#endif /* __ZENUX_MCONTROLLER_IO_H */

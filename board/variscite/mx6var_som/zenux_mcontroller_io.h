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
void readTestCmd(uint i2cAddr, u16 cmdId);

#endif

#endif /* __ZENUX_MCONTROLLER_IO_H */

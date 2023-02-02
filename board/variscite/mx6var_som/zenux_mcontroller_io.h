#ifndef __ZENUX_MCONTROLLER_IO_H
#define __ZENUX_MCONTROLLER_IO_H

#include <asm/types.h>
#include <stdbool.h>
#include <common.h>
#include <i2c.h>

#ifndef CONFIG_SPL_BUILD

u16 readCmd(struct udevice *dev, u16 cmdId, u8 *readBuff);

#endif

#endif /* __ZENUX_MCONTROLLER_IO_H */

#ifndef __ZENUX_MCONTROLLER_IO_H
#define __ZENUX_MCONTROLLER_IO_H

#include <asm/types.h>
#include <stdbool.h>

#ifndef CONFIG_SPL_BUILD

u16 readCmd(u16 cmdId, u8 *readBuff);

#endif

#endif /* __ZENUX_MCONTROLLER_IO_H */

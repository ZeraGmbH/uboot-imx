#ifndef __ZENUX_MCONTROLLER_CRC_H
#define __ZENUX_MCONTROLLER_CRC_H

#include <asm/types.h>

#ifndef CONFIG_SPL_BUILD

u8 calcBlockCRC(u8* block, u32 blocklen);
u8 calcByteCRC(u8 crc, u8 data);

#endif /* CONFIG_SPL_BUILD */

#endif /* __ZENUX_MCONTROLLER_CRC_H */

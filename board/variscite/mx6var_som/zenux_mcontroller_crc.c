#ifndef CONFIG_SPL_BUILD
#include "zenux_mcontroller_crc.h"
#include <asm/io.h>
#include <common.h>
#include <i2c.h>

u8 calcBlockCRC(u8* pBuff, u16 iBuffLen)
{
    u8 crc = 0;
    for (u16 i = 0; i < iBuffLen; i++)
        crc = calcByteCRC(crc, pBuff[i]);
    return crc;
}

u8 calcByteCRC(u8 crc, u8 data)
{
    crc = crc ^ data;
    for (u8 i = 0; i < 8; i++) {
        if (crc & 0x01)
            crc = (crc >> 1) ^ 0x8c;
        else
            crc >>= 1;
    }
    return crc;
}

#endif

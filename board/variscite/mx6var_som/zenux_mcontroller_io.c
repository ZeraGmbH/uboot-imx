#include "zenux_mcontroller_io.h"
#include "zenux_mcontroller_crc.h"

static void generateCmdRequest(u16 cmdId, u8 subDevice, u8* param, u16 paramLen, u8 *buffToSend);

u16 readCmd(struct udevice *dev, u16 cmdId, u8 *readBuff)
{
    u8 requestBuff[128];
    u16 bytesRead = 0;
    generateCmdRequest(cmdId, 0, NULL, 0, requestBuff);

    return bytesRead;
}

static void generateCmdRequest(u16 cmdId, u8 subDevice, u8* param, u16 paramLen, u8 *buffToSend)
{
    u16 i;
    u16 len = 6 + paramLen;
    u8* currBuff = buffToSend;
    // cmd len
    *currBuff++ = (u8*)(len >> 8);
    *currBuff++ = len & 0xFF;
    // cmd ID
    *currBuff++ = (cmdId >> 8) & 0xFF;
    *currBuff++ = cmdId & 0xFF;
    // sub device
    *buffToSend++ = subDevice;
    // param
    for (i = 0; i < paramLen; i++)
        *currBuff++ = *param++;
    // CRC
    *currBuff = calcBlockCRC(buffToSend, len-1);
}

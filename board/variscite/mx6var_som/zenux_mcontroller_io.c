#include "zenux_mcontroller_io.h"
#include "zenux_mcontroller_crc.h"

static u16 decodeRequestResponse(u8 *response);
static u16 generateCmdRequest(u16 cmdId, u8 subDevice, u8* param, u16 paramLen, u8 *buffToSend);

u16 readCmd(uint i2cAddr, u16 cmdId, u8 *readBuff)
{
    u8 requestBuff[128];
    u8 requestResponse[5];
    u16 bytesRead = 0;
    u16 reqLen = generateCmdRequest(cmdId, 0, NULL, 0, requestBuff);

    if(!i2c_write_no_stop(i2cAddr, 0, 0, requestBuff, reqLen)) {
        if(!i2c_read(i2cAddr, 0, 0, requestResponse, 5)) {
            bytesRead = decodeRequestResponse(requestResponse);
        }
        else
            puts("Could not read to I2c!\n");
    }
    else
        puts("Could not write to I2c!\n");
    return bytesRead;
}

static u16 decodeRequestResponse(u8 *response)
{
    u16 len = 0;
    if(response[0] == 0 && response[1] == 0) {
        len = response[2] * 256 + response[3];
    }
    else
        printf("I2c request returned error mask %02X%02X!\n", response[0], response[1]);
    return len;
}

static u16 generateCmdRequest(u16 cmdId, u8 subDevice, u8* param, u16 paramLen, u8 *buffToSend)
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
    *currBuff++ = subDevice;
    // param
    for (i = 0; i < paramLen; i++)
        *currBuff++ = *param++;
    // CRC
    *currBuff = calcBlockCRC(buffToSend, len-1);
    return len;
}

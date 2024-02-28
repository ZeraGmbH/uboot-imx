#ifndef CONFIG_SPL_BUILD
#include "zenux_mcontroller_io.h"
#include "zenux_mcontroller_crc.h"
#include <stdbool.h>

static bool decodeRequestResponse(u8 *response, u16* responseLen);
static u16 generateCmdRequest(u16 cmdId, u8 subDevice, u8* param, u16 paramLen, u8 *buffToSend);

u16 readCmd(uint i2cAddr, u16 cmdId, u8 *readBuff)
{
    u8 requestBuff[16];
    u8 requestResponse[5];
    u16 bytesRead = 0;
    u16 reqLen = generateCmdRequest(cmdId, 0, NULL, 0, requestBuff);

    if(!i2c_write(i2cAddr, 0, 0, requestBuff, reqLen)) { // cmd -> ctl
        udelay(50);   // could be the final workaround for LCD-detection
        if(!i2c_read(i2cAddr, 0, -1, requestResponse, 5)) { // <- ctl errmask/len
            if(decodeRequestResponse(requestResponse, &bytesRead)) {
                if(bytesRead > MAX_READ_LEN_ZHARD) {
                    printf("readCmd: Cannot not read more than %i bytes!\n", MAX_READ_LEN_ZHARD);
                    bytesRead = 0;
                }
                if(bytesRead <= 1) { // just checksum is not enough
                    printf("readCmd: Invalid response - is comannd ID %04X correct?\n", cmdId);
                    bytesRead = 0;
                }
                else {
                    udelay(50); // for secure I2C communication (s.o.)
                    if(!i2c_read(i2cAddr, 0, -1, readBuff, bytesRead)) { // <- ctl data
                        // TODO checksum & their error handling
                    }
                    else {
                        puts("Could not read data from I2c!\n");
                        bytesRead = 0;
                    }
                }
            }
        }
        else
            puts("Could not read error mask/length from I2c!\n");
    }
    else
        puts("Could not write to I2c!\n");
    return bytesRead;
}

bool writeCmd(uint i2cAddr, u16 cmdId, u8 *cmdParam, u8 paramLen)
{
    u8 requestBuff[512];
    u8 requestResponse[5];
    u16 bytesRead = 0;
    bool allOk = false;
    u16 reqLen = generateCmdRequest(cmdId, 0, cmdParam, paramLen, requestBuff);

    if(!i2c_write(i2cAddr, 0, 0, requestBuff, reqLen)) { // cmd+param -> ctl
        udelay(50);
        if(!i2c_read(i2cAddr, 0, -1, requestResponse, 5)) { // <- ctl errmask/len
            if(decodeRequestResponse(requestResponse, &bytesRead)) {
                if(bytesRead != 0)
                    printf("writeCmd: Do not expect to read %i bytes!\n", bytesRead);
                else
                    allOk = true;
            }
        }
        else
            puts("Could not read error mask/length from I2c!\n");
    }
    else
        puts("Could not write to I2c!\n");
    return allOk;

}

static bool decodeRequestResponse(u8 *response, u16* responseLen)
{
    *responseLen = 0;
    if(response[0] == 0 && response[1] == 0) {
        *responseLen = response[2] * 256 + response[3];
        // TODO checksum & their error handling
        return true;
    }
    printf("I2c request returned error mask %02X%02X!\n", response[0], response[1]);
    return false;
}

static u16 generateCmdRequest(u16 cmdId, u8 subDevice, u8* param, u16 paramLen, u8 *buffToSend)
{
    u16 i;
    u16 len = 6 + paramLen;
    u8* currBuff = buffToSend;
    // cmd len
    *currBuff++ = (u8)(len >> 8);
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

#endif // CONFIG_SPL_BUILD

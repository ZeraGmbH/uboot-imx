#ifndef CONFIG_SPL_BUILD
#include "zenux_mcontroller_io.h"
#include "zenux_mcontroller_crc.h"

static u16 decodeRequestResponse(u8 *response);
static u16 generateCmdRequest(u16 cmdId, u8 subDevice, u8* param, u16 paramLen, u8 *buffToSend);


void readTestCmd(uint i2cAddr, u16 cmdId)
{
    u8 requestBuff[16];
    u16 reqLen = generateCmdRequest(cmdId, 0, NULL, 0, requestBuff);
    u8 requestResponse[5];
    #define TRIALS 20
    bool writeReturn[TRIALS];    
    bool readReturn[TRIALS];
    u8 trials;

    puts("I2C-Adr 0x22 Write/Read loop:\n");

    for (trials=0; trials<TRIALS; trials++)
    {
        writeReturn[trials] = i2c_write(i2cAddr, 0, 0, requestBuff, reqLen);
        puts("wr..");  // small break
        readReturn[trials] = i2c_read(i2cAddr, 0, -1, requestResponse, 5);
        puts("rd\n");  // small break
    }
    puts("Result 0...TRIALS\n");

    for (trials=0; trials<TRIALS; trials++)
    {
        if (writeReturn[trials])
           puts(" Write ERROR");
        else 
           puts(" Write OK   ");
        if (readReturn[trials])
           puts(" Read ERROR\n");
        else
           puts(" Read OK\n");
    }


/*  old variant

    if (i2c_write(i2cAddr, 0, 0, requestBuff, reqLen))
    {
        puts("Write-2 ERROR!\n");
    }
    else
    {
        puts("Write-2 OK! -> ");
        if(i2c_read(i2cAddr, 0, -1, requestResponse, 5))
        {
            puts("READ-2 ERROR!\n");
        }
        else
        {
            puts("READ-2 OK!\n");
        }
    }
*/

    puts("Loop end\n");
}


u16 readCmd(uint i2cAddr, u16 cmdId, u8 *readBuff)
{
    u8 requestBuff[16];
    u8 requestResponse[5];
    u16 bytesRead = 0;
    u16 reqLen = generateCmdRequest(cmdId, 0, NULL, 0, requestBuff);

    if (!i2c_write(i2cAddr, 0, 0, requestBuff, reqLen))  { // cmd -> ctl
        puts("wait..");  // small break, could be the final workaround
        if(!i2c_read(i2cAddr, 0, -1, requestResponse, 5))   { // <- ctl errmask/len
            bytesRead = decodeRequestResponse(requestResponse);
            if(bytesRead > MAX_READ_LEN_ZHARD) {
                printf("Cannot not read more than %i bytes!\n", MAX_READ_LEN_ZHARD);
                bytesRead = 0;
            }
            if(bytesRead <= 1) { // just checksum is not enough
                printf("No data to read - is comannd ID %04X correct?\n", cmdId);
                bytesRead = 0;
            }
            else {
                if(!i2c_read(i2cAddr, 0, -1, readBuff, bytesRead)) { // <- ctl data
                    // TODO checksum & their error handling
                }
                else {
                    puts("Could not read data from I2c!\n");
                    bytesRead = 0;
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

static u16 decodeRequestResponse(u8 *response)
{
    u16 len = 0;
    if(response[0] == 0 && response[1] == 0) {
        len = response[2] * 256 + response[3];
        // TODO checksum & their error handling
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

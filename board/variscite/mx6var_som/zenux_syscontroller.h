#ifndef __ZENUX_SYSCONTROLLER_H
#define __ZENUX_SYSCONTROLLER_H

#include <asm/types.h>
#include <stdbool.h>

#ifndef CONFIG_SPL_BUILD

bool probeSysController(void);
void readI2cLoop(void);      // remove after I2C tests
bool readCTRLVersion(char* receivedVersion);
bool readInstrumentClass(char* receivedClass);
bool readDisplayType(u8* receivedType);

#endif /* CONFIG_SPL_BUILD */

#endif /* __ZENUX_SYSCONTROLLER_H */

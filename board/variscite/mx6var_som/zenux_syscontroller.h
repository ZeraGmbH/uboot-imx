#ifndef __ZENUX_SYSCONTROLLER_H
#define __ZENUX_SYSCONTROLLER_H

#include <asm/types.h>
#include <stdbool.h>

#ifndef CONFIG_SPL_BUILD

bool probeSysController(void);
bool readCTRLVersion(char* receivedVersion);
bool readInstrumentClass(char* receivedClass);
bool readDisplayType(u8* receivedType);
bool enableLcdStartWatchdog(void);

#endif /* CONFIG_SPL_BUILD */

#endif /* __ZENUX_SYSCONTROLLER_H */

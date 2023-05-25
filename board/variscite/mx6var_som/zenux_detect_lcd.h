#ifndef __ZENUX_DETECT_LCD_H
#define __ZENUX_DETECT_LCD_H

#include <asm/types.h>
#include <stdbool.h>

#ifndef CONFIG_SPL_BUILD

void deduceLcd(u8 displayType);
void setLcdCom5003Initial(void);
void setLcdMt310s2Initial(void);
void setUbootEnvLcd(void);

#endif /* CONFIG_SPL_BUILD */

#endif /* __ZENUX_DETECT_LCD_H */

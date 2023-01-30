#ifndef CONFIG_SPL_BUILD
#include "zenux_device_detect.h"
#include "zenux_syscontroller.h"
#include <asm/io.h>
#include <common.h>
#include <i2c.h>

void assumeInitialCom5003(void);
void deduceSettingsFromSysController(void);

struct DeviceInfo
{
	enum DeviceTypes devType;
	enum LcdTypes lcdType;
} devInfo;

void zenux_device_detect(void)
{
	puts("ZENUX device detection started...\n");
	if(!probeSysController())
		assumeInitialCom5003();
	else
		deduceSettingsFromSysController();
}

enum DeviceTypes getDevType(void)
{
	return devInfo.devType;
}

void assumeInitialCom5003(void)
{
	puts("Syscontroller not found - assuming COM5003 / initial LCD\n");
	devInfo.devType = DEV_COM5003;
	devInfo.lcdType = LCD_COM5003_INITIAL;
}

void deduceSettingsFromSysController(void)
{
	// TODO: COM5003 new/MT310s2
	puts("Syscontroller found - assuming MT310s2\n");
	devInfo.devType = DEV_MT310S2;
	devInfo.lcdType = LCD_MT310S2_INITIAL;
}

#endif /* CONFIG_SPL_BUILD */

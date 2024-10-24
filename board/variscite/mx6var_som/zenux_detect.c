#ifndef CONFIG_SPL_BUILD

#include "zenux_detect.h"
#include "zenux_detect_class.h"
#include "zenux_detect_lcd.h"
#include "zenux_syscontroller.h"
#include "zenux_mcontroller_io.h"
#include <common.h>

static void deduceSettingsFromSysController(void);
static bool logCtrlVersion(void);
static void assumeInitialCom5003(void);
static void assumeInitialMt310s2(void);

void zenux_detect(void)
{
	puts("ZENUX device detection started...\n");
	enforceStopBitOnSyscontrollerI2c();

	if(!logCtrlVersion()) {
		puts("Syscontroller not found!\n");
		assumeInitialCom5003();
	}
	else {
		puts("Syscontroller found - deduce more details...\n");
		deduceSettingsFromSysController();
	}
	setUbootEnvClass();
	setUbootEnvLcd();
}

static void deduceSettingsFromSysController(void)
{
	char classFromDevice[MAX_READ_LEN_ZHARD];
	u8 displayType;
	if(readInstrumentClass(classFromDevice) && readDisplayType(&displayType)) {
		printf("Instrument class: %s\n", classFromDevice);
		printf("Display type: %i\n", displayType);
		const char* envClassName = deduceClass(classFromDevice);
		deduceLcd(displayType, envClassName);
	}
	else {
		puts("Instrument class an/or display type read failed!\n");
		assumeInitialMt310s2();
	}
}

static bool logCtrlVersion(void)
{
	char receivedData[MAX_READ_LEN_ZHARD];
	if(readCTRLVersion(receivedData)) {
		printf("Syscontroller version: %s\n", receivedData);
		return true;
	}
	puts("Syscontroller read version failed!\n");
	return false;
}

static void assumeInitialCom5003(void)
{
	puts("Assume COM5003 / initial LCD.\n");
	setClassCom5003();
	setLcdCom5003Initial();
}

static void assumeInitialMt310s2(void)
{
	puts("Assume MT310s2 / initial LCD.\n");
	setClassMt310s2();
	setLcdMt310s2Initial();
}

#endif /* CONFIG_SPL_BUILD */

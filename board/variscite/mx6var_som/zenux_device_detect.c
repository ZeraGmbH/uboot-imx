#ifndef CONFIG_SPL_BUILD
#include "zenux_device_detect.h"
#include "zenux_syscontroller.h"
#include "zenux_mcontroller_io.h"
#include <common.h>

static void assumeInitialCom5003(void);
static void assumeInitialMt310s2(void);
static bool logCtrlVersion(void);
static void deduceSettingsFromSysController(void);
static void deduceDeviceType(const char* instrumentClass);
static void setEnvMachine(void);
static void setEnvLcd(void);

enum DeviceTypes
{
	DEV_COM5003,
	DEV_MT310S2
};
char* envDeviceNames[] = {
	"com5003",
	"mt310s2"
};

enum LcdTypes
{
	LCD_COM5003_INITIAL,
	LCD_COM5003_INITIAL_1280x800_TM101JVHG32,
	LCD_MT310S2_INITIAL
};
char* envLcdDeviceTreeFileNames[] = {
	"imx6q-var-som-zera-com.dtb",
	"imx6q-var-som-zera-com.1280x800_Tianma_TM101JVHG32.dtb",
	"imx6q-var-som-zera-mt.dtb"
};

struct DeviceInfo
{
	enum DeviceTypes devType;
	enum LcdTypes lcdType;
} devInfo;

void zenux_device_detect(void)
{
	puts("ZENUX device detection started...\n");
	/* Background:
	   Systemcontroller is always on and switches off I2C pull up voltage in
	   power off state. On power on the systemcontroller switches on I2C pull
	   up voltage. At the time of writing this can cause systemcontroller's I2C
	   unit to NAK first communication request. Future versions of
	   systemcontroller will fix this by enabling I2C once pullup voltage is on.
	   So myabe then we can remove probing but it can remain - there is no harm
	   expected.
	 */
	probeSysController();

	if(!logCtrlVersion()) {
		puts("Syscontroller not found!\n");
		assumeInitialCom5003();
	}
	else {
		puts("Syscontroller found - deduce more details...\n");
		deduceSettingsFromSysController();
	}
	setEnvMachine();
	setEnvLcd();
}

static void deduceSettingsFromSysController(void)
{
	u8* instrumentClass[MAX_READ_LEN_ZHARD];
	u8 displayType;
	if(readInstrumentClass(instrumentClass) && readDisplayType(displayType)) {
		printf("Instrument class: %s\n", instrumentClass);
		printf("Display type: %i\n", displayType);
		deduceDeviceType(instrumentClass);
		// TODO - We have no information regarding displays yet
		devInfo.lcdType = LCD_COM5003_INITIAL;
	}
	else {
		puts("Instrument class read failed!\n");
		assumeInitialMt310s2();
	}
}

static void deduceDeviceType(const char* instrumentClass)
{
	// TODO
	devInfo.devType = DEV_COM5003;
}

static bool logCtrlVersion(void)
{
	u8* receivedData[MAX_READ_LEN_ZHARD];
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
	devInfo.devType = DEV_COM5003;
	devInfo.lcdType = LCD_COM5003_INITIAL;
}

static void assumeInitialMt310s2(void)
{
	puts("Assume MT310s2 / initial LCD.\n");
	devInfo.devType = DEV_MT310S2;
	devInfo.lcdType = LCD_MT310S2_INITIAL;
}

static void setEnvMachine(void)
{
	const char* envVarName = "zera_device";
	const char* envDeviceName = envDeviceNames[devInfo.devType];
	printf("Set environment variable '%s' to '%s'\n", envVarName, envDeviceName);
	env_set(envVarName, envDeviceName);
}

static void setEnvLcd(void)
{
	const char* envVarName = "fdt_file";
	const char* envLcdDeviceTreeFileName = envLcdDeviceTreeFileNames[devInfo.lcdType];
	printf("Set environment variable '%s' to '%s'\n", envVarName, envLcdDeviceTreeFileName);
	env_set(envVarName, envLcdDeviceTreeFileName);
}

#endif /* CONFIG_SPL_BUILD */

#ifndef CONFIG_SPL_BUILD
#include "zenux_device_detect.h"
#include "zenux_syscontroller.h"
#include "zenux_mcontroller_io.h"
#include <common.h>

void assumeInitialCom5003(void);
void deduceSettingsFromSysController(void);
void setEnvMachine(void);
void setEnvLcd(void);

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
	LCD_MT310S2_INITIAL
};
char* envLcdDeviceTreeFileNames[] = {
	"imx6q-var-som-zera-com.dtb",
	"imx6q-var-som-zera-mt.dtb"
};

struct DeviceInfo
{
	enum DeviceTypes devType;
	enum LcdTypes lcdType;
} devInfo;

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
		puts("Syscontroller not found - assuming COM5003 / initial LCD\n");
		assumeInitialCom5003();
	}
	else {
		puts("Syscontroller found - deduce more details...\n");
		deduceSettingsFromSysController();
	}
	setEnvMachine();
	setEnvLcd();
}

void assumeInitialCom5003(void)
{
	devInfo.devType = DEV_COM5003;
	devInfo.lcdType = LCD_COM5003_INITIAL;
}

void deduceSettingsFromSysController(void)
{
	u8* receivedData[MAX_READ_LEN_ZHARD];
	if(readInstrumentClass(receivedData)) {
		printf("Instrument class: %s\n", receivedData);
		// TODO Temp
		devInfo.devType = DEV_COM5003;
		devInfo.lcdType = LCD_COM5003_INITIAL;
	}
	else {
		puts("Instrument class read failed! Assume MT310s2\n");
		devInfo.devType = DEV_MT310S2;
		devInfo.lcdType = LCD_MT310S2_INITIAL;
	}
	if(readDisplayType(receivedData)) {
		printf("Display type: %s\n", receivedData);
	}
	else
		puts("Display type read failed! Assume MT310s2\n");
}

void setEnvMachine(void)
{
	const char* envVarName = "zera_device";
	const char* envDeviceName = envDeviceNames[devInfo.devType];
	printf("Set environment variable '%s' to '%s'\n", envVarName, envDeviceName);
	env_set(envVarName, envDeviceName);
}

void setEnvLcd(void)
{
	const char* envVarName = "fdt_file";
	const char* envLcdDeviceTreeFileName = envLcdDeviceTreeFileNames[devInfo.lcdType];
	printf("Set environment variable '%s' to '%s'\n", envVarName, envLcdDeviceTreeFileName);
	env_set(envVarName, envLcdDeviceTreeFileName);
}

#endif /* CONFIG_SPL_BUILD */

#ifndef CONFIG_SPL_BUILD
#include "zenux_device_detect.h"
#include "zenux_syscontroller.h"
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

void zenux_device_detect(void)
{
	puts("ZENUX device detection started...\n");
	if(!probeSysController()) {
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
	// TODO: COM5003 new/MT310s2
	char* receivedVersion[128];
	if(readCTRLVersion(receivedVersion))
		puts("Syscontroller version\n");
	else
		puts("Syscontroller read version failed!\n");
	devInfo.devType = DEV_MT310S2;
	devInfo.lcdType = LCD_MT310S2_INITIAL;
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

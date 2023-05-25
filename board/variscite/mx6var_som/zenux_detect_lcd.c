#ifndef CONFIG_SPL_BUILD

#include "zenux_detect_lcd.h"
#include <common.h>

enum LcdTypes
{
	LCD_COM5003_INITIAL,
	LCD_COM5003_1280x800_Tianma_TM101JVHG32,
	LCD_MT310S2_INITIAL,
};
const char* envLcdDeviceTreeFileNames[] = {
	"imx6q-var-som-zera-com.dtb",
	"imx6q-var-som-zera-com.1280x800_Tianma_TM101JVHG32.dtb",
	"imx6q-var-som-zera-mt.dtb"
};

enum LcdTypes lcdType = LCD_COM5003_INITIAL;

void deduceLcd(u8 displayType)
{
	// TODO - we have no docs on LCD display type values yet
	lcdType = LCD_COM5003_1280x800_Tianma_TM101JVHG32;
}

void setLcdCom5003Initial(void)
{
	lcdType = LCD_COM5003_INITIAL;
}

void setLcdMt310s2Initial(void)
{
	lcdType = LCD_MT310S2_INITIAL;
}

void setUbootEnvLcd(void)
{
	const char* envVarName = "fdt_file";
	const char* envLcdDeviceTreeFileName = envLcdDeviceTreeFileNames[(int)lcdType];
	printf("Set environment variable '%s' to '%s'\n", envVarName, envLcdDeviceTreeFileName);
	env_set(envVarName, envLcdDeviceTreeFileName);
}

#endif /* CONFIG_SPL_BUILD */

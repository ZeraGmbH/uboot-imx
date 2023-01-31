#ifndef __ZENUX_DEVICE_DETECT_H
#define __ZENUX_DEVICE_DETECT_H

#ifndef CONFIG_SPL_BUILD

enum DeviceTypes
{
	DEV_COM5003,
	DEV_MT310S2
};

enum LcdTypes
{
	LCD_COM5003_INITIAL,
	LCD_MT310S2_INITIAL
};

void zenux_device_detect(void);

#endif /* CONFIG_SPL_BUILD */

#endif /* __ZENUX_DEVICE_DETECT_H */

#ifndef CONFIG_SPL_BUILD

#include "zenux_detect_class.h"
#include <common.h>

enum ClassTypes
{
	CLASS_COM5003,
	CLASS_MT310S2
};
const char* instrumentClassNames[] = {
	"COM5003",
	"MT310s2"
};
const char* ubootEnvClassNames[] = {
	"com5003",
	"mt310s2"
};

enum ClassTypes classType = CLASS_COM5003;

const char* deduceClass(const char* instrumentClass)
{
	if(!strcmp(instrumentClass, "COM5003"))
		setClassCom5003();
	else
		setClassMt310s2();
	return ubootEnvClassNames[(int)classType];
}

void setClassCom5003(void)
{
	classType = CLASS_COM5003;
}

void setClassMt310s2(void)
{
	classType = CLASS_MT310S2;
}

void setUbootEnvClass(void)
{
	const char* envVarName = "zera_device";
	const char* envDeviceName = ubootEnvClassNames[(int)classType];
	printf("Set environment variable '%s' to '%s'\n", envVarName, envDeviceName);
	env_set(envVarName, envDeviceName);
}

#endif /* CONFIG_SPL_BUILD */

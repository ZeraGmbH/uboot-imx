#ifndef CONFIG_SPL_BUILD

#include "zenux_detect_class.h"
#include <common.h>

enum ClassTypes
{
	CLASS_COM5003,
	CLASS_MT310S2,
	CLASS_MT581S2
};

const char* ubootEnvClassNames[] = {
	"com5003",
	"mt310s2",
	"mt581s2"
};

enum ClassTypes classType = CLASS_COM5003;

const char* deduceClass(const char* instrumentClass)
{
	if(!strcmp(instrumentClass, "COM5003"))
		setClassCom5003();
	else if(!strcmp(instrumentClass, "MT581s2"))
		setClassMt581s2();
	else
		setClassMt310s2();
	return ubootEnvClassNames[(int)classType];
}

bool isClassCom5003(void)
{
	return classType == CLASS_COM5003;
}

bool isClassMt310s2(void)
{
	return classType == CLASS_MT310S2;
}

void setClassCom5003(void)
{
	classType = CLASS_COM5003;
}

void setClassMt310s2(void)
{
	classType = CLASS_MT310S2;
}

void setClassMt581s2(void)
{
	classType = CLASS_MT581S2;
}

void setUbootEnvClass(void)
{
	const char* envVarName = "zera_device";
	const char* envDeviceName = ubootEnvClassNames[(int)classType];
	printf("Set environment variable '%s' to '%s'\n", envVarName, envDeviceName);
	env_set(envVarName, envDeviceName);
}

#endif /* CONFIG_SPL_BUILD */

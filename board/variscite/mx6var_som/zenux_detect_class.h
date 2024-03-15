#ifndef __ZENUX_DETECT_CLASS_H
#define __ZENUX_DETECT_CLASS_H

#include <stdbool.h>

#ifndef CONFIG_SPL_BUILD

const char* deduceClass(const char* instrumentClass);

bool isClassCom5003(void);
bool isClassMt310s2(void);

void setClassCom5003(void);
void setClassMt310s2(void);
void setUbootEnvClass(void);

#endif /* CONFIG_SPL_BUILD */

#endif /* __ZENUX_DETECT_CLASS_H */

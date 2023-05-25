#ifndef __ZENUX_DETECT_CLASS_H
#define __ZENUX_DETECT_CLASS_H

#ifndef CONFIG_SPL_BUILD

void deduceClass(const char* instrumentClass);
void setClassCom5003(void);
void setClassMt310s2(void);
void setUbootEnvClass(void);

#endif /* CONFIG_SPL_BUILD */

#endif /* __ZENUX_DETECT_CLASS_H */

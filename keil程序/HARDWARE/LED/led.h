
#ifndef	__LED_H
#define __LED_H
#include "stm32f10x.h"
#include "sys.h"
void LED_Init(void);//≥ı ºªØ

#define LEDA8_OFF   GPIO_ResetBits(GPIOA,GPIO_Pin_8);
#define LEDA8_ON   GPIO_SetBits(GPIOA,GPIO_Pin_8);
#define LEDA5_OFF   GPIO_ResetBits(GPIOA,GPIO_Pin_5);
#define LEDA5_ON   GPIO_SetBits(GPIOA,GPIO_Pin_5);
#endif

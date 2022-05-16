#ifndef __TIMER1_H
#define __TIMER1_H
#include "stm32f10x.h"

extern int up_level[4];
extern int direction[4];

void TIM3_Int_Init(u16 arr,u16 psc);
 
#endif




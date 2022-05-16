#ifndef __PARSES_H
#define __PARSES_H
#include "stm32f10x.h"

extern uint32_t trans[4];

extern float cosa;
extern float cosa1;
extern float angle1;
extern float angle;
extern float dis2;
extern float dis3;

extern float zonghe;
extern float gaodu1;
extern float zonghe2;

float Data_Parses(void);
int htoi(u8 s[]);
#endif

#ifndef __TIMER_H
#define __TIMER_H
#include "stm32f10x.h"



#define PWM1 TIM_SetCompare1  // pb4  修改占空比的函数
#define PWM2 TIM_SetCompare2  // pb5



#define lanya GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)

#define zuolunzhenzhuan GPIO_ResetBits(GPIOA,GPIO_Pin_4);GPIO_SetBits(GPIOA,GPIO_Pin_5)
#define youlunzhenzhuan GPIO_ResetBits(GPIOA,GPIO_Pin_6);GPIO_SetBits(GPIOA,GPIO_Pin_7)

#define zuolunfanzhuan GPIO_SetBits(GPIOA,GPIO_Pin_4);GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define youlunfanzhuan GPIO_SetBits(GPIOA,GPIO_Pin_6);GPIO_ResetBits(GPIOA,GPIO_Pin_7)

#define zuoluntingzhuan GPIO_ResetBits(GPIOA,GPIO_Pin_4);GPIO_ResetBits(GPIOA,GPIO_Pin_5);GPIO_SetBits(GPIOB,GPIO_Pin_4)
#define youluntingzhuan GPIO_ResetBits(GPIOA,GPIO_Pin_6);GPIO_ResetBits(GPIOA,GPIO_Pin_7);GPIO_SetBits(GPIOB,GPIO_Pin_5)



void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM2_PWM_Init(u16 arr,u16 psc);
void dianjiduankou_Init(void);
void gostraight(int a,int b,int c,int d);
void turnleft(int a,int b);
void turnright(int a,int b);
void yuandixuanzhuan(int a,int b);
void tingzhi(void);
void daoche(int a,int b);
void dianjiduankou_Init1(void);
void TIM2_Int_Init(u16 arr,u16 psc);
int abs(int a);
void pingyi(int a, int b);
void xuanzhuan(int a, int b);

#endif




#include "timer1.h" 
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"
#include "led.h"
#include "car.h"
#define  time_t 100
//通用定时器2中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M	即定时器的主频为72M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器2

void TIM3_Int_Init(u16 arr,u16 psc)
{
		
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;				//定时器配置结构体定义
	NVIC_InitTypeDef NVIC_InitStructure;						//中断配置结构体定义

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);        //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr;    //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc;  //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //设置时钟分割:TDTS = Tck_tim	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM2中断,允许更新中断	TIM_IT_Update

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;        //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

		LED_Init();
	TIM_Cmd(TIM3, ENABLE);  //使能TIM2			 
}

int time_num=0;

void PWM_0()//右后轮
{
	if(time_num<up_level[0])
	{
		if(direction[0]==0)
		{
			GPIO_ResetBits(GPIOC,GPIO_Pin_11);
			GPIO_SetBits(GPIOD,GPIO_Pin_0);
		}	
		if(direction[0]==1)
		{
			GPIO_ResetBits(GPIOD,GPIO_Pin_0);
			GPIO_SetBits(GPIOC,GPIO_Pin_11);
		}			
	}
	if((time_num>=up_level[0])&&(time_t>=time_num))
	{
		GPIO_ResetBits(GPIOD,GPIO_Pin_0);
		GPIO_ResetBits(GPIOC,GPIO_Pin_11);	
	}		
}

void PWM_1()//右前轮
{
	if(time_num<up_level[1])
	{
		if(direction[1]==0)
		{
			GPIO_ResetBits(GPIOG,GPIO_Pin_9);
			GPIO_SetBits(GPIOD,GPIO_Pin_6);
		}	
		if(direction[1]==1)
		{
			GPIO_ResetBits(GPIOD,GPIO_Pin_6);
			GPIO_SetBits(GPIOG,GPIO_Pin_9);
		}			
	}
	if((time_num>=up_level[1])&&(time_t>=time_num))
	{
		GPIO_ResetBits(GPIOD,GPIO_Pin_6);
		GPIO_ResetBits(GPIOG,GPIO_Pin_9);	
	}
		
}

void PWM_2()//左前轮
{
	if(time_num<up_level[2])
	{
		if(direction[2]==0)
		{
			GPIO_ResetBits(GPIOA,GPIO_Pin_7);
			GPIO_SetBits(GPIOA,GPIO_Pin_5);
		}	
		if(direction[2]==1)
		{
			GPIO_ResetBits(GPIOA,GPIO_Pin_5);
			GPIO_SetBits(GPIOA,GPIO_Pin_7);
		}			
	}
	if((time_num>=up_level[2])&&(time_t>=time_num))
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);	
	}
}


void PWM_3()//左后轮
{
	if(time_num<up_level[3])
	{
		if(direction[3]==0)
		{
			GPIO_ResetBits(GPIOD,GPIO_Pin_3);
			GPIO_SetBits(GPIOD,GPIO_Pin_1);
		}	
		if(direction[3]==1)
		{
			GPIO_ResetBits(GPIOD,GPIO_Pin_1);
			GPIO_SetBits(GPIOD,GPIO_Pin_3);
		}			
	}
	if((time_num>=up_level[3])&&(time_t>=time_num))
	{
		GPIO_ResetBits(GPIOD,GPIO_Pin_1);
		GPIO_ResetBits(GPIOD,GPIO_Pin_3);	
	}
}
//定时器2中断服务程序
void TIM3_IRQHandler(void)   //TIM2中断
{

	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM2更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update );   //清除TIM2更新中断标志 
		time_num++;
		PWM_0();
		PWM_1();
		PWM_2();
		PWM_3();
		if(time_num>100)
				time_num=0;
			
	}
}	














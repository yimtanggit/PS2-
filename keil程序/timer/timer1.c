#include "timer1.h" 
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"
#include "led.h"
#include "car.h"
#define  time_t 100
//ͨ�ö�ʱ��2�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M	����ʱ������ƵΪ72M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��2

void TIM3_Int_Init(u16 arr,u16 psc)
{
		
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;				//��ʱ�����ýṹ�嶨��
	NVIC_InitTypeDef NVIC_InitStructure;						//�ж����ýṹ�嶨��

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);        //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr;    //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc;  //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //����ʱ�ӷָ�:TDTS = Tck_tim	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM2�ж�,��������ж�	TIM_IT_Update

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;        //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

		LED_Init();
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM2			 
}

int time_num=0;

void PWM_0()//�Һ���
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

void PWM_1()//��ǰ��
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

void PWM_2()//��ǰ��
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


void PWM_3()//�����
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
//��ʱ��2�жϷ������
void TIM3_IRQHandler(void)   //TIM2�ж�
{

	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM2�����жϷ������
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update );   //���TIM2�����жϱ�־ 
		time_num++;
		PWM_0();
		PWM_1();
		PWM_2();
		PWM_3();
		if(time_num>100)
				time_num=0;
			
	}
}	














#include "wdg.h"
#include "led.h"
#include  "delay.h"


void IWDG_Init(u8 prer,u16 rlr) 
{	
 	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  //ʹ�ܶԼĴ���IWDG_PR��IWDG_RLR��д����
	
	IWDG_SetPrescaler(prer);  //����IWDGԤ��Ƶֵ:����IWDGԤ��ƵֵΪ64
	
	IWDG_SetReload(rlr);  //����IWDG��װ��ֵ
	
	IWDG_ReloadCounter();  //����IWDG��װ�ؼĴ�����ֵ��װ��IWDG������
	
	IWDG_Enable();  //ʹ��IWDG
}
//ι�������Ź�
void IWDG_Feed(void)
{   
 	IWDG_ReloadCounter();	//���ؼ���ֵ									   
}


//����WWDG������������ֵ,Ĭ��Ϊ���. 
u8 WWDG_CNT=0x7f; 
//��ʼ�����ڿ��Ź� 	
//tr   :T[6:0],�ݼ�������ֵ��ȡֵ��Χ��0x7f-0x40 
//wr   :W[6:0],����ֵ ,ȡֵ��Χ��0x7f-0x40 
//prvp:��Ƶϵ����WDGTB��,�����2λ��Ч 
//Fwwdg=PCLK1/(4096*2^prv). 
// * @arg WWDG_Prescaler_1: WWDG counter clock = (PCLK1(45MHz)/4096)/1
// Լ10968Hz 91us
// * @arg WWDG_Prescaler_2: WWDG counter clock = (PCLK1(45MHz)/4096)/2
// Լ5484Hz 182us
// * @arg WWDG_Prescaler_4: WWDG counter clock = (PCLK1(45MHz)/4096)/4
// Լ2742Hz 364us
// * @arg WWDG_Prescaler_8: WWDG counter clock = (PCLK1(45MHz)/4096)/8
// Լ1371Hz 728us

void WWDG_Init(u8 tr,u8 wr,u32 prv)
{ 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);  //   WWDGʱ��ʹ��

	WWDG_CNT=tr&WWDG_CNT;   //��ʼ��WWDG_CNT.   
	WWDG_SetPrescaler(prv);////����IWDGԤ��Ƶֵ
	WWDG_SetWindowValue(wr);//���ô���ֵ
	WWDG_Enable(WWDG_CNT);	 //ʹ�ܿ��Ź�             
	WWDG_ClearFlag();//�����ǰ�����жϱ�־λ 
	WWDG_NVIC_Init();//��ʼ�����ڿ��Ź� NVIC
	WWDG_EnableIT(); //�������ڿ��Ź��ж�
} 
//������WWDG��������ֵ
void WWDG_Set_Counter(u8 cnt)
{
    WWDG_Enable(cnt);//ʹ�ܿ��Ź� ,	���� counter .	 
}
//���ڿ��Ź��жϷ������
void WWDG_NVIC_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;    //WWDG�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   //��ռ2�������ȼ�3����2	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	 //��ռ2�������ȼ�3����2	
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE; 
	NVIC_Init(&NVIC_InitStructure);//NVIC��ʼ��
}

void WWDG_IRQHandler(void)
{

	WWDG_SetCounter(WWDG_CNT);	  //�������˾��,���ڿ��Ź���������λ

	WWDG_ClearFlag();	  //�����ǰ�����жϱ�־λ


	
}


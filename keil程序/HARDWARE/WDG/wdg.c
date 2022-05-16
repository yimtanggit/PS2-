#include "wdg.h"
#include "led.h"
#include  "delay.h"


void IWDG_Init(u8 prer,u16 rlr) 
{	
 	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  //使能对寄存器IWDG_PR和IWDG_RLR的写操作
	
	IWDG_SetPrescaler(prer);  //设置IWDG预分频值:设置IWDG预分频值为64
	
	IWDG_SetReload(rlr);  //设置IWDG重装载值
	
	IWDG_ReloadCounter();  //按照IWDG重装载寄存器的值重装载IWDG计数器
	
	IWDG_Enable();  //使能IWDG
}
//喂独立看门狗
void IWDG_Feed(void)
{   
 	IWDG_ReloadCounter();	//重载计数值									   
}


//保存WWDG计数器的设置值,默认为最大. 
u8 WWDG_CNT=0x7f; 
//初始化窗口看门狗 	
//tr   :T[6:0],递减计数器值，取值范围：0x7f-0x40 
//wr   :W[6:0],窗口值 ,取值范围：0x7f-0x40 
//prvp:分频系数（WDGTB）,仅最低2位有效 
//Fwwdg=PCLK1/(4096*2^prv). 
// * @arg WWDG_Prescaler_1: WWDG counter clock = (PCLK1(45MHz)/4096)/1
// 约10968Hz 91us
// * @arg WWDG_Prescaler_2: WWDG counter clock = (PCLK1(45MHz)/4096)/2
// 约5484Hz 182us
// * @arg WWDG_Prescaler_4: WWDG counter clock = (PCLK1(45MHz)/4096)/4
// 约2742Hz 364us
// * @arg WWDG_Prescaler_8: WWDG counter clock = (PCLK1(45MHz)/4096)/8
// 约1371Hz 728us

void WWDG_Init(u8 tr,u8 wr,u32 prv)
{ 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);  //   WWDG时钟使能

	WWDG_CNT=tr&WWDG_CNT;   //初始化WWDG_CNT.   
	WWDG_SetPrescaler(prv);////设置IWDG预分频值
	WWDG_SetWindowValue(wr);//设置窗口值
	WWDG_Enable(WWDG_CNT);	 //使能看门狗             
	WWDG_ClearFlag();//清除提前唤醒中断标志位 
	WWDG_NVIC_Init();//初始化窗口看门狗 NVIC
	WWDG_EnableIT(); //开启窗口看门狗中断
} 
//重设置WWDG计数器的值
void WWDG_Set_Counter(u8 cnt)
{
    WWDG_Enable(cnt);//使能看门狗 ,	设置 counter .	 
}
//窗口看门狗中断服务程序
void WWDG_NVIC_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;    //WWDG中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   //抢占2，子优先级3，组2	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	 //抢占2，子优先级3，组2	
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE; 
	NVIC_Init(&NVIC_InitStructure);//NVIC初始化
}

void WWDG_IRQHandler(void)
{

	WWDG_SetCounter(WWDG_CNT);	  //当禁掉此句后,窗口看门狗将产生复位

	WWDG_ClearFlag();	  //清除提前唤醒中断标志位


	
}


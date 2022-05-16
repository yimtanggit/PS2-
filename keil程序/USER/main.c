#include "sys.h"
#include "delay.h"
#include "led.h"
#include "timer.h"
#include "timer1.h"
#include "oled.h"
#include "usart.h"
#include "stdio.h"	
#include "string.h"
#include "ctype.h"
#include "math.h"
#include "kalman.h" 
#include "car.h" 
#include "parses.h" 
#include "bsp_adc.h"
#include "delay.h"
#include "ps2.h"
#include "SysTick.h"

float gaodu = 0.00000;
char shuju[10] = {0};
char shuju1[1] = {" "};

int main(void)
{
//	float kp = 1.5;   //0.9  小轮
//  float kd = 0.4;   //0.2
//	float lastzonghe1 = 0.000000;
	int PS2_KEY = 0, X1=0,Y1=0,X2=0,Y2=0; 
	int tang = 50;
		SysTick_Init();						  //SysTick函数初始化	
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;	 //使能总算法时钟
	delay_init();	    //延时函数初始化	  
	//OLED_Init();			//初始化OLED  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	dianjiduankou_Init();
	//dianjiduankou_Init1();
	TIM3_PWM_Init(199,7200);	
//	uart1_init(115200);	//串口初始化为115200   
//	uart3_init(9600);
	TIM2_PWM_Init(199,7200);
	//TIM2_Int_Init(9999,7199); 
	//printf("Start.\r\n");
	delay_ms(1000);                 //=====延时等待初始化稳定
	PS2_Init();											//=====ps2驱动端口初始化
	PS2_SetInit();		 							//=====ps2配置初始化,配置“红绿灯模式”，并选择是否可以修改
	delay_ms(1000);
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);

	while(1)	
	{	
		PS2_KEY = PS2_DataKey();
//		switch(PS2_KEY)
//			{
//				
//				case PSB_PAD_UP: gostraight(150,198,150,150);	  break;  
//				case PSB_PAD_RIGHT:	gostraight(150,198,150,150);  break;
//				case PSB_PAD_DOWN:gostraight(150,198,150,150);  break; 
//				case PSB_PAD_LEFT:	gostraight(150,198,150,150);  break;                                                                                                        
//				default:gostraight(0,0,0,0); break; 
//			}
		
					
		
		    X1 = PS2_AnologData(PSS_LX);
				Y1 = PS2_AnologData(PSS_LY);
				X2 = PS2_AnologData(PSS_RX);
				Y2 = PS2_AnologData(PSS_RY);

//    gostraight(0,(X1-127)/0.638,0,0);
//		delay_ms(100);	
//		if(PS2_KEY == PSB_R2)
//			GPIO_SetBits(GPIOA,GPIO_Pin_12);
//		else
//			GPIO_ResetBits(GPIOA,GPIO_Pin_12);
	//	 gostraight(0,(X1-127)/0.638,0,0);
	

	
	if((X2-127)*(X2-127) + (127-Y2)*(127-Y2) >= 900)
	   pingyi(X2,Y2);
	else if((X1-127)*(X1-127) + (127-Y1)*(127-Y1) >= 900)
		 xuanzhuan(X1,Y1);
	 else
		 gostraight(0,0,0,0);
	 
	 if(PS2_KEY == PSB_R2)
	 {
		 GPIO_SetBits(GPIOA,GPIO_Pin_12);
		 
	 }
	 else
		 GPIO_ResetBits(GPIOA,GPIO_Pin_12);
	 if(!PS2_RedLight)
		 gostraight(0,0,0,0);
      delay_ms(100);
	}
	
}




				 


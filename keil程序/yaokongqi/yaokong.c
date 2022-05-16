#include "ps2.h"
#include "systick.h"
#include "sys.h"
#include "delay.h"






#define DELAY_TIME  delay_us(5); 
u16 Handkey;																							 //用于存储按键值
u8 Comd[2]={0x01,0x42};																		 //0x01开始命令，0x42请求数据（根据数据手册）
u8 Data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //存储接收到的数据
u16 MASK[]={
    PSB_SELECT,
    PSB_L3,
    PSB_R3 ,
    PSB_START,
    PSB_PAD_UP,
    PSB_PAD_RIGHT,
    PSB_PAD_DOWN,
    PSB_PAD_LEFT,
    PSB_L2,
    PSB_R2,
    PSB_L1,
    PSB_R1 ,
    PSB_GREEN,
    PSB_RED,
    PSB_BLUE,
    PSB_PINK
	};	//按键值与按键明
 
/*****************************************************
* @function     PS2手柄引脚初始化
* @param        无
* @file         PS2.c
* @author       hui
* @version      V0.1
* @date         2020.7.29
* @brief        PS2手柄初始化
*****************************************************/
void PS2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	
	GPIO_PinRemapConfig(GPIO_Remap_USART1, DISABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//禁用JTAG 否则PB4不能当普通管脚使用 不能重映射
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  	/*开启GPIOA的外设时钟*/
	
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 |GPIO_Pin_12;	 //DO  CS CLK
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      /*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
GPIO_ResetBits(GPIOA,GPIO_Pin_12);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	//DI
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;     
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	DO_H;
	CLC_H;
	CS_H;  	 	 											  
}


/*****************************************************
* @function     向手柄发送指令
* @param        指令CMD
* @file         PS2.c
* @author       hui
* @version      V0.1
* @date         2020.7.29
* @brief        无返回值
*****************************************************/
void PS2_Cmd(u8 CMD)
{
	volatile u16 ref=0x01;
	Data[1] = 0;
	for(ref=0x01;ref<0x0100;ref<<=1)
	{
		if(ref&CMD)
		{
			DO_H;                   //输出一位控制位
		}
		else DO_L;

		CLC_H;                    //时钟拉高
		DELAY_TIME;
		CLC_L;
		DELAY_TIME;
		CLC_H;
		if(DI)
			Data[1] = ref|Data[1];
	}
	delay_us(16);
}


/*****************************************************
* @function     手柄模式：①红灯模式 ②绿灯模式
* @param        指令CMD
* @file         PS2.c
* @author       hui
* @version      V0.1
* @date         2020.7.29
* @brief        返回值：0（红灯模式），1（绿灯模式）
*								0x41=模拟绿灯，0x73=模拟红灯
*****************************************************/
u8 PS2_RedLight(void)
{
	CS_L;
	PS2_Cmd(Comd[0]);  //开始命令
	PS2_Cmd(Comd[1]);  //请求数据
	CS_H;
	if( Data[1] == 0X73)   
		return 0 ;
	else 
		return 1;
}


/*****************************************************
* @function     读取手柄数据
* @param        无
* @file         PS2.c
* @author       hui
* @version      V0.1
* @date         2020.7.29
* @brief        无返回值
*****************************************************/
void PS2_ReadData(void)
{
	volatile u8 byte=0;
	volatile u16 ref=0x01;
	CS_L;
	PS2_Cmd(Comd[0]);  					//开始命令
	PS2_Cmd(Comd[1]); 				  //请求数据
	for(byte=2;byte<9;byte++)   //开始接受数据
	{
		for(ref=0x01;ref<0x100;ref<<=1)
		{
			CLC_H;
			DELAY_TIME;
			CLC_L;
			DELAY_TIME;
			CLC_H;
		      if(DI)
		      Data[byte] = ref|Data[byte];
		}
        delay_us(16);
	}
	CS_H;
}

 
/*****************************************************
* @function     处理读取出来的手柄数据
* @param        无
* @file         PS2.c
* @author       hui
* @version      V0.1
* @date         2020.7.29
* @brief        只有一个按键按下时按下为0， 未按下为1
*****************************************************/
u8 PS2_DataKey()
{
	u8 index;

	PS2_ClearData();
	PS2_ReadData();

	Handkey=(Data[4]<<8)|Data[3];     //这是16个按键  按下为0， 未按下为1
	for(index=0;index<16;index++)
	{	    
		if((Handkey&(1<<(MASK[index]-1)))==0)
			return index+1;
	}
	return 0;          //没有任何按键按下
}


/*****************************************************
* @function     获取手柄遥感模拟值：范围0~256
* @param        button
* @file         PS2.c
* @author       hui
* @version      V0.1
* @date         2020.7.29
* @brief        返回按键值
*****************************************************/
u8 PS2_AnologData(u8 button)
{
	return Data[button];
}


/*****************************************************
* @function     清除数据缓冲区
* @param        无
* @file         PS2.c
* @author       hui
* @version      V0.1
* @date         2020.7.29
* @brief        
*****************************************************/
void PS2_ClearData()
{
	u8 index;
	for(index=0;index<9;index++)
		Data[index]=0x00;
}


/*****************************************************
* @function     手柄震动
* @param        motor1:右侧小震动电机，0x00关，其他开
*								motor2:左侧大震动电机，0x40~0xFF，电机开，值越大，震动越大
* @file         PS2.c
* @author       hui
* @version      V0.1
* @date         2020.7.29
* @brief        
*****************************************************/
void PS2_Vibration(u8 motor1, u8 motor2)
{
	CS_L;
	delay_us(16);
    PS2_Cmd(0x01);  //开始命令
	PS2_Cmd(0x42); 		//请求数据
	PS2_Cmd(0X00);
	PS2_Cmd(motor1);
	PS2_Cmd(motor2);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	CS_H;
	delay_us(16);  
}


/*****************************************************
* @function     小幅度震动
*****************************************************/
void PS2_ShortPoll(void)
{
	CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x42);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x00);
	CS_H;
	delay_us(16);	
}


/*****************************************************
* @function     PS2配置
*****************************************************/
void PS2_EnterConfing(void)
{
    CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01);
	PS2_Cmd(0x00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	CS_H;
	delay_us(16);
}


/*****************************************************
* @function     发送模式设置
*****************************************************/
void PS2_TurnOnAnalogMode(void)
{
	CS_L;
	PS2_Cmd(0x01);  
	PS2_Cmd(0x44);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01); 	//analog=0x01;digital=0x00  软件设置发送模式
	PS2_Cmd(0xEE);  //Ox03锁存设置，即不可通过按键“MODE”设置模式。
									//0xEE不锁存软件设置，可通过按键“MODE”设置模式。
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	CS_H;
	delay_us(16);
}


/*****************************************************
* @function    振动设置
*****************************************************/
void PS2_VibrationMode(void)
{
	CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x4D);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0X01);
	CS_H;
	delay_us(16);	
}


/*****************************************************
* @function    完成并保存配置
*****************************************************/
void PS2_ExitConfing(void)
{
    CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	CS_H;
	delay_us(16);
}


/*****************************************************
* @function    手柄配置初始化
*****************************************************/
void PS2_SetInit(void)
{
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_EnterConfing();			//进入配置模式
	PS2_TurnOnAnalogMode();	//“红绿灯”配置模式，并选择是否保存
	PS2_VibrationMode();	//开启震动模式
	PS2_ExitConfing();		  //完成并保存配置
}

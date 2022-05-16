#include "ps2.h"
#include "systick.h"
#include "sys.h"
#include "delay.h"






#define DELAY_TIME  delay_us(5); 
u16 Handkey;																							 //���ڴ洢����ֵ
u8 Comd[2]={0x01,0x42};																		 //0x01��ʼ���0x42�������ݣ����������ֲᣩ
u8 Data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //�洢���յ�������
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
	};	//����ֵ�밴����
 
/*****************************************************
* @function     PS2�ֱ����ų�ʼ��
* @param        ��
* @file         PS2.c
* @author       hui
* @version      V0.1
* @date         2020.7.29
* @brief        PS2�ֱ���ʼ��
*****************************************************/
void PS2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	
	GPIO_PinRemapConfig(GPIO_Remap_USART1, DISABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//����JTAG ����PB4���ܵ���ͨ�ܽ�ʹ�� ������ӳ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  	/*����GPIOA������ʱ��*/
	
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 |GPIO_Pin_12;	 //DO  CS CLK
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      /*��������ģʽΪͨ���������*/
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
* @function     ���ֱ�����ָ��
* @param        ָ��CMD
* @file         PS2.c
* @author       hui
* @version      V0.1
* @date         2020.7.29
* @brief        �޷���ֵ
*****************************************************/
void PS2_Cmd(u8 CMD)
{
	volatile u16 ref=0x01;
	Data[1] = 0;
	for(ref=0x01;ref<0x0100;ref<<=1)
	{
		if(ref&CMD)
		{
			DO_H;                   //���һλ����λ
		}
		else DO_L;

		CLC_H;                    //ʱ������
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
* @function     �ֱ�ģʽ���ٺ��ģʽ ���̵�ģʽ
* @param        ָ��CMD
* @file         PS2.c
* @author       hui
* @version      V0.1
* @date         2020.7.29
* @brief        ����ֵ��0�����ģʽ����1���̵�ģʽ��
*								0x41=ģ���̵ƣ�0x73=ģ����
*****************************************************/
u8 PS2_RedLight(void)
{
	CS_L;
	PS2_Cmd(Comd[0]);  //��ʼ����
	PS2_Cmd(Comd[1]);  //��������
	CS_H;
	if( Data[1] == 0X73)   
		return 0 ;
	else 
		return 1;
}


/*****************************************************
* @function     ��ȡ�ֱ�����
* @param        ��
* @file         PS2.c
* @author       hui
* @version      V0.1
* @date         2020.7.29
* @brief        �޷���ֵ
*****************************************************/
void PS2_ReadData(void)
{
	volatile u8 byte=0;
	volatile u16 ref=0x01;
	CS_L;
	PS2_Cmd(Comd[0]);  					//��ʼ����
	PS2_Cmd(Comd[1]); 				  //��������
	for(byte=2;byte<9;byte++)   //��ʼ��������
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
* @function     �����ȡ�������ֱ�����
* @param        ��
* @file         PS2.c
* @author       hui
* @version      V0.1
* @date         2020.7.29
* @brief        ֻ��һ����������ʱ����Ϊ0�� δ����Ϊ1
*****************************************************/
u8 PS2_DataKey()
{
	u8 index;

	PS2_ClearData();
	PS2_ReadData();

	Handkey=(Data[4]<<8)|Data[3];     //����16������  ����Ϊ0�� δ����Ϊ1
	for(index=0;index<16;index++)
	{	    
		if((Handkey&(1<<(MASK[index]-1)))==0)
			return index+1;
	}
	return 0;          //û���κΰ�������
}


/*****************************************************
* @function     ��ȡ�ֱ�ң��ģ��ֵ����Χ0~256
* @param        button
* @file         PS2.c
* @author       hui
* @version      V0.1
* @date         2020.7.29
* @brief        ���ذ���ֵ
*****************************************************/
u8 PS2_AnologData(u8 button)
{
	return Data[button];
}


/*****************************************************
* @function     ������ݻ�����
* @param        ��
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
* @function     �ֱ���
* @param        motor1:�Ҳ�С�𶯵����0x00�أ�������
*								motor2:�����𶯵����0x40~0xFF���������ֵԽ����Խ��
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
    PS2_Cmd(0x01);  //��ʼ����
	PS2_Cmd(0x42); 		//��������
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
* @function     С������
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
* @function     PS2����
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
* @function     ����ģʽ����
*****************************************************/
void PS2_TurnOnAnalogMode(void)
{
	CS_L;
	PS2_Cmd(0x01);  
	PS2_Cmd(0x44);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01); 	//analog=0x01;digital=0x00  ������÷���ģʽ
	PS2_Cmd(0xEE);  //Ox03�������ã�������ͨ��������MODE������ģʽ��
									//0xEE������������ã���ͨ��������MODE������ģʽ��
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	CS_H;
	delay_us(16);
}


/*****************************************************
* @function    ������
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
* @function    ��ɲ���������
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
* @function    �ֱ����ó�ʼ��
*****************************************************/
void PS2_SetInit(void)
{
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_EnterConfing();			//��������ģʽ
	PS2_TurnOnAnalogMode();	//�����̵ơ�����ģʽ����ѡ���Ƿ񱣴�
	PS2_VibrationMode();	//������ģʽ
	PS2_ExitConfing();		  //��ɲ���������
}

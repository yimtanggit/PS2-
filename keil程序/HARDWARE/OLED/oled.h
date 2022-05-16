#ifndef __OLED_H
#define __OLED_H
#include "stm32f10x.h"			  	 

#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����
		    						  
//-----------------OLED�˿ڶ���----------------  					   

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_3)
#define OLED_SCLK_Set() GPIO_SetBits(GPIOB,GPIO_Pin_3)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_4)
#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_4)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_5)
#define OLED_RST_Set() GPIO_SetBits(GPIOB,GPIO_Pin_5)

#define OLED_DC_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_6)
#define OLED_DC_Set() GPIO_SetBits(GPIOB,GPIO_Pin_6)

#define OLED_CS_Clr()  GPIO_ResetBits(GPIOB,GPIO_Pin_7)
#define OLED_CS_Set()  GPIO_SetBits(GPIOB,GPIO_Pin_7)


//OLEDģʽ����
//����8080ģʽ

#define SIZE 15
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	
extern int Bluetooth_Sign;
extern int WIFI_Sign;
extern int Flashlight_Sign;
extern int Electricity_Sign;

//OLED�����ú���
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr);//һ���ַ�
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2);
void OLED_ShowString(u8 x,u8 y, u8 *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_ShowCHinese32x32(u8 x,u8 y,u8 no);
void OLED_ShowCHinese64x64(u32 x,u32 y,u32 no);
void OLED_ShowCHinese16x32(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);

void BluetoothDisplay    (void);          //����
void SignalDisplay       (void);          //�ź�
void WifiDisplay         (void);          //WIFI
void Status_Bar          (void);          //״̬�� 		     
void Home_init(void);
void display_init(void);
void Ainformation_error(void);
void ATinformation_error(void);
#endif  
	 




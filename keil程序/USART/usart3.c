

/**********************************************************************************
 * ����3 ����
 * ����3 PB10 PB11���� �� �ж���Ӧ����
**********************************************************************************/

#include "usart3.h"
#include "usart.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"
#include "stdio.h"


//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 0
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      

		USART3->DR = (u32) ch; 
  	while((USART3->SR&0X40)==0);//ѭ������,ֱ���������   

	return ch;
}
#endif 

void uart3_init(u32 bound)
{
    //GPIO�˿�����
  GPIO_InitTypeDef	GPIO_InitStructure;				   //IO�����ýṹ��
	USART_InitTypeDef USART_InitStructure;				   //�������ýṹ��
//	NVIC_InitTypeDef NVIC_InitStructure;				   //�ж����ýṹ��
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��USART3��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
 	USART_DeInit(USART3);  //��λ����3
 //USART3_TX   PB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
 
	//USART3_RX	  PB.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ��PB11

   //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ


  USART_Init(USART3, &USART_InitStructure); //��ʼ������
   //Usart1 NVIC ����
//  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

 
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�
  USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 

}

void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

void Usart3_SendStr(u8 *s)
{
  while (*s)                  //����ַ���������־
  {
    while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);    
    USART_SendData(USART3,*s++);

  }
}
//���� ������һ��16λ����USART_RX_STA ����ʾ �ɼ������ݳ���  ����״̬�� �൱��һ���Ĵ���
//USART_RX_STA     15		    14	         13-0
//				 �������	���յ�0x0d	  ���յ����ݳ���  û���ռ�1 ��ʾ����һ���ֽ�
//USART_RX_STA=0 ��Ϊ����������׼��

//���ڽ����жϵ�ǰ���� ���ݵ�����Իس�Ϊ׼  ��  0x0d 0x0a  

void USART3_IRQHandler(void)                	//����1�ж���Ӧ����		 �����ֲ�����㶨��
	{
													//�����ڽ��յ�����  RXNE������1 
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
			USART_ClearITPendingBit(USART3, USART_IT_IDLE); 
		
     } 

} 

















































#ifndef __USART3_H
#define __USART3_H
#include "stdio.h"	

#include "string.h"
#include "ctype.h"
#include "stm32f10x.h"

#define USART_REC_LEN  			200  	//�����������ֽ��� 200



void Usart3_SendStr(u8 *s);

void uart3_init(u32 bound);	  	
//extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
//extern u16 USART_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��

#endif



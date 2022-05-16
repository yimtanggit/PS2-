#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "string.h"
#include "ctype.h"
#include "stm32f10x.h"

#define DMA_Rec_Len 400

extern u8  DMA_Rece_Buf[DMA_Rec_Len];	//DMA接收串口数据缓冲区
extern u16 Usart1_Rec_Cnt;            //本帧数据长度	

void uart1_init(u32 bound);
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx);
void uart3_init(u32 bound);
void USART3_Send_Data(char *buf,u8 len);
#endif




#ifndef __ADC_H
#define	__ADC_H


#include "stm32f10x.h"

#define    ADCx                          ADC1

// ADC 通道宏定义
#define    ADC_CHANNEL                   ADC_Channel_6

// ADC 中断相关宏定义
#define    ADC_IRQ                       ADC1_2_IRQn
#define    ADC_IRQHandler                ADC1_2_IRQHandler

//#define    ADC_IRQ                       ADC3_IRQn
//#define    ADC_IRQHandler                ADC3_IRQHandler

extern __IO uint16_t ADC_ConvertedValue;

void ADCx_Init(void);


#endif /* __ADC_H */


#include "bsp_adc.h"

__IO uint16_t ADC_ConvertedValue;


static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;// �� ADC IO�˿�ʱ��
	GPIO_PinRemapConfig(GPIO_Remap_USART1, DISABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//����JTAG
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOA , ENABLE );// ���� ADC IO ����ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	// ����Ϊģ������	
	GPIO_Init(GPIOA, &GPIO_InitStructure);	// ��ʼ�� ADC IO			
}


static void ADCx_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;	// ��ADCʱ��
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_ADC1, ENABLE );// ADC ģʽ����
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;// ֻʹ��һ��ADC�����ڶ���ģʽ	
	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 	// ��ֹɨ��ģʽ����ͨ����Ҫ����ͨ������Ҫ	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;// ����ת��ģʽ	
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;// �����ⲿ����ת���������������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;// ת������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;// ת��ͨ��1��	
	ADC_Init(ADCx, &ADC_InitStructure);// ��ʼ��ADC
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); // ����ADCʱ��ΪPCLK2��8��Ƶ����9MHz
	ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);// ���� ADC ͨ��ת��˳��Ͳ���ʱ��
	ADC_ITConfig(ADCx, ADC_IT_EOC, ENABLE);// ADC ת�����������жϣ����жϷ�������ж�ȡת��ֵ
	ADC_Cmd(ADCx, ENABLE);// ����ADC ������ʼת��
	ADC_ResetCalibration(ADCx);// ��ʼ��ADC У׼�Ĵ���  
	while(ADC_GetResetCalibrationStatus(ADCx));// �ȴ�У׼�Ĵ�����ʼ�����
	ADC_StartCalibration(ADCx);// ADC��ʼУ׼
	while(ADC_GetCalibrationStatus(ADCx));// �ȴ�У׼���
	ADC_SoftwareStartConvCmd(ADCx, ENABLE);// ����û�в����ⲿ����������ʹ���������ADCת�� 
}


void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_Mode_Config();

}
/*********************************************END OF FILE**********************/


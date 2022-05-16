#include "bsp_adc.h"

__IO uint16_t ADC_ConvertedValue;


static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;// 打开 ADC IO端口时钟
	GPIO_PinRemapConfig(GPIO_Remap_USART1, DISABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//禁用JTAG
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOA , ENABLE );// 配置 ADC IO 引脚模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	// 必须为模拟输入	
	GPIO_Init(GPIOA, &GPIO_InitStructure);	// 初始化 ADC IO			
}


static void ADCx_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;	// 打开ADC时钟
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_ADC1, ENABLE );// ADC 模式配置
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;// 只使用一个ADC，属于独立模式	
	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 	// 禁止扫描模式，多通道才要，单通道不需要	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;// 连续转换模式	
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;// 不用外部触发转换，软件开启即可
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;// 转换结果右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;// 转换通道1个	
	ADC_Init(ADCx, &ADC_InitStructure);// 初始化ADC
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); // 配置ADC时钟为PCLK2的8分频，即9MHz
	ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);// 配置 ADC 通道转换顺序和采样时间
	ADC_ITConfig(ADCx, ADC_IT_EOC, ENABLE);// ADC 转换结束产生中断，在中断服务程序中读取转换值
	ADC_Cmd(ADCx, ENABLE);// 开启ADC ，并开始转换
	ADC_ResetCalibration(ADCx);// 初始化ADC 校准寄存器  
	while(ADC_GetResetCalibrationStatus(ADCx));// 等待校准寄存器初始化完成
	ADC_StartCalibration(ADCx);// ADC开始校准
	while(ADC_GetCalibrationStatus(ADCx));// 等待校准完成
	ADC_SoftwareStartConvCmd(ADCx, ENABLE);// 由于没有采用外部触发，所以使用软件触发ADC转换 
}


void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_Mode_Config();

}
/*********************************************END OF FILE**********************/


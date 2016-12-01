#include "includes.h"

#if 0
#define RCC_POWER_DET        RCC_AHBPeriph_GPIOE
#define GPIO_PORT_POWER_DET  GPIOE
#define GPIO_PIN_POWER_DET   GPIO_Pin_10
#define ADC_POWER_DET_CH     ADC_Channel_25
#define ADC_VREF_VALUE       ((float)3.0) //ADC�ο���ѹֵ����λV
#else
#define RCC_POWER_DET        RCC_AHBPeriph_GPIOA
#define GPIO_PORT_POWER_DET  GPIOA
#define GPIO_PIN_POWER_DET   GPIO_Pin_1
#define ADC_POWER_DET_CH     ADC_Channel_1
#define ADC_VREF_VALUE       ((float)3.3)
#endif

#define RCC_CHARGE_DET       RCC_AHBPeriph_GPIOA
#define GPIO_PORT_CHARGE_DET GPIOA
#define GPIO_PIN_CHARGE_DET  GPIO_Pin_0
#define ADC_CHARGE_DET_CH    ADC_Channel_0

#define ADC_CONVERSION_TIME  ((uint32_t)0x1000)

void power_adc_conversion(float *power_voltage)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    uint32_t adc_time;
    uint16_t adc_value;

    RCC_AHBPeriphClockCmd(RCC_POWER_DET,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_POWER_DET;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;

    GPIO_Init(GPIO_PORT_POWER_DET,&GPIO_InitStructure);

    //stm32l1xxϵ�У�ADCת��ʱ��Ĭ��ΪHSI = 16MHz,�����޸�
    RCC_HSICmd(ENABLE);

    while(!RCC_GetFlagStatus(RCC_FLAG_HSIRDY));//�ȴ�HSIʱ���ȶ�

    ADC_DeInit(ADC1);

    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;

    ADC_Init(ADC1,&ADC_InitStructure);

    ADC_Cmd(ADC1,ENABLE);

    ADC_RegularChannelConfig(ADC1,ADC_POWER_DET_CH,1,ADC_SampleTime_24Cycles);
    
    ADC_SoftwareStartConv(ADC1);

    adc_time = ADC_CONVERSION_TIME;
    
    while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//�ȴ�ADCת������
    {
        if((adc_time--) == 0)
        {
            printf("ADC ERROR!\r\n");
        }
    }

    adc_value = ADC_GetConversionValue(ADC1) & 0x0FFF;

    *power_voltage = ((float)adc_value) * ADC_VREF_VALUE / ((float)4096);

    RCC_HSICmd(DISABLE);//�ر�HSIʱ���ѽڵ�
}



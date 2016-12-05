#include "includes.h"

void dismantle_int_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    RCC_AHBPeriphClockCmd(RCC_DISMANTLE,ENABLE);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin = GPIO_PIN_DISMANTLE;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIO_PORT_DISMANTLE,&GPIO_InitStruct);

    //GPIO_SetBits(GPIO_PORT_DISMANTLE,GPIO_PIN_DISMANTLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSource_DISMANTLE,EXTI_PinSource_DISMANTLE);
    EXTI_InitStruct.EXTI_Line = EXTI_LINE_DISMANTLE;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = EXTI_IRQn_DISMANTLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}


/*
*********************************************************************************************************
*	�� �� ��:bool app_check_dismantle_key_status(void)
*	����˵��:�жϷ��𰴼�״̬
*	��    ��:��  
*	�� �� ֵ:TRUE:���£�FALSE:δ����
*********************************************************************************************************
*/
bool app_check_dismantle_key_status(void)
{
    uint8_t temp;

    temp =  GPIO_ReadInputDataBit(GPIO_PORT_DISMANTLE,GPIO_PIN_DISMANTLE);

    if(temp)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}



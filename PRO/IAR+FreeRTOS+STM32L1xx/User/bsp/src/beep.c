#include "includes.h"

#ifdef DEBUG_SYSTEM

    #define RCC_BEEP       RCC_AHBPeriph_GPIOA
    #define GPIO_PORT_BEEP GPIOA
    #define GPIO_PIN_BEEP  GPIO_Pin_4

#else

    #define RCC_BEEP       RCC_AHBPeriph_GPIOC
    #define GPIO_PORT_BEEP GPIOC
    #define GPIO_PIN_BEEP  GPIO_Pin_9

#endif

/*
*********************************************************************************************************
*	�� �� ��:beep_init(void)
*	����˵��:��ʼ���������������� 
*	��    ��:��  
*	�� �� ֵ:�� 
*********************************************************************************************************
*/
void beep_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_BEEP,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_BEEP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;

	GPIO_Init(GPIO_PORT_BEEP,&GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��:beep(FunctionalState NewState)
*	����˵��:������������ֹͣ����
*	��    ��:NewState:ENABLE,������DISABLE,ֹͣ����  
*	�� �� ֵ:�� 
*********************************************************************************************************
*/
void beep(FunctionalState NewState)
{
	  if (NewState != DISABLE)
	  {
	  	GPIO_PORT_BEEP->BSRRL = GPIO_PIN_BEEP;
	  }
	  else
	  {
	  	GPIO_PORT_BEEP->BRR = GPIO_PIN_BEEP;
	  }
}







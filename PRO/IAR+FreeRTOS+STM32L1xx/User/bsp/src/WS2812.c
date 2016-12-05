#include "includes.h"

//LED��Դ��������
#define RCC_LED1_CTL    RCC_AHBPeriph_GPIOC
#define GPIO_PORT_LED1  GPIOC
#define GPIO_PIN_LED1   GPIO_Pin_3

#define RCC_LED2_CTL    RCC_AHBPeriph_GPIOE
#define GPIO_PORT_LED2  GPIOE
#define GPIO_PIN_LED2   GPIO_Pin_7

#define RCC_LED3_CTL    RCC_AHBPeriph_GPIOC
#define GPIO_PORT_LED3  GPIOC
#define GPIO_PIN_LED3   GPIO_Pin_5

#define RCC_LED4_CTL    RCC_AHBPeriph_GPIOB
#define GPIO_PORT_LED4  GPIOB
#define GPIO_PIN_LED4   GPIO_Pin_0

#define RCC_LED5_CTL    RCC_AHBPeriph_GPIOB
#define GPIO_PORT_LED5  GPIOB
#define GPIO_PIN_LED5   GPIO_Pin_1

//LED�����������
#ifdef DEBUG_SYSTEM

    #define RCC_LED_DATA         RCC_AHBPeriph_GPIOA
    #define GPIO_PORT_LED_DATA   GPIOA
    #define GPIO_PIN_LED_DATA    GPIO_Pin_15
    #define GPIO_SOURCE_LED_DATA GPIO_PinSource15
    #define GPIO_AF_LED_DATA     GPIO_AF_TIM2

#else

    #define RCC_LED_DATA         RCC_AHBPeriph_GPIOC
    #define GPIO_PORT_LED_DATA   GPIOC
    #define GPIO_PIN_LED_DATA    GPIO_Pin_8
    #define GPIO_SOURCE_LED_DATA GPIO_PinSource8
    #define GPIO_AF_LED_DATA     GPIO_AF_TIM3

#endif

#ifdef DEBUG_SYSTEM

    //LED_DATA��Ӧ��ʱ��ʱ��
    #define RCC_LED_TIMER        RCC_APB1Periph_TIM2
    //LED_DATA��Ӧ��ʱ��
    #define TIMER_PORT_LED_DATA  TIM2
    //���ڴ���LED_DATA���ݵ�DMAͨ����ʱ��
    #define RCC_DMA_LED_DATA     RCC_AHBPeriph_DMA1
    //���ڴ���LED_DATA���ݵ�DMAͨ��
    #define DMA_LED_DATA         DMA1_Channel5
    //LED_DATA��Ӧ��DMAͨ��Դ
    #define DMA_SOURCE_LED_DATA  TIM_DMA_CC1
    //LED_DATA��Ӧ��DMAͨ��������ɱ�־
    #define DMA_TC_LED_DATA      DMA1_FLAG_TC5

#else

    //LED_DATA��Ӧ��ʱ��ʱ��
    #define RCC_LED_TIMER        RCC_APB1Periph_TIM3
    //LED_DATA��Ӧ��ʱ��
    #define TIMER_PORT_LED_DATA  TIM3
    //���ڴ���LED_DATA���ݵ�DMAͨ����ʱ��
    #define RCC_DMA_LED_DATA     RCC_AHBPeriph_DMA1
    //���ڴ���LED_DATA���ݵ�DMAͨ��
    #define DMA_LED_DATA         DMA1_Channel2
    //LED_DATA��Ӧ��DMAͨ��Դ
    #define DMA_SOURCE_LED_DATA  TIM_DMA_CC3
    //LED_DATA��Ӧ��DMAͨ��������ɱ�־
    #define DMA_TC_LED_DATA      DMA1_FLAG_TC2

#endif


void led_on(ws2812_position_t ws2812_position);
void led_off(ws2812_position_t ws2812_position);

uint16_t LED_BYTE_Buffer[LED_BIT_CNT] = {
                                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                         PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,
                                         PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,
                                         PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,
                                         PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,
                                         PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,
                                         PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,
                                         PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,
                                         PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,
                                         PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,
                                         PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,
                                         PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,
                                         PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,PWM_DUTY_0,
                                         };

void WS2812_pwr_ctl_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_AHBPeriphClockCmd(RCC_LED1_CTL | RCC_LED2_CTL | RCC_LED3_CTL | RCC_LED4_CTL | RCC_LED5_CTL,ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  
  GPIO_Init(GPIO_PORT_LED1,&GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED2;
  GPIO_Init(GPIO_PORT_LED2,&GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED3;
  GPIO_Init(GPIO_PORT_LED3,&GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED4;
  GPIO_Init(GPIO_PORT_LED4,&GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED5;
  GPIO_Init(GPIO_PORT_LED5,&GPIO_InitStructure);
  
  //�ر�����led�ĵ�Դ
  led_off(bottom_right);
  led_off(bottom_left);
  led_off(top_left);
  led_off(top_middle);
  led_off(top_right);
}

void led_on(ws2812_position_t ws2812_position)
{
  if(ws2812_position == bottom_right)
  {
    GPIO_PORT_LED1->BSRRL = GPIO_PIN_LED1;
  }
  else if(ws2812_position == bottom_left)
  {
    GPIO_PORT_LED2->BSRRL = GPIO_PIN_LED2;
  }
  else if(ws2812_position == top_left)
  {
    GPIO_PORT_LED3->BSRRL = GPIO_PIN_LED3;
  }
  else if(ws2812_position == top_middle)
  {
    GPIO_PORT_LED4->BSRRL = GPIO_PIN_LED4;
  }
  else if(ws2812_position == top_right)
  {
    GPIO_PORT_LED5->BSRRL = GPIO_PIN_LED5;
  }
}

void led_off(ws2812_position_t ws2812_position)
{
  if(ws2812_position == bottom_right)
  {
    GPIO_PORT_LED1->BSRRH = GPIO_PIN_LED1;
  }
  else if(ws2812_position == bottom_left)
  {
    GPIO_PORT_LED2->BSRRH = GPIO_PIN_LED2;
  }
  else if(ws2812_position == top_left)
  {
    GPIO_PORT_LED3->BSRRH = GPIO_PIN_LED3;
  }
  else if(ws2812_position == top_middle)
  {
    GPIO_PORT_LED4->BSRRH = GPIO_PIN_LED4;
  }
  else if(ws2812_position == top_right)
  {
    GPIO_PORT_LED5->BSRRH = GPIO_PIN_LED5;
  }
}

//TIM3_CH3(PC8)����LED_DATA���
void WS2812_Timer_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_LED_DATA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED_DATA; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIO_PORT_LED_DATA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIO_PORT_LED_DATA,GPIO_SOURCE_LED_DATA,GPIO_AF_LED_DATA);

	RCC_APB1PeriphClockCmd(RCC_LED_TIMER,ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = PWM_PERIOD; 
	TIM_TimeBaseStructure.TIM_Prescaler = 0;  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIMER_PORT_LED_DATA, &TIM_TimeBaseStructure); 
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC1Init(TIMER_PORT_LED_DATA, &TIM_OCInitStructure); 
	
	RCC_AHBPeriphClockCmd(RCC_DMA_LED_DATA,ENABLE);
	TIM3->CCMR1 |= 0x0008;//����Ƚ�Ԥװ��ʹ��
	
	DMA_DeInit(DMA_LED_DATA);
    DMA_Cmd(DMA_LED_DATA,DISABLE);;//DMA_CNDTRx�Ĵ������ڶ�Ӧͨ��ʧ��ʱ����д
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)TIM2_CCR1_ADDRESS;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)LED_BYTE_Buffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 42;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	DMA_Init(DMA_LED_DATA,&DMA_InitStructure);//DMAͨ����ʼ��
	
	TIM_DMACmd(TIMER_PORT_LED_DATA,DMA_SOURCE_LED_DATA,ENABLE);//ʹ��TIM3��DMA����
}

void low_level_setled(ws2812_position_t position,ws2812_color_t color,uint8_t sequence,uint8_t brightness)
{
	uint8_t bit_cnt;
  base_color_t base_color;
	uint8_t i;
	
	bit_cnt = position * 24 + 48;
	
	switch(color)
	{
		case black:
			base_color.red = 0;
		  base_color.green = 0;
		  base_color.blue = 0;
		  break;
		
		case red:
			base_color.red = brightness;
		  base_color.green = 0;
		  base_color.blue = 0;
		  break;  
		
		case yellow:
			base_color.red = brightness;
		  base_color.green = brightness;
		  base_color.blue = 0;
		  break;
		
		case white:
			base_color.red = brightness;
		  base_color.green = brightness;
		  base_color.blue = brightness;
			break;
		
		case purple:
			base_color.red = brightness;
		  base_color.green = 0;
		  base_color.blue = brightness;
			break;
		
		case blue:
			base_color.red = 0;
		  base_color.green = 0;
		  base_color.blue = brightness;
			break;
		
		case skyblue:
			base_color.red = 0;
		  base_color.green = brightness;
		  base_color.blue = brightness;
			break;
		
		case green:
			base_color.red = 0;
		  base_color.green = brightness;
		  base_color.blue = 0;
			break;
		
		default:
			break;
	}
	
	for(i = 0;i < 8;i++)
	{
		if((base_color.green << i) & 0x80)
		{
			LED_BYTE_Buffer[bit_cnt] = PWM_DUTY_1;
		}
		else
		{
			LED_BYTE_Buffer[bit_cnt] = PWM_DUTY_0;
		}
		bit_cnt++;
	}
	
	for(i = 0;i < 8;i++)
	{
		if((base_color.red << i) & 0x80)
		{
			LED_BYTE_Buffer[bit_cnt] = PWM_DUTY_1;
		}
		else
		{
			LED_BYTE_Buffer[bit_cnt] = PWM_DUTY_0;
		}
		bit_cnt++;
	}
	
	for(i = 0;i < 8;i++)
	{
		if((base_color.blue << i) & 0x80)
		{
			LED_BYTE_Buffer[bit_cnt] = PWM_DUTY_1;
		}
		else
		{
			LED_BYTE_Buffer[bit_cnt] = PWM_DUTY_0;
		}
		bit_cnt++;
	}
    
	DMA_Cmd(DMA_LED_DATA,DISABLE);
	DMA_SetCurrDataCounter(DMA_LED_DATA,LED_BIT_CNT);//���õ�ǰDMAͨ����Ҫ��������ݸ���
	DMA_Cmd(DMA_LED_DATA,ENABLE);//ʹ��DMAͨ��
    TIM_Cmd(TIMER_PORT_LED_DATA,ENABLE);//ʹ�ܶ�ʱ��
	while(!DMA_GetFlagStatus(DMA_TC_LED_DATA));//�ȴ��������
	TIM_Cmd(TIMER_PORT_LED_DATA,DISABLE);//ʧ�ܶ�ʱ��
	DMA_Cmd(DMA_LED_DATA,DISABLE);//ʧ��DMAͨ��
	DMA_ClearFlag(DMA_TC_LED_DATA);//�崫����ɱ�־λ

    GPIO_PORT_LED_DATA->BSRRH = GPIO_PIN_LED_DATA;
}

void set_led(ws2812_position_t ws2812_position,ws2812_color_t color,uint8_t sequence,uint8_t brightness)
{
  //��ʼ��led�Ƶ�Դ��������
  //WS2812_pwr_ctl_init();
  //��ʼ����ʱ����DMAͨ��,WS2812_Timer3_Init(),���ܷ��ڴ˴���ʼ��������ᵼ�µ�һ��LED����ɫ����
  //WS2812_Timer3_Init();
  //����LED��Դ
  
    /*if(color == black)
    {
      led_off(ws2812_position);//���Ǻ�ɫ��رն�Ӧλ�õĵ�Դ���Խڵ�
    }
    else
    {
      led_on(ws2812_position);//�����Ǻ�ɫ�����Ӧλ�õĵ�Դ
    }*/
  
  //��Ӧ�ĵ�����Ӧ����ɫ�����º�ر�DMA����ʱ���ѽڵ�
  low_level_setled(ws2812_position,color,sequence,brightness);
}

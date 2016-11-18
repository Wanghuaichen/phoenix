#include "includes.h"

/* ����ÿ�����ڽṹ����� */
#if UART1_FIFO_EN == 1
	static UART_T g_tUart1;
	static uint8_t g_TxBuf1[UART1_TX_BUF_SIZE];		/* ���ͻ����� */
	static uint8_t g_RxBuf1[UART1_RX_BUF_SIZE];		/* ���ջ����� */
#endif

#if UART2_FIFO_EN == 1
	static UART_T g_tUart2;
	static uint8_t g_TxBuf2[UART2_TX_BUF_SIZE];		/* ���ͻ����� */
	static uint8_t g_RxBuf2[UART2_RX_BUF_SIZE];		/* ���ջ����� */
	//static uint8_t g_TxBuf2Temp[128];
#endif

#if UART3_FIFO_EN == 1
	static UART_T g_tUart3;
	static uint8_t g_TxBuf3[UART3_TX_BUF_SIZE];		/* ���ͻ����� */
	static uint8_t g_RxBuf3[UART3_RX_BUF_SIZE];		/* ���ջ����� */
#endif

#if UART1_FIFO_EN == 1
	#define RCC_USART1        RCC_APB2Periph_USART1
	#define RCC_USART1_PORT   RCC_AHBPeriph_GPIOA
	#define GPIO_PORT_USART1  GPIOA
	#define GPIO_PIN_TX1      GPIO_Pin_9
	#define GPIO_PIN_RX1      GPIO_Pin_10
	#define GPIO_PinSourceTX1 GPIO_PinSource9
	#define GPIO_PinSourceRX1 GPIO_PinSource10
#endif

#if UART2_FIFO_EN == 1
	#define RCC_USART2        RCC_APB1Periph_USART2
	#define RCC_USART2_PORT   RCC_AHBPeriph_GPIOA
	#define GPIO_PORT_USART2  GPIOA
	#define GPIO_PIN_TX2      GPIO_Pin_2
	#define GPIO_PIN_RX2      GPIO_Pin_3
	#define GPIO_PinSourceTX2 GPIO_PinSource2
	#define GPIO_PinSourceRX2 GPIO_PinSource3
#endif

#if UART3_FIFO_EN == 1

  #if 0
	#define RCC_USART3        RCC_APB1Periph_USART3
	#define RCC_USART3_PORT   RCC_AHBPeriph_GPIOD
	#define GPIO_PORT_USART3  GPIOD
	#define GPIO_PIN_TX3      GPIO_Pin_8
	#define GPIO_PIN_RX3      GPIO_Pin_9
	#define GPIO_PinSourceTX3 GPIO_PinSource8
	#define GPIO_PinSourceRX3 GPIO_PinSource9
  #else
  #define RCC_USART3        RCC_APB1Periph_USART3
	#define RCC_USART3_PORT   RCC_AHBPeriph_GPIOB
	#define GPIO_PORT_USART3  GPIOB
	#define GPIO_PIN_TX3      GPIO_Pin_10
	#define GPIO_PIN_RX3      GPIO_Pin_11
	#define GPIO_PinSourceTX3 GPIO_PinSource10
	#define GPIO_PinSourceRX3 GPIO_PinSource11
  #endif
  
#endif

#if UART1_FIFO_EN == 1

  #define UART1_TX_DMA_Channel  DMA1_Channel4
  #define UART1_TX_DMA_Flag     DMA1_FLAG_TC4
  #define UART1_DR_ADDRESS      0x40013804
  
#endif

#if UART2_FIFO_EN == 1

  #define UART2_TX_DMA_Channel  DMA1_Channel7
  #define UART2_TX_DMA_Flag     DMA1_FLAG_TC7
  #define UART2_DR_ADDRESS      0x40004404
	
#endif

#if UART3_FIFO_EN == 1

  #define UART3_TX_DMA_Channel  DMA1_Channel2
  #define UART3_TX_DMA_Flag  DMA1_FLAG_TC2
  #define UART3_DR_ADDRESS      0x40004804
	
#endif


static void UartVarInit(void);
static void InitHardUart(void);
static void UartSend(UART_T *_pUart, uint8_t *_ucaBuf, uint16_t _usLen);
static uint8_t UartGetChar(UART_T *_pUart, uint8_t *_pByte);
static void UartIRQ(UART_T *_pUart);
static void ConfigUartNVIC(void);
static UART_T *ComToUart(COM_PORT_E _ucPort);

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitUart
*	����˵��: ��ʼ������Ӳ��������ȫ�ֱ�������ֵ.
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitUart(void)
{
	UartVarInit();		/* �����ȳ�ʼ��ȫ�ֱ���,������Ӳ�� */

	InitHardUart();		/* ���ô��ڵ�Ӳ������(�����ʵ�) */

	ConfigUartNVIC();	/* ���ô����ж� */
}

/*
*********************************************************************************************************
*	�� �� ��: UARTx_TX_DMA_Init(uint32_t UARTxDRAdd,uint32_t TxDMAMemBaseAdd,DMA_Channel_TypeDef * DMAx_Channely)
*	����˵��: ��ʼ������X��TX��DMAͨ��
*	��    ��: UARTxDRAdd:����x��Tx�ķ��ͼĴ�����ַ 
            TxDMAMemBaseAdd:TX��DMA������ڴ����ַ
            DMAx_Channely������x����Ӧ��DMAͨ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void UARTx_TX_DMA_Init(uint32_t UARTxDRAdd,uint32_t TxDMAMemBaseAdd,DMA_Channel_TypeDef * DMAx_Channely)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_DeInit(DMAx_Channely);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = UARTxDRAdd;
	DMA_InitStructure.DMA_MemoryBaseAddr = TxDMAMemBaseAdd;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 64;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	DMA_Init(DMAx_Channely,&DMA_InitStructure);	
}


/*
*********************************************************************************************************
*	�� �� ��: UARTx_TX_DMA_Enable(UART_T *pUart,uint16_t _len)
*	����˵��: ��ʼ������X��TX��DMAͨ��
*	��    ��: 
*	�� �� ֵ: ��
*********************************************************************************************************
*/

void UARTx_TX_DMA_Enable(UART_T *pUart,uint16_t _len)
{
  if(pUart->uart == USART1)
  {
  	DMA_Cmd(UART1_TX_DMA_Channel,DISABLE);
	DMA_SetCurrDataCounter(UART1_TX_DMA_Channel,_len);
	DMA_Cmd(UART1_TX_DMA_Channel,ENABLE);
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
	while(DMA_GetFlagStatus(UART1_TX_DMA_Flag) == RESET);
	DMA_ClearFlag(UART1_TX_DMA_Flag);//ǧ�����������־λ
	comClearTxFifo(COM1);
  }
  else if(pUart->uart == USART2)
  {		
    DMA_Cmd(UART2_TX_DMA_Channel,DISABLE);
	DMA_SetCurrDataCounter(UART2_TX_DMA_Channel,_len);
	DMA_Cmd(UART2_TX_DMA_Channel,ENABLE);
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
	while(DMA_GetFlagStatus(UART2_TX_DMA_Flag) == RESET);
	DMA_ClearFlag(UART2_TX_DMA_Flag);//ǧ�����������־λ
	comClearTxFifo(COM2);
  }
  else if(pUart->uart == USART3)
  {		
    DMA_Cmd(UART3_TX_DMA_Channel,DISABLE);
	DMA_SetCurrDataCounter(UART3_TX_DMA_Channel,_len);
    DMA_Cmd(UART3_TX_DMA_Channel,ENABLE);
    USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);
    while(DMA_GetFlagStatus(UART3_TX_DMA_Flag) == RESET);
    DMA_ClearFlag(UART3_TX_DMA_Flag);//ǧ�����������־λ
    comClearTxFifo(COM3);
  }
}

/*
*********************************************************************************************************
*	�� �� ��: Uart1_TX_DMA_Enable(uint16_t _len)
*	����˵��: ����һ�δ�1TX��DMA����
*	��    ��:  _len:һ����Ҫ��������ݵĳ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Uart1_TX_DMA_Enable(uint16_t _len)
{
	DMA_Cmd(DMA1_Channel4,DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel4,_len);
	DMA_Cmd(DMA1_Channel4,ENABLE);
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
	comClearTxFifo(COM1);
}

/*
*********************************************************************************************************
*	�� �� ��: Uart3_TX_DMA_Enable(uint16_t _len)
*	����˵��: ����һ�δ�3TX��DMA����
*	��    ��:  _len:һ����Ҫ��������ݵĳ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Uart3_TX_DMA_Enable(uint16_t _len)
{
	DMA_Cmd(DMA1_Channel2,DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel2,_len);
	DMA_Cmd(DMA1_Channel2,ENABLE);
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);
	comClearTxFifo(COM3);
}

/*
*********************************************************************************************************
*	�� �� ��: ComToUart
*	����˵��: ��COM�˿ں�ת��ΪUARTָ��
*	��    ��: _ucPort: �˿ں�(COM1 - COM6)
*	�� �� ֵ: uartָ��
*********************************************************************************************************
*/
UART_T *ComToUart(COM_PORT_E _ucPort)
{
	if (_ucPort == COM1)
	{
		#if UART1_FIFO_EN == 1
			return &g_tUart1;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM2)
	{
		#if UART2_FIFO_EN == 1
			return &g_tUart2;
		#else
			return;
		#endif
	}
	else if (_ucPort == COM3)
	{
		#if UART3_FIFO_EN == 1
			return &g_tUart3;
		#else
			return 0;
		#endif
	}
	else
	{
		/* �����κδ��� */
		return 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: comSendBuf
*	����˵��: �򴮿ڷ���һ�����ݡ����ݷŵ����ͻ��������������أ���DMA���䵽����
*	��    ��: _ucPort: �˿ں�(COM1 - COM6)
*			  _ucaBuf: �����͵����ݻ�����
*			  _usLen : ���ݳ���
*	�� �� ֵ: ��
* ע�����������ݱȽϳ�ʱ�����ε���֮�����Ҫ��һ����ʱ�������Եȴ�DMA�������
*********************************************************************************************************
*/
void comSendBuf(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen)
{
	UART_T *pUart;

	pUart = ComToUart(_ucPort);
	if (pUart == 0)
	{
		return;
	}

	if (pUart->SendBefor != 0)
	{
		pUart->SendBefor();	
	}

	UartSend(pUart, _ucaBuf, _usLen);
}

/*
*********************************************************************************************************
*	�� �� ��: comSendChar
*	����˵��: �򴮿ڷ���1���ֽڡ����ݷŵ����ͻ��������������أ����жϷ�������ں�̨��ɷ���
*	��    ��: _ucPort: �˿ں�(COM1 - COM6)
*			  _ucByte: �����͵�����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void comSendChar(COM_PORT_E _ucPort, uint8_t _ucByte)
{
	UART_T *pUart;

	pUart = ComToUart(_ucPort);
	/* дһ���ֽڵ�USARTx */
	USART_SendData(pUart->uart,_ucByte);

	/* �ȴ����ͽ��� */
	while (USART_GetFlagStatus(pUart->uart, USART_FLAG_TC) == RESET);

}

/*
*********************************************************************************************************
*	�� �� ��: comGetChar
*	����˵��: �Ӵ��ڻ�������ȡ1�ֽڣ��������������������ݾ���������
*	��    ��: _ucPort: �˿ں�(COM1 - COM6)
*			  _pByte: ���յ������ݴ���������ַ
*	�� �� ֵ: 0 ��ʾ������, 1 ��ʾ��ȡ����Ч�ֽ�
*********************************************************************************************************
*/
uint8_t comGetChar(COM_PORT_E _ucPort, uint8_t *_pByte)
{
	UART_T *pUart;

	pUart = ComToUart(_ucPort);
	if (pUart == 0)
	{
		return 0;
	}

	return UartGetChar(pUart, _pByte);
}

/*
*********************************************************************************************************
*	�� �� ��: comClearTxFifo
*	����˵��: ���㴮�ڷ��ͻ�����
*	��    ��: _ucPort: �˿ں�(COM1 - COM6)
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void comClearTxFifo(COM_PORT_E _ucPort)
{
	UART_T *pUart;

	pUart = ComToUart(_ucPort);
	if (pUart == 0)
	{
		return;
	}

	pUart->usTxWrite = 0;
	pUart->usTxRead = 0;
	pUart->usTxCount = 0;
}

/*
*********************************************************************************************************
*	�� �� ��: comClearRxFifo
*	����˵��: ���㴮�ڽ��ջ�����
*	��    ��: _ucPort: �˿ں�(COM1 - COM6)
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void comClearRxFifo(COM_PORT_E _ucPort)
{
	UART_T *pUart;

	pUart = ComToUart(_ucPort);
	if (pUart == 0)
	{
		return;
	}

	pUart->usRxWrite = 0;
	pUart->usRxRead = 0;
	pUart->usRxCount = 0;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_SetUart1Baud
*	����˵��: �޸�UART1������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_SetUart1Baud(uint32_t _baud)
{
	USART_InitTypeDef USART_InitStructure;

	/* ��2���� ���ô���Ӳ������ */
	USART_InitStructure.USART_BaudRate = _baud;	/* ������ */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_SetUart2Baud
*	����˵��: �޸�UART2������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_SetUart2Baud(uint32_t _baud)
{
	USART_InitTypeDef USART_InitStructure;

	/* ��2���� ���ô���Ӳ������ */
	USART_InitStructure.USART_BaudRate = _baud;	/* ������ */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
}


/*
*********************************************************************************************************
*	�� �� ��: UartVarInit
*	����˵��: ��ʼ��������صı���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void UartVarInit(void)
{
#if UART1_FIFO_EN == 1
	g_tUart1.uart = USART1;						/* STM32 �����豸 */
	g_tUart1.pTxBuf = g_TxBuf1;					/* ���ͻ�����ָ�� */
	g_tUart1.pRxBuf = g_RxBuf1;					/* ���ջ�����ָ�� */
	g_tUart1.usTxBufSize = UART1_TX_BUF_SIZE;	/* ���ͻ�������С */
	g_tUart1.usRxBufSize = UART1_RX_BUF_SIZE;	/* ���ջ�������С */
	g_tUart1.usTxWrite = 0;						/* ����FIFOд���� */
	g_tUart1.usTxRead = 0;						/* ����FIFO������ */
	g_tUart1.usRxWrite = 0;						/* ����FIFOд���� */
	g_tUart1.usRxRead = 0;						/* ����FIFO������ */
	g_tUart1.usRxCount = 0;						/* ���յ��������ݸ��� */
	g_tUart1.usTxCount = 0;						/* �����͵����ݸ��� */
	g_tUart1.SendBefor = 0;						/* ��������ǰ�Ļص����� */
	g_tUart1.SendOver = 0;						/* ������Ϻ�Ļص����� */
	g_tUart1.ReciveNew = 0;						/* ���յ������ݺ�Ļص����� */
#endif

#if UART2_FIFO_EN == 1
	g_tUart2.uart = USART2;						/* STM32 �����豸 */
	g_tUart2.pTxBuf = g_TxBuf2;					/* ���ͻ�����ָ�� */
	g_tUart2.pRxBuf = g_RxBuf2;					/* ���ջ�����ָ�� */
	g_tUart2.usTxBufSize = UART2_TX_BUF_SIZE;	/* ���ͻ�������С */
	g_tUart2.usRxBufSize = UART2_RX_BUF_SIZE;	/* ���ջ�������С */
	g_tUart2.usTxWrite = 0;						/* ����FIFOд���� */
	g_tUart2.usTxRead = 0;						/* ����FIFO������ */
	g_tUart2.usRxWrite = 0;						/* ����FIFOд���� */
	g_tUart2.usRxRead = 0;						/* ����FIFO������ */
	g_tUart2.usRxCount = 0;						/* ���յ��������ݸ��� */
	g_tUart2.usTxCount = 0;						/* �����͵����ݸ��� */
	g_tUart2.SendBefor = 0;						/* ��������ǰ�Ļص����� */
	g_tUart2.SendOver = 0;						/* ������Ϻ�Ļص����� */
	g_tUart2.ReciveNew = 0;						/* ���յ������ݺ�Ļص����� */
#endif

#if UART3_FIFO_EN == 1
	g_tUart3.uart = USART3;						/* STM32 �����豸 */
	g_tUart3.pTxBuf = g_TxBuf3;					/* ���ͻ�����ָ�� */
	g_tUart3.pRxBuf = g_RxBuf3;					/* ���ջ�����ָ�� */
	g_tUart3.usTxBufSize = UART3_TX_BUF_SIZE;	/* ���ͻ�������С */
	g_tUart3.usRxBufSize = UART3_RX_BUF_SIZE;	/* ���ջ�������С */
	g_tUart3.usTxWrite = 0;						/* ����FIFOд���� */
	g_tUart3.usTxRead = 0;						/* ����FIFO������ */
	g_tUart3.usRxWrite = 0;						/* ����FIFOд���� */
	g_tUart3.usRxRead = 0;						/* ����FIFO������ */
	g_tUart3.usRxCount = 0;						/* ���յ��������ݸ��� */
	g_tUart3.usTxCount = 0;						/* �����͵����ݸ��� */
	g_tUart3.SendBefor = 0;		/* ��������ǰ�Ļص����� */
	g_tUart3.SendOver = 0;			/* ������Ϻ�Ļص����� */
	g_tUart3.ReciveNew = 0;		/* ���յ������ݺ�Ļص����� */
#endif
}

/*
*********************************************************************************************************
*	�� �� ��: InitHardUart
*	����˵��: ���ô��ڵ�Ӳ�������������ʣ�����λ��ֹͣλ����ʼλ��У��λ���ж�ʹ�ܣ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void InitHardUart(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

#if UART1_FIFO_EN == 1		/* ����1 TX = PA9   RX = PA10 �� TX = PB6   RX = PB7*/

	RCC_AHBPeriphClockCmd(RCC_USART1_PORT, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_USART1, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_TX1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIO_PORT_USART1, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIO_PORT_USART1,GPIO_PinSourceTX1,GPIO_AF_USART1);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_RX1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIO_PORT_USART1, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIO_PORT_USART1,GPIO_PinSourceRX1,GPIO_AF_USART1);
	
	USART_InitStructure.USART_BaudRate = UART1_BAUD;	/* ������ */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	/* ʹ�ܽ����ж� */

	USART_Cmd(USART1, ENABLE);		/* ʹ�ܴ��� */

	USART_ClearFlag(USART1, USART_FLAG_TC);     /* �巢����ɱ�־��Transmission Complete flag */
	
	UARTx_TX_DMA_Init(UART1_DR_ADDRESS,(uint32_t)(g_TxBuf1),UART1_TX_DMA_Channel);
#endif

#if UART2_FIFO_EN == 1		/* ����2 TX = PA2�� RX = PA3  */

	RCC_AHBPeriphClockCmd(RCC_USART2_PORT, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_USART2, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_TX2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIO_PORT_USART2, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIO_PORT_USART2,GPIO_PinSourceTX2,GPIO_AF_USART2);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_RX2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIO_PORT_USART2, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIO_PORT_USART2,GPIO_PinSourceRX2,GPIO_AF_USART2);

	USART_InitStructure.USART_BaudRate = UART2_BAUD;	/* ������ */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;		
	USART_Init(USART2, &USART_InitStructure);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	/* ʹ�ܽ����ж� */
	
	USART_Cmd(USART2, ENABLE);		/* ʹ�ܴ��� */

	USART_ClearFlag(USART2, USART_FLAG_TC);     /* �巢����ɱ�־��Transmission Complete flag */
	
	UARTx_TX_DMA_Init(UART2_DR_ADDRESS,(uint32_t)(g_TxBuf2),UART2_TX_DMA_Channel);
#endif

#if UART3_FIFO_EN == 1			/* ����3 TX = PB10   RX = PB11 */

	RCC_AHBPeriphClockCmd(RCC_USART3_PORT, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_USART3, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_TX3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIO_PORT_USART3, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIO_PORT_USART3,GPIO_PinSourceTX3,GPIO_AF_USART3);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_RX3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIO_PORT_USART3, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIO_PORT_USART3,GPIO_PinSourceRX3,GPIO_AF_USART3);

	USART_InitStructure.USART_BaudRate = UART3_BAUD;	/* ������ */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;		
	USART_Init(USART3, &USART_InitStructure);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	/* ʹ�ܽ����ж� */
	
	USART_Cmd(USART3, ENABLE);		/* ʹ�ܴ��� */

	USART_ClearFlag(USART3, USART_FLAG_TC);     /* �巢����ɱ�־��Transmission Complete flag */
	
	UARTx_TX_DMA_Init(UART3_DR_ADDRESS,(uint32_t)(g_TxBuf3),UART3_TX_DMA_Channel);
#endif
}

/*
*********************************************************************************************************
*	�� �� ��: ConfigUartNVIC
*	����˵��: ���ô���Ӳ���ж�.
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void ConfigUartNVIC(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

#if UART1_FIFO_EN == 1
	/* ʹ�ܴ���1�ж� */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

#if UART2_FIFO_EN == 1
	/* ʹ�ܴ���2�ж� */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

#if UART3_FIFO_EN == 1
	/* ʹ�ܴ���3�ж�t */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
}

/*
*********************************************************************************************************
*	�� �� ��: UartSend
*	����˵��: ��д���ݵ�UART���ͻ�����,�����������жϡ��жϴ�����������Ϻ��Զ��رշ����ж�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void UartSend(UART_T *_pUart, uint8_t *_ucaBuf, uint16_t _usLen)
{
	uint16_t i;

	for (i = 0; i < _usLen; i++)
	{
		/* ������ͻ������Ѿ����ˣ���ȴ��������� */
	#if 0

		while (1)
		{
			uint16_t usRead;

			DISABLE_INT();
			usRead = _pUart->usTxRead;
			ENABLE_INT();

			if (++usRead >= _pUart->usTxBufSize)
			{
				usRead = 0;
			}

			if (usRead != _pUart->usTxWrite)
			{
				break;
			}
		}
	#else
		/* �� _pUart->usTxBufSize == 1 ʱ, ����ĺ���������(������) */
		while (1)
		{
			__IO uint16_t usCount;

			usCount = _pUart->usTxCount;

			if (usCount < _pUart->usTxBufSize)
			{
				break;
			}
		}
	#endif

		/* �����������뷢�ͻ����� */
		_pUart->pTxBuf[_pUart->usTxWrite] = _ucaBuf[i];

		if (++_pUart->usTxWrite >= _pUart->usTxBufSize)
		{
			_pUart->usTxWrite = 0;
		}
		_pUart->usTxCount++;
	}
	
    UARTx_TX_DMA_Enable(_pUart,_usLen);
}

/*
*********************************************************************************************************
*	�� �� ��: UartGetChar
*	����˵��: �Ӵ��ڽ��ջ�������ȡ1�ֽ����� ��������������ã�
*	��    ��: _pUart : �����豸
*			  _pByte : ��Ŷ�ȡ���ݵ�ָ��
*	�� �� ֵ: 0 ��ʾ������  1��ʾ��ȡ������
*********************************************************************************************************
*/
static uint8_t UartGetChar(UART_T *_pUart, uint8_t *_pByte)
{
	uint16_t usCount;

	/* usRxWrite �������жϺ����б���д���������ȡ�ñ���ʱ����������ٽ������� */
	DISABLE_INT();
	usCount = _pUart->usRxCount;
	ENABLE_INT();

	/* �������д������ͬ���򷵻�0 */
	//if (_pUart->usRxRead == usRxWrite)
	if (usCount == 0)	/* �Ѿ�û������ */
	{
		return 0;
	}
	else
	{
		*_pByte = _pUart->pRxBuf[_pUart->usRxRead];		/* �Ӵ��ڽ���FIFOȡ1������ */

		/* ��дFIFO������ */
		DISABLE_INT();
		if (++_pUart->usRxRead >= _pUart->usRxBufSize)
		{
			_pUart->usRxRead = 0;
		}
		_pUart->usRxCount--;
		ENABLE_INT();
		return 1;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: UartIRQ
*	����˵��: ���жϷ��������ã�ͨ�ô����жϴ�����
*	��    ��: _pUart : �����豸
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void UartIRQ(UART_T *_pUart)
{
	/* ��������ж�  */
	if (USART_GetITStatus(_pUart->uart, USART_IT_RXNE) != RESET)
	{
		/* �Ӵ��ڽ������ݼĴ�����ȡ���ݴ�ŵ�����FIFO */
		uint8_t ch;

		ch = USART_ReceiveData(_pUart->uart);

		_pUart->pRxBuf[_pUart->usRxWrite] = ch;
		if (++_pUart->usRxWrite >= _pUart->usRxBufSize)
		{
			_pUart->usRxWrite = 0;
		}
		if (_pUart->usRxCount < _pUart->usRxBufSize)
		{
			_pUart->usRxCount++;
		}

    if (_pUart->ReciveNew)
		{
			_pUart->ReciveNew(ch);
		}
	}

}

/*
*********************************************************************************************************
*	�� �� ��: USART1_IRQHandler  USART2_IRQHandler USART3_IRQHandler UART4_IRQHandler UART5_IRQHandler
*	����˵��: USART�жϷ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
#if UART1_FIFO_EN == 1
void USART1_IRQHandler(void)
{
	UartIRQ(&g_tUart1);
}
#endif

#if UART2_FIFO_EN == 1
void USART2_IRQHandler(void)
{
	UartIRQ(&g_tUart2);
}
#endif

#if UART3_FIFO_EN == 1
void USART3_IRQHandler(void)
{
	UartIRQ(&g_tUart3);
}
#endif

/*
*********************************************************************************************************
*	�� �� ��: fputc
*	����˵��: �ض���putc��������������ʹ��printf�����Ӵ���1��ӡ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int fputc(int ch, FILE *f)
{
#if 0	/* ����Ҫprintf���ַ�ͨ�������ж�FIFO���ͳ�ȥ��printf�������������� */
	comSendChar(COM2, ch);

	return ch;
#else	/* ����������ʽ����ÿ���ַ�,�ȴ����ݷ������ */
	/* дһ���ֽڵ�USART2 */
	USART_SendData(USART2, (uint8_t) ch);

	/* �ȴ����ͽ��� */
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
	{}

	return ch;
#endif
}

/*
*********************************************************************************************************
*	�� �� ��: fgetc
*	����˵��: �ض���getc��������������ʹ��getchar�����Ӵ���1��������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int fgetc(FILE *f)
{

#if 1	/* �Ӵ��ڽ���FIFO��ȡ1������, ֻ��ȡ�����ݲŷ��� */
	uint8_t ucData;

	while(comGetChar(COM2, &ucData) == 0);

	return ucData;
#else
	/* �ȴ�����1�������� */
	while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);

	return (int)USART_ReceiveData(USART2);
#endif
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/

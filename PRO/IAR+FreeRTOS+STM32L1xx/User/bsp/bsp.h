/*
*********************************************************************************************************
*
*	ģ������ : BSPģ��
*	�ļ����� : bsp.h
*	˵    �� : ���ǵײ�����ģ�����е�h�ļ��Ļ����ļ��� Ӧ�ó���ֻ�� #include bsp.h ���ɣ�
*			  ����Ҫ#include ÿ��ģ��� h �ļ�
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_H_
#define _BSP_H

#define STM32_V4
//#define STM32_X2

/* ����Ƿ����˿������ͺ� */
#if !defined (STM32_V4) && !defined (STM32_X2)
	#error "Please define the board model : STM32_X2 or STM32_V4"
#endif

/* ���� BSP �汾�� */
#define __STM32F1_BSP_VERSION		"1.1"

/* CPU����ʱִ�еĺ��� */
//#define CPU_IDLE()		bsp_Idle()

/* ����ȫ���жϵĺ� */
#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */

/* ���������ڵ��Խ׶��Ŵ� */
#define BSP_Printf		printf
//#define BSP_Printf(...)

#include "stm32l1xx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

#include "bsp_led.h"
#include "bsp_timer.h"
#include "bsp_key.h"
#include "beep.h"
#include "platform.h"
#include "sys_reset.h"
#include "usart.h"
#include "hal_beep.h"
#include "hal_platform.h"

//--- Error definitions --------------------------------------------------------
typedef enum
{    // Note: Enums are zigzag-encoded to write the values correctly by just writing/reading the bytes directly
	NO_ERROR,
	SYNC,
	INVALID_CMD,
	CORRUPT_SIZE,
	CORRUPT_DATA,
	COMMAND_NOT_FOUND,
	NO_COMMAND_HANDLER,
	BUSY,
	TIMEOUT,
	INVALID_ARG,
	ACCESS_DENIED,
	CHECKSUM_ERROR,
	FLASH_WRITE_ERROR,
	INTERNAL_ERROR,
	NTAG_ERROR,
	ACCELEROMETER_ERROR,
	LIGHT_SENSOR_ERROR,
	LOG_ERROR,
	FALL_OFF_ERROR,
	CLAMP_ERROR,
	LOCK_ERROR,
	PSV_ERROR,
	TEMP_RH_ERROR,
	end
}error_t;

/* �ṩ������C�ļ����õĺ��� */
void bsp_Init(void);
void bsp_Idle(void);

#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/

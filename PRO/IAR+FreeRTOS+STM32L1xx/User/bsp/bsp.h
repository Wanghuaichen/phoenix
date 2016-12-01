#ifndef _BSP_H_
#define _BSP_H


/* ����ȫ���жϵĺ� */
#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */

typedef enum {FALSE = 0, TRUE = !FALSE} bool;


#include "stm32l1xx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "i2c.h"
#include "beep.h"
#include "platform.h"
#include "opt3001.h"
#include "SHT20.h"
#include "ADXL345.h"
#include "nt3h1101.h"
#include "WS2812.h"
#include "usart.h"
#include "eeprom.h"
#include "adc.h"
#include "dismantle.h"
#include "hal_beep.h"
#include "hal_platform.h"
#include "hal_brightness.h"
#include "hal_tempRh.h"
#include "hal_acc_sensor.h"
#include "hal_nfc.h"
#include "hal_led.h"
#include "nfc_module.h"
#include "hal_energy.h"
#include "hal_dismantle.h"

void bsp_Init(void);

#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/

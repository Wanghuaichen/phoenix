#ifndef __OPT3001_H__
#define __OPT3001_H__

#include "stm32l1xx.h"

#define OPT3001InterruptMode

#define OPT3001ReadAddress            0x89
#define OPT3001WriteAddress           0x88

#define OPT3001ResultAdd              0x00
#define OPT3001ConfigAdd              0x01
#define OPT3001LowLimitAdd            0x02
#define OPT3001HighLimitAdd           0x03
#define OPT3001MFCIDAdd               0x7E
#define OPT3001DeviceIDAdd            0x7F

#define OPT3001AutoFullScale          0x0C
#define OPT3001OneThousandScale       0x05
#define OPT3001ConvertTime800ms       0x01
#define OPT3001ConvertTime100ms       0x00
#define OPT3001ContinueConvert        0x02
#define OPT3001InterruptUnLatch       0x00
#define OPT3001InterruptLatch         0x01
#define OPT3001INTPinPolarityLow      0x00
#define OPT3001UnMSKExFiled           0x00
#define OPT3001FaultTriggerINTCount   0x00

#define OPT3001HighLimitRange         0x05 //1310.4
#define OPT3001LowLimitRange          0x00 //40.95
#define OPT3001HighLimit              0xFFF
#define OPT3001LowLimit               0xFFF

typedef union
{
	uint16_t opt3001_config_reg_total;
	
	struct
	{
		uint16_t FC          :2;
		uint16_t ME          :1;
		uint16_t POL         :1;
		uint16_t L           :1;
		uint16_t FL          :1;
		uint16_t FH          :1;
		uint16_t CRF         :1;
		uint16_t OVF         :1;
		uint16_t M           :2;
		uint16_t CT          :1;
	  uint16_t RN          :4;
	}opt3001_config_reg_bits;
	
}opt3001_config_reg_t;

typedef union
{
	uint16_t opt3001_result_reg_total;
	
	struct
	{
		uint16_t R           :12;
		uint16_t E           :4;
	}opt3001_result_reg_bits;
	
}opt3001_result_reg_t;

typedef enum
{
	OPT3001_ERR = 0,
	OPT3001_OK,
}opt3001_status_t;

typedef struct
{
	float lux;
	opt3001_status_t opt3001_status;
}opt3001_result_t;

extern uint8_t LowHighLimitTrigFlag;//opt3001�жϱ�־λ��=1�����жϣ�=0��û���жϣ���������
extern opt3001_status_t opt3001_reg_init(void);//opt3001�Ĵ�����ʼ������Ҫ�ǳ�ʼ�����üĴ���
extern void opt3001_int_init(void);//opt3001�жϳ�ʼ������Ҫ��ʼ���ж����ţ����ô�����ʽ���ж����ȼ�
extern opt3001_status_t opt3001_alarm_init(float opt3001_low_limit,float opt3001_high_limit);//opt3001��ֵ��ʼ����������ֵ��Χ������ж�
extern opt3001_result_t opt3001_get_lux(void);//��õ�ǰ����ֵ
extern opt3001_result_t opt3001_int_get_lux(void);//�ж�ģʽ�»�õ�ǰ����ֵ

#endif

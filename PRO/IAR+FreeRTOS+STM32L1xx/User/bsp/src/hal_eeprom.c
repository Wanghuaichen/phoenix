#include "includes.h"

/*
*********************************************************************************************************
*	�� �� ��:void hal_eeprom_read_bytes(uint16_t addr,uint8_t *buffer,uint16_t length)
*	����˵��:���ֽڶ�ȡMCU�ڲ���eeprom�ڵ�����
*	��    ��:uint16_t addr:��Ҫ��ȡ���ڲ�eeprom�ĵ�ַ����Χ:0x0000 - 0x2000
*            uint8_t *buffer:��Ŷ�ȡ���������ݵĻ�����
*            uint16_t length:��Ҫ��ȡ�����ݵĸ�������λ:byte
*	�� �� ֵ:�� 
*********************************************************************************************************
*/
void hal_eeprom_read_bytes(uint16_t addr,uint8_t *buffer,uint16_t length)
{
    eeprom_read_bytes(addr,buffer,length);
}

/*
*********************************************************************************************************
*	�� �� ��:FLASH_Status hal_eeprom_write_bytes(uint16_t addr,uint8_t *buffer,uint16_t length)
*	����˵��:���ֽڰ�����д��MCU�ڲ���eeprom
*	��    ��:uint16_t addr:��Ҫд���ڲ�eeprom����ʼ��ַ����Χ:0x0000 - 0x2000
*            uint8_t *buffer:���д�����ݵĻ�����
*            uint16_t length:��Ҫд������ݵĸ�������λ:byte
*	�� �� ֵ:�� 
*********************************************************************************************************
*/
FLASH_Status hal_eeprom_write_bytes(uint16_t addr,uint8_t *buffer,uint16_t length)
{
    FLASH_Status status;

    status = eeprom_write_bytes(addr,buffer,length);

    return status;
}


#ifndef __EEPROM_H__
#define __EEPROM_H__

#define EEPROM_BASE_ADDR 0x08080000
#define EEPROM_BYTE_SZIE 0x1FFF

//�ɵ���FLASH_Status DATA_EEPROM_ProgramByte(uint32_t Address, uint8_t Data)��
//����ǰ�ȵ���DATA_EEPROM_Unlock()�����eeprom������
//�ٵ���DATA_EEPROM_FixedTimeProgramCmd()��д֮ǰ���Զ�����
//����ٵ���DATA_EEPROM_Lock()������eeprom����ֹ�������

FLASH_Status eeprom_write_bytes(uint16_t addr,uint8_t *buffer,uint16_t length);

void eeprom_read_bytes(uint16_t addr,uint8_t *buffer,uint16_t length);

#endif

#ifndef __HAL_ACC_SENSOR__
#define __HAL_ACC_SENSOR__

void hal_acc_init(void);

void hal_acc_int_init(void);//�����ж�

adxl345_result_t hal_getAcceleration(void);//��ȡ������ٶ�ֵ

adxl345_status_t hal_observeActivity(int8_t thresh , bool x, bool y , bool z);//����activity�ж�

adxl345_status_t hal_observeFreefall(void);//����freefall�ж�

adxl345_int_result_t hal_act_freefall_int_process(void);//��ѯ�жϱ�־�������ж�

#endif
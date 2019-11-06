/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      Timer.c
  * @brief:     ��ʱ����ص�Ӧ�ó���
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-11-05
  * @update:    [2019-11-05][Lei][creat]
  */
 
#ifndef __TIMER_H_
#define __TIMER_H_

#include <stdint.h>

void TimerInit(void);						//��ʱ����ʼ������������ʱ��
uint32_t GetNodeOvertime(void);				//��ȡ��ǰ�����ʱ��ڵ�
uint8_t CheckOvertime(uint32_t startTime, uint32_t overtime);		//����Ƿ�ʱ

#endif

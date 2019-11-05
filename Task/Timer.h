/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      Timer.c
  * @brief:     定时器相关的应用程序
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-11-05
  * @update:    [2019-11-05][Lei][creat]
  */
 
#ifndef __TIMER_H_
#define __TIMER_H_

#include <stdint.h>

void TimerInit(void);
uint32_t GetNodeOvertime(void);
uint8_t CheckOvertime(uint32_t startTime, uint32_t overtime);

#endif

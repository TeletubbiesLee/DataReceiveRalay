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

void TimerInit(void);						//定时器初始化，并开启定时器
uint32_t GetNodeOvertime(void);				//获取当前的相对时间节点
uint8_t CheckOvertime(uint32_t startTime, uint32_t overtime);		//检测是否超时

#endif

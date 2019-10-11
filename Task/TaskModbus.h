/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      TaskModbus.h
  * @brief:     ModBus从机任务
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-06-19
  * @update:    [2019-06-19][Lei][creat]
  */


#ifndef __TASK_MODBUS_H_
#define __TASK_MODBUS_H_

#include <stdint.h>

extern uint32_t g_ModbusBandrate;
extern uint8_t g_ModbusParity;

int   TaskModbusInit(void);

#endif

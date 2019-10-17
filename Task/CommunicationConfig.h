/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      CommunicationConfig.h
  * @brief:     上位机对数据接收中继的通信配置相关程序文件
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-10-17
  * @update:    [2019-10-17][Lei][creat]
  */

#ifndef __COMMUNICATION_CONFIG_H_
#define __COMMUNICATION_CONFIG_H_

#include <stdint.h>

extern uint32_t g_ModbusBandrate;
extern uint8_t g_ModbusParity;
extern uint8_t g_ModbusSlaveAddress;
extern uint8_t g_ModbusUartNumber;

#endif



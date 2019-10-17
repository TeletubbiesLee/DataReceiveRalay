/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      CommunicationConfig.c
  * @brief:     上位机对数据接收中继的通信配置相关程序文件
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-10-17
  * @update:    [2019-10-17][Lei][creat]
  */

/********************************include**************************************/
#include "CommunicationConfig.h"
#include "user_mb_app.h"


/*************************************global********************************************/
uint32_t g_ModbusBandrate = 115200;		//Modbus的波特率
uint8_t  g_ModbusParity = MB_PAR_NONE;	//Modbus的奇偶校验方式，无奇偶校验位
uint8_t  g_ModbusSlaveAddress = 0x01;	//Modbus的从机地址
uint8_t  g_ModbusUartNumber = 1;		//Modbus使用的串口号


/********************************function**************************************/




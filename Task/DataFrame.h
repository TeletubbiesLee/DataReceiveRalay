/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      DataFrame.c
  * @brief:     传输的数据帧等相关应用
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-06-21
  * @update:    [2019-06-21][Lei][creat]
  */

#ifndef __DATA_FRAME_H_
#define __DATA_FRAME_H_

#include "stdint.h"


extern uint16_t g_TemperatureAndVoltage[128];

void WriteDataBuf(uint16_t* data, uint16_t* dataBuffer, uint8_t dataLenth);

#endif

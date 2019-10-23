/**
 * @file JsonFileOperation.c
 * @brief 实现Json文件操作
 * @copyright Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * @company SOJO
 * @date 2018.06.15
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 
#ifndef __JSON_FILE_OPERATION_
#define __JSON_FILE_OPERATION_

#include "stdint.h"


extern uint8_t vPort_s2j_init(void);  //初始化struct2json
extern uint8_t Create_JsonFile(void);
extern uint8_t Get_JsonFile(void);

#endif


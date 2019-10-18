/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      TaskConfig.h
  * @brief:     debug调试配置
  * @version:   V1.0.0 
  * @author:    Gang
  * @date:      2019-10-18
  * @update:    [2019-10-18][Gang][creat]
  */

#ifndef __TASK_CONFIG_H_
#define __TASK_CONFIG_H_


#define DEBUG_PRINTF        //开启串口调试

    /**
      * @brief 调试打印的函数定义
      */
    #ifdef DEBUG_PRINTF
    //开启调试模式
    #define DebugPrintf(format,...)    rt_kprintf(format, ##__VA_ARGS__)
 
    #else
 
    //关闭调试模式
    #define DebugPrintf(format,...)

    #endif

#endif

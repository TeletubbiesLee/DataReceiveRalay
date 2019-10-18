/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      TaskDataReceive.c
  * @brief:     测试任务函数，用于测试产品ID号
  * @version:   V1.0.0 
  * @author:    Gang
  * @date:      2019-10-18
  * @update:    [2019-10-18][Gang][creat]
  */
#include "Test.h"
#include <rtthread.h>
#include "user_mb_app.h"

#define thread_ModbusSlaveData_Prio         9

static rt_uint8_t thread_ModbusSlaveData_stack[512];
struct rt_thread thread_ModbusSlaveData;

extern USHORT usSRegHoldBuf[S_REG_HOLDING_NREGS];	//保持寄存器缓冲区

/**
  * @brief : ModBus从机相关寄存器修改
  * @param : void
  * @return: void 
  * @updata: [2019-09-11][Gang][creat]
  */

static void thread_entry_ModbusSlaveData(void* parameter)
{
    while (1)
	{
        usSRegHoldBuf[2055] = 0xFFFF;
        usSRegHoldBuf[2056] = 0xFFFF;
        rt_thread_mdelay(1000);
	}    
}

int  TaskTestInit(void)
    {
    //创建静态线程   从机寄存器数据改变
    rt_thread_init(&thread_ModbusSlaveData,                 //线程handle
				   "MBSlaveData",                           //线程名
                   thread_entry_ModbusSlaveData,            //线程入口函数
				   RT_NULL,                                 //线程入口参数
				   thread_ModbusSlaveData_stack,            //线程栈地址
                   sizeof(thread_ModbusSlaveData_stack),    //线程栈大小
				   thread_ModbusSlaveData_Prio,             //线程优先级
                   5);                                      //线程时间片
				   
    rt_thread_startup(&thread_ModbusSlaveData);             //启动线程      
     return 0;
}

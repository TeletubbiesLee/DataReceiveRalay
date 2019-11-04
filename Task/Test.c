/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      TaskDataReceive.c
  * @brief:     测试任务函数，用于测试产品ID号
  * @version:   V1.0.0 
  * @author:    Gang
  * @date:      2019-10-18
  * @update:    [2019-10-18][Gang][creat]
  */
/*************************************include********************************************/
#include "Test.h"
#include <rtthread.h>
#include "user_mb_app.h"
#include "DataFrame.h"
#include "CommunicationConfig.h"

/*************************************static********************************************/
/* 测试任务优先级，栈空间，任务结构体及入口函数 */
#define THREAD_TASK_TEST_PRIO 9
static rt_uint8_t TaskTestStack[4096];
static struct rt_thread ThreadModbusSlaveData;
static void ModbusSlaveDataThreadEntry(void* parameter);

/*************************************extern********************************************/
extern USHORT usSRegHoldBuf[S_REG_HOLDING_NREGS];    //保持寄存器缓冲区

/*************************************function********************************************/
/**
  * @brief : ModBus从机相关寄存器修改
  * @param : void
  * @return: void 
  * @updata: [2019-09-11][Gang][creat]
  */
static void ModbusSlaveDataThreadEntry(void* parameter)
{
	
    //rt_thread_mdelay(5000);
    
	
    while (1)
    {
        /* 将所有存放地址的空间清零*/
        usSRegHoldBuf[0x708] = 0x5AA5;
        usSRegHoldBuf[0x709] = 0xABBA;
        usSRegHoldBuf[0x710] = 0x5AA5;
        usSRegHoldBuf[0x711] = 0xABBA;
        usSRegHoldBuf[0x806] = 0xFFFF;
        usSRegHoldBuf[0x807] = 0xFFFF;
        
		
        rt_thread_mdelay(5000);
    }
}

int TaskTestInit(void)
{
    //创建静态线程   从机寄存器数据改变
    rt_thread_init(&ThreadModbusSlaveData,                 //线程handle
                   "TaskTest",                             //线程名
                   ModbusSlaveDataThreadEntry,             //线程入口函数
                   RT_NULL,                                //线程入口参数
                   TaskTestStack,                          //线程栈地址
                   sizeof(TaskTestStack),                  //线程栈大小
                   THREAD_TASK_TEST_PRIO,                  //线程优先级
                   5);                                     //线程时间片
    rt_thread_startup(&ThreadModbusSlaveData);             //启动线程
                   
    return 0;
}

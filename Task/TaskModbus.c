/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      TaskModbus.c
  * @brief:     ModBus从机任务
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-06-19
  * @update:    [2019-06-19][Lei][creat]
                [2019-09-11][Gang][update][增加Mdobus从机修改数据的任务]
  */

/********************************include**************************************/
#include "TaskModbus.h"
#include "DataFrame.h"
#include <stdlib.h>
#include <stdio.h>
#include <rthw.h>
#include <rtthread.h>
#include "user_mb_app.h"
#include "mbfunc.h"
#include "mbrtu.h"

/*************************************global********************************************/
uint32_t g_ModbusBandrate = 115200;		//Modbus的波特率
uint8_t  g_ModbusParity = MB_PAR_NONE;	//Modbus的奇偶校验方式，无奇偶校验位



/*************************************static********************************************/
/* Modbus从机轮训的任务优先级，栈空间，任务结构体及入口函数 */
#define THREAD_MODBUS_SLAVE_POLL_PRIO	10
static rt_uint8_t ThreadModbusSlavePollStack[512];
struct rt_thread ThreadModbusSlavePoll;
static void ModbusSlavePollThreadEntry(void* parameter);



/*************************************extern********************************************/
extern USHORT usSRegInBuf[S_REG_INPUT_NREGS];	    //输入寄存器缓冲区
extern USHORT usSRegHoldBuf[S_REG_HOLDING_NREGS];	//保持寄存器缓冲区




/*************************************function******************************************/

/**
  * @brief : ModBus从机轮询入口
  * @param : void
  * @return: void 
  * @updata: [2019-06-19][Lei][creat]
  */
static void ModbusSlavePollThreadEntry(void* parameter)
{
    /* 初始化Modbus-RTU模式，从机地址为1，串口使用USART1，波特率115200，无校验 */
	eMBInit(MB_RTU, 0x01, 1, g_ModbusBandrate,  g_ModbusParity);
    
    eMBEnable();			//启动FreeModbus
	
    while(1)
	{
        //FreeModbus从机不断查询
        eMBPoll();		
    }
}


/**
  * @brief : 创建多个Modbus线程
  * @param : void
  * @return: void 
  * @updata: [2019-09-11][Gang][creat]
  */
int  TaskModbusInit(void)
{
    //创建静态线程   从机轮询
   rt_thread_init(&ThreadModbusSlavePoll,                 //线程handle
				   "ModbusSlavePoll",                           //线程名
                   ModbusSlavePollThreadEntry,            //线程入口函数
				   RT_NULL,                                 //线程入口参数
				   ThreadModbusSlavePollStack,            //线程栈地址
                   sizeof(ThreadModbusSlavePollStack),    //线程栈大小
				   THREAD_MODBUS_SLAVE_POLL_PRIO,             //线程优先级
                   5);                                      //线程时间片
				   
    rt_thread_startup(&ThreadModbusSlavePoll);             //启动线程

				   
    return 0;              
}

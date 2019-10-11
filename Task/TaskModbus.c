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

uint32_t g_ModbusBandrate = 115200;		//Modbus的波特率
uint8_t  g_ModbusParity = MB_PAR_NONE;	//Modbus的奇偶校验方式，无奇偶校验位


#define thread_ModbusSlavePoll_Prio         10
#define thread_ModbusSlaveData_Prio         11


static rt_uint8_t thread_ModbusSlavePoll_stack[512];
struct rt_thread thread_ModbusSlavePoll;
static rt_uint8_t thread_ModbusSlaveData_stack[512];
struct rt_thread thread_ModbusSlaveData;


static void thread_entry_ModbusSlavePoll(void* parameter);
static void thread_entry_ModbusSlaveData(void* parameter);

extern USHORT usSRegInBuf[S_REG_INPUT_NREGS];	    //输入寄存器缓冲区
extern USHORT usSRegHoldBuf[S_REG_HOLDING_NREGS];	//保持寄存器缓冲区
       USHORT  usModbusUserData[MB_PDU_SIZE_MAX];
       UCHAR   ucModbusUserData[MB_PDU_SIZE_MAX];

/********************************function*************************************/


/**
  * @brief : ModBus从机轮询入口
  * @param : void
  * @return: void 
  * @updata: [2019-06-19][Lei][creat]
  */
static void thread_entry_ModbusSlavePoll(void* parameter)
{

    //端口选择Uart1
    //初始化 RTU模式 从机地址为1 USART1 115200 无校验
   // eMBInit(MB_RTU, 0x01, 1, g_ModbusBandrate,  g_ModbusParity);
    eMBInit(MB_RTU, 0x01, 1, 115200, g_ModbusParity);
    //启动FreeModbus
    eMBEnable();
	
    while(1)
	{
        //FreeModbus从机不断查询
        eMBPoll();		
    }
}

/**
  * @brief : ModBus从机寄存器修改
  * @param : void
  * @return: void 
  * @updata: [2019-09-11][Gang][creat]
  */

static void thread_entry_ModbusSlaveData(void* parameter)
{
    usSRegHoldBuf[1] = 0x10;
    while (1)
	{
		//usSRegInBuf[0]++;
        usSRegHoldBuf[1]++;
        rt_thread_mdelay(1000);
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
   rt_thread_init(&thread_ModbusSlavePoll,                 //线程handle
				   "MBSlavePoll",                           //线程名
                   thread_entry_ModbusSlavePoll,            //线程入口函数
				   RT_NULL,                                 //线程入口参数
				   thread_ModbusSlavePoll_stack,            //线程栈地址
                   sizeof(thread_ModbusSlavePoll_stack),    //线程栈大小
				   thread_ModbusSlavePoll_Prio,             //线程优先级
                   5);                                      //线程时间片
				   
    rt_thread_startup(&thread_ModbusSlavePoll);             //启动线程

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

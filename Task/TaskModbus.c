/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      TaskModbus.c
  * @brief:     ModBus从机任务
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-06-19
  * @update:    [2019-06-19][Lei][creat]
				[2019-09-11][Gang][update][增加Mdobus从机修改数据的任务]
				[2019-10-17][Lei][update][删除Mdobus从机修改数据的任务]
  */

/********************************include**************************************/
#include <rtthread.h>
#include "user_mb_app.h"
#include "CommunicationConfig.h"
#include "TaskModbus.h"
#include "dfs_fs.h"
#include "JsonFileOperation.h"




/*************************************static********************************************/
/* Modbus从机轮训的任务优先级，栈空间，任务结构体及入口函数 */
#define THREAD_MODBUS_SLAVE_POLL_PRIO	20
static rt_uint8_t ModbusSlavePollThreadStack[1024];
static struct rt_thread ModbusSlavePollThreadHandle;
static void ModbusSlavePollThreadEntry(void* parameter);

/* 保存配置参数到文件的任务优先级，栈空间，任务结构体及入口函数 */
#define THREAD_SAVE_CONFIG_PRIO	20
static rt_uint8_t SaveConfigThreadStack[1024];
static struct rt_thread SaveConfigThreadHandle;
static void SaveConfigThreadEntry(void* parameter);



/*************************************extern********************************************/
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
	eMBInit(MB_RTU, g_ModbusSlaveAddress, g_ModbusUartNumber, g_ModbusBandrate,  MB_PAR_NONE);
		
	eMBEnable();			//启动FreeModbus
	
	while(1)
	{
		eMBPoll();		//FreeModbus从机不断查询
	}

}


/**
  * @brief : 保存配置参数到文件的任务入口
  * @param : void
  * @return: void 
  * @updata: [2019-10-24][Lei][creat]
  */
static void SaveConfigThreadEntry(void* parameter)
{
	uint8_t ret = 0;
	
	vPort_s2j_init();			//!< 初始化json
	ret = Get_JsonFile();		//!< 获取json文件
	if(0 != ret)
	{
		rt_kprintf("Get ConfigFile.json Fail.\r\n");
		rt_kprintf("Use default Configuration.\r\n");
	}
	else
	{
		rt_kprintf("Get ConfigFile.json Success.\r\n");
		SetModbusParameter();				//由配置文件中设置Modbus参数
	}
	
	while(1)
	{
		/* TODO:判断是否有参数下发，有的话，就保存文件 */
        
		
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
	//创建静态线程   保存配置
	rt_thread_init(&SaveConfigThreadHandle,				//线程handle
					"SaveConfig",						//线程名
					SaveConfigThreadEntry,				//线程入口函数
					RT_NULL,							//线程入口参数
					SaveConfigThreadStack,				//线程栈地址
					sizeof(SaveConfigThreadStack),		//线程栈大小
					THREAD_SAVE_CONFIG_PRIO,			//线程优先级
					5);									//线程时间片
					 
	rt_thread_startup(&SaveConfigThreadHandle);			//启动线程
	
	//创建静态线程   从机轮询
	rt_thread_init(&ModbusSlavePollThreadHandle,			//线程handle
					"ModbusSlavePoll",						//线程名
					ModbusSlavePollThreadEntry,				//线程入口函数
					RT_NULL,								//线程入口参数
					ModbusSlavePollThreadStack,				//线程栈地址
					sizeof(ModbusSlavePollThreadStack),		//线程栈大小
					THREAD_MODBUS_SLAVE_POLL_PRIO,			//线程优先级
					5);										//线程时间片
					 
	rt_thread_startup(&ModbusSlavePollThreadHandle);		//启动线程

	return 0;              
}

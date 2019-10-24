/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      TaskModbus.c
  * @brief:     ModBus�ӻ�����
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-06-19
  * @update:    [2019-06-19][Lei][creat]
				[2019-09-11][Gang][update][����Mdobus�ӻ��޸����ݵ�����]
				[2019-10-17][Lei][update][ɾ��Mdobus�ӻ��޸����ݵ�����]
  */

/********************************include**************************************/
#include <rtthread.h>
#include "user_mb_app.h"
#include "CommunicationConfig.h"
#include "TaskModbus.h"
#include "dfs_fs.h"
#include "JsonFileOperation.h"




/*************************************static********************************************/
/* Modbus�ӻ���ѵ���������ȼ���ջ�ռ䣬����ṹ�弰��ں��� */
#define THREAD_MODBUS_SLAVE_POLL_PRIO	20
static rt_uint8_t ModbusSlavePollThreadStack[1024];
static struct rt_thread ModbusSlavePollThreadHandle;
static void ModbusSlavePollThreadEntry(void* parameter);

/* �������ò������ļ����������ȼ���ջ�ռ䣬����ṹ�弰��ں��� */
#define THREAD_SAVE_CONFIG_PRIO	20
static rt_uint8_t SaveConfigThreadStack[1024];
static struct rt_thread SaveConfigThreadHandle;
static void SaveConfigThreadEntry(void* parameter);



/*************************************extern********************************************/
extern USHORT usSRegHoldBuf[S_REG_HOLDING_NREGS];	//���ּĴ���������




/*************************************function******************************************/
/**
  * @brief : ModBus�ӻ���ѯ���
  * @param : void
  * @return: void 
  * @updata: [2019-06-19][Lei][creat]
  */
static void ModbusSlavePollThreadEntry(void* parameter)
{
	
	/* ��ʼ��Modbus-RTUģʽ���ӻ���ַΪ1������ʹ��USART1��������115200����У�� */
	eMBInit(MB_RTU, g_ModbusSlaveAddress, g_ModbusUartNumber, g_ModbusBandrate,  MB_PAR_NONE);
		
	eMBEnable();			//����FreeModbus
	
	while(1)
	{
		eMBPoll();		//FreeModbus�ӻ����ϲ�ѯ
	}

}


/**
  * @brief : �������ò������ļ����������
  * @param : void
  * @return: void 
  * @updata: [2019-10-24][Lei][creat]
  */
static void SaveConfigThreadEntry(void* parameter)
{
	uint8_t ret = 0;
	
	vPort_s2j_init();			//!< ��ʼ��json
	ret = Get_JsonFile();		//!< ��ȡjson�ļ�
	if(0 != ret)
	{
		rt_kprintf("Get ConfigFile.json Fail.\r\n");
		rt_kprintf("Use default Configuration.\r\n");
	}
	else
	{
		rt_kprintf("Get ConfigFile.json Success.\r\n");
		SetModbusParameter();				//�������ļ�������Modbus����
	}
	
	while(1)
	{
		/* TODO:�ж��Ƿ��в����·����еĻ����ͱ����ļ� */
        
		
	}
}


/**
  * @brief : �������Modbus�߳�
  * @param : void
  * @return: void 
  * @updata: [2019-09-11][Gang][creat]
  */
int  TaskModbusInit(void)
{
	//������̬�߳�   ��������
	rt_thread_init(&SaveConfigThreadHandle,				//�߳�handle
					"SaveConfig",						//�߳���
					SaveConfigThreadEntry,				//�߳���ں���
					RT_NULL,							//�߳���ڲ���
					SaveConfigThreadStack,				//�߳�ջ��ַ
					sizeof(SaveConfigThreadStack),		//�߳�ջ��С
					THREAD_SAVE_CONFIG_PRIO,			//�߳����ȼ�
					5);									//�߳�ʱ��Ƭ
					 
	rt_thread_startup(&SaveConfigThreadHandle);			//�����߳�
	
	//������̬�߳�   �ӻ���ѯ
	rt_thread_init(&ModbusSlavePollThreadHandle,			//�߳�handle
					"ModbusSlavePoll",						//�߳���
					ModbusSlavePollThreadEntry,				//�߳���ں���
					RT_NULL,								//�߳���ڲ���
					ModbusSlavePollThreadStack,				//�߳�ջ��ַ
					sizeof(ModbusSlavePollThreadStack),		//�߳�ջ��С
					THREAD_MODBUS_SLAVE_POLL_PRIO,			//�߳����ȼ�
					5);										//�߳�ʱ��Ƭ
					 
	rt_thread_startup(&ModbusSlavePollThreadHandle);		//�����߳�

	return 0;              
}

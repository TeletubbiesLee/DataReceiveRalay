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




/*************************************static********************************************/
/* Modbus�ӻ���ѵ���������ȼ���ջ�ռ䣬����ṹ�弰��ں��� */
#define THREAD_MODBUS_SLAVE_POLL_PRIO	10
static rt_uint8_t ModbusSlavePollThreadStack[512];
static struct rt_thread ModbusSlavePollThreadHandle;
static void ModbusSlavePollThreadEntry(void* parameter);


#define RT_SPI_FLASH_NAME "W25Q256FV"


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
	eMBInit(MB_RTU, g_ModbusSlaveAddress, g_ModbusUartNumber, g_ModbusBandrate,  g_ModbusParity);
		
	eMBEnable();			//����FreeModbus
	
	while(1)
	{
		eMBPoll();		//FreeModbus�ӻ����ϲ�ѯ
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
	//������̬�߳�   �ӻ���ѯ
	rt_thread_init(&ModbusSlavePollThreadHandle,           //�߳�handle
					"ModbusSlavePoll",                     //�߳���
					ModbusSlavePollThreadEntry,            //�߳���ں���
					RT_NULL,                               //�߳���ڲ���
					ModbusSlavePollThreadStack,            //�߳�ջ��ַ
					sizeof(ModbusSlavePollThreadStack),    //�߳�ջ��С
					THREAD_MODBUS_SLAVE_POLL_PRIO,         //�߳����ȼ�
					5);                                    //�߳�ʱ��Ƭ
					 
	rt_thread_startup(&ModbusSlavePollThreadHandle);             //�����߳�

					 
	return 0;              
}

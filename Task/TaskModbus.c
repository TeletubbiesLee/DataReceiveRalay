/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      TaskModbus.c
  * @brief:     ModBus�ӻ�����
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-06-19
  * @update:    [2019-06-19][Lei][creat]
                [2019-09-11][Gang][update][����Mdobus�ӻ��޸����ݵ�����]
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

uint32_t g_ModbusBandrate = 115200;		//Modbus�Ĳ�����
uint8_t  g_ModbusParity = MB_PAR_NONE;	//Modbus����żУ�鷽ʽ������żУ��λ


#define thread_ModbusSlavePoll_Prio         10
#define thread_ModbusSlaveData_Prio         11


static rt_uint8_t thread_ModbusSlavePoll_stack[512];
struct rt_thread thread_ModbusSlavePoll;
static rt_uint8_t thread_ModbusSlaveData_stack[512];
struct rt_thread thread_ModbusSlaveData;


static void thread_entry_ModbusSlavePoll(void* parameter);
static void thread_entry_ModbusSlaveData(void* parameter);

extern USHORT usSRegInBuf[S_REG_INPUT_NREGS];	    //����Ĵ���������
extern USHORT usSRegHoldBuf[S_REG_HOLDING_NREGS];	//���ּĴ���������
       USHORT  usModbusUserData[MB_PDU_SIZE_MAX];
       UCHAR   ucModbusUserData[MB_PDU_SIZE_MAX];

/********************************function*************************************/


/**
  * @brief : ModBus�ӻ���ѯ���
  * @param : void
  * @return: void 
  * @updata: [2019-06-19][Lei][creat]
  */
static void thread_entry_ModbusSlavePoll(void* parameter)
{

    //�˿�ѡ��Uart1
    //��ʼ�� RTUģʽ �ӻ���ַΪ1 USART1 115200 ��У��
   // eMBInit(MB_RTU, 0x01, 1, g_ModbusBandrate,  g_ModbusParity);
    eMBInit(MB_RTU, 0x01, 1, 115200, g_ModbusParity);
    //����FreeModbus
    eMBEnable();
	
    while(1)
	{
        //FreeModbus�ӻ����ϲ�ѯ
        eMBPoll();		
    }
}

/**
  * @brief : ModBus�ӻ��Ĵ����޸�
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
  * @brief : �������Modbus�߳�
  * @param : void
  * @return: void 
  * @updata: [2019-09-11][Gang][creat]
  */
int  TaskModbusInit(void)
{              
    //������̬�߳�   �ӻ���ѯ
   rt_thread_init(&thread_ModbusSlavePoll,                 //�߳�handle
				   "MBSlavePoll",                           //�߳���
                   thread_entry_ModbusSlavePoll,            //�߳���ں���
				   RT_NULL,                                 //�߳���ڲ���
				   thread_ModbusSlavePoll_stack,            //�߳�ջ��ַ
                   sizeof(thread_ModbusSlavePoll_stack),    //�߳�ջ��С
				   thread_ModbusSlavePoll_Prio,             //�߳����ȼ�
                   5);                                      //�߳�ʱ��Ƭ
				   
    rt_thread_startup(&thread_ModbusSlavePoll);             //�����߳�

    //������̬�߳�   �ӻ��Ĵ������ݸı�
    rt_thread_init(&thread_ModbusSlaveData,                 //�߳�handle
				   "MBSlaveData",                           //�߳���
                   thread_entry_ModbusSlaveData,            //�߳���ں���
				   RT_NULL,                                 //�߳���ڲ���
				   thread_ModbusSlaveData_stack,            //�߳�ջ��ַ
                   sizeof(thread_ModbusSlaveData_stack),    //�߳�ջ��С
				   thread_ModbusSlaveData_Prio,             //�߳����ȼ�
                   5);                                      //�߳�ʱ��Ƭ
				   
    rt_thread_startup(&thread_ModbusSlaveData);             //�����߳�                      
    return 0;              
}

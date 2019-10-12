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

/*************************************global********************************************/
uint32_t g_ModbusBandrate = 115200;		//Modbus�Ĳ�����
uint8_t  g_ModbusParity = MB_PAR_NONE;	//Modbus����żУ�鷽ʽ������żУ��λ



/*************************************static********************************************/
/* Modbus�ӻ���ѵ���������ȼ���ջ�ռ䣬����ṹ�弰��ں��� */
#define THREAD_MODBUS_SLAVE_POLL_PRIO	10
static rt_uint8_t ThreadModbusSlavePollStack[512];
struct rt_thread ThreadModbusSlavePoll;
static void ModbusSlavePollThreadEntry(void* parameter);



/*************************************extern********************************************/
extern USHORT usSRegInBuf[S_REG_INPUT_NREGS];	    //����Ĵ���������
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
	eMBInit(MB_RTU, 0x01, 1, g_ModbusBandrate,  g_ModbusParity);
    
    eMBEnable();			//����FreeModbus
	
    while(1)
	{
        //FreeModbus�ӻ����ϲ�ѯ
        eMBPoll();		
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
   rt_thread_init(&ThreadModbusSlavePoll,                 //�߳�handle
				   "ModbusSlavePoll",                           //�߳���
                   ModbusSlavePollThreadEntry,            //�߳���ں���
				   RT_NULL,                                 //�߳���ڲ���
				   ThreadModbusSlavePollStack,            //�߳�ջ��ַ
                   sizeof(ThreadModbusSlavePollStack),    //�߳�ջ��С
				   THREAD_MODBUS_SLAVE_POLL_PRIO,             //�߳����ȼ�
                   5);                                      //�߳�ʱ��Ƭ
				   
    rt_thread_startup(&ThreadModbusSlavePoll);             //�����߳�

				   
    return 0;              
}

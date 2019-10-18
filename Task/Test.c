/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      TaskDataReceive.c
  * @brief:     ���������������ڲ��Բ�ƷID��
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

extern USHORT usSRegHoldBuf[S_REG_HOLDING_NREGS];	//���ּĴ���������

/**
  * @brief : ModBus�ӻ���ؼĴ����޸�
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

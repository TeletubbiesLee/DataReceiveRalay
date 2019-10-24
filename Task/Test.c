/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      TaskDataReceive.c
  * @brief:     ���������������ڲ��Բ�ƷID��
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

/*************************************static********************************************/
/* �����������ȼ���ջ�ռ䣬����ṹ�弰��ں��� */
#define THREAD_TASK_TEST_PRIO 9
static rt_uint8_t TaskTestStack[512];
static struct rt_thread ThreadModbusSlaveData;
static void ModbusSlaveDataThreadEntry(void* parameter);

/*************************************extern********************************************/
extern USHORT usSRegHoldBuf[S_REG_HOLDING_NREGS];    //���ּĴ���������

/*************************************function********************************************/
/**
  * @brief : ModBus�ӻ���ؼĴ����޸�
  * @param : void
  * @return: void 
  * @updata: [2019-09-11][Gang][creat]
  */
static void ModbusSlaveDataThreadEntry(void* parameter)
{
	
	
    while (1)
    {
        /* �����д�ŵ�ַ�Ŀռ�����*/
        uint16_t i;
        for (i = 0; i <= 255; i++)
        {
           usSRegHoldBuf[NODE_DEVICE_ID_FIRST_ADDRESS + 2 * i]= 0;		   
         }
        usSRegHoldBuf[0x710] = 0xFFFF;
        usSRegHoldBuf[0x711] = 0xFFFF; 
		
        rt_thread_mdelay(1000);
    }
}

int TaskTestInit(void)
{
    //������̬�߳�   �ӻ��Ĵ������ݸı�
    rt_thread_init(&ThreadModbusSlaveData,                 //�߳�handle
                   "TaskTest",                             //�߳���
                   ModbusSlaveDataThreadEntry,             //�߳���ں���
                   RT_NULL,                                //�߳���ڲ���
                   TaskTestStack,                          //�߳�ջ��ַ
                   sizeof(TaskTestStack),                  //�߳�ջ��С
                   THREAD_TASK_TEST_PRIO,                  //�߳����ȼ�
                   5);                                     //�߳�ʱ��Ƭ
    rt_thread_startup(&ThreadModbusSlaveData);             //�����߳�
                   
    return 0;
}

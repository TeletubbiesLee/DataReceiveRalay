/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      TaskDataReceive.c
  * @brief:     ���ݽ�����������ļ�
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-10-11
  * @update:    [2019-10-11][Lei][creat]
  */

/*************************************include*******************************************/
#include <rtthread.h>
#include <string.h>
#include "CC1101.h"
#include "TaskDataReceive.h"
#include "DataFrame.h"
#include "Timer.h"
#include "ConfigFile.h"
#include "user_mb_app.h"
//���Դ�ӡ�����Ҫ
#include "TaskConfig.h"
#include <stdio.h>

extern USHORT usSRegHoldBuf[S_REG_HOLDING_NREGS];
/*************************************static********************************************/
/* CC1101�������ݽ��յ��������ȼ���ջ�ռ䣬����ṹ�弰��ں��� */
#define THREAD_TASK_DATA_RECEIVE_PRIO    11
static rt_uint8_t TaskDataReceiveStack[1024];
static struct rt_thread ThreadTaskDataReceive;
static void TaskDataReceiveThreadEntry(void* parameter);

/* ����������ݽ����Ƿ�ʱ���������ȼ���ջ�ռ䣬����ṹ�弰��ں��� */
#define THREAD_TASK_CHECK_OVER_TIME_PRIO    11
static rt_uint8_t TaskCheckOverTimeStack[1024];
static struct rt_thread ThreadTaskCheckOverTime;
static void TaskCheckOverTimeThreadEntry(void* parameter);


/*************************************function********************************************/
/**
  * @brief : CC1101�������ݽ���
  * @param : void
  * @return: void 
  * @updata: [2019-10-12][Gang][creat]
  */
static void TaskDataReceiveThreadEntry(void* parameter)
{
    uint8_t ret = 0;                    //�Ӻ�������ֵ
    uint8_t num = 0;                    //num���ڼ�ʱ1s ��CC1101��λ
    uint8_t rxBuffer[12] = {0};         //����֡�������Լ�����
    uint8_t leng = 0;
    NodeDataStruct nodeData;            //����ڵ����ݵĽṹ��
    
    /* CC1101��ʼ������ */
    CC1101_PowerUp();
    CC1101_SettingsReg();

    NodeDataStructInit(&nodeData);      //��ʼ���ڵ����ݽṹ��

    while(1)
    {
        /* �������ݣ������д��� */
        if(CC1101_ReceivePacket(rxBuffer, &leng))
        {
            
            DebugPrintf("ѧϰ��������ݣ�0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X\r\n", 
            rxBuffer[0], rxBuffer[1], rxBuffer[2], rxBuffer[3], rxBuffer[4], rxBuffer[5], rxBuffer[6], rxBuffer[7] , rxBuffer[8], rxBuffer[9], rxBuffer[10], rxBuffer[11]);
            rt_thread_mdelay(10);
            
            /* �Խ��յ������ݽ��д��� */
            ret = DataFrameAnalysis(rxBuffer, &nodeData);
                        
            /* ���ڴ�ӡ��� */
            uint8_t temperatureString[10] = {0};
            uint8_t voltageString[10] = {0};

            DebugPrintf("���߷�����Ψһ����Ϊ��0x%X \r\n", nodeData.deviceId);
            sprintf((char*)temperatureString, "%.1f", nodeData.temperatureValue);
            DebugPrintf("�¶ȴ������¶�ֵΪ��%s��\r\n", temperatureString );
            sprintf((char*)voltageString, "%.1f", nodeData.voltageValue);
            DebugPrintf("��õ�ѹ����Ϊ��%smV\r\n", voltageString ); 
            DebugPrintf("�ź�ǿ��Ϊ��%d \r\n", nodeData.RSSI_Value ); 
            DebugPrintf("LQIΪ��%d \r\n", nodeData.LQI_Value );
            
            if (0 == ret)
            {
                ret = nodeData.getDeviceNumber(&nodeData);      //����Ψһ�豸ID�Ż�ȡ����������
                
                if (0 == ret)
                {
                    DebugPrintf("�豸�����ݱ��еı�ţ�%d\r\n", nodeData.deviceNumber);
                    
                    /* �����¶�ֵ�͵�ѹֵ���ź�ǿ�ȡ�����ʱ�� */
                    nodeData.saveTemperature(nodeData);
                    nodeData.saveVoltage(nodeData);
                    nodeData.saveSignalStrength(nodeData);
                    nodeData.saveLaunchNumber(nodeData);
                    nodeData.saveReceiveTime(nodeData);
                }
                else
                {
                    DebugPrintf("�豸ID�Ų�����");
                }
            }
            else
            {
                /* ���ݷ���ֵ���� */
                if (1 == ret)
                { 
                    DebugPrintf("У��ʹ���"); 
                }
                else if (2 == ret)
                { 
                    DebugPrintf("��������"); 
                }
            }

            memset(rxBuffer, 0, leng);
            nodeData.isDataValid = false;		//һ�����ݴ�����ϣ���������Ч��־��Ϊ��Ч

            CC1101_Reset();
            CC1101_SettingsReg();
            num = 0;
        }
        if (20 == num)
        {
            CC1101_Reset();
            CC1101_SettingsReg();
            num = 0;
        }   
        num++;

        rt_thread_mdelay(50);
    }
}


/**
  * @brief : ����������ݽ����Ƿ�ʱ
  * @param : void
  * @return: void 
  * @updata: [2019-11-05][Lei][creat]
  */
static void TaskCheckOverTimeThreadEntry(void* parameter)
{
    uint32_t startTime = 0;           //���ĳһ�ڵ��ϴν���ʱ��
    uint32_t overtime = g_ConfigFile[2].parameter * 60;         //���õĳ�ʱʱ��
    uint8_t ret = 0;
    
    TimerInit();        //��ʼ�����ڵ㳬ʱ�Ķ�ʱ��
    while (1)
    {   /* �����ж�256���ڵ��Ƿ�ʱ�������ʱ�������������� */
        for (uint16_t i = 0; i < 256; i++)
        {
            /* TODO:��usSRegHoldBuf�л�ȡ��i���Ľ���ʱ�� */
            startTime = usSRegHoldBuf[RECEIVING_MOMENT_FIRAT_ADDRESS + 2 * i]\
            + (usSRegHoldBuf[RECEIVING_MOMENT_FIRAT_ADDRESS + 1 + 2 * i] << 16);
            if (0 == startTime)		//�����һ�ν���ʱ��Ϊ0��ֱ��������һ�ڵ��ж�
            {
                continue;
            }
            
            ret = CheckOvertime(startTime, overtime);
            rt_kprintf("checkovertime ret = %d\n",ret);
            if (0 != ret)
            {
                /* TODO:��ճ�ʱ�Ľڵ������ */
                usSRegHoldBuf[TEMPERATURE_FIRST_ADDRESS + i] = 0;  //�¶�������0;
                usSRegHoldBuf[VOLTAGE_FIRST_ADDRESS + i] = 0;      //��ѹ������0;  
                usSRegHoldBuf[NODE_STATUS_FIRST_ADDRESS + 2 + 3 * i] = 0; //�����Ⱥ��ź�ǿ����0;
                usSRegHoldBuf[LAUNCH_NUMBER_FIRAT_ADDRESS + 2 * i] = 0;
                usSRegHoldBuf[LAUNCH_NUMBER_FIRAT_ADDRESS + 1 + 2 * i] = 0;  //���������0
                usSRegHoldBuf[RECEIVING_MOMENT_FIRAT_ADDRESS + 2 * i] = 0;
                usSRegHoldBuf[RECEIVING_MOMENT_FIRAT_ADDRESS + 1 + 2 * i] = 0;  //����ʱ����0           
            }
         }
         rt_thread_mdelay(50);
    }

}


/**
  * @brief : ����CC1101�������ݽ�������
  * @param : void
  * @return: void 
  * @updata: [2019-10-12][Gang][creat]
  */
int TaskDataReceiveInit(void)
{
    /* �����������ݽ��յľ�̬���� */
    rt_thread_init(&ThreadTaskDataReceive,
                   "TaskDataReceive",
                   TaskDataReceiveThreadEntry,
                   RT_NULL,
                   TaskDataReceiveStack,
                   sizeof(TaskDataReceiveStack),
                   THREAD_TASK_DATA_RECEIVE_PRIO,
                   5);
    rt_thread_startup(&ThreadTaskDataReceive);
         
    /* ����������߽��������Ƿ�ʱ�ľ�̬���� */
    rt_thread_init(&ThreadTaskCheckOverTime,
                   "TaskCheckOverTime",
                   TaskCheckOverTimeThreadEntry,
                   RT_NULL,
                   TaskCheckOverTimeStack,
                   sizeof(TaskCheckOverTimeStack),
                   THREAD_TASK_CHECK_OVER_TIME_PRIO,
                   5);
    rt_thread_startup(&ThreadTaskCheckOverTime);

    return 0;
}


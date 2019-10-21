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

//���Դ�ӡ�����Ҫ
#include "TaskConfig.h"
#include <stdio.h>

/*************************************static********************************************/
/* CC1101�������ݽ��յ��������ȼ���ջ�ռ䣬����ṹ�弰��ں��� */
#define THREAD_TASK_DATA_RECEIVE_PRIO    11
static rt_uint8_t TaskDataReceiveStack[1024];
static struct rt_thread ThreadTaskDataReceive;
static void TaskDataReceiveThreadEntry(void* parameter);

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

    while (1)
    {
        /* �������ݣ������д��� */
        if (CC1101_ReceivePacket(rxBuffer, &leng))
        {
            
            DebugPrintf("ѧϰ��������ݣ�0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X\r\n", 
            rxBuffer[0], rxBuffer[1], rxBuffer[2], rxBuffer[3], rxBuffer[4], rxBuffer[5], rxBuffer[6], rxBuffer[7] , rxBuffer[8], rxBuffer[9], rxBuffer[10], rxBuffer[11]);
            rt_thread_mdelay(10);
            
            /* �Խ��յ������ݽ��д��� */
            ret = DataFrameAnalysis(rxBuffer, &nodeData);       //�������߽��յ�����
            
            /* ���ڴ�ӡ��� */
            uint8_t temperatureString[10] = "";
            uint8_t voltageString[10] = "";

            DebugPrintf("���߷�����Ψһ����Ϊ��0x%X \r\n", nodeData.deviceId);
            sprintf((char*)temperatureString, "%.1f", nodeData.temperatureValue);
            DebugPrintf("�¶ȴ������¶�ֵΪ��%s��\r\n", temperatureString );
            sprintf((char*)voltageString, "%.1f", nodeData.voltageValue);
            DebugPrintf("��õ�ѹ����Ϊ��%smV\r\n", voltageString ); 
                        
            if (0 == ret)
            {
            /* �������� */
            ret = nodeData.getDeviceNumber(&nodeData);      //����Ψһ�豸ID�Ż�ȡ����������
            
                DebugPrintf("�豸�����ݱ��еı�ţ�%d\r\n", nodeData.deviceNumber);
                
                if (0 == ret)
                {
                /* �����¶�ֵ�͵�ѹֵ */
                    nodeData.saveTemperature(nodeData);
                    nodeData.saveVoltage(nodeData);
                }
            }
            else
            {
                /* ���ݷ���ֵ���� */
                if (1 == ret)
                { 
                DebugPrintf("У��ʹ���" ); 
                }
                if (2 == ret)
                { 
                DebugPrintf("��������" ); 
                }
            }

            memset(rxBuffer, 0, leng);
            nodeData.isDataValid = false;

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

    return 0;
}


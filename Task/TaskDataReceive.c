/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      TaskDataReceive.c
  * @brief:     数据接收任务相关文件
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

//测试打印输出需要
#include "TaskConfig.h"
#include <stdio.h>

/*************************************static********************************************/
/* CC1101无线数据接收的任务优先级，栈空间，任务结构体及入口函数 */
#define THREAD_TASK_DATA_RECEIVE_PRIO    11
static rt_uint8_t TaskDataReceiveStack[1024];
static struct rt_thread ThreadTaskDataReceive;
static void TaskDataReceiveThreadEntry(void* parameter);

/*************************************function********************************************/
/**
  * @brief : CC1101无线数据接收
  * @param : void
  * @return: void 
  * @updata: [2019-10-12][Gang][creat]
  */
static void TaskDataReceiveThreadEntry(void* parameter)
{
    uint8_t ret = 0;                    //子函数返回值
    uint8_t num = 0;                    //num用于计时1s 将CC1101复位
    uint8_t rxBuffer[12] = {0};         //数据帧的数据以及长度
    uint8_t leng = 0;
    NodeDataStruct nodeData;            //保存节点数据的结构体
    
    /* CC1101初始化配置 */
    CC1101_PowerUp();
    CC1101_SettingsReg();

    NodeDataStructInit(&nodeData);      //初始化节点数据结构体

    while (1)
    {
        /* 接收数据，并进行处理 */
        if (CC1101_ReceivePacket(rxBuffer, &leng))
        {
            
            DebugPrintf("学习板接收数据：0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X\r\n", 
            rxBuffer[0], rxBuffer[1], rxBuffer[2], rxBuffer[3], rxBuffer[4], rxBuffer[5], rxBuffer[6], rxBuffer[7] , rxBuffer[8], rxBuffer[9], rxBuffer[10], rxBuffer[11]);
            rt_thread_mdelay(10);
            
            /* 对接收到的数据进行处理 */
            ret = DataFrameAnalysis(rxBuffer, &nodeData);       //解析无线接收的数据
            
            /* 串口打印输出 */
            uint8_t temperatureString[10] = "";
            uint8_t voltageString[10] = "";

            DebugPrintf("无线发送器唯一编码为：0x%X \r\n", nodeData.deviceId);
            sprintf((char*)temperatureString, "%.1f", nodeData.temperatureValue);
            DebugPrintf("温度传感器温度值为：%s℃\r\n", temperatureString );
            sprintf((char*)voltageString, "%.1f", nodeData.voltageValue);
            DebugPrintf("测得电压数据为：%smV\r\n", voltageString ); 
                        
            if (0 == ret)
            {
            /* 处理数据 */
            ret = nodeData.getDeviceNumber(&nodeData);      //根据唯一设备ID号获取数据区编码
            
                DebugPrintf("设备在数据表中的编号：%d\r\n", nodeData.deviceNumber);
                
                if (0 == ret)
                {
                /* 保存温度值和电压值 */
                    nodeData.saveTemperature(nodeData);
                    nodeData.saveVoltage(nodeData);
                }
            }
            else
            {
                /* 根据返回值报错 */
                if (1 == ret)
                { 
                DebugPrintf("校验和错误" ); 
                }
                if (2 == ret)
                { 
                DebugPrintf("其他错误" ); 
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
  * @brief : 创建CC1101无线数据接收任务
  * @param : void
  * @return: void 
  * @updata: [2019-10-12][Gang][creat]
  */
int TaskDataReceiveInit(void)
{
    /* 创建无线数据接收的静态任务 */
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


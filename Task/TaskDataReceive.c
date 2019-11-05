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
#include "Timer.h"
#include "ConfigFile.h"
#include "user_mb_app.h"
//测试打印输出需要
#include "TaskConfig.h"
#include <stdio.h>

extern USHORT usSRegHoldBuf[S_REG_HOLDING_NREGS];
/*************************************static********************************************/
/* CC1101无线数据接收的任务优先级，栈空间，任务结构体及入口函数 */
#define THREAD_TASK_DATA_RECEIVE_PRIO    11
static rt_uint8_t TaskDataReceiveStack[1024];
static struct rt_thread ThreadTaskDataReceive;
static void TaskDataReceiveThreadEntry(void* parameter);

/* 检测无线数据接收是否超时的任务优先级，栈空间，任务结构体及入口函数 */
#define THREAD_TASK_CHECK_OVER_TIME_PRIO    11
static rt_uint8_t TaskCheckOverTimeStack[1024];
static struct rt_thread ThreadTaskCheckOverTime;
static void TaskCheckOverTimeThreadEntry(void* parameter);


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

    while(1)
    {
        /* 接收数据，并进行处理 */
        if(CC1101_ReceivePacket(rxBuffer, &leng))
        {
            
            DebugPrintf("学习板接收数据：0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X\r\n", 
            rxBuffer[0], rxBuffer[1], rxBuffer[2], rxBuffer[3], rxBuffer[4], rxBuffer[5], rxBuffer[6], rxBuffer[7] , rxBuffer[8], rxBuffer[9], rxBuffer[10], rxBuffer[11]);
            rt_thread_mdelay(10);
            
            /* 对接收到的数据进行处理 */
            ret = DataFrameAnalysis(rxBuffer, &nodeData);
                        
            /* 串口打印输出 */
            uint8_t temperatureString[10] = {0};
            uint8_t voltageString[10] = {0};

            DebugPrintf("无线发送器唯一编码为：0x%X \r\n", nodeData.deviceId);
            sprintf((char*)temperatureString, "%.1f", nodeData.temperatureValue);
            DebugPrintf("温度传感器温度值为：%s℃\r\n", temperatureString );
            sprintf((char*)voltageString, "%.1f", nodeData.voltageValue);
            DebugPrintf("测得电压数据为：%smV\r\n", voltageString ); 
            DebugPrintf("信号强度为：%d \r\n", nodeData.RSSI_Value ); 
            DebugPrintf("LQI为：%d \r\n", nodeData.LQI_Value );
            
            if (0 == ret)
            {
                ret = nodeData.getDeviceNumber(&nodeData);      //根据唯一设备ID号获取数据区编码
                
                if (0 == ret)
                {
                    DebugPrintf("设备在数据表中的编号：%d\r\n", nodeData.deviceNumber);
                    
                    /* 保存温度值和电压值、信号强度、接收时刻 */
                    nodeData.saveTemperature(nodeData);
                    nodeData.saveVoltage(nodeData);
                    nodeData.saveSignalStrength(nodeData);
                    nodeData.saveLaunchNumber(nodeData);
                    nodeData.saveReceiveTime(nodeData);
                }
                else
                {
                    DebugPrintf("设备ID号不存在");
                }
            }
            else
            {
                /* 根据返回值报错 */
                if (1 == ret)
                { 
                    DebugPrintf("校验和错误"); 
                }
                else if (2 == ret)
                { 
                    DebugPrintf("其他错误"); 
                }
            }

            memset(rxBuffer, 0, leng);
            nodeData.isDataValid = false;		//一组数据处理完毕，将数据有效标志置为无效

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
  * @brief : 检测无线数据接收是否超时
  * @param : void
  * @return: void 
  * @updata: [2019-11-05][Lei][creat]
  */
static void TaskCheckOverTimeThreadEntry(void* parameter)
{
    uint32_t startTime = 0;           //存放某一节点上次接收时刻
    uint32_t overtime = g_ConfigFile[2].parameter * 60;         //设置的超时时间
    uint8_t ret = 0;
    
    TimerInit();        //初始化检测节点超时的定时器
    while (1)
    {   /* 轮流判断256个节点是否超时，如果超时，就清空相关数据 */
        for (uint16_t i = 0; i < 256; i++)
        {
            /* TODO:从usSRegHoldBuf中获取第i个的接收时刻 */
            startTime = usSRegHoldBuf[RECEIVING_MOMENT_FIRAT_ADDRESS + 2 * i]\
            + (usSRegHoldBuf[RECEIVING_MOMENT_FIRAT_ADDRESS + 1 + 2 * i] << 16);
            if (0 == startTime)		//如果上一次接收时刻为0，直接跳过这一节点判断
            {
                continue;
            }
            
            ret = CheckOvertime(startTime, overtime);
            rt_kprintf("checkovertime ret = %d\n",ret);
            if (0 != ret)
            {
                /* TODO:清空超时的节点的数据 */
                usSRegHoldBuf[TEMPERATURE_FIRST_ADDRESS + i] = 0;  //温度数据清0;
                usSRegHoldBuf[VOLTAGE_FIRST_ADDRESS + i] = 0;      //电压数据清0;  
                usSRegHoldBuf[NODE_STATUS_FIRST_ADDRESS + 2 + 3 * i] = 0; //灵敏度和信号强度清0;
                usSRegHoldBuf[LAUNCH_NUMBER_FIRAT_ADDRESS + 2 * i] = 0;
                usSRegHoldBuf[LAUNCH_NUMBER_FIRAT_ADDRESS + 1 + 2 * i] = 0;  //发射次数清0
                usSRegHoldBuf[RECEIVING_MOMENT_FIRAT_ADDRESS + 2 * i] = 0;
                usSRegHoldBuf[RECEIVING_MOMENT_FIRAT_ADDRESS + 1 + 2 * i] = 0;  //接收时刻清0           
            }
         }
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
         
    /* 创建检测无线接收数据是否超时的静态任务 */
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


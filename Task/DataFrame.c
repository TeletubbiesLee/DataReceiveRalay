/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      DataFrame.c
  * @brief:     传输的数据帧等相关应用
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-06-21
  * @update:    [2019-06-21][Lei][creat]
                [2019-10-18][Gang][update][对一些函数内容进行补充]
  */
/********************************include**************************************/
#include "DataFrame.h"
#include "math.h"
#include "user_mb_app.h"

//测试打印输出需要的头文件
#include "TaskConfig.h"
#include <stdio.h>

/********************************static**************************************/
static uint8_t GetDeviceNumber(struct NodeData* nodeData);
static void SaveTemperature(struct NodeData nodeData);
static void SaveVoltage(struct NodeData nodeData);
static uint8_t CheckSum(uint8_t* data, uint8_t lenth);
static uint32_t CalculateDeviceId(uint8_t* data);
static float CalculateVolatge(uint8_t* data);
static float CalculateTemperature(uint8_t* data);

/********************************extern*************************************/
extern USHORT usSRegHoldBuf[S_REG_HOLDING_NREGS];    //保持寄存器缓冲区

/********************************function*************************************/
/**
  * @brief : 保存数据节点结构体的初始化函数
  * @param : nodeData 保存节点数据的结构体的指针
  * @return: void 
  * @updata: [2019-10-17][Lei][creat]
  */
void NodeDataStructInit(NodeDataStruct* nodeData)
{
    nodeData->isDataValid = false;
    nodeData->getDeviceNumber = GetDeviceNumber;
    nodeData->saveTemperature = SaveTemperature;
    nodeData->saveVoltage = SaveVoltage;
}


/**
  * @brief : CC1101接收数据帧解析
  * @param : sourceData CC1101接收到的数据
  * @param : nodeData 保存节点数据的结构体
  * @return: 0:解析正确; 1:校验码错误; 2:其他 
  * @updata: [2019-10-17][Lei][creat]
  */
uint8_t DataFrameAnalysis(uint8_t* sourceData, NodeDataStruct* nodeData)
{
    uint8_t ret = 0;
    if (0x5A == sourceData[0])                   //判断数据帧头
    {
        if (0 == CheckSum(sourceData, 12))       //判断累加校验和
        {
            nodeData->deviceId = CalculateDeviceId(&sourceData[2]);
            nodeData->temperatureValue = CalculateTemperature(&sourceData[6]);
            nodeData->voltageValue = CalculateVolatge(&sourceData[8]);
            nodeData->isDataValid = true;
        }
        else
        {
            ret = 1;
        }
    }
    else
    {
        ret = 2;
    }

    return ret;
}


/**
  * @brief : 由设备ID号获取在数据表中对应的编号，将编号记录在结构体中
  * @param : nodeData 保存节点数据的结构体的指针
  * @return: 0:存在ID号; 1:ID号不存在
  * @updata: [2019-10-17][Lei][creat]
             [2019-10-18][Gang][update][补充函数内容]
  */
static uint8_t GetDeviceNumber(struct NodeData* nodeData)
{
    uint8_t ret = 1;
    uint16_t i;
    
    for (i = 0; i <= 255; i++)
    {
        if ((nodeData->deviceId) == (usSRegHoldBuf[0x708 + i] + (usSRegHoldBuf[0x709 + i] << 16)))
        {
            nodeData->deviceNumber = i;
            ret = 0;
        }
        else
        {
            ret = 1;
        }        
    }
    
    return ret;
}


/**
  * @brief : 将温度值保存在数据表中的正确位置
  * @param : nodeData 保存节点数据的结构体
  * @return: void 
  * @updata: [2019-10-17][Lei][creat]
             [2019-10-18][Gang][update][补充函数内容]
  */
static void SaveTemperature(struct NodeData nodeData)
{    
    float temperature = 0;
    uint16_t temperatureFormat = 0;
    temperature =((float)((int)((nodeData.temperatureValue + 0.05) * 10))) / 10;
    
    uint8_t temperatureString[10] = "";
    sprintf(temperatureString, "%f", temperature);
    DebugPrintf("温度保留一位小数：%s\r\n", temperatureString);
    if (nodeData.isDataValid == true)
    {
        if (temperature > 0)
        {
            temperatureFormat = (uint16_t)(temperature * 10);
        }            
        else
        {
            temperatureFormat = 2000 + (uint16_t)(temperature * 10);
        }
        usSRegHoldBuf[0x008 + nodeData.deviceNumber] = temperatureFormat;
     }
}


/**
  * @brief : 将电压值保存在数据表中的正确位置
  * @param : nodeData 保存节点数据的结构体
  * @return: void 
  * @updata: [2019-10-17][Lei][creat]
             [2019-10-18][Gang][update][补充函数内容]
  */
static void SaveVoltage(struct NodeData nodeData)
{
    int16_t voltage = 0;
    
    if (nodeData.isDataValid == true)
    {
        voltage = (int16_t)(nodeData.voltageValue);
        DebugPrintf("电压取整数据：%d\r\n", voltage);
        usSRegHoldBuf[0x108 + nodeData.deviceNumber] = voltage;  
    }

}


/**
  * @brief : 累加校验和计算
  * @param : data 要计算的数据
  * @param : lenth 数据的总长度
  * @return: 0:校验正确; 1:校验不正确
  * @updata: [2019-10-17][Lei][creat]
             [2019-10-18][Gang][update][补充函数内容]
  */
static uint8_t CheckSum(uint8_t* data, uint8_t lenth)
{   
    uint8_t ret = 1;
    uint16_t result = 0;
    
    for (uint8_t i = 0; i < (lenth - 2); i++)
    {
      result += data[i];
    }
    
    if (data[lenth - 2] == (result & 0xFF00) >> 8 && data[lenth - 1] == (result & 0x00FF))
    {
        ret = 0;
    }
    else
    {
        ret = 1;
    }

    return ret;
}


/**
  * @brief : 计算设备唯一ID号
  * @param : data ID号原始值
  * @return: 得出的ID号
  * @updata: [2019-10-17][Lei][creat]
             [2019-10-17][Gang][update][补充函数内容,修改参数]
  */
static uint32_t CalculateDeviceId(uint8_t* data)
{
    uint32_t deviceId = 0;

    deviceId = (data[0] << 24) + (data[1] << 16) + (data[2] << 8) + data[3];
    
    return deviceId;
}


/**
  * @brief : 计算电压值
  * @param : data 电压原始值
  * @return: 计算出的电压值
  * @updata: [2019-10-17][Lei][creat]
             [2019-10-18][Gang][update][补充函数内容,修改参数]
  */
static float CalculateVolatge (uint8_t* data)
{    
    uint16_t voltageInt = 0; 
    float voltageFloat = 0;
    
    voltageInt = (data[0] << 8)+ data[1];
    voltageFloat = voltageInt * (2500.0 / 1023) * 2;
    
    return voltageFloat;    
}


/**
  * @brief : 计算温度值
  * @param : data 温度原始值
  * @return: 计算出的温度值
  * @updata: [2019-10-17][Lei][creat]
             [2019-10-17][Gang][update][补充函数内容，修改参数]
  */
static float CalculateTemperature(uint8_t* data)
{
    uint16_t temperatureInt = 0;
    float temperatureFloat = 0;
    float temperature = 0;
    
    temperatureInt = (data[0] << 8) + data[1];
    temperatureFloat = temperatureInt * (2500.0 / 1023);
    temperature = (5.506 - sqrt(pow(-5.506, 2) + 4 * 0.00176 * (870.6 - temperatureFloat))) / (2 * (-0.00176)) + 30;
    
    return temperature;
}


/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      DataFrame.c
  * @brief:     传输的数据帧等相关应用
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-06-21
  * @update:    [2019-06-21][Lei][creat]
  */

/********************************include**************************************/
#include "DataFrame.h"



/********************************static**************************************/
static uint8_t GetDeviceNumber(struct NodeData* nodeData);
static void SaveTemperature(struct NodeData nodeData);
static void SaveVoltage(struct NodeData nodeData);
static uint8_t CheckSum(uint8_t* data, uint8_t lenth);
static uint32_t CalculateDeviceId(uint8_t* data, uint8_t lenth);
static float CalculateVolatge(uint8_t* data, uint8_t lenth);
static float CalculateTemperature(uint8_t* data, uint8_t lenth);




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
	if(0x5A == sourceData[0])				//判断数据帧头
	{
		if(0 == CheckSum(sourceData, 12))		//判断累加校验和
		{
			nodeData->deviceId = CalculateDeviceId(&sourceData[2], 4);
			nodeData->temperatureValue = CalculateTemperature(&sourceData[6], 2);
			nodeData->voltageValue = CalculateVolatge(&sourceData[8], 2);
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
  */
static uint8_t GetDeviceNumber(struct NodeData* nodeData)
{

}


/**
  * @brief : 将温度值保存在数据表中的正确位置
  * @param : nodeData 保存节点数据的结构体
  * @return: void 
  * @updata: [2019-10-17][Lei][creat]
  */
static void SaveTemperature(struct NodeData nodeData)
{

}


/**
  * @brief : 将电压值保存在数据表中的正确位置
  * @param : nodeData 保存节点数据的结构体
  * @return: void 
  * @updata: [2019-10-17][Lei][creat]
  */
static void SaveVoltage(struct NodeData nodeData)
{

}


/**
  * @brief : 累加校验和计算
  * @param : data 要计算的数据
  * @param : lenth 数据的总长度
  * @return: 0:校验正确; 1:校验不正确
  * @updata: [2019-10-17][Lei][creat]
  */
static uint8_t CheckSum(uint8_t* data, uint8_t lenth)
{

}


/**
  * @brief : 计算设备唯一ID号
  * @param : data ID号原始值
  * @param : lenth 原始数据的长度
  * @return: 得出的ID号
  * @updata: [2019-10-17][Lei][creat]
  */
static uint32_t CalculateDeviceId(uint8_t* data, uint8_t lenth)
{

}


/**
  * @brief : 计算电压值
  * @param : data 电压原始值
  * @param : lenth 电压数据的长度
  * @return: 计算出的电压值
  * @updata: [2019-10-17][Lei][creat]
  */
static float CalculateVolatge(uint8_t* data, uint8_t lenth)
{

}


/**
  * @brief : 计算温度值
  * @param : data 温度原始值
  * @param : lenth 温度数据的长度
  * @return: 计算出的温度值
  * @updata: [2019-10-17][Lei][creat]
  */
static float CalculateTemperature(uint8_t* data, uint8_t lenth)
{

}





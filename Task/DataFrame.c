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




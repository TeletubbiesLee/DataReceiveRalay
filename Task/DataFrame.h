/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      DataFrame.c
  * @brief:     传输的数据帧等相关应用
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-06-21
  * @update:    [2019-06-21][Lei][creat]
  */

#ifndef __DATA_FRAME_H_
#define __DATA_FRAME_H_

#include <stdint.h>
#include <stdbool.h>

#define CONFIG_FLAG_ADDRESS				0x007		//配置标志位地址
#define TEMPERATURE_FIRST_ADDRESS		0x008		//温度存储首地址
#define VOLTAGE_FIRST_ADDRESS			0x108		//电压存储首地址
#define LAUNCH_NUMBER_FIRAT_ADDRESS		0x208		//发射次数存储首地址
#define NODE_STATUS_FIRST_ADDRESS		0x408		//发射器节点状态存储首地址
#define NODE_DEVICE_ID_FIRST_ADDRESS	0x708		//发射器节点设备ID号存储首地址
#define MEASUREMENT_TYPE_FIRAT_ADDRESS	0x908		//发射器测量类型存储首地址



/* 保存传感器节点数据，以及对数据操作的结构体 */
typedef struct NodeData{
    bool isDataValid;           //数据有效标志位
    uint32_t deviceId;          //设备的唯一ID号
    float temperatureValue;     //温度值
    float voltageValue;         //电压值
    uint8_t deviceNumber;       //设备在数据表中的编号
	int8_t RSSI_Value;
	uint8_t LQI_Value;
    uint8_t (*getDeviceNumber)(struct NodeData* nodeData);       //根据设备ID号获取设备编号
    void (*saveTemperature)(struct NodeData nodeData);          //保存温度值
    void (*saveVoltage)(struct NodeData nodeData);                //保存电压值
	void (*saveSignalStrength)(struct NodeData nodeData);		//保存信号强度
	void (*saveLaunchNumber)(struct NodeData nodeData);			//保存发射次数

}NodeDataStruct;

void NodeDataStructInit(NodeDataStruct* nodeData);          //传感器节点数据结构体初始化函数
uint8_t DataFrameAnalysis(uint8_t* sourceData, NodeDataStruct* nodeData);		//无线接收数据解析

#endif

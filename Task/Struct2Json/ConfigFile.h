/**
 * @file ConfigFile.h
 * @brief 程序部分参数配置文件
 * @copyright Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * @company  SOJO
 * @date 2018.6.15
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 

#ifndef __CONFIG_FILE_H
#define	__CONFIG_FILE_H

#include <stdint.h>


/******************说明******************/
/**
 * @note 配置文件
 */
/******************END******************/

/**********************************BEGIN**********************************/
/****************************宏定义****************************/
#define DIS_ENABLE  0               //禁止
#define ENABLE      !DIS_ENABLE     //使能

#define PARAM_ERROR  -1   //参数错误

#define CHECK_PARAM(parameter) {parameter > 1 ? PARAM_ERROR : 1}    //检测参数

/****************************枚举定义****************************/
enum ConfigFileEnum
{
    ConfigFileEnum_ModbusBandrate = 0,		//(00)Modbus波特率
    ConfigFileEnum_ModbusSlaveAddress,		//(01)Modbus从机地址
	ConfigFileEnum_NodeOvertimeTime,		//(02)传感器节点超时时间

    ConfigFileEnum_Over = 0xFF
};

/****************************结构体定义****************************/

typedef struct tagConfiguiation
{
    char *name;         //名称
	int  parameter;     //参数
    int  number;        //序号，作为识别名称作用

}ConfigFile;

/****************************全局变量定义****************************/

extern ConfigFile g_ConfigFile[];

/****************************函数定义****************************/
extern uint16_t GetConfigFileLength(void);


/**********************************END**********************************/


#endif



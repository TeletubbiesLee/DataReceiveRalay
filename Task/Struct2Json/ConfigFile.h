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

#include "SystemConfig.h" 



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
    ConfigFileEnum_USART1 = 0,  //(00)串口1，配置参数
    ConfigFileEnum_USART2,      //(01)串口2，配置参数
    ConfigFileEnum_USART6,      //(02)串口6，配置参数

    ConfigFileEnum_Sample,      //(03)加速度采用频率，配置参数
    ConfigFileEnum_GPS,         //(04)GPS开关，配置参数
    ConfigFileEnum_GPS_Sample,  //(05)GPS参数获取频率
    ConfigFileEnum_PRESSURE,    //(06)获取压力
    ConfigFileEnum_TEMPER,      //(07)获取温度

    ConfigFileEnum_MAC_ADDR,    //(08)本机地址，采用十进制进行编码
    ConfigFileEnum_ASDU_ADDR,   //(09)ASDU公用地址
    ConfigFileEnum_MQTT,        //(10)MQTT，配置参数
    ConfigFileEnum_Mode,        //(11)101传输模式
    ConfigFileEnum_SleepMode,   //(12)睡眠模式
    ConfigFileEnum_SleepMode_F, //(13)进入睡眠模式的频率
    ConfigFileEnum_ScanKey,     //(14)按键扫描频率
	
	ConfigFileEnum_HorizontalAcceleration,	//(15)水平加速度
	ConfigFileEnum_VerticalAcceleration,	//(16)垂直加速度
	ConfigFileEnum_HorizontalAngle,			//(17)水平倾角
	ConfigFileEnum_VerticalAngle,			//(18)垂直倾角

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



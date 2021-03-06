/**
 * @file ConfigFile.c
 * @brief 程序部分参数配置文件
 * @copyright Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * @company  SOJO
 * @date 2018.6.15
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 

#include "ConfigFile.h"


/*************************************************静态变量*************************************************/
ConfigFile g_ConfigFile[] = \
{
    { 
        .name = "Modbus_Baudrate",			//Modbus波特率
        .parameter = 115200,    			//kbps
        .number = ConfigFileEnum_ModbusBandrate, },
    { 
        .name = "Modbus_SlaveAddress",			//Modbus从机地址
        .parameter = 1,
        .number = ConfigFileEnum_ModbusSlaveAddress, },
	{ 
        .name = "NodeOvertimeTime",			//节点超时时间
        .parameter = 1,
        .number = ConfigFileEnum_NodeOvertimeTime, },
	
};
/*************************************************END*************************************************/

/*************************************************公有函数实现*************************************************/
/**
 * @fn GetConfigFileLength
 * @brief 获取配置文件结构体数组长度
 * @return 返回数组长度
 * 
 */
inline uint16_t GetConfigFileLength(void)
{
    return (sizeof(g_ConfigFile) / sizeof(g_ConfigFile[0]));    //配置文件结构体数组长度;
}


/*************************************************END*************************************************/










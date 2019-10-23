/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      CommunicationConfig.c
  * @brief:     上位机对数据接收中继的通信配置相关程序文件
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-10-17
  * @update:    [2019-10-17][Lei][creat]
  */

/********************************include**************************************/
#include "CommunicationConfig.h"
#include "user_mb_app.h"
#include "ConfigFile.h"

/*************************************global********************************************/
uint32_t g_ModbusBandrate = 115200;		//Modbus的波特率
uint8_t  g_ModbusSlaveAddress = 0x01;	//Modbus的从机地址
uint8_t  g_ModbusUartNumber = 1;		//Modbus使用的串口号


/*************************************extern********************************************/
extern USHORT usSRegHoldBuf[S_REG_HOLDING_NREGS];	//保持寄存器缓冲区


/********************************function**************************************/
/**
  * @brief : 设置Modbus通信参数
  * @param : void
  * @return: void 
  * @updata: [2019-10-23][Lei][creat]
             [2019-10-23][Gang][update][补充函数内容]
  */
void SetModbusParameter(void)
{
    g_ModbusBandrate = g_ConfigFile[0].parameter;
    g_ModbusSlaveAddress = g_ConfigFile[1].parameter;
    g_ModbusUartNumber = g_ConfigFile[2].parameter;	
}

/**
  * @brief : 获取Modbus串口波特率
  * @param : void
  * @return: void 
  * @updata: [2019-10-17][Lei][creat]
  */
void GetModbusBandRate(void)
{
    switch((usSRegHoldBuf[0] & 0x0300) >> 8)
    {
        case 0:
            g_ModbusBandrate = 2400;
            break;
        case 1:
            g_ModbusBandrate = 4800;
            break;
        case 2:
            g_ModbusBandrate = 9600;
            break;
        case 3:
            g_ModbusBandrate = 115200;
            break;
    }
    
}


/**
  * @brief : 获取Modbus从机地址
  * @param : void
  * @return: void 
  * @updata: [2019-10-17][Lei][creat]
  */
void GetModbusSlaveAddress(void)
{
    g_ModbusSlaveAddress = usSRegHoldBuf[0] & 0xFF;
}



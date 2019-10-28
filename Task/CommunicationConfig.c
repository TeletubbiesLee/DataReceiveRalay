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



/*************************************extern********************************************/
extern USHORT usSRegHoldBuf[S_REG_HOLDING_NREGS];	//保持寄存器缓冲区


/********************************function**************************************/

/**
  * @brief : 设置Modbus通信参数
  * @param : void
  * @return: void 
  * @updata: [2019-10-23][Lei][creat]
             [2019-10-23][Gang][update][补充函数内容]
			 [2019-10-28][Lei][函数对参数之间操作，不再分成单个函数]
  */
void HostSetModbusParameter(void)
{
	/* 获取波特率 */
    switch((usSRegHoldBuf[0] & 0x0300) >> 8)
    {
        case 0:
            g_ConfigFile[0].parameter = 2400;
            break;
        case 1:
            g_ConfigFile[0].parameter = 4800;
            break;
        case 2:
            g_ConfigFile[0].parameter = 9600;
            break;
        case 3:
            g_ConfigFile[0].parameter = 115200;
            break;
    }
	
    g_ConfigFile[1].parameter = usSRegHoldBuf[0] & 0x00FF;		//获取从机地址
	
    usSRegHoldBuf[0] &= ~(1 << 12);			//还原配置更新标志位
}





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

#include "ff.h"
#include "DataFrame.h"
#include <stdio.h>
#include "string.h"


/*************************************extern********************************************/
extern USHORT usSRegHoldBuf[S_REG_HOLDING_NREGS];	//保持寄存器缓冲区
/*************************************static********************************************/
static FIL FilePositon;
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

/**
  * @brief : 将上位机唯一编码保存到csv文件中
  * @param : fileName 
  * @return: 返回值 
  * @updata: [2019-10-31][Gang][creat]
  */
int SaveIDCsvFile(void)
{
    char str[1024] = {0};     //存放需要保存到文件中的字符
    uint8_t res = 0;
    uint32_t id = 0;		//保存Modbus中读出的id值
    unsigned int writeNum = 0;
    res = f_open(&FilePositon, "/SaveID.csv", FA_CREATE_ALWAYS|FA_WRITE|FA_READ); //在根目录下创建一个可读写文件
    for (uint16_t i = 0; i <= 255; i++)
    {        
        id = (usSRegHoldBuf[NODE_DEVICE_ID_FIRST_ADDRESS + 2 * i] \
		+ (usSRegHoldBuf[NODE_DEVICE_ID_FIRST_ADDRESS + 1 + 2 * i] << 16));
        sprintf(str,"%u,%u,\n", i, id);
        res = f_write(&FilePositon, str, strlen(str), &writeNum); 
        memset(str, 0, strlen(str));       
    }
    f_close(&FilePositon);
    return res;
 }

 
 /**
  * @brief : 将csv文件中的地址保存到modbus寄存器中
  * @param : fileName 
  * @return: 返回值 
  * @updata: [2019-11-01][Gang][creat]
  */
 
int ReadCsvFile(void)
{
    //TERMINAL_PRODUCT_SERIAL_NUMBER
    uint8_t* string = NULL;
    uint8_t* record = NULL;
    FRESULT res = FR_OK;
    uint16_t fileSize = 0;
    uint32_t temp = 0;     
    uint8_t  num = 0;
    int j = 0;
    //获取json文件
	res = f_open(&FilePositon, "/SaveID.csv", FA_READ);
    fileSize = f_size(&FilePositon);   //获取文件大小
    string  = rt_malloc(fileSize);   //申请内存
    while ((string = f_gets(string ,fileSize, &FilePositon)) != NULL)
    {        
        record = strtok(string,",");
        num =  Str2Int(record);
        while (record != NULL)
        {			                        
			if (j == 1)                
			{
                break;
            }
			record = strtok(NULL, ",");            
            temp =  Str2Int(record);
            usSRegHoldBuf[NODE_DEVICE_ID_FIRST_ADDRESS + 2 * num] = temp&0x0000FFFF;           
		    usSRegHoldBuf[NODE_DEVICE_ID_FIRST_ADDRESS + 1 + 2 * num] = (temp&0xFFFF0000) >> 16;            
			j++;		   
        }
        j = 0;           
    } 
     f_close (&FilePositon);
     return 0;   
 }
 /**
  * @brief : 将字符转为32位无符号数
  * @param : uint8_t *str  字符串输入
  * @return: uint32_t   32位无符号数作为返回值
  * @updata: [2019-11-01][Gang][creat]
  */
uint32_t Str2Int(uint8_t *str)
{
	uint32_t m = 0;
	while (*str != '\0')
	{
		if (*str< '0' || *str > '9')
		return 0;
		m *= 10;
		m += *str - '0';// '0'的ASCII码
		str++;
	}
	return m;
}



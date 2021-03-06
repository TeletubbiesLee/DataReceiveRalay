/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      CommunicationConfig.c
  * @brief:     上位机对数据接收中继的通信配置相关程序文件
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:       2019-10-17
  * @update:    [2019-10-17][Lei][creat]
                [2019-11-01][Gang][update]
  */

/********************************include**************************************/
#include "CommunicationConfig.h"
#include "user_mb_app.h"
#include "ConfigFile.h"

#include "ff.h"
#include "DataFrame.h"
#include <stdio.h>
#include "string.h"

//测试打印需要
#include "TaskConfig.h"
/*************************************extern********************************************/
extern USHORT usSRegHoldBuf[S_REG_HOLDING_NREGS];    //保持寄存器缓冲区


/*************************************static********************************************/
static FIL FileStructure;               //DeviceId.csv该文件的文件结构体


/********************************function**************************************/
/**
  * @brief : 设置Modbus通信参数
  * @param : void
  * @return: void 
  * @updata: [2019-10-23][Lei][creat]
  *          [2019-10-23][Gang][update][补充函数内容]
  *          [2019-10-28][Lei][函数对参数之间操作，不再分成单个函数]
  */
void HostSetModbusParameter(void)
{
    /* 获取波特率 */
    switch ((usSRegHoldBuf[0] & 0x0300) >> 8)
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
        default:
            break;
    }
    
    g_ConfigFile[1].parameter = usSRegHoldBuf[0] & 0x00FF;        //获取从机地址
    
}


/**
  * @brief : 根据json文件或者默认设置Modbus保持寄存器
  * @param : void
  * @return: void 
  * @updata: [2019-11-04][Gang][creat]
  *          [2019-11-05][Lei][修改位操作方式，添加配置发射器超时时间]
  */
void ConfigModbusHoldRegister(void)
{   
    usSRegHoldBuf[0] = g_ConfigFile[1].parameter;		//配置Modbus通讯地址
	
	/* 配置Modbus波特率 */
    if (2400 == g_ConfigFile[0].parameter)
    {
        usSRegHoldBuf[0] |= (0 << 9 | 0 << 8);
    }
    else if (4800 == g_ConfigFile[0].parameter)
    {
        usSRegHoldBuf[0] |= (0 << 9 | 1 << 8);
    }
    else if (9600 == g_ConfigFile[0].parameter)
    {
        usSRegHoldBuf[0] |= (1 << 9 | 0 << 8);
    }
    else if (115200 == g_ConfigFile[0].parameter)
    {
        usSRegHoldBuf[0] |= (1 << 9 | 1 << 8);
    }
    
    usSRegHoldBuf[0] |= (1 << 11 | 1 << 10);            //配置串口格式
    
    usSRegHoldBuf[1] = g_ConfigFile[2].parameter;       //配置发射器超时时间
}


/**
  * @brief : 将上位机唯一编码保存到csv文件中
  * @param : fileName 
  * @return: 返回值 
  * @updata: [2019-10-31][Gang][creat]
  */
uint8_t SaveDeviceIdFile(void)
{
    uint8_t str[1024] = {0};       //存放需要保存到文件中的字符
    uint8_t res = 0;
    uint32_t id = 0;               //保存Modbus中读出的id值
    res = f_open(&FileStructure, "/DeviceId.csv", FA_CREATE_ALWAYS|FA_WRITE|FA_READ); //在根目录下创建一个可读写DeviceId.csv文件
    /*将编号0到255共256个发射器编号保存到DeviceId.csv*/
    for (uint16_t i = 0; i <= 255; i++)
    {
        /* 将发射器编号由整形转成字符串并存放在DeviceId.csv文件中*/
        IntToStr(str,(uint32_t)i);
        f_puts((TCHAR *)str,&FileStructure);
        memset((char *)str, 0, strlen((char *)str)); 
        f_putc(',',&FileStructure);   
        /* 将发射器唯一编号由整形转成字符串并存放在DeviceId.csv文件中*/
        id = (usSRegHoldBuf[NODE_DEVICE_ID_FIRST_ADDRESS + 2 * i] \
        + (usSRegHoldBuf[NODE_DEVICE_ID_FIRST_ADDRESS + 1 + 2 * i] << 16));        
        IntToStr(str,id);
        f_puts((char *)str,&FileStructure);
        memset(str, 0, strlen((char *)str)); 
        f_putc(',',&FileStructure);
        f_putc('\n',&FileStructure);            
    }
    res = f_close(&FileStructure);
    return res;
 }
 
 /**
  * @brief : 将csv文件中的地址保存到modbus寄存器中
  * @param : fileName 
  * @return: 返回值 
  * @updata: [2019-11-01][Gang][creat]
  */
 
uint8_t ReadDeviceIdFile(void)
{
    //TERMINAL_PRODUCT_SERIAL_NUMBER
    uint8_t* string = NULL;
    uint8_t* record = NULL;
    FRESULT res = FR_OK;
    uint16_t fileSize = 0;
    uint32_t temp = 0;      //用于存放发射器唯一编号
    uint8_t  num = 0;       //用于存放发射器编号
    int j = 0;
    //获取json文件
    res = f_open(&FileStructure, "/DeviceId.csv", FA_READ);
    fileSize = f_size(&FileStructure);   //获取文件大小
    string  = rt_malloc(fileSize);       //申请内存
    while ((string = f_gets(string, fileSize, &FileStructure)) != NULL)
    {        
        /*strtok函数分解字符串为一组字符串*/
        record = strtok(string, ",");
        num =  StrToInt(record);
        while (NULL != record)
        {                        
            if (1 == j)                
            {
                break;
            }
            /*strtok函数分解字符串为一组字符串在第一次调用时，strtok()必需给予参数s字符串，往后的调用则将参数s设置成NULL。*/
            record = strtok(NULL, ",");
            temp = StrToInt(record);
            usSRegHoldBuf[NODE_DEVICE_ID_FIRST_ADDRESS + 2 * num] = temp&0x0000FFFF;           
            usSRegHoldBuf[NODE_DEVICE_ID_FIRST_ADDRESS + 1 + 2 * num] = (temp&0xFFFF0000) >> 16;
            j++;  
        }
        j = 0;           
    } 
    f_close(&FileStructure);
    return res;   
 }
 /**
  * @brief : 将字符转为32位无符号数
  * @param : uint8_t *str  字符串输入
  * @return: uint32_t   32位无符号数作为返回值
  * @updata: [2019-11-01][Gang][creat]
  */
uint32_t StrToInt(uint8_t *str)
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

/**
 * @brief : 将整数转换为字符串
 * @param : intnum  要转换的整数
 * @param : p_str  字符串输出指针
 * @return: 无 
 * @updata: [2019-11-01][Gang][creat]
*/
void IntToStr(uint8_t *p_str, uint32_t intnum)
{
    uint32_t i;
    uint32_t divider = 1000000000;
    uint32_t pos = 0; 
    uint32_t status = 0;

    for (i = 0; i < 10; i++)
    {
        p_str[pos++] = (intnum / divider) + '0';   //'0'的ASCII码为48

        intnum = intnum % divider;
        divider /= 10;
        if (('0' == p_str[pos-1]) & (0 == status))
        {
             pos = 0;
        }
        else
        {
             status++;
        }
    }
}

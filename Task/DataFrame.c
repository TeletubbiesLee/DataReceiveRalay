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
#include "user_mb_app.h"
#include "math.h"


extern USHORT usSRegHoldBuf[S_REG_HOLDING_NREGS];

extern uint32_t g_ModbusBandrate;

/********************************function*************************************/



/**
  * @brief : 根据0000H寄存器的位10和位11来转换温度的格式（目前只实现一种）
  * @param : void
  * @return: void 
  * @updata: [2019-10-12][Gang][creat]
  */
uint16_t  TemperatureDataStorage (double temperature, uint16_t configRegister)
{ 
    uint8_t formatFlag = 0;
    formatFlag = (configRegister & 0x0c00) >> 10;
    uint16_t  temperatureFormat = 0;
    
    switch(formatFlag)
    {
        case 0:
        {
            if(temperature > 0)
            temperatureFormat = (uint16_t) (temperature * 10);
            else
            temperatureFormat = 2000 +  (uint16_t) (temperature * 10);               
        
            break ;
        }
        case 1:
        {
            if(temperature > 0)
            {
              temperatureFormat = (uint16_t) (temperature * 10);
            }            
            else
            {
              temperatureFormat = 2000 +  (uint16_t) (temperature * 10);
            }
                          
            
            break ;
        }    
        case 2:
        {
            if(temperature>0)
            temperatureFormat = (uint16_t) (temperature*10);
            else
            temperatureFormat = 2000 +  (uint16_t) (temperature*10);               
            
            break ;
        } 
        case 3:
        {
            if(temperature>0)
            temperatureFormat = (uint16_t) (temperature*10);
            else
            temperatureFormat = 2000 +  (uint16_t) (temperature*10);               
            
            break ;
        }           
    }
    return temperatureFormat;

}

/**
  * @brief : 波特率配置信息
  * @param : dataBuffer 配置寄存器
  * @return: void 
  * @updata: [2019-06-21][Lei][creat]
  */
uint32_t BandrateConfig(uint16_t configRegister)
{
	/* 配置波特率 */
	switch(configRegister & 0x0300)
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
   return g_ModbusBandrate;

}
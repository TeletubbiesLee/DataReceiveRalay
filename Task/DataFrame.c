/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      DataFrame.c
  * @brief:     ���������֡�����Ӧ��
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
  * @brief : ����0000H�Ĵ�����λ10��λ11��ת���¶ȵĸ�ʽ��Ŀǰֻʵ��һ�֣�
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
  * @brief : ������������Ϣ
  * @param : dataBuffer ���üĴ���
  * @return: void 
  * @updata: [2019-06-21][Lei][creat]
  */
uint32_t BandrateConfig(uint16_t configRegister)
{
	/* ���ò����� */
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
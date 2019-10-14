/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      TaskDataReceive.c
  * @brief:     数据接收任务相关文件
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-10-11
  * @update:    [2019-10-11][Lei][creat]
  */

#include "TaskDataReceive.h"
#include <rtthread.h>
#include <stdio.h>
#include "CC1101.h"
#include "math.h"
#include "string.h"

/*************************************global********************************************/



/*************************************static********************************************/
/* CC1101无线数据接收的任务优先级，栈空间，任务结构体及入口函数 */
#define THREAD_TASK_DATA_RECEIVE_PRIO	11
static rt_uint8_t TaskDataReceiveStack[512];
struct rt_thread ThreadTaskDataReceive;
static void TaskDataReceiveThreadEntry(void* parameter);

/*************************************extern********************************************/
extern uint8_t g_RSSI_Value;
extern uint8_t g_LQI_Value;
     
 int8_t  CalculateRSSI(uint8_t RSSI_dec);
 double  CalculateTemperature(uint8_t temperatureTmpH,uint8_t temperatureTmpL);
 double  CalculateVolatge(uint8_t voltageTmpH,uint8_t voltageTmpL);
 uint16_t TX_Checksum(uint8_t *dataBuf, uint8_t len);

/*************************************function******************************************/
/**
  * @brief : CC1101无线数据接收
  * @param : void
  * @return: void 
  * @updata: [2019-10-12][Gang][creat]
  */
static void TaskDataReceiveThreadEntry(void* parameter)
{
    int8_t absolutePower = 0;	
	uint8_t rxBuffer[10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    uint8_t leng = 0;
    uint8_t num = 0;
    uint8_t LQI_RealValue = 0;
    double  temperature = 0;
    double  volatgeDouble = 0;
    uint16_t checksumValue = 0;
    uint8_t    checksumValueHigh = 0;
    uint8_t    checksumValueLow = 0;
    CC1101_PowerUp();
	CC1101_SettingsReg();
    while(1)
	{        
       rt_thread_mdelay(50);
       num++;
		if(CC1101_ReceivePacket(rxBuffer, &leng))
		{
		
			//HAL_UART_Transmit(&UART1_Handler,(uint8_t*)rxBuffer, leng ,100);
			rt_kprintf("学习板接收数据：0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X,\r\n", 	
            rxBuffer[0], rxBuffer[1], rxBuffer[2], rxBuffer[3], rxBuffer[4], rxBuffer[5], rxBuffer[6], rxBuffer[7] ,rxBuffer[8],rxBuffer[9]);
			rt_thread_mdelay(10);
            checksumValue = TX_Checksum (rxBuffer,8) ;
            checksumValueHigh = checksumValue>>8;
            checksumValueLow  = (checksumValue << 8) >> 8;
            if(rxBuffer[0] == 0x55 && rxBuffer[8]==checksumValueHigh&&rxBuffer[9] ==checksumValueLow)
            {
            //计算温度传感器温度值
            temperature =  CalculateTemperature(rxBuffer[4],rxBuffer[5]);
            rt_kprintf("温度传感器温度值为：%f °C\r\n", temperature );    
            //计算电源的电压值
            volatgeDouble =  CalculateVolatge(rxBuffer[6],rxBuffer[7]);
            rt_kprintf("测得电压数据为：%f mV\r\n", volatgeDouble ); 
                
            rt_kprintf("CC1101的RSSI为：0x%X 与 LQI 为 0x%X： \r\n", g_RSSI_Value, g_LQI_Value);
            absolutePower = CalculateRSSI(g_RSSI_Value);
            rt_kprintf("CC1101的绝对功率为：%d dBm \r\n", absolutePower);
            //LQI_RealValue = g_LQI_Value&0x7F;
            LQI_RealValue = g_LQI_Value&(~(1<<7));
            rt_kprintf("CC1101的LQI为：%d \r\n", LQI_RealValue);            
			memset(rxBuffer, 0, leng);			
			CC1101_Reset();
			CC1101_SettingsReg();
            num = 0;           
            }
            else
            {
                rt_kprintf("数据校验错误" ); 
            }
		}
        if(num == 20)
        {
            CC1101_Reset();
			CC1101_SettingsReg();
            num = 0;
        }   
        
        
    }
}
/**
  * @brief : 计算测温节点温度
  * @param : temperatureTmpH
  * @param : temperatureTmpL
  * @return: temperatureC 
  * @updata: [2019-10-12][Gang][creat]
  */
double  CalculateTemperature(uint8_t temperatureTmpH,uint8_t temperatureTmpL)
{
    uint16_t temperatureInt =0;
    double  temperatureDouble = 0;
    double  temperature = 0;
    temperatureInt = (temperatureTmpH<<8 )+ temperatureTmpL;
    temperatureDouble = temperatureInt * (2500.0/1023); 
   // rt_kprintf("温度传感器电压输入数据为：%f mV\r\n", temperatureDouble );            
    temperature = (5.506 - sqrt( pow(-5.506,2) + 4 * 0.00176 * (870.6 - temperatureDouble))) /( 2 * (-0.00176)) + 30;           
    return temperature;
}

/**
  * @brief : 计算测温节点电源电压
  * @param : voltageTmpH
  * @param : voltageTmpL
  * @return: voltageDouble 
  * @updata: [2019-10-12][Gang][creat]
  */
double  CalculateVolatge(uint8_t voltageTmpH,uint8_t voltageTmpL)
{
   uint16_t voltageInt = 0; 
   double  voltageDouble = 0;
   voltageInt = (voltageTmpH<<8 )+ voltageTmpL;
   voltageDouble = voltageInt * (2500.0/1023) * 2; //因为外部电路，所以需要乘以2 
  // rt_kprintf("测得电压数据为：%f mV\r\n", voltageDouble );
   return voltageDouble;
}

/**
  * @brief : 计算测温节点信号的绝对功率
  * @param : RSSI_dec
  * @return: temp 
  * @updata: [2019-10-12][Gang][creat]
  */

int8_t CalculateRSSI(uint8_t RSSI_dec)
{
    int8_t temp = 0;
    int8_t RSSI_offset = 75;
    if(RSSI_dec >= 128)
    {
        temp = (int8_t)((int8_t)(RSSI_dec-256)/2)-RSSI_offset;
    }        
    else 
    {
        temp = (RSSI_dec/2)- RSSI_offset;    
    }
    return temp;
}

/**
  * @brief : 创建CC1101无线数据接收任务
  * @param : void
  * @return: void 
  * @updata: [2019-10-12][Gang][creat]
  */
int TaskDataReceiveInit(void)
{
    //创建静态线程   数据接收
   rt_thread_init(&ThreadTaskDataReceive,                 //线程handle
				   "TaskDataReceive",                           //线程名
                   TaskDataReceiveThreadEntry,            //线程入口函数
				   RT_NULL,                                 //线程入口参数
				   TaskDataReceiveStack,            //线程栈地址
                   sizeof(TaskDataReceiveStack),    //线程栈大小
				   THREAD_TASK_DATA_RECEIVE_PRIO,             //线程优先级
                   5);                                      //线程时间片
				   
    rt_thread_startup(&ThreadTaskDataReceive);             //启动线程
				   
    return 0;    
             
}




uint16_t TX_Checksum(uint8_t *dataBuf, uint8_t len)
{
    uint8_t i;
    uint16_t ret = 0;
    
    for(i=0;i<len; i++)
    {
      ret +=*(dataBuf++);
    }
    return ret;
}



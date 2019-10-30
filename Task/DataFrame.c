/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      DataFrame.c
  * @brief:     传输的数据帧等相关应用
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-06-21
  * @update:    [2019-06-21][Lei][creat]
                [2019-10-18][Gang][update][对一些函数内容进行补充]
  */
/********************************include**************************************/
#include "DataFrame.h"
#include "math.h"
#include "user_mb_app.h"
#include "CC1101.h"




/********************************static**************************************/
static uint8_t GetDeviceNumber(struct NodeData* nodeData);
static void SaveTemperature(struct NodeData nodeData);
static void SaveVoltage(struct NodeData nodeData);
static uint8_t CheckSum(uint8_t* data, uint8_t lenth);
static uint32_t CalculateDeviceId(uint8_t* data);
static float CalculateVolatge(uint8_t* data);
static float CalculateTemperature(uint8_t* data);
static void CalculateSignalStrength(struct NodeData* nodeData);
static void SaveSignalStrength(struct NodeData nodeData);
static void SaveLaunchNumber(struct NodeData nodeData);


/********************************extern*************************************/
extern USHORT usSRegHoldBuf[S_REG_HOLDING_NREGS];    //保持寄存器缓冲区


/********************************function*************************************/
/**
  * @brief : 保存数据节点结构体的初始化函数
  * @param : nodeData 保存节点数据的结构体的指针
  * @return: void 
  * @updata: [2019-10-17][Lei][creat]
  */
void NodeDataStructInit(NodeDataStruct* nodeData)
{
    nodeData->isDataValid = false;
    nodeData->getDeviceNumber = GetDeviceNumber;
    nodeData->saveTemperature = SaveTemperature;
    nodeData->saveVoltage = SaveVoltage;
	nodeData->saveSignalStrength = SaveSignalStrength;
	nodeData->saveLaunchNumber = SaveLaunchNumber;
}


/**
  * @brief : CC1101接收数据帧解析
  * @param : sourceData CC1101接收到的数据
  * @param : nodeData 保存节点数据的结构体
  * @return: 0:解析正确; 1:校验码错误; 2:其他 
  * @updata: [2019-10-17][Lei][creat]
  */
uint8_t DataFrameAnalysis(uint8_t* sourceData, NodeDataStruct* nodeData)
{
    uint8_t ret = 0;
    if (0x5A == sourceData[0])                   //判断数据帧头
    {
        if (0 == CheckSum(sourceData, 12))       //判断累加校验和
        {
			CalculateSignalStrength(nodeData);			//获取信号强度
            nodeData->deviceId = CalculateDeviceId(&sourceData[2]);				//获取设备ID号
            nodeData->temperatureValue = CalculateTemperature(&sourceData[6]);	//获取温度值
            nodeData->voltageValue = CalculateVolatge(&sourceData[8]);			//获取电压值
            nodeData->isDataValid = true;				//使能数据有效标志位
        }
        else
        {
            ret = 1;        //校验码错误
        }
    }
    else
    {
        ret = 2;            //头校验错误
    }

    return ret;
}


/**
  * @brief : 由设备ID号获取在数据表中对应的编号，将编号记录在结构体中
  * @param : nodeData 保存节点数据的结构体的指针
  * @return: 0:存在ID号; 1:ID号不存在
  * @updata: [2019-10-17][Lei][creat]
             [2019-10-18][Gang][update][补充函数内容]
             [2019-10-24][Gang][update][修改函数bug]
  */
static uint8_t GetDeviceNumber(struct NodeData* nodeData)
{
    uint8_t ret = 1;
    uint16_t id = 0;		//保存Modbus中读出的id值
    
    /*循环判断设备ID号是否属于Modbus保持寄存器地址表中的地址*/
    for (uint16_t i = 0; i <= 255; i++)
    {
		id = (usSRegHoldBuf[NODE_DEVICE_ID_FIRST_ADDRESS + 2*i] \
		+ (usSRegHoldBuf[NODE_DEVICE_ID_FIRST_ADDRESS + 1 + 2 * i] << 16));
		
        /*Modbus保持寄存器地址表中的地址是从0x708开始4个字节  0x708低16位  0x709高16位  共255个*/
        if (nodeData->deviceId == id)
        {
            nodeData->deviceNumber = i;
            ret = 0;                //Modbus保持寄存器地址表存在此ID
            break;
        }
        else
        {
            ret = 1;                //Modbus保持寄存器地址表不存在此ID
        }        
    }
    
    return ret;
}


/**
  * @brief : 将温度值保存在数据表中的正确位置
  * @param : nodeData 保存节点数据的结构体
  * @return: void 
  * @updata: [2019-10-17][Lei][creat]
             [2019-10-18][Gang][update][补充函数内容]
  */
static void SaveTemperature(struct NodeData nodeData)
{    
    uint16_t temperatureFormat = 0;
    int16_t temp = 0;

    if (true == nodeData.isDataValid)
    {
        /*当节点温度值大于0，则保留一位小数，并将该值乘以10。温度整数部分为(0，1999]表示温度0.0℃~199.9℃*/
        if (nodeData.temperatureValue > 0)
        {
            temperatureFormat = (uint16_t)(nodeData.temperatureValue * 10);
        }
        /*当节点温度值小于0，则将该值取反，并加2000代表值为负，保留一位小数，并将该值乘以10。[2000,2500]表示温度-0.0℃~-50.0℃*/
        else
        {
            temp = -nodeData.temperatureValue;
            temperatureFormat = 2000 + (uint16_t)(temp * 10);
        }
        /*将转换好格式的数据存储在保持寄存器0x008开始相对应的寄存器中*/
        usSRegHoldBuf[TEMPERATURE_FIRST_ADDRESS + nodeData.deviceNumber] = temperatureFormat;
     }
}


/**
  * @brief : 将电压值保存在数据表中的正确位置
  * @param : nodeData 保存节点数据的结构体
  * @return: void 
  * @updata: [2019-10-17][Lei][creat]
             [2019-10-18][Gang][update][补充函数内容]
  */
static void SaveVoltage(struct NodeData nodeData)
{
    int16_t voltage = 0;
    
    if (true == nodeData.isDataValid)
    {
        voltage = (int16_t)(nodeData.voltageValue);
        /*将转换好格式的数据存储在保持寄存器0x108开始相对应寄存器中*/
        usSRegHoldBuf[VOLTAGE_FIRST_ADDRESS + nodeData.deviceNumber] = voltage;  
    }

}


/**
  * @brief : 累加校验和计算
  * @param : data 要计算的数据
  * @param : lenth 数据的总长度
  * @return: 0:校验正确; 1:校验不正确
  * @updata: [2019-10-17][Lei][creat]
             [2019-10-18][Gang][update][补充函数内容]
  */
static uint8_t CheckSum(uint8_t* data, uint8_t lenth)
{   
    uint8_t ret = 1;
    uint16_t result = 0;
    
    for (uint8_t i = 0; i < (lenth - 2); i++)
    {
      result += data[i];
    }
    /*判断计算得到的校验和与数据帧中的校验和是否相等*/
    if (data[lenth - 2] == (result & 0xFF00) >> 8 && data[lenth - 1] == (result & 0x00FF))
    {
        ret = 0;
    }
    else
    {
        ret = 1;
    }

    return ret;
}


/**
  * @brief : 计算设备唯一ID号
  * @param : data ID号原始值
  * @return: 得出的ID号
  * @updata: [2019-10-17][Lei][creat]
             [2019-10-17][Gang][update][补充函数内容,修改参数]
  */
static uint32_t CalculateDeviceId(uint8_t* data)
{
    uint32_t deviceId = 0;
    /*由接收到的数据帧计算设备的唯一ID号*/
    deviceId = (data[0] << 24) + (data[1] << 16) + (data[2] << 8) + data[3];
    
    return deviceId;
}


/**
  * @brief : 计算电压值
  * @param : data 电压原始值
  * @return: 计算出的电压值
  * @updata: [2019-10-17][Lei][creat]
             [2019-10-18][Gang][update][补充函数内容,修改参数]
  */
static float CalculateVolatge (uint8_t* data)
{    
    uint16_t voltageInt = 0; 
    float voltageFloat = 0;
    /*将接收到的电压数据转成16位*/
    voltageInt = (data[0] << 8) + data[1];
    /*根据ADC公式转换电压值*/
    voltageFloat = voltageInt * (2500.0 / 1023) * 2;
    
    return voltageFloat;    
}


/**
  * @brief : 计算温度值
  * @param : data 温度原始值
  * @return: 计算出的温度值
  * @updata: [2019-10-17][Lei][creat]
             [2019-10-17][Gang][update][补充函数内容，修改参数]
  */
static float CalculateTemperature(uint8_t* data)
{
    uint16_t temperatureInt = 0;
    float temperatureFloat = 0;
    float temperature = 0;
    
    temperatureInt = (data[0] << 8) + data[1];
    temperatureFloat = temperatureInt * (2500.0 / 1023);
    /*根据温度传感器数据手册公式转换温度值*/
    temperature = (5.506 - sqrt(pow(-5.506, 2) + 4 * 0.00176 * (870.6 - temperatureFloat))) / (2 * (-0.00176)) + 30;
    
    return temperature;
}


/**
  * @brief : 计算RSSI和LQI的值，获取信号强度
  * @param : nodeData 结构体指针
  * @return: void
  * @updata: [2019-10-24][Lei][creat]
             [2019-10-24][Gang][update][补充函数内容]
  */
static void CalculateSignalStrength(struct NodeData* nodeData)
{
	uint8_t value[2] = {0};		//value[0] = RSSI; value[1] = LQI
	
	Read_RSSI_LQI_Register(value);		//获取RSSI和LQI的原始值
	
	/* 由RSSI和LQI寄存器值，获取它的实际值，并赋值给结构体 */
    int8_t RSSI_offset = 75;
    if(value[0] >= 128)
    {
        nodeData->RSSI_Value = (int8_t)((int8_t)(value[0] - 256) / 2) - RSSI_offset;
    }
    else 
    {
        nodeData->RSSI_Value = (value[0] / 2) - RSSI_offset;    
    }
    
    nodeData->LQI_Value = value[1] & 0x7F;
	
}


/**
  * @brief : 保存信号强度值
  * @param : nodeData 数据结构体
  * @return: void
  * @updata: [2019-10-24][Lei][creat]
             [2019-10-25][Gang][update][补充函数内容]
  */
static void SaveSignalStrength(struct NodeData nodeData)
{
    if (true == nodeData.isDataValid)
    {
        /*将转换好格式的数据存储在保持寄存器0x408开始相对应寄存器中*/
        usSRegHoldBuf[NODE_STATUS_FIRST_ADDRESS + 3 * nodeData.deviceNumber] = nodeData.RSSI_Value;  
    }
}


/**
  * @brief : 保存发射器发射次数
  * @param : nodeData 数据结构体
  * @return: void
  * @updata: [2019-10-24][Lei][creat]
  */
static void SaveLaunchNumber(struct NodeData nodeData)
{
    if (true == nodeData.isDataValid)
    {
        /*将转换好格式的数据存储在保持寄存器0x208开始相对应寄存器中*/
        usSRegHoldBuf[LAUNCH_NUMBER_FIRAT_ADDRESS + nodeData.deviceNumber]++;
    }
}


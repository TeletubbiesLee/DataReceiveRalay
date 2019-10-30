/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      DataFrame.c
  * @brief:     ���������֡�����Ӧ��
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-06-21
  * @update:    [2019-06-21][Lei][creat]
                [2019-10-18][Gang][update][��һЩ�������ݽ��в���]
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
extern USHORT usSRegHoldBuf[S_REG_HOLDING_NREGS];    //���ּĴ���������


/********************************function*************************************/
/**
  * @brief : �������ݽڵ�ṹ��ĳ�ʼ������
  * @param : nodeData ����ڵ����ݵĽṹ���ָ��
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
  * @brief : CC1101��������֡����
  * @param : sourceData CC1101���յ�������
  * @param : nodeData ����ڵ����ݵĽṹ��
  * @return: 0:������ȷ; 1:У�������; 2:���� 
  * @updata: [2019-10-17][Lei][creat]
  */
uint8_t DataFrameAnalysis(uint8_t* sourceData, NodeDataStruct* nodeData)
{
    uint8_t ret = 0;
    if (0x5A == sourceData[0])                   //�ж�����֡ͷ
    {
        if (0 == CheckSum(sourceData, 12))       //�ж��ۼ�У���
        {
			CalculateSignalStrength(nodeData);			//��ȡ�ź�ǿ��
            nodeData->deviceId = CalculateDeviceId(&sourceData[2]);				//��ȡ�豸ID��
            nodeData->temperatureValue = CalculateTemperature(&sourceData[6]);	//��ȡ�¶�ֵ
            nodeData->voltageValue = CalculateVolatge(&sourceData[8]);			//��ȡ��ѹֵ
            nodeData->isDataValid = true;				//ʹ��������Ч��־λ
        }
        else
        {
            ret = 1;        //У�������
        }
    }
    else
    {
        ret = 2;            //ͷУ�����
    }

    return ret;
}


/**
  * @brief : ���豸ID�Ż�ȡ�����ݱ��ж�Ӧ�ı�ţ�����ż�¼�ڽṹ����
  * @param : nodeData ����ڵ����ݵĽṹ���ָ��
  * @return: 0:����ID��; 1:ID�Ų�����
  * @updata: [2019-10-17][Lei][creat]
             [2019-10-18][Gang][update][���亯������]
             [2019-10-24][Gang][update][�޸ĺ���bug]
  */
static uint8_t GetDeviceNumber(struct NodeData* nodeData)
{
    uint8_t ret = 1;
    uint16_t id = 0;		//����Modbus�ж�����idֵ
    
    /*ѭ���ж��豸ID���Ƿ�����Modbus���ּĴ�����ַ���еĵ�ַ*/
    for (uint16_t i = 0; i <= 255; i++)
    {
		id = (usSRegHoldBuf[NODE_DEVICE_ID_FIRST_ADDRESS + 2*i] \
		+ (usSRegHoldBuf[NODE_DEVICE_ID_FIRST_ADDRESS + 1 + 2 * i] << 16));
		
        /*Modbus���ּĴ�����ַ���еĵ�ַ�Ǵ�0x708��ʼ4���ֽ�  0x708��16λ  0x709��16λ  ��255��*/
        if (nodeData->deviceId == id)
        {
            nodeData->deviceNumber = i;
            ret = 0;                //Modbus���ּĴ�����ַ����ڴ�ID
            break;
        }
        else
        {
            ret = 1;                //Modbus���ּĴ�����ַ�����ڴ�ID
        }        
    }
    
    return ret;
}


/**
  * @brief : ���¶�ֵ���������ݱ��е���ȷλ��
  * @param : nodeData ����ڵ����ݵĽṹ��
  * @return: void 
  * @updata: [2019-10-17][Lei][creat]
             [2019-10-18][Gang][update][���亯������]
  */
static void SaveTemperature(struct NodeData nodeData)
{    
    uint16_t temperatureFormat = 0;
    int16_t temp = 0;

    if (true == nodeData.isDataValid)
    {
        /*���ڵ��¶�ֵ����0������һλС����������ֵ����10���¶���������Ϊ(0��1999]��ʾ�¶�0.0��~199.9��*/
        if (nodeData.temperatureValue > 0)
        {
            temperatureFormat = (uint16_t)(nodeData.temperatureValue * 10);
        }
        /*���ڵ��¶�ֵС��0���򽫸�ֵȡ��������2000����ֵΪ��������һλС����������ֵ����10��[2000,2500]��ʾ�¶�-0.0��~-50.0��*/
        else
        {
            temp = -nodeData.temperatureValue;
            temperatureFormat = 2000 + (uint16_t)(temp * 10);
        }
        /*��ת���ø�ʽ�����ݴ洢�ڱ��ּĴ���0x008��ʼ���Ӧ�ļĴ�����*/
        usSRegHoldBuf[TEMPERATURE_FIRST_ADDRESS + nodeData.deviceNumber] = temperatureFormat;
     }
}


/**
  * @brief : ����ѹֵ���������ݱ��е���ȷλ��
  * @param : nodeData ����ڵ����ݵĽṹ��
  * @return: void 
  * @updata: [2019-10-17][Lei][creat]
             [2019-10-18][Gang][update][���亯������]
  */
static void SaveVoltage(struct NodeData nodeData)
{
    int16_t voltage = 0;
    
    if (true == nodeData.isDataValid)
    {
        voltage = (int16_t)(nodeData.voltageValue);
        /*��ת���ø�ʽ�����ݴ洢�ڱ��ּĴ���0x108��ʼ���Ӧ�Ĵ�����*/
        usSRegHoldBuf[VOLTAGE_FIRST_ADDRESS + nodeData.deviceNumber] = voltage;  
    }

}


/**
  * @brief : �ۼ�У��ͼ���
  * @param : data Ҫ���������
  * @param : lenth ���ݵ��ܳ���
  * @return: 0:У����ȷ; 1:У�鲻��ȷ
  * @updata: [2019-10-17][Lei][creat]
             [2019-10-18][Gang][update][���亯������]
  */
static uint8_t CheckSum(uint8_t* data, uint8_t lenth)
{   
    uint8_t ret = 1;
    uint16_t result = 0;
    
    for (uint8_t i = 0; i < (lenth - 2); i++)
    {
      result += data[i];
    }
    /*�жϼ���õ���У���������֡�е�У����Ƿ����*/
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
  * @brief : �����豸ΨһID��
  * @param : data ID��ԭʼֵ
  * @return: �ó���ID��
  * @updata: [2019-10-17][Lei][creat]
             [2019-10-17][Gang][update][���亯������,�޸Ĳ���]
  */
static uint32_t CalculateDeviceId(uint8_t* data)
{
    uint32_t deviceId = 0;
    /*�ɽ��յ�������֡�����豸��ΨһID��*/
    deviceId = (data[0] << 24) + (data[1] << 16) + (data[2] << 8) + data[3];
    
    return deviceId;
}


/**
  * @brief : �����ѹֵ
  * @param : data ��ѹԭʼֵ
  * @return: ������ĵ�ѹֵ
  * @updata: [2019-10-17][Lei][creat]
             [2019-10-18][Gang][update][���亯������,�޸Ĳ���]
  */
static float CalculateVolatge (uint8_t* data)
{    
    uint16_t voltageInt = 0; 
    float voltageFloat = 0;
    /*�����յ��ĵ�ѹ����ת��16λ*/
    voltageInt = (data[0] << 8) + data[1];
    /*����ADC��ʽת����ѹֵ*/
    voltageFloat = voltageInt * (2500.0 / 1023) * 2;
    
    return voltageFloat;    
}


/**
  * @brief : �����¶�ֵ
  * @param : data �¶�ԭʼֵ
  * @return: ��������¶�ֵ
  * @updata: [2019-10-17][Lei][creat]
             [2019-10-17][Gang][update][���亯�����ݣ��޸Ĳ���]
  */
static float CalculateTemperature(uint8_t* data)
{
    uint16_t temperatureInt = 0;
    float temperatureFloat = 0;
    float temperature = 0;
    
    temperatureInt = (data[0] << 8) + data[1];
    temperatureFloat = temperatureInt * (2500.0 / 1023);
    /*�����¶ȴ����������ֲṫʽת���¶�ֵ*/
    temperature = (5.506 - sqrt(pow(-5.506, 2) + 4 * 0.00176 * (870.6 - temperatureFloat))) / (2 * (-0.00176)) + 30;
    
    return temperature;
}


/**
  * @brief : ����RSSI��LQI��ֵ����ȡ�ź�ǿ��
  * @param : nodeData �ṹ��ָ��
  * @return: void
  * @updata: [2019-10-24][Lei][creat]
             [2019-10-24][Gang][update][���亯������]
  */
static void CalculateSignalStrength(struct NodeData* nodeData)
{
	uint8_t value[2] = {0};		//value[0] = RSSI; value[1] = LQI
	
	Read_RSSI_LQI_Register(value);		//��ȡRSSI��LQI��ԭʼֵ
	
	/* ��RSSI��LQI�Ĵ���ֵ����ȡ����ʵ��ֵ������ֵ���ṹ�� */
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
  * @brief : �����ź�ǿ��ֵ
  * @param : nodeData ���ݽṹ��
  * @return: void
  * @updata: [2019-10-24][Lei][creat]
             [2019-10-25][Gang][update][���亯������]
  */
static void SaveSignalStrength(struct NodeData nodeData)
{
    if (true == nodeData.isDataValid)
    {
        /*��ת���ø�ʽ�����ݴ洢�ڱ��ּĴ���0x408��ʼ���Ӧ�Ĵ�����*/
        usSRegHoldBuf[NODE_STATUS_FIRST_ADDRESS + 3 * nodeData.deviceNumber] = nodeData.RSSI_Value;  
    }
}


/**
  * @brief : ���淢�����������
  * @param : nodeData ���ݽṹ��
  * @return: void
  * @updata: [2019-10-24][Lei][creat]
  */
static void SaveLaunchNumber(struct NodeData nodeData)
{
    if (true == nodeData.isDataValid)
    {
        /*��ת���ø�ʽ�����ݴ洢�ڱ��ּĴ���0x208��ʼ���Ӧ�Ĵ�����*/
        usSRegHoldBuf[LAUNCH_NUMBER_FIRAT_ADDRESS + nodeData.deviceNumber]++;
    }
}


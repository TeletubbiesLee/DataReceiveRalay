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



/********************************static**************************************/
static uint8_t GetDeviceNumber(struct NodeData* nodeData);
static void SaveTemperature(struct NodeData nodeData);
static void SaveVoltage(struct NodeData nodeData);
static uint8_t CheckSum(uint8_t* data, uint8_t lenth);
static uint32_t CalculateDeviceId(uint8_t* data, uint8_t lenth);
static float CalculateVolatge(uint8_t* data, uint8_t lenth);
static float CalculateTemperature(uint8_t* data, uint8_t lenth);




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
	if(0x5A == sourceData[0])				//�ж�����֡ͷ
	{
		if(0 == CheckSum(sourceData, 12))		//�ж��ۼ�У���
		{
			nodeData->deviceId = CalculateDeviceId(&sourceData[2], 4);
			nodeData->temperatureValue = CalculateTemperature(&sourceData[6], 2);
			nodeData->voltageValue = CalculateVolatge(&sourceData[8], 2);
		}
		else
		{
			ret = 1;
		}
		
	}
	else
	{
		ret = 2;
	}
	
	return ret;
}


/**
  * @brief : ���豸ID�Ż�ȡ�����ݱ��ж�Ӧ�ı�ţ�����ż�¼�ڽṹ����
  * @param : nodeData ����ڵ����ݵĽṹ���ָ��
  * @return: 0:����ID��; 1:ID�Ų�����
  * @updata: [2019-10-17][Lei][creat]
  */
static uint8_t GetDeviceNumber(struct NodeData* nodeData)
{

}


/**
  * @brief : ���¶�ֵ���������ݱ��е���ȷλ��
  * @param : nodeData ����ڵ����ݵĽṹ��
  * @return: void 
  * @updata: [2019-10-17][Lei][creat]
  */
static void SaveTemperature(struct NodeData nodeData)
{

}


/**
  * @brief : ����ѹֵ���������ݱ��е���ȷλ��
  * @param : nodeData ����ڵ����ݵĽṹ��
  * @return: void 
  * @updata: [2019-10-17][Lei][creat]
  */
static void SaveVoltage(struct NodeData nodeData)
{

}


/**
  * @brief : �ۼ�У��ͼ���
  * @param : data Ҫ���������
  * @param : lenth ���ݵ��ܳ���
  * @return: 0:У����ȷ; 1:У�鲻��ȷ
  * @updata: [2019-10-17][Lei][creat]
  */
static uint8_t CheckSum(uint8_t* data, uint8_t lenth)
{

}


/**
  * @brief : �����豸ΨһID��
  * @param : data ID��ԭʼֵ
  * @param : lenth ԭʼ���ݵĳ���
  * @return: �ó���ID��
  * @updata: [2019-10-17][Lei][creat]
  */
static uint32_t CalculateDeviceId(uint8_t* data, uint8_t lenth)
{

}


/**
  * @brief : �����ѹֵ
  * @param : data ��ѹԭʼֵ
  * @param : lenth ��ѹ���ݵĳ���
  * @return: ������ĵ�ѹֵ
  * @updata: [2019-10-17][Lei][creat]
  */
static float CalculateVolatge(uint8_t* data, uint8_t lenth)
{

}


/**
  * @brief : �����¶�ֵ
  * @param : data �¶�ԭʼֵ
  * @param : lenth �¶����ݵĳ���
  * @return: ��������¶�ֵ
  * @updata: [2019-10-17][Lei][creat]
  */
static float CalculateTemperature(uint8_t* data, uint8_t lenth)
{

}





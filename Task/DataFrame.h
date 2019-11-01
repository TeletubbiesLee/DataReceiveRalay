/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      DataFrame.c
  * @brief:     ���������֡�����Ӧ��
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-06-21
  * @update:    [2019-06-21][Lei][creat]
  */

#ifndef __DATA_FRAME_H_
#define __DATA_FRAME_H_

#include <stdint.h>
#include <stdbool.h>

#define CONFIG_FLAG_ADDRESS				0x007		//���ñ�־λ��ַ
#define TEMPERATURE_FIRST_ADDRESS		0x008		//�¶ȴ洢�׵�ַ
#define VOLTAGE_FIRST_ADDRESS			0x108		//��ѹ�洢�׵�ַ
#define LAUNCH_NUMBER_FIRAT_ADDRESS		0x208		//��������洢�׵�ַ
#define NODE_STATUS_FIRST_ADDRESS		0x408		//�������ڵ�״̬�洢�׵�ַ
#define NODE_DEVICE_ID_FIRST_ADDRESS	0x708		//�������ڵ��豸ID�Ŵ洢�׵�ַ
#define MEASUREMENT_TYPE_FIRAT_ADDRESS	0x908		//�������������ʹ洢�׵�ַ



/* ���洫�����ڵ����ݣ��Լ������ݲ����Ľṹ�� */
typedef struct NodeData{
    bool isDataValid;           //������Ч��־λ
    uint32_t deviceId;          //�豸��ΨһID��
    float temperatureValue;     //�¶�ֵ
    float voltageValue;         //��ѹֵ
    uint8_t deviceNumber;       //�豸�����ݱ��еı��
	int8_t RSSI_Value;
	uint8_t LQI_Value;
    uint8_t (*getDeviceNumber)(struct NodeData* nodeData);       //�����豸ID�Ż�ȡ�豸���
    void (*saveTemperature)(struct NodeData nodeData);          //�����¶�ֵ
    void (*saveVoltage)(struct NodeData nodeData);                //�����ѹֵ
	void (*saveSignalStrength)(struct NodeData nodeData);		//�����ź�ǿ��
	void (*saveLaunchNumber)(struct NodeData nodeData);			//���淢�����

}NodeDataStruct;

void NodeDataStructInit(NodeDataStruct* nodeData);          //�������ڵ����ݽṹ���ʼ������
uint8_t DataFrameAnalysis(uint8_t* sourceData, NodeDataStruct* nodeData);		//���߽������ݽ���

#endif

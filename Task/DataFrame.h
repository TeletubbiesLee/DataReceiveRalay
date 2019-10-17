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


/* ���洫�����ڵ����ݣ��Լ������ݲ����Ľṹ�� */
typedef struct NodeData{
    bool isDataValid;           //������Ч��־λ
    uint32_t deviceId;          //�豸��ΨһID��
    float temperatureValue;     //�¶�ֵ
    float voltageValue;         //��ѹֵ
    uint8_t deviceNumber;       //�豸�����ݱ��еı��
    uint8_t (*getDeviceNumber)(struct NodeData* nodeData);       //�����豸ID�Ż�ȡ�豸���
    void (*saveTemperature)(struct NodeData nodeData);        //�����¶�ֵ
    void (*saveVoltage)(struct NodeData nodeData);                //�����ѹֵ

}NodeDataStruct;

void NodeDataStructInit(NodeDataStruct* nodeData);          //�������ڵ����ݽṹ���ʼ������
uint8_t DataFrameAnalysis(uint8_t* sourceData, NodeDataStruct* nodeData);

#endif

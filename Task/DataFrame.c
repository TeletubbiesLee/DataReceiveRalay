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
#include "user_mb_app.h"
#include "TaskModbus.h"
#include "DataFrame.h"


uint16_t g_TemperatureAndVoltage[128] = {0};		//��ŵ�ѹֵ���¶�ֵ
uint16_t g_NodeNumber[20] = {0};

/********************************function*************************************/
/**
  * @brief : ��������д�����ݻ�������
  * @param : data Ҫ��������
  * @param : dataBuffer ���ݻ�����
  * @param : dataLenth Ҫ�������ݵĳ���
  * @return: void 
  * @updata: [2019-06-21][Lei][creat]
  */
void WriteDataBuf(uint16_t* data, uint16_t* dataBuffer, uint8_t dataLenth)
{
	uint8_t i = 0;
	for(i = 0; i < dataLenth; i++)
	{
		dataBuffer[i] = data[i];
	}
	for(; i < S_REG_HOLDING_NREGS; i++)
	{
		dataBuffer[i] = 0;
	}
}


/**
  * @brief : ��ȡ������Ϣ
  * @param : dataBuffer ���������Ϣ�����ݻ�����
  * @return: void 
  * @updata: [2019-06-21][Lei][creat]
  */
void ReadConfig(uint8_t* dataBuffer)
{
	/* ���ò����� */
	switch(dataBuffer[0] & 0x0F)
	{
		case 1:
			g_ModbusBandrate = 300;
			break;
		case 2:
			g_ModbusBandrate = 600;
			break;
		case 3:
			g_ModbusBandrate = 1200;
			break;
		case 4:
			g_ModbusBandrate = 2400;
			break;
		case 5:
			g_ModbusBandrate = 4800;
			break;
		case 6:
			g_ModbusBandrate = 9600;
			break;
		case 7:
			g_ModbusBandrate = 14400;
			break;
		case 8:
			g_ModbusBandrate = 19200;
			break;
		case 9:
			g_ModbusBandrate = 38400;
			break;
		case 10:
			g_ModbusBandrate = 56000;
			break;
		case 11:
			g_ModbusBandrate = 57600;
			break;
		case 12:
			g_ModbusBandrate = 115200;
			break;
		case 13:
			g_ModbusBandrate = 128000;
			break;
		case 14:
			g_ModbusBandrate = 230400;
			break;
		case 15:
			g_ModbusBandrate = 256000;
			break;
		default:
			break;
	}
	/* ������żУ�� */
	switch((dataBuffer[0] & 0x30) >> 4)
	{
		case 0:
			g_ModbusParity = MB_PAR_NONE;
			break;
		case 1:
			g_ModbusParity = MB_PAR_ODD;
			break;
		case 2:
			g_ModbusParity = MB_PAR_EVEN;
			break;
		default:
			break;
	}
	/* ����Ҫ���յĽڵ� */
	for(uint8_t i = 0; i < 20; i++)
	{
		g_NodeNumber[i] = dataBuffer[i+5];
	}
}



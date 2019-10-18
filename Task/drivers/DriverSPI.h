#ifndef __DRIVER_SPI_H
#define __DRIVER_SPI_H
#include "stm32f4xx_hal.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//SPI��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/16/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

extern SPI_HandleTypeDef SPI2_Handler;  //SPI2���
extern SPI_HandleTypeDef SPI5_Handler;  //SPI5���

void SPI2_Init(void);
void SPI2_SetSpeed(uint8_t SPI_BaudRatePrescaler);
uint8_t SPI2_ReadWriteByte(uint8_t TxData);

void SPI5_Init(void);
void SPI5_SetSpeed(uint8_t SPI_BaudRatePrescaler);
uint8_t SPI5_ReadWriteByte(uint8_t TxData);
#endif

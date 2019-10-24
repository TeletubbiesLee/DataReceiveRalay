/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      drv_spi.h
  * @brief:     The driver of SPI.
  * @version:   V1.0.0(first implementation)
  * @author:    aozima
  * @date:      2012-01-01
  * @update:    [2018-01-14][Lexun][make the code cleanup]
  */
#ifndef __DRV_SPI_H__
#define __DRV_SPI_H__


/* INCLUDE FILES -------------------------------------------------------------*/
#include <rtthread.h>
#include <drivers/spi.h>

#include "stm32f4xx_hal.h"


/* DEFINES -------------------------------------------------------------------*/
//#define SPI_USE_DMA

#define DEBUG

#ifdef  DEBUG
#define SPI_PRINTF(...)   rt_kprintf(__VA_ARGS__)
#else
#define SPI_PRINTF(...)   
#endif /* DEBUG */


/* STRUCTS -------------------------------------------------------------------*/
struct stm32f4_spi
{
    SPI_HandleTypeDef spi_handle;
#ifdef SPI_USE_DMA
    DMA_HandleTypeDef hdma_rx;
    IRQn_Type hdma_rx_irq;

    DMA_HandleTypeDef hdma_tx;
    IRQn_Type hdma_tx_irq;
#endif /* SPI_USE_DMA */
};

struct stm32_spi_bus
{
    struct rt_spi_bus parent;
    
    SPI_TypeDef * SPI;
    
#ifdef SPI_USE_DMA
    DMA_Stream_TypeDef * DMA_Stream_TX;
    uint32_t DMA_Channel_TX;

    DMA_Stream_TypeDef * DMA_Stream_RX;
    uint32_t DMA_Channel_RX;

    uint32_t DMA_Channel_TX_FLAG_TC;
    uint32_t DMA_Channel_RX_FLAG_TC;
#endif /* #ifdef SPI_USE_DMA */    
};

struct stm32_spi_cs
{
    GPIO_TypeDef* GPIOx;
    uint16_t GPIO_Pin;
};


/* PUBLIC FUNCTION -----------------------------------------------------------*/
extern rt_err_t stm32_spi_bus_register(SPI_TypeDef * SPI, struct stm32_spi_bus * stm32_spi, const char * spi_bus_name);

#endif /* __DRV_SPI_H__ */ 


/* END OF FILE ---------------------------------------------------------------*/

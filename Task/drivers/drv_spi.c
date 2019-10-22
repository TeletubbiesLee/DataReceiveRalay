 /**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      drv_spi.c
  * @brief:     The driver of SPI.
  * @version:   V1.0.0
  * @author:    tanek
  * @date:      2017-06-05
  * @update:    [2018-01-14][Lexun][make the code cleanup]
  */

/* INCLUDE FILES -------------------------------------------------------------*/
#include <board.h>
#include <finsh.h>

#include "drv_spi.h"


/* PRIVATE FUNCTION ----------------------------------------------------------*/
static rt_err_t configure(struct rt_spi_device* device, struct rt_spi_configuration* configuration);
static rt_uint32_t xfer(struct rt_spi_device* device, struct rt_spi_message* message);


/* PRIVATE VARIABLES ---------------------------------------------------------*/
static struct rt_spi_ops stm32_spi_ops =
{
    configure,
    xfer
};

#ifdef SPI_USE_DMA
static uint8_t dummy = 0xFF;
#endif /* SPI_USE_DMA */

#ifdef RT_USING_SPI1
static struct rt_spi_bus spi1_bus;
static struct stm32f4_spi stm32f4_spi1 = 
{
    SPI1,
#ifdef SPI_USE_DMA
    {   
        DMA2_Stream2,
        DMA_CHANNEL_3,
    },
    DMA2_Stream2_IRQn,

    {
        DMA2_Stream3,
        DMA_CHANNEL_3,
    },
    DMA2_Stream3_IRQn,
#endif /* SPI_USE_DMA */
};
#endif /* RT_USING_SPI1 */

#ifdef RT_USING_SPI2
static struct rt_spi_bus spi2_bus;
struct stm32f4_spi stm32f4_spi2 = 
{
    SPI2,
#ifdef SPI_USE_DMA
    {   
        DMA1_Stream3,
        DMA_CHANNEL_0,
    },
    DMA1_Stream3_IRQn,

    {
        DMA1_Stream4,
        DMA_CHANNEL_0,
    },
    DMA1_Stream4_IRQn,
#endif /* SPI_USE_DMA */
};
#endif /* RT_USING_SPI2 */

#ifdef RT_USING_SPI3
static struct rt_spi_bus spi3_bus;
struct stm32f4_spi stm32f4_spi3 = 
{
    SPI3,
#ifdef SPI_USE_DMA
    {   
        DMA1_Stream0,
        DMA_CHANNEL_0,
    },
    DMA1_Stream0_IRQn,

    {
        DMA1_Stream2,
        DMA_CHANNEL_0,
    },
    DMA1_Stream2_IRQn,
#endif /* SPI_USE_DMA */
};
#endif /* RT_USING_SPI3 */

#ifdef RT_USING_SPI4
static struct rt_spi_bus spi4_bus;
struct stm32f4_spi stm32f4_spi4 = 
{
    SPI4,
#ifdef SPI_USE_DMA
    {   
        DMA2_Stream0,
        DMA_CHANNEL_4,
    },
    DMA2_Stream0_IRQn,

    {
        DMA2_Stream1,
        DMA_CHANNEL_4,
    },
    DMA2_Stream1_IRQn,
#endif /* SPI_USE_DMA */
};
#endif /* RT_USING_SPI4 */

#ifdef RT_USING_SPI5
static struct rt_spi_bus spi5_bus;
struct stm32f4_spi stm32f4_spi5 = 
{
    SPI5,
#ifdef SPI_USE_DMA
    {   
        DMA2_Stream3,
        DMA_CHANNEL_2,
    },
    DMA2_Stream3_IRQn,

    {
        DMA2_Stream4,
        DMA_CHANNEL_2,
    },
    DMA2_Stream4_IRQn,
#endif /* SPI_USE_DMA */
};
#endif /* RT_USING_SPI5 */

#ifdef RT_USING_SPI6
static struct rt_spi_bus spi6_bus;
struct stm32f4_spi stm32f4_spi6 = 
{
    SPI6,
#ifdef SPI_USE_DMA
    {   
        DMA2_Stream6,
        DMA_CHANNEL_2,
    },
    DMA2_Stream6_IRQn,

    {
        DMA2_Stream5,
        DMA_CHANNEL_2,
    },
    DMA2_Stream5_IRQn,
#endif /* SPI_USE_DMA */
};
#endif /* RT_USING_SPI6 */


/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/
#ifdef SPI_USE_DMA
/**
  * @brief  DMA rx configuration.
  * @param  [spi_bus] The rt_spi_bus pointer descriptor.
  * @param  [message] The rt_spi_message pointer parameter.
  * @retval None.
  */
static void DMA_RxConfiguration(struct rt_spi_bus * spi_bus, struct rt_spi_message* message)
{
    struct stm32f4_spi *f4_spi = (struct stm32f4_spi *)spi_bus->parent.user_data;
    
    DMA_HandleTypeDef * hdma_tx = &f4_spi->hdma_tx;
    DMA_HandleTypeDef * hdma_rx = &f4_spi->hdma_rx;
     
    HAL_DMA_DeInit(hdma_tx);
    HAL_DMA_DeInit(hdma_rx);
        
    /* Check if the DMA Stream is disabled before enabling it.
       Note that this step is useful when the same Stream is used multiple times:
       enabled, then disabled then re-enabled... In this case, the DMA Stream disable
       will be effective only at the end of the ongoing data transfer and it will
       not be possible to re-configure it before making sure that the Enable bit
       has been cleared by hardware. If the Stream is used only once, this step might
       be bypassed. */    
    while (hdma_tx->Instance->CR & DMA_SxCR_EN);
    while (hdma_rx->Instance->CR & DMA_SxCR_EN);

    if (message->recv_buf != RT_NULL)
    {
        hdma_rx->Init.MemInc = DMA_MINC_ENABLE;
    }
    else
    {
        message->recv_buf = &dummy;
        hdma_rx->Init.MemInc = DMA_MINC_DISABLE;
    }
    HAL_DMA_Init(hdma_rx);
    
    __HAL_LINKDMA(&f4_spi->spi_handle, hdmarx, f4_spi->hdma_rx);
        
    if (message->send_buf != RT_NULL)
    {
        hdma_tx->Init.MemInc = DMA_MINC_ENABLE;
    }
    else
    {
        dummy = 0xFF;
        message->send_buf = &dummy;
        hdma_tx->Init.MemInc = DMA_MINC_DISABLE;
    }
    HAL_DMA_Init(hdma_tx);
    
    __HAL_LINKDMA(&f4_spi->spi_handle, hdmatx, f4_spi->hdma_tx);
    
    /* NVIC configuration for DMA transfer complete interrupt*/
    HAL_NVIC_SetPriority(f4_spi->hdma_tx_irq, (INT_SPI_DMA_TX_PRIO >> 4) & 0x0F, INT_SPI_DMA_TX_PRIO & 0x0F);
    HAL_NVIC_EnableIRQ(f4_spi->hdma_tx_irq);

    /* NVIC configuration for DMA transfer complete interrupt*/
    HAL_NVIC_SetPriority(f4_spi->hdma_rx_irq, (INT_SPI_DMA_RX_PRIO >> 4) & 0x0F, INT_SPI_DMA_RX_PRIO & 0x0F);   
    HAL_NVIC_EnableIRQ(f4_spi->hdma_rx_irq);
}
#endif /* SPI_USE_DMA */

/**
  * @brief  SPI configuration.
  * @param  [device] The SPI device descriptor.
  * @param  [configuration] The SPI device configuration parameter.
  * @retval [RT_EOK] sucess.
  */
static rt_err_t configure(struct rt_spi_device* device, struct rt_spi_configuration* configuration)
{
    struct rt_spi_bus *spi_bus = (struct rt_spi_bus *)device->bus;	
    struct stm32f4_spi *f4_spi = (struct stm32f4_spi *)spi_bus->parent.user_data;
    SPI_HandleTypeDef *SpiHandle = &f4_spi->spi_handle;

    RT_ASSERT(device != RT_NULL);
    RT_ASSERT(configuration != RT_NULL);

    /* data_width */
    if (configuration->data_width <= 8)
    {
        SpiHandle->Init.DataSize = SPI_DATASIZE_8BIT;
    }
    else if (configuration->data_width <= 16)
    {
        SpiHandle->Init.DataSize = SPI_DATASIZE_16BIT;
    }
    else
    {
        return RT_EIO;
    }

    /* baudrate */
    {
        uint32_t SPI_APB_CLOCK;
        uint32_t max_hz;

        max_hz = configuration->max_hz;

        SPI_PRINTF("sys   freq: %d\n", HAL_RCC_GetSysClockFreq());
        SPI_PRINTF("pclk2 freq: %d\n", HAL_RCC_GetPCLK2Freq());

        SPI_APB_CLOCK = HAL_RCC_GetPCLK2Freq();

        if (max_hz >= SPI_APB_CLOCK / 2)
        {
            SpiHandle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
        }
        else if (max_hz >= SPI_APB_CLOCK / 4)
        {
            SpiHandle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
        }
        else if (max_hz >= SPI_APB_CLOCK / 8)
        {
            SpiHandle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
        }
        else if (max_hz >= SPI_APB_CLOCK / 16)
        {
            SpiHandle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
        }
        else if (max_hz >= SPI_APB_CLOCK/32)
        {
            SpiHandle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
        }
        else if (max_hz >= SPI_APB_CLOCK/64)
        {
            SpiHandle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
        }
        else if (max_hz >= SPI_APB_CLOCK/128)
        {
            SpiHandle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
        }
        else
        {
            /*  min prescaler 256 */
            SpiHandle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
        }
    } /* baudrate */

    /* CPOL */
    if (configuration->mode & RT_SPI_CPOL)
    {
        SpiHandle->Init.CLKPolarity = SPI_POLARITY_HIGH;
    }
    else
    {
        SpiHandle->Init.CLKPolarity = SPI_POLARITY_LOW;
    }
    /* CPHA */
    if (configuration->mode & RT_SPI_CPHA)
    {
        SpiHandle->Init.CLKPhase = SPI_PHASE_2EDGE;
    }
    else
    {
        SpiHandle->Init.CLKPhase = SPI_PHASE_1EDGE;
    }
    /* MSB or LSB */
    if(configuration->mode & RT_SPI_MSB)
    {
        SpiHandle->Init.FirstBit = SPI_FIRSTBIT_MSB;
    }
    else
    {
        SpiHandle->Init.FirstBit = SPI_FIRSTBIT_LSB;
    }
    SpiHandle->Init.Direction = SPI_DIRECTION_2LINES;
    SpiHandle->Init.Mode = SPI_MODE_MASTER;
    SpiHandle->Init.NSS  = SPI_NSS_SOFT;
    SpiHandle->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    SpiHandle->Init.TIMode = SPI_TIMODE_DISABLE;

    /* init SPI */
    if (HAL_SPI_Init(SpiHandle) != HAL_OK)
    {
        return RT_ERROR;
    }
    /* Enable SPI_MASTER */
    __HAL_SPI_ENABLE(SpiHandle);
    
    SPI_PRINTF("spi configuration\n");

    return(RT_EOK);
}

/**
  * @brief  SPI xfer function.
  * @param  [device] The SPI device descriptor.
  * @param  [message] The SPI device message.
  * @retval return the length of message.
  */
static rt_uint32_t xfer(struct rt_spi_device* device, struct rt_spi_message* message)
{
    struct rt_spi_bus *stm32_spi_bus = (struct rt_spi_bus *)device->bus;
    struct stm32f4_spi *f4_spi = (struct stm32f4_spi *)stm32_spi_bus->parent.user_data;
    struct rt_spi_configuration * config = &device->config;
    SPI_TypeDef *SPI = f4_spi->spi_handle.Instance;
    struct stm32_spi_cs *stm32_spi_cs = device->parent.user_data;
    rt_uint32_t size = message->length;

    RT_ASSERT(device != NULL);
    RT_ASSERT(message != NULL);
	
    /* take CS */
    if((message->cs_take)&&(stm32_spi_cs != NULL))
    {
        HAL_GPIO_WritePin(stm32_spi_cs->GPIOx, stm32_spi_cs->GPIO_Pin, GPIO_PIN_RESET);
    }

#ifdef SPI_USE_DMA
    if(message->length > 32)
    {
        if(config->data_width <= 8)
        {
            HAL_StatusTypeDef state; 
            SPI_PRINTF("spi dma transfer start\n");
            DMA_RxConfiguration(stm32_spi_bus, message);
            SPI_PRINTF("dma configuration finish , send buf %X, rec buf %X, length: %d\n", 
                    (uint32_t)message->send_buf, (uint32_t)message->recv_buf, message->length);
            
            state = HAL_SPI_TransmitReceive_DMA(&f4_spi->spi_handle, (uint8_t*)message->send_buf, (uint8_t*)message->recv_buf, message->length);
            if (state != HAL_OK) 
            {
                SPI_PRINTF("spi flash configuration error : %d\n", state);
                message->length = 0;
                //while(1);
            }
            else
            {
                SPI_PRINTF("spi dma transfer finish\n");            
            }
            
            while (HAL_SPI_GetState(&f4_spi->spi_handle) != HAL_SPI_STATE_READY);
            SPI_PRINTF("spi get state finish\n");
        }
        else
        {
            // Todo 
        }
    }
    else
#endif /* SPI_USE_DMA */
    {
        if(config->data_width <= 8)
        {
            const rt_uint8_t * send_ptr = message->send_buf;
            rt_uint8_t * recv_ptr = message->recv_buf;

            while(size--)
            {
                rt_uint8_t data = 0xFF;

                if(send_ptr != RT_NULL)
                {
                    data = *send_ptr++;
                }
                
                // Todo: replace register read/write by stm32f4 lib
                //Wait until the transmit buffer is empty
                while ((SPI->SR & SPI_FLAG_TXE) == RESET);
                // Send the byte
                SPI->DR = data;

                //Wait until a data is received
                while ((SPI->SR & SPI_FLAG_RXNE) == RESET);
                // Get the received data
                data = SPI->DR;

                if(recv_ptr != RT_NULL)
                {
                    *recv_ptr++ = data;
                }
            }
        }
        else if(config->data_width <= 16)
        {
            const rt_uint16_t * send_ptr = message->send_buf;
            rt_uint16_t * recv_ptr = message->recv_buf;

            while(size--)
            {
                rt_uint16_t data = 0xFF;

                if(send_ptr != RT_NULL)
                {
                    data = *send_ptr++;
                }

                //Wait until the transmit buffer is empty
                while ((SPI->SR & SPI_FLAG_TXE) == RESET);
                // Send the byte
                SPI->DR = data;

                //Wait until a data is received
                while ((SPI->SR & SPI_FLAG_RXNE) == RESET);
                // Get the received data
                data = SPI->DR;

                if(recv_ptr != RT_NULL)
                {
                    *recv_ptr++ = data;
                }
            }
        }
    }

    /* release CS */
    if((message->cs_release)&&(stm32_spi_cs != NULL))
    {
        HAL_GPIO_WritePin(stm32_spi_cs->GPIOx, stm32_spi_cs->GPIO_Pin, GPIO_PIN_SET);
    }

    return message->length;
}


/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
#ifdef RT_USING_SPI1
/**
  * @brief  This function handles DMA Rx interrupt request.
  * @param  None
  * @retval None  
  */
void DMA2_Stream2_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stm32f4_spi1.spi_handle.hdmarx);
}    
/**
  * @brief  This function handles DMA Tx interrupt request.  
  * @param  None
  * @retval None    
  */
void DMA2_Stream3_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stm32f4_spi1.spi_handle.hdmatx);
}
#endif /* RT_USING_SPI1 */

#ifdef RT_USING_SPI2
/**
  * @brief  This function handles DMA Rx interrupt request.
  * @param  None
  * @retval None  
  */
void DMA1_Stream3_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stm32f4_spi2.spi_handle.hdmarx);
}    
/**
  * @brief  This function handles DMA Tx interrupt request.  
  * @param  None
  * @retval None    
  */
void DMA1_Stream4_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stm32f4_spi2.spi_handle.hdmatx);
}
#endif /* RT_USING_SPI2 */

#ifdef RT_USING_SPI3
/**
  * @brief  This function handles DMA Rx interrupt request.
  * @param  None
  * @retval None  
  */
void DMA1_Stream0_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stm32f4_spi3.spi_handle.hdmarx);
}    
/**
  * @brief  This function handles DMA Tx interrupt request.  
  * @param  None
  * @retval None    
  */
void DMA1_Stream2_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stm32f4_spi3.spi_handle.hdmatx);
}
#endif /* RT_USING_SPI3 */

#ifdef RT_USING_SPI4
/**
  * @brief  This function handles DMA Rx interrupt request.
  * @param  None
  * @retval None  
  */
void DMA2_Stream0_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stm32f4_spi4.spi_handle.hdmarx);
}    
/**
  * @brief  This function handles DMA Tx interrupt request.  
  * @param  None
  * @retval None    
  */
void DMA2_Stream1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stm32f4_spi4.spi_handle.hdmatx);
}
#endif /* RT_USING_SPI4 */

#ifdef RT_USING_SPI5
/**
  * @brief  This function handles DMA Rx interrupt request.
  * @param  None
  * @retval None  
  */
void DMA2_Stream3_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stm32f4_spi5.spi_handle.hdmarx);
}    
/**
  * @brief  This function handles DMA Tx interrupt request.  
  * @param  None
  * @retval None    
  */
void DMA2_Stream4_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stm32f4_spi5.spi_handle.hdmatx);
}
#endif /* RT_USING_SPI5 */

#ifdef RT_USING_SPI6
/**
  * @brief  This function handles DMA Rx interrupt request.
  * @param  None
  * @retval None  
  */
void DMA2_Stream6_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stm32f4_spi6.spi_handle.hdmarx);
}    
/**
  * @brief  This function handles DMA Tx interrupt request.  
  * @param  None
  * @retval None    
  */
void DMA2_Stream5_IRQHandler(void)
{
    HAL_DMA_IRQHandler(stm32f4_spi6.spi_handle.hdmatx);
}
#endif /* RT_USING_SPI6 */

/**
  * @brief  init and register stm32 spi bus.
  * @param  [SPI] STM32 SPI, e.g: SPI1,SPI2,SPI3.
  * @param  [stm32_spi] The stm32_spi_bus pointer.
  * @param  [spi_bus_name] spi bus name, e.g: "spi1".
  * @retval [RT_ENOSYS] error; others is successful.  
  */
rt_err_t stm32_spi_bus_register(SPI_TypeDef *SPI, struct stm32_spi_bus *stm32_spi, const char *spi_bus_name)
{
#ifdef SPI_USE_DMA
    struct stm32f4_spi *p_spi_bus;
#endif /* SPI_USE_DMA */
	
    struct rt_spi_bus *spi_bus;
    
    RT_ASSERT(SPI != RT_NULL);
//    RT_ASSERT(stm32_spi != RT_NULL);
    RT_ASSERT(spi_bus_name != RT_NULL);
        
#ifdef RT_USING_SPI1
    if (SPI == SPI1)
    {
    #ifdef SPI_USE_DMA
        __HAL_RCC_DMA2_CLK_ENABLE();
        p_spi_bus = &stm32f4_spi1;
    #endif /* SPI_USE_DMA */
        __HAL_RCC_SPI1_CLK_ENABLE();
        spi_bus = &spi1_bus;
		
        /* add by sun */
        spi_bus->parent.user_data = &stm32f4_spi1;
    }
#endif /* RT_USING_SPI1 */
    
#ifdef RT_USING_SPI2
    if (SPI == SPI2)
    {
    #ifdef SPI_USE_DMA
        __HAL_RCC_DMA1_CLK_ENABLE();
        p_spi_bus = &stm32f4_spi2;
    #endif /* SPI_USE_DMA */
        __HAL_RCC_SPI2_CLK_ENABLE();
        spi_bus = &spi2_bus;
		
        /* add by sun */
        spi_bus->parent.user_data = &stm32f4_spi2;
    }
#endif /* RT_USING_SPI2 */

#ifdef RT_USING_SPI3
    if (SPI == SPI3)
    {
    #ifdef SPI_USE_DMA
        __HAL_RCC_DMA1_CLK_ENABLE();
        p_spi_bus = &stm32f4_spi3;
    #endif /* SPI_USE_DMA */
        __HAL_RCC_SPI3_CLK_ENABLE();
        spi_bus = &spi3_bus;
		
        /* add by sun */
        spi_bus->parent.user_data = &stm32f4_spi3;
    }
#endif /* RT_USING_SPI3 */
    
#ifdef RT_USING_SPI4
    if (SPI == SPI4)
    {
    #ifdef SPI_USE_DMA
        __HAL_RCC_DMA2_CLK_ENABLE();
			  p_spi_bus = &stm32f4_spi4;
    #endif /* SPI_USE_DMA */
        __HAL_RCC_SPI4_CLK_ENABLE();
        spi_bus = &spi4_bus;
		
        /* add by sun */
        spi_bus->parent.user_data = &stm32f4_spi4;
    }
#endif /* RT_USING_SPI4 */

#ifdef RT_USING_SPI5
    if (SPI == SPI5)
    {
    #ifdef SPI_USE_DMA
        __HAL_RCC_DMA2_CLK_ENABLE();
        p_spi_bus = &stm32f4_spi5;		
    #endif /* SPI_USE_DMA */
        __HAL_RCC_SPI5_CLK_ENABLE();
        spi_bus = &spi5_bus;
		
        /* add by sun */
        spi_bus->parent.user_data = &stm32f4_spi5;
    }
#endif /* RT_USING_SPI5 */
 
#ifdef RT_USING_SPI6
    if (SPI == SPI6)
    {
    #ifdef SPI_USE_DMA
        __HAL_RCC_DMA2_CLK_ENABLE();
        p_spi_bus = &stm32f4_spi6;
    #endif /* SPI_USE_DMA */
        __HAL_RCC_SPI6_CLK_ENABLE();
        spi_bus = &spi6_bus;
		
        /* add by sun */
        spi_bus->parent.user_data = &stm32f4_spi6;
    }
#endif /* RT_USING_SPI6 */

    if ((SPI != SPI1) && (SPI != SPI2) && (SPI != SPI3) && (SPI != SPI4) && (SPI != SPI5) && (SPI != SPI6))
    {
        return (RT_ENOSYS);
    }
    
#ifdef SPI_USE_DMA
    /* Configure the DMA handler for Transmission process */
    p_spi_bus->hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    p_spi_bus->hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
    //p_spi_bus->hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
    p_spi_bus->hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    p_spi_bus->hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    p_spi_bus->hdma_tx.Init.Mode                = DMA_NORMAL;
    p_spi_bus->hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
    p_spi_bus->hdma_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;         
    p_spi_bus->hdma_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    p_spi_bus->hdma_tx.Init.MemBurst            = DMA_MBURST_INC4;
    p_spi_bus->hdma_tx.Init.PeriphBurst         = DMA_PBURST_INC4;
    
    p_spi_bus->hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    p_spi_bus->hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
    //p_spi_bus->hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
    p_spi_bus->hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    p_spi_bus->hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    p_spi_bus->hdma_rx.Init.Mode                = DMA_NORMAL;
    p_spi_bus->hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
    p_spi_bus->hdma_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;         
    p_spi_bus->hdma_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    p_spi_bus->hdma_rx.Init.MemBurst            = DMA_MBURST_INC4;
    p_spi_bus->hdma_rx.Init.PeriphBurst         = DMA_PBURST_INC4;
#endif /* SPI_USE_DMA */
    
    return rt_spi_bus_register(spi_bus, spi_bus_name, &stm32_spi_ops);
}


/* END OF FILE ---------------------------------------------------------------*/


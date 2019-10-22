/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      drv_spi_flash.c
  * @brief:     The driver of spi flash.This file is part of RT-Thread RTOS.
  * @version:   V1.0.0(first implementation)
  * @author:    aozima
  * @date:      2012-01-01 
  * @update:    [2012-07-27][aozima][fixed variable uninitialized]
  * @update:    [2018-01-14][Lexun][make the code cleanup]
  */
	
/* INCLUDE FILES -------------------------------------------------------------*/
#include <rthw.h>
#include <finsh.h>

#include <board.h>
#include "drv_spi.h"
#include "spi_flash.h"
#include "spi_flash_sfud.h"


/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @brief : SPI4 initialization. 
  * @param : [none]
  * @return: [RT_ERROR] error;[RT_EOK] successful.
  */
static int rt_hw_spi4_init(void)
{
    /* register spi bus */
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        rt_err_t result;

        __HAL_RCC_GPIOE_CLK_ENABLE();

        GPIO_InitStructure.Pin = GPIO_PIN_2 | GPIO_PIN_5 | GPIO_PIN_6;
        GPIO_InitStructure.Alternate  = GPIO_AF5_SPI4;
        GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStructure.Pull  = GPIO_PULLUP;
        GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
        HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);

//        result = stm32_spi_bus_register(SPI4, 0, "spi4");
        result = stm32_spi_bus_register(SPI4, 0, RT_SPI4_BUS_NAME);
        if (result != RT_EOK)
        {
            return(result);
        }
    }

    /* attach cs */
    {
        static struct rt_spi_device spi_device;
        static struct stm32_spi_cs  spi_cs;
        rt_err_t result;

        GPIO_InitTypeDef GPIO_InitStructure;

        GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStructure.Pull  = GPIO_PULLUP;
        GPIO_InitStructure.Speed = GPIO_SPEED_FAST;

        spi_cs.GPIOx = GPIOE;
        spi_cs.GPIO_Pin = GPIO_PIN_4;
        __HAL_RCC_GPIOE_CLK_ENABLE();

        GPIO_InitStructure.Pin = spi_cs.GPIO_Pin;
        HAL_GPIO_WritePin(spi_cs.GPIOx, spi_cs.GPIO_Pin, GPIO_PIN_SET);
        HAL_GPIO_Init(spi_cs.GPIOx, &GPIO_InitStructure);

//        result = rt_spi_bus_attach_device(&spi_device, "spi40", "spi4", (void*)&spi_cs);
        result = rt_spi_bus_attach_device(&spi_device, RT_SPI4_DEVICE_NAME, RT_SPI4_BUS_NAME, (void*)&spi_cs);
        if (result != RT_EOK)
        {
            return(result);
        }
    }
		
    return(RT_EOK);
}
INIT_DEVICE_EXPORT(rt_hw_spi4_init);

#if (0)
/**
  * @brief : spi flash test. 
  * @param : [none]
  * @return: [RT_ERROR] error;[RT_EOK] successful.
  * @updata: [2017-12-21][Sunxr][newly increased]
  */
static int spi_flash_test(void)
{
    rt_uint8_t buf1[10] = {1,2,3,4,5,6,7,8,9};
    rt_uint8_t buf2[10] = {0};
    rt_device_t device = RT_NULL;
		
    device = rt_device_find(RT_SPI_FLASH_NAME);
	
    if (device == NULL)
    {
        SPI_PRINTF("W25Q256FV is not found! \r\n"); 
    }
    else
    {
        rt_device_open(device, RT_DEVICE_FLAG_RDWR);	  
        rt_device_write(device, 0x01, &buf1, 10);
        rt_device_read(device, 0x01, &buf2, 10);  
        rt_device_close(device);		
    }  
   
    return(0);	
}
#endif

/**
  * @brief : spi flash initialization. 
  * @param : [none]
  * @return: [RT_ERROR] error;[RT_EOK] successful.
  */
static int rt_hw_spi_flash_with_sfud_init(void)
{
//    if (RT_NULL == rt_sfud_flash_probe("W25Q256FV", "spi40"))
    if (RT_NULL == rt_sfud_flash_probe(RT_SPI_FLASH_NAME, RT_SPI4_DEVICE_NAME))
    {
        return(RT_ERROR);
    }
		
    return(RT_EOK);
}
INIT_COMPONENT_EXPORT(rt_hw_spi_flash_with_sfud_init);


/* END OF FILE ---------------------------------------------------------------*/

/**
  *            Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file    : spi_flash.h
  * @brief   : The driver of spi_flash.
  * @version : V03.001
  * @author  : Sun
  * @date    : 2017-12-21
  * @updata  : [2017-12-21][Sunxr][make the code cleanup]
  */
  
#ifndef SPI_FLASH_H__
#define SPI_FLASH_H__

/* STRUCT --------------------------------------------------------------------*/	
struct spi_flash_device
{
    struct rt_device                flash_device;
    struct rt_device_blk_geometry   geometry;
    struct rt_spi_device *          rt_spi_device;
    struct rt_mutex                 lock;
    void *                          user_data;
};

typedef struct spi_flash_device *rt_spi_flash_device_t;

#ifdef RT_USING_MTD_NOR
struct spi_flash_mtd
{
    struct rt_mtd_nor_device 			mtd_device;
    struct rt_spi_device *     			rt_spi_device;
    struct rt_mutex            			lock;
    void *                     			user_data;
};
#endif

#endif

/* END OF FILE ---------------------------------------------------------------*/

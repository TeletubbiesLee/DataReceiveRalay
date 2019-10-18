/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "w25qxx.h"

/* Definitions of physical drive number for each drive */
#define SD_CARD	 	0  			//SD卡,卷标为0
#define EX_FLASH 	1			//外部spi flash,卷标为1
#define EX_NAND  	2			//外部nand flash,卷标为2

//对于W25Q256
//前25M字节给fatfs用,25M字节后,用于存放字库,字库占用6.01M.	剩余部分,给客户自己用	 
#define FLASH_SECTOR_SIZE 	512	
#define FLASH_SECTOR_COUNT 	1024*25*2	//W25Q256,前25M字节给FATFS占用	
#define FLASH_BLOCK_SIZE   	8     		//每个BLOCK有8个扇区	


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	//int result;

	switch (pdrv) {
	case SD_CARD :
		//result = RAM_disk_status();

		// translate the reslut code here

		return stat;

	case EX_FLASH :
		//result = MMC_disk_status();

		// translate the reslut code here

		stat = RES_OK;
		
		return stat;

	case EX_NAND :
		//result = USB_disk_status();

		// translate the reslut code here

		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case SD_CARD :
		//result = RAM_disk_initialize();

		// translate the reslut code here

		return stat;

	case EX_FLASH :
		//result = MMC_disk_initialize();

		// translate the reslut code here

		W25QXX_Init();  //W25QXX初始化
		stat = RES_OK;
		return stat;

	case EX_NAND :
		//result = USB_disk_initialize();

		// translate the reslut code here

		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case SD_CARD :
		// translate the arguments here

		//result = RAM_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;

	case EX_FLASH :
		// translate the arguments here

		//result = MMC_disk_read(buff, sector, count);

		// translate the reslut code here

		for(;count>0;count--)
		{
			W25QXX_Read(buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
			sector++;
			buff+=FLASH_SECTOR_SIZE;
		}
		res = RES_OK;
		return res;

	case EX_NAND :
		// translate the arguments here

		//result = USB_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case SD_CARD :
		// translate the arguments here

		//result = RAM_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;

	case EX_FLASH :
		// translate the arguments here

		//result = MMC_disk_write(buff, sector, count);

		// translate the reslut code here

		for(;count>0;count--)
		{										    
			W25QXX_Write((uint8_t*)buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
			sector++;
			buff+=FLASH_SECTOR_SIZE;
		}
		res = RES_OK;
		return res;

	case EX_NAND :
		// translate the arguments here

		//result = USB_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case SD_CARD :

		// Process of the command for the RAM drive

		return res;

	case EX_FLASH :

		// Process of the command for the MMC/SD card
	
		switch(cmd)
	    {
		    case CTRL_SYNC:
				res = RES_OK; 
		        break;	 
		    case GET_SECTOR_SIZE:
		        *(WORD*)buff = FLASH_SECTOR_SIZE;
		        res = RES_OK;
		        break;	 
		    case GET_BLOCK_SIZE:
		        *(WORD*)buff = FLASH_BLOCK_SIZE;
		        res = RES_OK;
		        break;	 
		    case GET_SECTOR_COUNT:
		        *(DWORD*)buff = FLASH_SECTOR_COUNT;
		        res = RES_OK;
		        break;
		    default:
		        res = RES_PARERR;
		        break;
	    }

		return res;

	case EX_NAND :

		// Process of the command the USB drive

		return res;
	}

	return RES_PARERR;
}


//获得时间
//User defined function to give a current time to fatfs module      */
//31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */                                                                                                                                                                                                                                          
//15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */                                                                                                                                                                                                                                                
DWORD get_fattime (void)
{				 
	return 0;
}

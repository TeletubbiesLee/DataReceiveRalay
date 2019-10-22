#ifndef RT_CONFIG_H__
#define RT_CONFIG_H__

/* Automatically generated file; DO NOT EDIT. */
/* RT-Thread Configuration */

/* RT-Thread Kernel */

#define RT_NAME_MAX 8
#define RT_ALIGN_SIZE 4
#define RT_THREAD_PRIORITY_32
#define RT_THREAD_PRIORITY_MAX 32
#define RT_TICK_PER_SECOND 1000
#define RT_USING_OVERFLOW_CHECK
#define RT_USING_HOOK
#define RT_USING_IDLE_HOOK
#define RT_IDEL_HOOK_LIST_SIZE 4
#define IDLE_THREAD_STACK_SIZE 1024
#define RT_DEBUG
#define RT_DEBUG_COLOR

/* Inter-Thread communication */

#define RT_USING_SEMAPHORE
#define RT_USING_MUTEX
#define RT_USING_EVENT
#define RT_USING_MAILBOX
#define RT_USING_MESSAGEQUEUE

/* Memory Management */

//#define RT_USING_MEMPOOL
//#define RT_USING_MEMHEAP
//#define RT_USING_MEMHEAP_AS_HEAP
//#define RT_USING_HEAP

/* Memory Management */
#define RT_USING_MEMPOOL
#define RT_USING_MEMHEAP
/* RT_USING_NOHEAP is not set */
#define RT_USING_SMALL_MEM
/* RT_USING_SLAB is not set */
/* RT_USING_MEMHEAP_AS_HEAP is not set */
#define RT_USING_HEAP

/* Kernel Device Object */

#define RT_USING_DEVICE
#define RT_USING_CONSOLE
#define RT_CONSOLEBUF_SIZE 128
#define RT_CONSOLE_DEVICE_NAME "uart4"
#define RT_VER_NUM 0x40002
#define ARCH_ARM
#define RT_USING_CPU_FFS
#define ARCH_ARM_CORTEX_M
#define ARCH_ARM_CORTEX_M4

/* RT-Thread Components */

#define RT_USING_COMPONENTS_INIT
#define RT_USING_USER_MAIN
#define RT_MAIN_THREAD_STACK_SIZE 2048
#define RT_MAIN_THREAD_PRIORITY 10

/* C++ features */


/* Command shell */

#define RT_USING_FINSH
#define FINSH_THREAD_NAME "tshell"
#define FINSH_USING_HISTORY
#define FINSH_HISTORY_LINES 5
#define FINSH_USING_SYMTAB
#define FINSH_USING_DESCRIPTION
#define FINSH_THREAD_PRIORITY 20
#define FINSH_THREAD_STACK_SIZE 4096
#define FINSH_CMD_SIZE 80
#define FINSH_USING_MSH
//#define FINSH_USING_MSH_DEFAULT
#define FINSH_ARG_MAX 10

/* Device virtual file system */
#define RT_USING_DFS
#define DFS_USING_WORKDIR
#define RT_USING_DFS_ELMFAT
#define DFS_FILESYSTEMS_MAX                                            4
#define DFS_FILESYSTEM_TYPES_MAX                                       4
#define DFS_FD_MAX                                                     4

/* elm-chan's FatFs, Generic FAT Filesystem Module */
#define RT_DFS_ELM_WORD_ACCESS
#define RT_DFS_ELM_USE_LFN_0
#define RT_DFS_ELM_CODE_PAGE                                           437
/* RT_DFS_ELM_USE_LFN_1 is not set */
/* RT_DFS_ELM_USE_LFN_2 is not set */
/* RT_DFS_ELM_USE_LFN_3 is not set */
#define RT_DFS_ELM_USE_LFN                                             3
#define RT_DFS_ELM_MAX_LFN                                             255
#define RT_DFS_ELM_DRIVES                                              1
#define RT_DFS_ELM_MAX_SECTOR_SIZE                                     4096

/* RT_DFS_ELM_USE_ERASE is not set */
//#define RT_DFS_ELM_REENTRANT
#define RT_USING_DFS_DEVFS
#define RT_USING_DFS_NET
#define HAVE_SYS_SELECT_H
/* HAVE_SYS_SOCKET_H is not set */
#define RT_USING_DFS_ROMFS
//#define RT_USING_DFS_UFFS
/* RT_USING_DFS_RAMFS is not set */
/* RT_USING_DFS_UFFS is not set */
/* RT_USING_DFS_NFS is not set */


/* Device Drivers */

#define RT_USING_DEVICE_IPC
#define RT_PIPE_BUFSZ 512
#define RT_USING_SERIAL
#define RT_SERIAL_USING_DMA
#define RT_SERIAL_RB_BUFSZ 64
#define RT_USING_PIN

/* Using Hardware Crypto drivers */


/* Using WiFi */


/* Using USB */


/* POSIX layer and C standard library */
#define RT_USING_LIBC
#define RT_USING_POSIX

/* Network */

/* Socket abstraction layer */


/* Network interface device */


/* light weight TCP/IP stack */


/* Modbus master and slave stack */


/* AT commands */


/* VBUS(Virtual Software BUS) */


/* Utilities */


/* RT-Thread online packages */

/* IoT - internet of things */


/* Wi-Fi */

/* Marvell WiFi */


/* Wiced WiFi */


/* IoT Cloud */


/* security packages */


/* language packages */


/* multimedia packages */


/* tools packages */


/* system packages */


/* peripheral libraries and drivers */


/* miscellaneous packages */


/* samples: kernel and components samples */

#define SOC_FAMILY_STM32
#define SOC_SERIES_STM32F4

/* Hardware Drivers Config */

#define SOC_STM32F429IG

/* Onboard Peripheral Drivers */

#define BSP_USING_USB_TO_USART

/* On-chip Peripheral Drivers */

#define BSP_USING_GPIO
#define BSP_USING_UART
#define BSP_USING_UART1
#define BSP_USING_UART2
#define BSP_USING_UART4

/* Board extended module Drivers */

/* Using SPI framework */
#define RT_USING_SPI
#define RT_USING_SPI2
#define RT_USING_SPI4
#define RT_USING_SPI5

#define RT_SPI2_DEVICE_NAME                                            "spi20"
#define RT_SPI2_BUS_NAME                                               "spi2"
#define RT_SPI4_DEVICE_NAME                                            "spi40"
#define RT_SPI4_BUS_NAME                                               "spi4"
#define RT_SPI5_DEVICE_NAME                                            "spi50"
#define RT_SPI5_BUS_NAME                                               "spi5"
#define RT_IO_SPI1_DEVICE_NAME                                         "io_spi10"
#define RT_IO_SPI1_BUS_NAME                                            "io_spi1"

#define RT_SPI_FRAM_NAME                                               "fram0"
#define RT_SPI_ETH_NAME                                                "w5500"
#define RT_SPI_ENCRYPTION_NAME                                         "sc1161y"
#define RT_SPI_FLASH_NAME                                              "W25Q256FV"

#define RT_USING_SFUD
#define RT_SFUD_USING_SFDP
#define RT_SFUD_USING_FLASH_INFO_TABLE


#endif

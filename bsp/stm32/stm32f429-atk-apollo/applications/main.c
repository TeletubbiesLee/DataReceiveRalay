/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include "TaskModbus.h"
#include "TaskDataReceive.h"


/* defined the LED0 pin: PB1 */
#define LED0_PIN    GET_PIN(B, 0)

int main(void)
{
    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
	
	rt_thread_mdelay(1000);
	
    TaskModbusInit();			//创建Modbus相关任务
	TaskDataReceiveInit();		//创建CC1101无线数据接收任务
	
	
    while (1)
    {
        rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED0_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }

    return RT_EOK;
}

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
#include "DataFrame.h"
#include "user_mb_app.h"

extern USHORT usSRegHoldBuf[S_REG_HOLDING_NREGS];

/* defined the LED0 pin: PB1 */
#define LED0_PIN    GET_PIN(B, 1)

int main(void)
{
    int count = 0;
    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
	
	rt_thread_mdelay(500);
	rt_kprintf("\r\n");
    TaskModbusInit();
	
    while (++count)
    {
		//rt_kprintf("[%d]\r\n", count);
		//g_TemperatureAndVoltage[count%100] = (uint16_t)(count&0xFFFF);
		//WriteDataBuf(g_TemperatureAndVoltage, (uint16_t*)usSRegHoldBuf, 100);
        rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED0_PIN, PIN_LOW);
        
        rt_thread_mdelay(500);
    }

    return RT_EOK;
}

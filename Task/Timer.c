/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      Timer.c
  * @brief:     定时器相关的应用程序
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-11-05
  * @update:    [2019-11-05][Lei][creat]
  */


/********************************include**************************************/
#include <rtthread.h>
#include "Timer.h"


static uint32_t NodeOvertimeTimerNumber = 0;		//节点超时定时器计数
static void NodeOvertimeTiming(void* parameter);


/********************************function**************************************/
/**
  * @brief : 创建检测超时的定时器
  * @param : void
  * @return: void 
  * @updata: [2019-11-05][Lei][creat]
  */
void TimerInit(void)
{
	rt_timer_t checkOvertime;		//用作发射器接收超时的计时
	
	checkOvertime = rt_timer_create("NodeOvertimeTiming",
									NodeOvertimeTiming,
									RT_NULL,
									RT_TICK_PER_SECOND,			//定时一秒
									RT_TIMER_FLAG_PERIODIC);
	if(RT_NULL != checkOvertime)
	{
		rt_timer_start(checkOvertime);
	}
	
}


/**
  * @brief : 检测节点超时定时器
  * @param : void
  * @return: void 
  * @updata: [2019-11-05][Lei][creat]
  */
static void NodeOvertimeTiming(void* parameter)
{
	NodeOvertimeTimerNumber++;
}


/**
  * @brief : 获取节点定时器时间
  * @param : void
  * @return: void 
  * @updata: [2019-11-05][Lei][creat]
  */
uint32_t GetNodeOvertime(void)
{
	return NodeOvertimeTimerNumber;
}


/**
  * @brief : 判断是否超时
  * @param : startTime 起始时间
  * @param : overtime 超时的定时时长
  * @return: 0：未超时；1：超时
  * @updata: [2019-11-05][Lei][creat]
  */
uint8_t CheckOvertime(uint32_t startTime, uint32_t overtime)
{
	/* TODO:判断是否超时 */
	
	return 0;
}




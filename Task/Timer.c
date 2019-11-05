/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      Timer.c
  * @brief:     ��ʱ����ص�Ӧ�ó���
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-11-05
  * @update:    [2019-11-05][Lei][creat]
  */


/********************************include**************************************/
#include <rtthread.h>
#include "Timer.h"


static uint32_t NodeOvertimeTimerNumber = 0;		//�ڵ㳬ʱ��ʱ������
static void NodeOvertimeTiming(void* parameter);


/********************************function**************************************/
/**
  * @brief : ������ⳬʱ�Ķ�ʱ��
  * @param : void
  * @return: void 
  * @updata: [2019-11-05][Lei][creat]
  */
void TimerInit(void)
{
	rt_timer_t checkOvertime;		//�������������ճ�ʱ�ļ�ʱ
	
	checkOvertime = rt_timer_create("NodeOvertimeTiming",
									NodeOvertimeTiming,
									RT_NULL,
									RT_TICK_PER_SECOND,			//��ʱһ��
									RT_TIMER_FLAG_PERIODIC);
	if(RT_NULL != checkOvertime)
	{
		rt_timer_start(checkOvertime);
	}
	
}


/**
  * @brief : ���ڵ㳬ʱ��ʱ��
  * @param : void
  * @return: void 
  * @updata: [2019-11-05][Lei][creat]
  */
static void NodeOvertimeTiming(void* parameter)
{
	NodeOvertimeTimerNumber++;
}


/**
  * @brief : ��ȡ�ڵ㶨ʱ��ʱ��
  * @param : void
  * @return: void 
  * @updata: [2019-11-05][Lei][creat]
  */
uint32_t GetNodeOvertime(void)
{
	return NodeOvertimeTimerNumber;
}


/**
  * @brief : �ж��Ƿ�ʱ
  * @param : startTime ��ʼʱ��
  * @param : overtime ��ʱ�Ķ�ʱʱ��
  * @return: 0��δ��ʱ��1����ʱ
  * @updata: [2019-11-05][Lei][creat]
  */
uint8_t CheckOvertime(uint32_t startTime, uint32_t overtime)
{
	/* TODO:�ж��Ƿ�ʱ */
	
	return 0;
}




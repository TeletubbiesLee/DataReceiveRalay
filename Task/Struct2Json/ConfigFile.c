/**
 * @file ConfigFile.c
 * @brief 程序部分参数配置文件
 * @copyright Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * @company  SOJO
 * @date 2018.6.15
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 

#include "ConfigFile.h"


/*************************************************静态变量*************************************************/
ConfigFile g_ConfigFile[] = \
{
    { 
        .name = "UART1_Baudrate",			//串口1波特率
        .parameter = 115200,    			//kbps
        .number = ConfigFileEnum_USART1, },
    { 
        .name = "UART2_Baudrate",			//串口2波特率
        .parameter = 115200,    			//kbps
        .number = ConfigFileEnum_USART2, },
    { 
        .name = "UART6_Baudrate",			//串口6波特率
        .parameter = 9600,   		 		//kbps
        .number = ConfigFileEnum_USART6, },
    {
        .name = "SampleFrequency",			//采样频率
        .parameter = 3,     				//s
        .number = ConfigFileEnum_Sample, },
    {
        .name = "GPS_Enable",				//GPS使能
        .parameter = DIS_ENABLE,     		//0则为OFF，1为ON
        .number = ConfigFileEnum_GPS, },
    {
        .name = "GPS_Frequency",			//GPS频率
        .parameter = 3,     				//单位为s（秒）
        .number = ConfigFileEnum_GPS_Sample, },
    {
        .name = "GetPressureEnable",		//获取压力使能
        .parameter = DIS_ENABLE,     		//0则为OFF，1为ON
        .number = ConfigFileEnum_PRESSURE, },
    {
        .name = "GetTemperatureEnable",		//获取温度使能
        .parameter = DIS_ENABLE,     		//0则为OFF，1为ON
        .number = ConfigFileEnum_TEMPER, },
    {
        .name = "LocalAddress",				//本机地址
        .parameter = 1,     				//本机地址，采用十进制进行编码
        .number = ConfigFileEnum_MAC_ADDR, },
    {
        .name = "ASDU_PublicAddress",		//ASDU公用地址
        .parameter = 1,     				//ASDU公用地址
        .number = ConfigFileEnum_ASDU_ADDR, },
    {
        .name = "MQTT_Enable",				//MQTT使能
        .parameter = DIS_ENABLE,     		//1为开启，0为关闭
        .number = ConfigFileEnum_MQTT, },
    {
        .name = "TransmissionMode",			//传输模式
        .parameter = 0,     				//0为平衡，1为非平衡
        .number = ConfigFileEnum_Mode, },
    {
        .name = "SleepMode",				//睡眠模式
        .parameter = 1,     				//1为开启，0为关闭
        .number = ConfigFileEnum_SleepMode, },
    {
        .name = "SleepModeIntervalTime",	//睡眠模式间隔时间
        .parameter = 3,     				//单位 *GPS频率 s(秒)
        .number = ConfigFileEnum_SleepMode_F, },
    {
        .name = "KeyDitheringElimination",	//按键消抖
        .parameter = 50,     				//单位 *10 ms(毫秒)
        .number = ConfigFileEnum_ScanKey, },
	{
        .name = "HorizontalAccelerationThreshold",		//水平加速度阈值
        .parameter = 30,     							//单位：m/s^2
        .number = ConfigFileEnum_HorizontalAcceleration, },
	{
        .name = "VerticalAccelerationThreshold",		//垂直加速度阈值
        .parameter = 15,     						//单位：m/s^2
        .number = ConfigFileEnum_VerticalAcceleration, },
	{
        .name = "HorizontalAngleThreshold",			//水平倾角阈值
        .parameter = 10,     						//单位：°
        .number = ConfigFileEnum_HorizontalAngle, },
	{
        .name = "VerticalAngleThreshold",			//垂直倾角阈值
        .parameter = 15,     						//单位：°
        .number = ConfigFileEnum_VerticalAngle, },
	
};
/*************************************************END*************************************************/

/*************************************************公有函数实现*************************************************/
/**
 * @fn GetConfigFileLength
 * @brief 获取配置文件结构体数组长度
 * @return 返回数组长度
 * 
 */
inline uint16_t GetConfigFileLength(void)
{
    return (sizeof(g_ConfigFile) / sizeof(g_ConfigFile[0]));    //配置文件结构体数组长度;
}


/*************************************************END*************************************************/










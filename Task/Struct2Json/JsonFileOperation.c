/**
 * @file JsonFileOperation.c
 * @brief 实现Json文件操作
 * @copyright Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * @company SOJO
 * @date 2018.05.14
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 

#include "string.h"
#include "Interface_S2J.h"
#include "JsonFileOperation.h"
//#include "md5.h"
	

#include "ff.h"
#include "UserFreeRTOS.h"
	
static FIL FilePositon;

/**
 * @fn rt_s2j_init
 * @brief 初始化struct2json
 * @return  RT_EOK    初始化成功
 *          其他      失败
 */
uint8_t vPort_s2j_init(void)
{
    s2jHook.malloc_fn = pvPortMalloc;  //初始化内存申请函数
    s2jHook.free_fn = vPortFree;

    s2j_init(&s2jHook);   //初始化struct2json的内存申请

    return 0;
}

/**
 * @fn Create_JsonFile
 * @brief 结构体转换为json并写入到文件
 * @param fileName    要写入的文件名称
 * @param length    转换的长度
 * @param name    需要转换的结构体
 * 
 */
uint8_t Create_JsonFile(void)
{
//    uint8_t md5Buffer[16];
    char* string;
    unsigned int writeNum = 0;
    uint8_t res = 0;

    res = f_mkdir("/sojo"); //创建一个目录

    uint16_t length = GetConfigFileLength();    //获取长度

    if(res == 0)   //ID号能对应上则退出
    {
        return 0;
    }

    res = f_open(&FilePositon, "/sojo/ConfigFile.json", FA_CREATE_ALWAYS|FA_WRITE|FA_READ); //在sojo目录下创建一个可读写文件
    
    if(res != 0)
    {
        res = 1;
    }
    
    res = f_write(&FilePositon, " \"ConfigFile\":[  \n", sizeof(" \"ConfigFile\":[ \n"), &writeNum);    //依照标准格式进行写入
    
    for(uint16_t i = 0; i < length; i++)
    {
        cJSON* struct_json = ConfigFile_StructToJson(&g_ConfigFile[i]);
        
        string = vPort_Print_cJSON(struct_json);

        res = f_write(&FilePositon, string, strlen(string), &writeNum);    //依照标准格式进行写入
        
		if(string != NULL)
		{
			memset(string, 0, (strlen(string) + 1));
			s2jHook.free_fn(string);
		}
        if(i < (length - 1))
        {
            res = f_write(&FilePositon, ",\n", 2, &writeNum);    //依照标准格式进行写入
        }
        else
        {
            res = f_write(&FilePositon, "\n", 1, &writeNum);    //依照标准格式进行写入
        }
    }

    f_write(&FilePositon, "]\n", 2, &writeNum);    //依照标准格式进行写入

    res = f_write(&FilePositon, "}\n", 2, &writeNum);    //依照标准格式进行写入

	f_close(&FilePositon);
    
    return res;
}


/**
 * @fn Get_JsonFile
 * @brief 获取json文件，获得配置信息
 * @return 0    成功获得json文件
 *         1    目录中无json文件，使用默认配置
 *         2    其他错误
 */
uint8_t Get_JsonFile(void)
{
    //TERMINAL_PRODUCT_SERIAL_NUMBER
    char* string;
    FRESULT res = FR_OK;
    uint16_t fileSize = 0;
    cJSON * item;
    cJSON *readJson;
	cJSON * _item;

    res = f_open(&FilePositon, "/sojo/ConfigFile.json", FA_READ); //在sojo目录下创建一个可读写文件
    if(res != FR_OK)
    {
        goto JSON_RES;
    }
    //获取json文件
    fileSize = f_size(&FilePositon);   //获取文件大小
    string  = pvPortMalloc(fileSize);   //申请内存

    res = f_read(&FilePositon, string, fileSize, NULL);

    if(res != FR_OK)
    {
        goto JSON_RES;
    }

    readJson = vPort_Get_cJSON(string);
    if(NULL == readJson)
    {
        goto JSON_RES;
    }
	
	for (int i = 0; i < cJSON_GetArraySize(readJson); i++)   //遍历最外层json键值对
	{
		item = cJSON_GetArrayItem(readJson, i);
		if (cJSON_Object != item->type)		//值不为json对象则查找child是否为空，为空即不包含json
		{
			if (item->child != NULL)
			{
				for (int j = 0; j < cJSON_GetArraySize(item); j++)   //遍历外层json键值对
				{
					_item = cJSON_GetArrayItem(item, j);
					if (cJSON_Object == _item->type)
					{
						/* deserialize Student structure object */
						g_ConfigFile[j] = * ConfigFile_JsonToStruct(_item);
					}
				}
			}
		}
	}
    if(readJson != NULL)
	{
		s2j_delete_json_obj(readJson);
	}
    res = FR_OK;
    
JSON_RES:
	f_close(&FilePositon);
    vPortFree(string);  //释放内存
    return (uint8_t)res;
}



/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-09-29     huwenhuan    first version
 */
 
 
#include <rtthread.h>
//#include "sys.h"
 
#if defined(BSP_USING_SPI_FLASH)

//#include <dfs_elm.h>
//#include <dfs_fs.h>
//#include <dfs_posix.h>

//#define DBG_TAG "app.mnt"
//#define DBG_LVL DBG_INFO
//#include <rtdbg.h>

//#include "sys.h"


//extern int alarm_init(void);
//void sf_mount(void *parameter){
//	while(1){
//		if(rt_device_find("W25Q128") != RT_NULL)
//		{
//			if (dfs_mount("W25Q128", "/", "elm", 0, 0) == RT_EOK)
//			{
//					LOG_I("sf mount to '/'");
//					//读取config文件
//					if(sync_data_from_config_files()<0){
//						sync_data_to_config_files();
//					}
//					//读取para文件
//					if(sync_data_from_parameter_files()<0){
//						sync_para_to_para_files();
//					}
//					//读取wifi文件
////					extern int sync_wifi_info_form_files(void);
////					extern int sync_wifi_info_to_files(void);
////					int esp8266_device_register(void);
////					if(sync_wifi_info_form_files()<0){
////						sync_wifi_info_to_files();
////					}
//					alarm_init();
//					break;
//			}
//			else
//			{
//					LOG_W("sf mount to '/' failed!");
//				break;
//			}
//		}
//		rt_thread_delay(1000);			
//	}
//}

//int stm32_sf_mount(void)
//{
//    rt_thread_t tid;
//    tid = rt_thread_create("sf_mount", sf_mount, RT_NULL,
//                           2048, RT_THREAD_PRIORITY_MAX - 2, 20);
//    if (tid != RT_NULL)
//    {
//        rt_thread_startup(tid);
//    }
//    else
//    {
//        LOG_E("create sd_mount thread err!");
//    }
//    return RT_EOK;
//}
//INIT_APP_EXPORT(stm32_sf_mount);

#endif /*BSP_USING_SPI_FLASH*/



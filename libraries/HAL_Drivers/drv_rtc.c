/*
 * File      : drv_rtc.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009-2013 RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date			Author		Notes
 * 2017-04-05   lizhen9880  the first version for STM32F429
 */

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "drv_rtc.h"
#include <rtdevice.h>

#if defined(RT_USING_RTC)
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#ifdef RT_RTC_DEBUG
#define rtc_debug(format,args...) 			rt_kprintf(format, ##args)
#else
#define rtc_debug(format,args...)
#endif

/* Private variables ---------------------------------------------------------*/
static struct rt_device rtc;

RTC_HandleTypeDef RTC_Handler;  //RTC


rt_uint8_t RTC_Init(void)
{      
	RTC_Handler.Instance=RTC;
	RTC_Handler.Init.HourFormat=RTC_HOURFORMAT_24;
	RTC_Handler.Init.AsynchPrediv=0X7F;           //(1~0X7F)
	RTC_Handler.Init.SynchPrediv=0XFF;            //(0~7FFF)   
	RTC_Handler.Init.OutPut=RTC_OUTPUT_DISABLE;     
	RTC_Handler.Init.OutPutPolarity=RTC_OUTPUT_POLARITY_HIGH;
	RTC_Handler.Init.OutPutType=RTC_OUTPUT_TYPE_OPENDRAIN;
	if(HAL_RTC_Init(&RTC_Handler)!=HAL_OK) return 2;
		
	if(HAL_RTCEx_BKUPRead(&RTC_Handler,RTC_BKP_DR0)!=0X5050)
	{ 
			HAL_RTCEx_BKUPWrite(&RTC_Handler,RTC_BKP_DR0,0X5050);
	}
	return 0;
}

void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_PWR_EnableBkUpAccess();

	RCC_OscInitStruct.OscillatorType=RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.PLL.PLLState=RCC_PLL_NONE;
	RCC_OscInitStruct.LSEState=RCC_LSE_ON;                  
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	PeriphClkInitStruct.PeriphClockSelection=RCC_PERIPHCLK_RTC;
	PeriphClkInitStruct.RTCClockSelection=RCC_RTCCLKSOURCE_LSE;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
			
	__HAL_RCC_RTC_ENABLE();
}

void RTC_Set_AlarmA(rt_uint8_t week,rt_uint8_t hour,rt_uint8_t min,rt_uint8_t sec)
{ 
    RTC_AlarmTypeDef RTC_AlarmSturuct;
    
    RTC_AlarmSturuct.AlarmTime.Hours=hour;  
    RTC_AlarmSturuct.AlarmTime.Minutes=min; 
    RTC_AlarmSturuct.AlarmTime.Seconds=sec; 
    RTC_AlarmSturuct.AlarmTime.SubSeconds=0;
    RTC_AlarmSturuct.AlarmTime.TimeFormat=RTC_HOURFORMAT12_AM;
    
    RTC_AlarmSturuct.AlarmMask=RTC_ALARMMASK_DATEWEEKDAY;   
    RTC_AlarmSturuct.AlarmSubSecondMask=RTC_ALARMSUBSECONDMASK_NONE;
    RTC_AlarmSturuct.AlarmDateWeekDaySel=RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;
    RTC_AlarmSturuct.AlarmDateWeekDay=week; 
    RTC_AlarmSturuct.Alarm=RTC_ALARM_A;     
    HAL_RTC_SetAlarm_IT(&RTC_Handler,&RTC_AlarmSturuct,RTC_FORMAT_BIN);
    
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn,0x01,0x02); 
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}

//
/*wksel:  @ref RTCEx_Wakeup_Timer_Definitions
#define RTC_WAKEUPCLOCK_RTCCLK_DIV16        ((uint32_t)0x00000000)
#define RTC_WAKEUPCLOCK_RTCCLK_DIV8         ((uint32_t)0x00000001)
#define RTC_WAKEUPCLOCK_RTCCLK_DIV4         ((uint32_t)0x00000002)
#define RTC_WAKEUPCLOCK_RTCCLK_DIV2         ((uint32_t)0x00000003)
#define RTC_WAKEUPCLOCK_CK_SPRE_16BITS      ((uint32_t)0x00000004)
#define RTC_WAKEUPCLOCK_CK_SPRE_17BITS      ((uint32_t)0x00000006)
*/
//cnt:
void RTC_Set_WakeUp(rt_uint32_t wksel,rt_uint16_t cnt)
{ 
    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&RTC_Handler, RTC_FLAG_WUTF);
	
	HAL_RTCEx_SetWakeUpTimer_IT(&RTC_Handler,cnt,wksel);            
	
    HAL_NVIC_SetPriority(RTC_WKUP_IRQn,0x02,0x02); 
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
}


void RTC_Alarm_IRQHandler(void)
{
    HAL_RTC_AlarmIRQHandler(&RTC_Handler);
}
    
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
    rt_kprintf("ALARM A!\r\n");
    
#if defined(RT_USING_ALARM)
    rt_alarm_update(&rtc, NULL);
#endif
    
}

void RTC_WKUP_IRQHandler(void)
{
    HAL_RTCEx_WakeUpTimerIRQHandler(&RTC_Handler); 
}
time_t GetRTCTimeStamp(void)
{
    RTC_TimeTypeDef RTC_TimeStruct;
    RTC_DateTypeDef RTC_DateStruct;
    struct tm tm_new;
    
    HAL_RTC_GetTime(&RTC_Handler,&RTC_TimeStruct,RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&RTC_Handler,&RTC_DateStruct,RTC_FORMAT_BIN);
    tm_new.tm_sec  = RTC_TimeStruct.Seconds; 
    tm_new.tm_min  = RTC_TimeStruct.Minutes; 
    tm_new.tm_hour = RTC_TimeStruct.Hours;
    tm_new.tm_mday = RTC_DateStruct.Date;
    tm_new.tm_mon  = RTC_DateStruct.Month-1; 
    tm_new.tm_year = RTC_DateStruct.Year+100;
    return mktime(&tm_new);
  
}
rt_err_t SetRTCTimeStamp(time_t time_stamp)
{
    RTC_TimeTypeDef RTC_TimeStruct;
    RTC_DateTypeDef RTC_DateStruct;
    struct tm *p_tm;
    p_tm = localtime(&time_stamp);
    if(p_tm->tm_year<100)
    {
        return RT_ERROR;
    }
    RTC_TimeStruct.Seconds = p_tm->tm_sec ; 
    RTC_TimeStruct.Minutes = p_tm->tm_min ; 
    RTC_TimeStruct.Hours   = p_tm->tm_hour;
    RTC_DateStruct.Date    = p_tm->tm_mday;
    RTC_DateStruct.Month   = p_tm->tm_mon+1 ; 
    RTC_DateStruct.Year    = p_tm->tm_year-100;
    RTC_DateStruct.WeekDay = p_tm->tm_wday+1;
    HAL_RTC_SetTime(&RTC_Handler,&RTC_TimeStruct,RTC_FORMAT_BIN);
    HAL_RTC_SetDate(&RTC_Handler,&RTC_DateStruct,RTC_FORMAT_BIN);
  
    return RT_EOK;
}
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static rt_err_t rt_rtc_open(rt_device_t dev, rt_uint16_t oflag)
{
    if (dev->rx_indicate != RT_NULL)
    {
        /* Open Interrupt */
    }

    return RT_EOK;
}

static rt_size_t rt_rtc_read(
	rt_device_t 	dev,
	rt_off_t 		pos,
	void* 			buffer,
	rt_size_t 		size)
{
    
    return 0;
}

/***************************************************************************//**
 * @brief
 *  Configure RTC device
 *
 * @details
 *
 * @note
 *
 * @param[in] dev
 *  Pointer to device descriptor
 *
 * @param[in] cmd
 *  RTC control command
 *
 * @param[in] args
 *  Arguments
 *
 * @return
 *  Error code
 ******************************************************************************/
static rt_err_t rt_rtc_control(rt_device_t dev, int cmd, void *args)
{
    rt_err_t result;
    RT_ASSERT(dev != RT_NULL);
    switch (cmd)
    {
        case RT_DEVICE_CTRL_RTC_GET_TIME:
					{
            *(rt_uint32_t *)args = GetRTCTimeStamp();
            rtc_debug("RTC: get rtc_time %x\n", *(rt_uint32_t *)args);
					}
        break;
        case RT_DEVICE_CTRL_RTC_SET_TIME:
					{
            result = SetRTCTimeStamp(*(rt_uint32_t *)args);
            rtc_debug("RTC: set rtc_time %x\n", *(rt_uint32_t *)args);
					}
        break;
        case RT_DEVICE_CTRL_RTC_SET_ALARM:
					{
            struct rt_rtc_wkalarm *wkalarm;
            wkalarm = (struct rt_rtc_wkalarm *)args;
            RTC_Set_AlarmA(NULL, wkalarm->tm_hour, wkalarm->tm_min, wkalarm->tm_sec);
					}
        break;
    }
    return result;
}



/***************************************************************************//**
 * @brief
 *  Register RTC device
 *
 * @details
 *
 * @note
 *
 * @param[in] device
 *  Pointer to device descriptor
 *
 * @param[in] name
 *  Device name
 *
 * @param[in] flag
 *  Configuration flags
 *
 * @return
 *  Error code
 ******************************************************************************/
rt_err_t rt_hw_rtc_register(
	rt_device_t		device,
	const char		*name,
	rt_uint32_t		flag)
{
	RT_ASSERT(device != RT_NULL);

	device->type 		= RT_Device_Class_RTC;
	device->rx_indicate = RT_NULL;
	device->tx_complete = RT_NULL;
	device->init 		= RT_NULL;
	device->open		= rt_rtc_open;
	device->close		= RT_NULL;
	device->read 		= rt_rtc_read;
	device->write 		= RT_NULL;
	device->control 	= rt_rtc_control;
	device->user_data	= RT_NULL; /* no private */

	/* register a character device */
	return rt_device_register(device, name, RT_DEVICE_FLAG_RDWR | flag);
}


/***************************************************************************//**
 * @brief
 *  Initialize all RTC module related hardware and register RTC device to kernel
 *
 * @details
 *
 * @note
 ******************************************************************************/
int  rt_hw_rtc_init(void)
{
    RTC_Init();
    /* register rtc device */
		rt_hw_rtc_register(&rtc, "rtc", 0);
    return RT_EOK;
}
INIT_BOARD_EXPORT(rt_hw_rtc_init);
#endif

/***************************************************************************//**
 * @}
 ******************************************************************************/


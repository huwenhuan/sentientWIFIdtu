///*
// * File      : drv_rtc.h
// * This file is part of RT-Thread RTOS
// * COPYRIGHT (C) 2015, RT-Thread Development Team
// *
// *  This program is free software; you can redistribute it and/or modify
// *  it under the terms of the GNU General Public License as published by
// *  the Free Software Foundation; either version 2 of the License, or
// *  (at your option) any later version.
// *
// *  This program is distributed in the hope that it will be useful,
// *  but WITHOUT ANY WARRANTY; without even the implied warranty of
// *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// *  GNU General Public License for more details.
// *
// *  You should have received a copy of the GNU General Public License along
// *  with this program; if not, write to the Free Software Foundation, Inc.,
// *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
// *
// * Change Logs:
// * Date           Author       Notes
// * 2017-10-25     ZYH      first implementation
// */
// 
//#ifndef __DRV_RTC_H__
//#define __DRV_RTC_H__
//extern int rt_hw_rtc_init(void);
//#endif
 /***************************************************************************//**
 * @file    drv_rtc.h
 * @brief   RTC driver of RT-Thread RTOS for EFM32
 *  COPYRIGHT (C) 2012, RT-Thread Development Team
 * @author  Bernard, onelife
 * @version 1.0
 *******************************************************************************
 * @section License
 * The license and distribution terms for this file may be found in the file
 *  LICENSE in this distribution or at http://www.rt-thread.org/license/LICENSE
 *******************************************************************************
 * @section Change Logs
 * Date			Author		Notes
 * 2009-01-05   Bernard     the first version
 * 2010-12-27   onelife     Modification for EFM32
 *********************************************************************/
#ifndef __DRV_RTC_H__
#define __DRV_RTC_H__
#include <time.h>
#include <string.h>
/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int  rt_hw_rtc_init(void);

#endif /* __DRV_RTC_H__ */

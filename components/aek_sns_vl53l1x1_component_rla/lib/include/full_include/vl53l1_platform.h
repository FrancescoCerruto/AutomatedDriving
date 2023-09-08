/****************************************************************************
*
* Copyright � 2020 STMicroelectronics - All Rights Reserved
*
* License terms: STMicroelectronics Proprietary in accordance with licensing
* terms SLA0098 at www.st.com.
*
* THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
* INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
* EVALUATION ONLY � NOT FOR USE IN PRODUCTION
*
*****************************************************************************/

/**
 * @file    vl53l1_platform.h
 * @brief   SPC5xx vl53l1_platform.h header file.
 *
 * @addtogroup vl53l1_platform
 * @{
 */

#ifndef _VL53L1_PLATFORM_H_
#define _VL53L1_PLATFORM_H_


#include <i2c_lld_cfg.h>
#include "vl53l1_ll_def.h"
#include "vl53l1_platform_log.h"

#define VL53L1_IPP_API
#include "vl53l1_platform_user_data.h"
/*debug*/
#include "serial_lld_cfg.h"

#ifdef __cplusplus
extern "C"
{
#endif
extern VL53L1_Error status;
VL53L1_Error VL53L1_CommsInitialise(
	VL53L1_Dev_t *pdev,
	uint8_t       comms_type,
	uint16_t      comms_speed_khz);

VL53L1_Error VL53L1_CommsClose(
	VL53L1_Dev_t *pdev);

VL53L1_Error VL53L1_WriteMulti(VL53L1_DEV dev, uint16_t reg_addr, uint8_t *data,uint8_t size);

VL53L1_Error VL53L1_ReadMulti(VL53L1_DEV dev, uint16_t reg_addr, uint8_t *data,uint8_t size);

VL53L1_Error VL53L1_WrByte(
		VL53L1_Dev_t *pdev,
		uint16_t      index,
		uint8_t       VL53L1_PRM_00005);

VL53L1_Error VL53L1_WrWord(
		VL53L1_Dev_t *pdev,
		uint16_t      index,
		uint16_t      VL53L1_PRM_00005);

VL53L1_Error VL53L1_WrDWord(
		VL53L1_Dev_t *pdev,
		uint16_t      index,
		uint32_t      VL53L1_PRM_00005);

VL53L1_Error VL53L1_RdByte(
		VL53L1_Dev_t *pdev,
		uint16_t      index,
		uint8_t      *pdata);

VL53L1_Error VL53L1_RdWord(
		VL53L1_Dev_t *pdev,
		uint16_t      index,
		uint16_t     *pdata);

VL53L1_Error VL53L1_RdDWord(
		VL53L1_Dev_t *pdev,
		uint16_t      index,
		uint32_t     *pdata);

VL53L1_Error VL53L1_WaitUs(
		VL53L1_Dev_t *pdev,
		int32_t       wait_us);

VL53L1_Error VL53L1_WaitMs(
		VL53L1_Dev_t *pdev,
		int32_t       wait_ms);

VL53L1_Error VL53L1_GetTimerFrequency(int32_t *ptimer_freq_hz);

VL53L1_Error VL53L1_GetTimerValue(int32_t *ptimer_count);

VL53L1_Error VL53L1_GpioSetMode(uint8_t pin, uint8_t mode);

VL53L1_Error VL53L1_GpioSetValue(uint8_t pin, uint8_t value);

VL53L1_Error VL53L1_GpioGetValue(uint8_t pin, uint8_t *pvalue);

VL53L1_Error VL53L1_GpioXshutdown(uint8_t value);

VL53L1_Error VL53L1_GpioCommsSelect(uint8_t value);

VL53L1_Error VL53L1_GpioPowerEnable(uint8_t value);

VL53L1_Error  VL53L1_GpioInterruptEnable(void (*function)(void), uint8_t edge_type);

VL53L1_Error  VL53L1_GpioInterruptDisable(void);

VL53L1_Error VL53L1_GetTickCount(
	uint32_t *ptime_ms);

VL53L1_Error VL53L1_WaitValueMaskEx(
		VL53L1_Dev_t *pdev,
		uint32_t      timeout_ms,
		uint16_t      index,
		uint8_t       value,
		uint8_t       mask,
		uint32_t      poll_delay_ms);

#ifdef __cplusplus
}
#endif

#endif/*_VL53L1_PLATFORM_H_*/
/** @} */

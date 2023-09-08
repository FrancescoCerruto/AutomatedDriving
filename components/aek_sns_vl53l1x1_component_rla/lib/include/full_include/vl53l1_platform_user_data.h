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
 * @file    vl53l1_platform_user_data.h
 * @brief   SPC5xx vl53l1_platform_user_data.h header file.
 *
 * @addtogroup vl53l1_platform_user_data
 * @{
 */

#ifndef _VL53L1_PLATFORM_USER_DATA_H_
#define _VL53L1_PLATFORM_USER_DATA_H_

#include "vl53l1_def.h"
#ifdef __cplusplus
extern "C"
{
#endif


typedef struct {
    uint32_t dummy;
} I2C_HandleTypeDef;

typedef struct {

	VL53L1_DevData_t   Data;

	uint8_t   I2cDevAddr;
	uint8_t   comms_type;
	uint16_t  comms_speed_khz;
	uint32_t  new_data_ready_poll_duration_ms;
	I2C_HandleTypeDef *I2cHandle;

} VL53L1_Dev_t;

typedef VL53L1_Dev_t *VL53L1_DEV;

#define VL53L1DevDataGet(Dev, field) (Dev->Data.field)
#define VL53L1DevDataSet(Dev, field, VL53L1_PRM_00005) ((Dev->Data.field) = (VL53L1_PRM_00005))
#define VL53L1DevStructGetLLDriverHandle(Dev) (&Dev->Data.LLData)
#define VL53L1DevStructGetLLResultsHandle(Dev) (&Dev->Data.llresults)
#ifdef __cplusplus
}
#endif

#endif /*_VL53L1_PLATFORM_USER_DATA_H_*/
/** @} */

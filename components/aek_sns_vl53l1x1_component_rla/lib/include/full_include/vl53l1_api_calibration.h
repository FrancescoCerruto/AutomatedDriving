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
 * @file    vl53l1_api_calibration.h
 * @brief   SPC5xx vl53l1_api_calibration.h header file.
 *
 * @addtogroup vl53l1_api_calibration
 * @{
 */

#ifndef _VL53L1_API_CALIBRATION_H_
#define _VL53L1_API_CALIBRATION_H_

#include "vl53l1_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef VL53L1_NOCALIB
VL53L1_Error VL53L1_run_ref_spad_char(VL53L1_DEV Dev, VL53L1_Error *pcal_status);
#endif

#ifndef VL53L1_NOCALIB
VL53L1_Error VL53L1_run_device_test(
	VL53L1_DEV                 Dev,
	VL53L1_DeviceTestMode      device_test_mode);
#endif

#ifndef VL53L1_NOCALIB
VL53L1_Error VL53L1_run_spad_rate_map(
	VL53L1_DEV                 Dev,
	VL53L1_DeviceTestMode      device_test_mode,
	VL53L1_DeviceSscArray      array_select,
	uint32_t                   ssc_config_timeout_us,
	VL53L1_spad_rate_data_t   *pspad_rate_data);
#endif

#ifndef VL53L1_NOCALIB
VL53L1_Error   VL53L1_run_offset_calibration(
	VL53L1_DEV	                  Dev,
	int16_t                       cal_distance_mm,
	VL53L1_Error                 *pcal_status);
#endif


#ifdef __cplusplus
}
#endif


#endif /* _VL53L1_API_CALIBRATION_H_ */
/** @} */


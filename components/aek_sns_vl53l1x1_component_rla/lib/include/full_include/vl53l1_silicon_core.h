/****************************************************************************
*
* Copyright © 2020 STMicroelectronics - All Rights Reserved
*
* License terms: STMicroelectronics Proprietary in accordance with licensing
* terms SLA0098 at www.st.com.
*
* THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
* INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
* EVALUATION ONLY – NOT FOR USE IN PRODUCTION
*
*****************************************************************************/

/**
 * @file    vl53l1_silicon_core.h
 * @brief   SPC5xx vl53l1_silicon_core.h header file.
 *
 * @addtogroup vl53l1_silicon_core
 * @{
 */

#ifndef _VL53L1_SILICON_CORE_H_
#define _VL53L1_SILICON_CORE_H_

#include "vl53l1_platform.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief  Checks if the firmware is ready for ranging (Silicon variant)
 *
 * @param[in]   Dev           : Device Handle
 * @param[out]  pready        : pointer to data ready flag \n
 *                                 0 = firmware not ready \n
 *                                 1 = firmware ready
 *
 * @return  VL53L1_ERROR_NONE     Success
 * @return  "Other error code"    See ::VL53L1_Error
 */

VL53L1_Error VL53L1_is_firmware_ready_silicon(
	VL53L1_DEV      Dev,
	uint8_t        *pready);


#ifdef __cplusplus
}
#endif

#endif /* _VL53L1_SILICON_CORE_H_ */


 /** @} */

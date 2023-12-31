
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
 * @file    vl53l1_silicon_core.c
 * @brief   SPC5xx vl53l1_silicon_core.c driver code.
 *
 * @addtogroup vl53l1_silicon_core
 * @{
 */



#include "full_include/vl53l1_ll_def.h"
#include "full_include/vl53l1_platform.h"
#include "full_include/vl53l1_register_map.h"
#include "full_include/vl53l1_core.h"
#include "full_include/vl53l1_silicon_core.h"


#define LOG_FUNCTION_START(fmt, ...) \
	_LOG_FUNCTION_START(VL53L1_TRACE_MODULE_CORE, fmt, ##__VA_ARGS__)
#define LOG_FUNCTION_END(status, ...) \
	_LOG_FUNCTION_END(VL53L1_TRACE_MODULE_CORE, status, ##__VA_ARGS__)
#define LOG_FUNCTION_END_FMT(status, fmt, ...) \
	_LOG_FUNCTION_END_FMT(VL53L1_TRACE_MODULE_CORE, status, fmt, ##__VA_ARGS__)


VL53L1_Error VL53L1_is_firmware_ready_silicon(
	VL53L1_DEV     Dev,
	uint8_t       *pready)
{
	/**
	 * Determines if the firmware is ready to range
	 *
	 * There are 2 different behaviors depending on whether
	 * power force is enabled or not
	 */

	VL53L1_Error status = VL53L1_ERROR_NONE;
	VL53L1_LLDriverData_t *pdev = VL53L1DevStructGetLLDriverHandle(Dev);

	uint8_t  comms_buffer[5];

	//LOG_FUNCTION_START("");

	/* read interrupt and power force reset status */

	status = VL53L1_ReadMulti(
					Dev,
					VL53L1_INTERRUPT_MANAGER__ENABLES,
					comms_buffer,
					5);

	if (status == VL53L1_ERROR_NONE) {

		pdev->dbg_results.interrupt_manager__enables =
				comms_buffer[0];
		pdev->dbg_results.interrupt_manager__clear =
				comms_buffer[1];
		pdev->dbg_results.interrupt_manager__status =
				comms_buffer[2];
		pdev->dbg_results.mcu_to_host_bank__wr_access_en =
				comms_buffer[3];
		pdev->dbg_results.power_management__go1_reset_status =
				comms_buffer[4];

		if ((pdev->sys_ctrl.power_management__go1_power_force & 0x01) == 0x01) {

				if (((pdev->dbg_results.interrupt_manager__enables & 0x1F) == 0x1F) &&
					((pdev->dbg_results.interrupt_manager__clear   & 0x1F) == 0x1F))
					*pready = 0x01;
				else
					*pready = 0x00;

		} else {

			/* set ready flag if bit 0 is zero i.g G01 is in reset */
			if ((pdev->dbg_results.power_management__go1_reset_status & 0x01) == 0x00)
				*pready = 0x01;
			else
				*pready = 0x00;
		}

	}

	LOG_FUNCTION_END(status);

	return status;
}

 /** @} */


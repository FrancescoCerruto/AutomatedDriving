
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
 * @file    vl53l1_wait.c
 * @brief   SPC5xx vl53l1_wait.c driver code.
 *
 * @addtogroup vl53l1_wait
 * @{
 */

#include "full_include/vl53l1_ll_def.h"
#include "full_include/vl53l1_ll_device.h"
#include "full_include/vl53l1_platform.h"
#include "full_include/vl53l1_core.h"
#include "full_include/vl53l1_silicon_core.h"
#include "full_include/vl53l1_wait.h"
#include "full_include/vl53l1_register_settings.h"
#include "full_include/vl53l1_register_map.h"

#define LOG_FUNCTION_START(fmt, ...) \
	_LOG_FUNCTION_START(VL53L1_TRACE_MODULE_CORE, fmt, ##__VA_ARGS__)
#define LOG_FUNCTION_END(status, ...) \
	_LOG_FUNCTION_END(VL53L1_TRACE_MODULE_CORE, status, ##__VA_ARGS__)
#define LOG_FUNCTION_END_FMT(status, fmt, ...) \
	_LOG_FUNCTION_END_FMT(VL53L1_TRACE_MODULE_CORE, status, \
		fmt, ##__VA_ARGS__)


VL53L1_Error VL53L1_wait_for_boot_completion(
	VL53L1_DEV     Dev)
{

	/* Waits for firmware boot to finish
	 */

	VL53L1_Error status = VL53L1_ERROR_NONE;
	VL53L1_LLDriverData_t *pdev = VL53L1DevStructGetLLDriverHandle(Dev);

	uint8_t      fw_ready  = 0;

	LOG_FUNCTION_START("");

	if (pdev->wait_method == VL53L1_WAIT_METHOD_BLOCKING) {

		/* blocking version */

		status =
			VL53L1_poll_for_boot_completion(
				Dev,
				VL53L1_BOOT_COMPLETION_POLLING_TIMEOUT_MS);

	} else {

		/* implement non blocking version below */

		fw_ready = 0;
		while (fw_ready == 0x00 && status == VL53L1_ERROR_NONE) {
			status = VL53L1_is_boot_complete(
				Dev,
				&fw_ready);

			if (status == VL53L1_ERROR_NONE) {
				status = VL53L1_WaitMs(
					Dev,
					VL53L1_POLLING_DELAY_MS);
			}
		}
	}

	LOG_FUNCTION_END(status);

	return status;

}


VL53L1_Error VL53L1_wait_for_firmware_ready(
	VL53L1_DEV     Dev)
{

	/* If in timed mode or single shot then check firmware is ready
	 * before sending handshake
	 */

	VL53L1_Error status = VL53L1_ERROR_NONE;
	VL53L1_LLDriverData_t *pdev = VL53L1DevStructGetLLDriverHandle(Dev);

	uint8_t      fw_ready  = 0;
	uint8_t      mode_start  = 0;

	LOG_FUNCTION_START("");

	/* Filter out tje measure mode part of the mode
	 * start register
	 */
	mode_start =
		pdev->sys_ctrl.system__mode_start &
		VL53L1_DEVICEMEASUREMENTMODE_MODE_MASK;

	/*
	 * conditional wait for firmware ready
	 * only waits for timed and single shot modes
	 */

	if ((mode_start == VL53L1_DEVICEMEASUREMENTMODE_TIMED) ||
		(mode_start == VL53L1_DEVICEMEASUREMENTMODE_SINGLESHOT)) {

		if (pdev->wait_method == VL53L1_WAIT_METHOD_BLOCKING) {

			/* blocking version */

			status =
				VL53L1_poll_for_firmware_ready(
					Dev,
					VL53L1_RANGE_COMPLETION_POLLING_TIMEOUT_MS);

		} else {

			/* implement non blocking version below */

			fw_ready = 0;
			while (fw_ready == 0x00 && status == VL53L1_ERROR_NONE) {
				status = VL53L1_is_firmware_ready(
					Dev,
					&fw_ready);

				if (status == VL53L1_ERROR_NONE) {
					status = VL53L1_WaitMs(
						Dev,
						VL53L1_POLLING_DELAY_MS);
				}
			}
		}
	}

	LOG_FUNCTION_END(status);

	return status;
}


VL53L1_Error VL53L1_wait_for_range_completion(
	VL53L1_DEV     Dev)
{

	/* Wrapper function for waiting for range completion
	 */

	VL53L1_Error status = VL53L1_ERROR_NONE;
	VL53L1_LLDriverData_t *pdev = VL53L1DevStructGetLLDriverHandle(Dev);

	uint8_t      data_ready  = 0;

	LOG_FUNCTION_START("");

	if (pdev->wait_method == VL53L1_WAIT_METHOD_BLOCKING) {

		/* blocking version */

		status =
			VL53L1_poll_for_range_completion(
				Dev,
				VL53L1_RANGE_COMPLETION_POLLING_TIMEOUT_MS);

	} else {

		/* implement non blocking version below */

		data_ready = 0;
		while (data_ready == 0x00 && status == VL53L1_ERROR_NONE) {
			status = VL53L1_is_new_data_ready(
				Dev,
				&data_ready);

			if (status == VL53L1_ERROR_NONE) {
				status = VL53L1_WaitMs(
					Dev,
					VL53L1_POLLING_DELAY_MS);
			}
		}
	}

	LOG_FUNCTION_END(status);

	return status;
}


VL53L1_Error VL53L1_wait_for_test_completion(
	VL53L1_DEV     Dev)
{

	/* Wrapper function for waiting for test mode completion
	 */

	VL53L1_Error status = VL53L1_ERROR_NONE;
	VL53L1_LLDriverData_t *pdev = VL53L1DevStructGetLLDriverHandle(Dev);

	uint8_t      data_ready  = 0;

	LOG_FUNCTION_START("");

	if (pdev->wait_method == VL53L1_WAIT_METHOD_BLOCKING) {

		/* blocking version */

		status =
			VL53L1_poll_for_range_completion(
				Dev,
				VL53L1_TEST_COMPLETION_POLLING_TIMEOUT_MS);

	} else {

		/* implement non blocking version below */

		data_ready = 0;
		while (data_ready == 0x00 && status == VL53L1_ERROR_NONE) {
			status = VL53L1_is_new_data_ready(
				Dev,
				&data_ready);

			if (status == VL53L1_ERROR_NONE) {
				status = VL53L1_WaitMs(
					Dev,
					VL53L1_POLLING_DELAY_MS);
			}
		}
	}

	LOG_FUNCTION_END(status);

	return status;
}




VL53L1_Error VL53L1_is_boot_complete(
	VL53L1_DEV     Dev,
	uint8_t       *pready)
{
	/**
	 * Determines if the firmware finished booting by reading
	 * bit 0 of firmware__system_status register
	 */

	VL53L1_Error status = VL53L1_ERROR_NONE;
	uint8_t  firmware__system_status = 0;

	LOG_FUNCTION_START("");

	/* read current range interrupt state */

	status =
		VL53L1_RdByte(
			Dev,
			VL53L1_FIRMWARE__SYSTEM_STATUS,
			&firmware__system_status);

	/* set *pready = 1 if new range data ready complete
	 * zero otherwise
	 */

	if ((firmware__system_status & 0x01) == 0x01) {
		*pready = 0x01;
		VL53L1_init_ll_driver_state(
			Dev,
			VL53L1_DEVICESTATE_SW_STANDBY);
	} else {
		*pready = 0x00;
		VL53L1_init_ll_driver_state(
			Dev,
			VL53L1_DEVICESTATE_FW_COLDBOOT);
	}

	LOG_FUNCTION_END(status);

	return status;
}


VL53L1_Error VL53L1_is_firmware_ready(
	VL53L1_DEV     Dev,
	uint8_t       *pready)
{
	/**
	 * Determines if the firmware is ready to range
	 */

	VL53L1_Error status = VL53L1_ERROR_NONE;
	VL53L1_LLDriverData_t *pdev = VL53L1DevStructGetLLDriverHandle(Dev);

	LOG_FUNCTION_START("");

	status = VL53L1_is_firmware_ready_silicon(
					Dev,
					pready);

	pdev->fw_ready = *pready;

	LOG_FUNCTION_END(status);

	return status;
}


VL53L1_Error VL53L1_is_new_data_ready(
	VL53L1_DEV     Dev,
	uint8_t       *pready)
{
	/**
	 * Determines if new range data is ready by reading bit 0 of
	 * VL53L1_GPIO__TIO_HV_STATUS to determine the current state
	 * of output interrupt pin
	 */

	VL53L1_Error status = VL53L1_ERROR_NONE;
	VL53L1_LLDriverData_t *pdev = VL53L1DevStructGetLLDriverHandle(Dev);

	uint8_t  gpio__mux_active_high_hv = 0;
	uint8_t  gpio__tio_hv_status      = 0;
	uint8_t  interrupt_ready          = 0;

	LOG_FUNCTION_START("");

	gpio__mux_active_high_hv =
			pdev->stat_cfg.gpio_hv_mux__ctrl &
			VL53L1_DEVICEINTERRUPTLEVEL_ACTIVE_MASK;

	if (gpio__mux_active_high_hv == VL53L1_DEVICEINTERRUPTLEVEL_ACTIVE_HIGH)
		interrupt_ready = 0x01;
	else
		interrupt_ready = 0x00;

	/* read current range interrupt state */

	status = VL53L1_RdByte(
					Dev,
					VL53L1_GPIO__TIO_HV_STATUS,
					&gpio__tio_hv_status);

	/* set *pready = 1 if new range data ready complete zero otherwise */

	if ((gpio__tio_hv_status & 0x01) == interrupt_ready)
		*pready = 0x01;
	else
		*pready = 0x00;

	LOG_FUNCTION_END(status);

	return status;
}




VL53L1_Error VL53L1_poll_for_boot_completion(
	VL53L1_DEV    Dev,
	uint32_t      timeout_ms)
{
	/**
	 * Polls the bit 0 of the FIRMWARE__SYSTEM_STATUS register to see if
	 * the firmware is ready.
	 */

	VL53L1_Error status       = VL53L1_ERROR_NONE;

	LOG_FUNCTION_START("");

	/* after reset for the firmware blocks I2C access while
	 * it copies the NVM data into the G02 host register banks
	 * The host must wait the required time to allow the copy
	 * to complete before attempting to read the firmware status
	 */

	status = VL53L1_WaitUs(
			Dev,
			VL53L1_FIRMWARE_BOOT_TIME_US);

	if (status == VL53L1_ERROR_NONE)
		status =
			VL53L1_WaitValueMaskEx(
				Dev,
				timeout_ms,
				VL53L1_FIRMWARE__SYSTEM_STATUS,
				0x01,
				0x01,
				VL53L1_POLLING_DELAY_MS);

	if (status == VL53L1_ERROR_NONE)
		VL53L1_init_ll_driver_state(Dev, VL53L1_DEVICESTATE_SW_STANDBY);

	LOG_FUNCTION_END(status);

	return status;
}


VL53L1_Error VL53L1_poll_for_firmware_ready(
	VL53L1_DEV    Dev,
	uint32_t      timeout_ms)
{
	/**
	 * Polls the bit 0 of the FIRMWARE__SYSTEM_STATUS register to see if
	 * the firmware is ready.
	 */

	VL53L1_Error status          = VL53L1_ERROR_NONE;
	VL53L1_LLDriverData_t *pdev = VL53L1DevStructGetLLDriverHandle(Dev);

	uint32_t     start_time_ms   = 0;
	uint32_t     current_time_ms = 0;
	int32_t      poll_delay_ms   = VL53L1_POLLING_DELAY_MS;
	uint8_t      fw_ready        = 0;

	/* calculate time limit in absolute time */

	VL53L1_GetTickCount(&start_time_ms); /*lint !e534 ignoring return*/
	pdev->fw_ready_poll_duration_ms = 0;

	/* wait until firmware is ready, timeout reached on error occurred */

	while ((status == VL53L1_ERROR_NONE) &&
		   (pdev->fw_ready_poll_duration_ms < timeout_ms) &&
		   (fw_ready == 0)) {

		status = VL53L1_is_firmware_ready(
			Dev,
			&fw_ready);

		if (status == VL53L1_ERROR_NONE &&
			fw_ready == 0 &&
			poll_delay_ms > 0) {
			status = VL53L1_WaitMs(
				Dev,
				poll_delay_ms);
		}

		/*
		 * Update polling time (Compare difference rather than
		 * absolute to negate 32bit wrap around issue)
		 */
		VL53L1_GetTickCount(&current_time_ms);  /*lint !e534 ignoring return*/
		pdev->fw_ready_poll_duration_ms =
				current_time_ms - start_time_ms;
	}

	if (fw_ready == 0 && status == VL53L1_ERROR_NONE)
		status = VL53L1_ERROR_TIME_OUT;

	LOG_FUNCTION_END(status);

	return status;
}


VL53L1_Error VL53L1_poll_for_range_completion(
	VL53L1_DEV     Dev,
	uint32_t       timeout_ms)
{
	/**
	 * Polls bit 0 of VL53L1_GPIO__TIO_HV_STATUS to determine
	 * the state of output interrupt pin
	 *
	 * Interrupt may be either active high or active low. Use active_high to
	 * select the required level check
	 */

	VL53L1_Error status = VL53L1_ERROR_NONE;
	VL53L1_LLDriverData_t *pdev = VL53L1DevStructGetLLDriverHandle(Dev);

	uint8_t  gpio__mux_active_high_hv = 0;
	uint8_t  interrupt_ready          = 0;

	LOG_FUNCTION_START("");

	gpio__mux_active_high_hv =
			pdev->stat_cfg.gpio_hv_mux__ctrl &
			VL53L1_DEVICEINTERRUPTLEVEL_ACTIVE_MASK;

	if (gpio__mux_active_high_hv == VL53L1_DEVICEINTERRUPTLEVEL_ACTIVE_HIGH)
		interrupt_ready = 0x01;
	else
		interrupt_ready = 0x00;

	status =
		VL53L1_WaitValueMaskEx(
			Dev,
			timeout_ms,
			VL53L1_GPIO__TIO_HV_STATUS,
			interrupt_ready,
			0x01,
			VL53L1_POLLING_DELAY_MS);

	LOG_FUNCTION_END(status);

	return status;
}

 /** @} */


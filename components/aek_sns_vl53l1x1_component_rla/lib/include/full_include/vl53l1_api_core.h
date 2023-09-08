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
 * @file    vl53l1_api_core.h
 * @brief   SPC5xx vl53l1_api_core.h header file.
 *
 * @addtogroup vl53l1_api_core
 * @{
 */

#ifndef _VL53L1_API_CORE_H_
#define _VL53L1_API_CORE_H_


#include "vl53l1_platform.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Get LL Driver version information
 *
 * @param[in]   Dev       : Device handle
 * @param[out]  pversion  : pointer to VL53L1_ll_version_t
 *
 * @return  VL53L1_ERROR_NONE     Success
 */

#ifdef VL53L1_DEBUG
VL53L1_Error VL53L1_get_version(
	VL53L1_DEV            Dev,
	VL53L1_ll_version_t  *pversion);

/**
 * @brief Gets Device Firmware version
 *
 * @param[in]   Dev           : Device handle
 * @param[out]  pfw_version   : pointer to uint16_t FW version
 *
 * @return  VL53L1_ERROR_NONE     Success
 */

VL53L1_Error VL53L1_get_device_firmware_version(
	VL53L1_DEV         Dev,
	uint16_t          *pfw_version);
#endif


VL53L1_Error VL53L1_data_init(
	VL53L1_DEV         Dev,
	uint8_t            read_p2p_data);


VL53L1_Error VL53L1_read_p2p_data(
	VL53L1_DEV      Dev);


VL53L1_Error VL53L1_software_reset(
	VL53L1_DEV      Dev);


VL53L1_Error VL53L1_set_part_to_part_data(
	VL53L1_DEV                            Dev,
	VL53L1_calibration_data_t            *pcal_data);



VL53L1_Error VL53L1_get_part_to_part_data(
	VL53L1_DEV                            Dev,
	VL53L1_calibration_data_t            *pcal_data);


#ifdef VL53L1_DEBUG
VL53L1_Error VL53L1_get_tuning_debug_data(
	VL53L1_DEV                            Dev,
	VL53L1_tuning_parameters_t            *ptun_data);
#endif



VL53L1_Error VL53L1_set_inter_measurement_period_ms(
	VL53L1_DEV          Dev,
	uint32_t            inter_measurement_period_ms);



VL53L1_Error VL53L1_get_inter_measurement_period_ms(
	VL53L1_DEV          Dev,
	uint32_t           *pinter_measurement_period_ms);


VL53L1_Error VL53L1_set_timeouts_us(
	VL53L1_DEV          Dev,
	uint32_t            phasecal_config_timeout_us,
	uint32_t            mm_config_timeout_us,
	uint32_t            range_config_timeout_us);



VL53L1_Error VL53L1_get_timeouts_us(
	VL53L1_DEV          Dev,
	uint32_t           *pphasecal_config_timeout_us,
	uint32_t           *pmm_config_timeout_us,
	uint32_t           *prange_config_timeout_us);


VL53L1_Error VL53L1_set_calibration_repeat_period(
	VL53L1_DEV          Dev,
	uint16_t            cal_config__repeat_period);



VL53L1_Error VL53L1_get_calibration_repeat_period(
	VL53L1_DEV          Dev,
	uint16_t           *pcal_config__repeat_period);



VL53L1_Error VL53L1_set_sequence_config_bit(
	VL53L1_DEV                   Dev,
	VL53L1_DeviceSequenceConfig  bit_id,
	uint8_t                      value);




VL53L1_Error VL53L1_get_sequence_config_bit(
	VL53L1_DEV                   Dev,
	VL53L1_DeviceSequenceConfig  bit_id,
	uint8_t                     *pvalue);


VL53L1_Error VL53L1_set_interrupt_polarity(
	VL53L1_DEV                       Dev,
	VL53L1_DeviceInterruptPolarity  interrupt_polarity);



VL53L1_Error VL53L1_get_interrupt_polarity(
	VL53L1_DEV                      Dev,
	VL53L1_DeviceInterruptPolarity  *pinterrupt_polarity);


#ifndef VL53L1_NOCALIB
VL53L1_Error VL53L1_get_refspadchar_config_struct(
	VL53L1_DEV                     Dev,
	VL53L1_refspadchar_config_t   *pdata);
#endif



#ifndef VL53L1_NOCALIB
VL53L1_Error VL53L1_set_refspadchar_config_struct(
	VL53L1_DEV                     Dev,
	VL53L1_refspadchar_config_t   *pdata);
#endif


VL53L1_Error VL53L1_set_range_ignore_threshold(
	VL53L1_DEV              Dev,
	uint8_t                 range_ignore_thresh_mult,
	uint16_t                range_ignore_threshold_mcps);



VL53L1_Error VL53L1_get_range_ignore_threshold(
	VL53L1_DEV              Dev,
	uint8_t                *prange_ignore_thresh_mult,
	uint16_t               *prange_ignore_threshold_mcps_internal,
	uint16_t               *prange_ignore_threshold_mcps_current);




VL53L1_Error VL53L1_set_user_zone(
	VL53L1_DEV          Dev,
	VL53L1_user_zone_t *puser_zone);



VL53L1_Error VL53L1_get_user_zone(
	VL53L1_DEV          Dev,
	VL53L1_user_zone_t *puser_zone);


VL53L1_Error VL53L1_get_mode_mitigation_roi(
	VL53L1_DEV          Dev,
	VL53L1_user_zone_t *pmm_roi);


VL53L1_Error VL53L1_set_preset_mode(
	VL53L1_DEV                   Dev,
	VL53L1_DevicePresetModes     device_preset_mode,
	uint16_t                     dss_config__target_total_rate_mcps,
	uint32_t                     phasecal_config_timeout_us,
	uint32_t                     mm_config_timeout_us,
	uint32_t                     range_config_timeout_us,
	uint32_t                     inter_measurement_period_ms);



VL53L1_Error VL53L1_get_preset_mode_timing_cfg(
	VL53L1_DEV                   Dev,
	VL53L1_DevicePresetModes     device_preset_mode,
	uint16_t                    *pdss_config__target_total_rate_mcps,
	uint32_t                    *pphasecal_config_timeout_us,
	uint32_t                    *pmm_config_timeout_us,
	uint32_t                    *prange_config_timeout_us);

VL53L1_Error VL53L1_enable_xtalk_compensation(
	VL53L1_DEV                 Dev);


VL53L1_Error VL53L1_disable_xtalk_compensation(
	VL53L1_DEV                 Dev);


void VL53L1_get_xtalk_compensation_enable(
	VL53L1_DEV    Dev,
	uint8_t       *pcrosstalk_compensation_enable);


VL53L1_Error VL53L1_init_and_start_range(
	VL53L1_DEV                      Dev,
	uint8_t                         measurement_mode,
	VL53L1_DeviceConfigLevel        device_config_level);



VL53L1_Error VL53L1_stop_range(
	VL53L1_DEV  Dev);


VL53L1_Error VL53L1_get_measurement_results(
	VL53L1_DEV                  Dev,
	VL53L1_DeviceResultsLevel   device_result_level);


VL53L1_Error VL53L1_get_device_results(
	VL53L1_DEV                 Dev,
	VL53L1_DeviceResultsLevel  device_result_level,
	VL53L1_range_results_t    *prange_results);



VL53L1_Error VL53L1_clear_interrupt_and_enable_next_range(
	VL53L1_DEV       Dev,
	uint8_t          measurement_mode);



void VL53L1_copy_sys_and_core_results_to_range_results(
	int32_t                           gain_factor,
	VL53L1_system_results_t          *psys,
	VL53L1_core_results_t            *pcore,
	VL53L1_range_results_t           *presults);


VL53L1_Error VL53L1_set_GPIO_interrupt_config(
	VL53L1_DEV                      Dev,
	VL53L1_GPIO_Interrupt_Mode	intr_mode_distance,
	VL53L1_GPIO_Interrupt_Mode	intr_mode_rate,
	uint8_t				intr_new_measure_ready,
	uint8_t				intr_no_target,
	uint8_t				intr_combined_mode,
	uint16_t			thresh_distance_high,
	uint16_t			thresh_distance_low,
	uint16_t			thresh_rate_high,
	uint16_t			thresh_rate_low
	);


VL53L1_Error VL53L1_set_GPIO_interrupt_config_struct(
	VL53L1_DEV                      Dev,
	VL53L1_GPIO_interrupt_config_t	intconf);


VL53L1_Error VL53L1_get_GPIO_interrupt_config(
	VL53L1_DEV                      Dev,
	VL53L1_GPIO_interrupt_config_t	*pintconf);


VL53L1_Error VL53L1_set_offset_calibration_mode(
	VL53L1_DEV                      Dev,
	VL53L1_OffsetCalibrationMode   offset_cal_mode);



VL53L1_Error VL53L1_get_offset_calibration_mode(
	VL53L1_DEV                      Dev,
	VL53L1_OffsetCalibrationMode  *poffset_cal_mode);



VL53L1_Error VL53L1_set_offset_correction_mode(
	VL53L1_DEV                     Dev,
	VL53L1_OffsetCalibrationMode   offset_cor_mode);



VL53L1_Error VL53L1_get_offset_correction_mode(
	VL53L1_DEV                    Dev,
	VL53L1_OffsetCorrectionMode  *poffset_cor_mode);


VL53L1_Error VL53L1_get_lite_xtalk_margin_kcps(
	VL53L1_DEV                          Dev,
	int16_t                           *pxtalk_margin);

VL53L1_Error VL53L1_set_lite_xtalk_margin_kcps(
	VL53L1_DEV                          Dev,
	int16_t                             xtalk_margin);


VL53L1_Error VL53L1_get_lite_min_count_rate(
	VL53L1_DEV                          Dev,
	uint16_t                           *plite_mincountrate);


VL53L1_Error VL53L1_set_lite_min_count_rate(
	VL53L1_DEV                          Dev,
	uint16_t                            lite_mincountrate);


VL53L1_Error VL53L1_get_lite_sigma_threshold(
	VL53L1_DEV                          Dev,
	uint16_t                           *plite_sigma);



VL53L1_Error VL53L1_set_lite_sigma_threshold(
	VL53L1_DEV                          Dev,
	uint16_t                            lite_sigma);


VL53L1_Error VL53L1_restore_xtalk_nvm_default(
	VL53L1_DEV                     Dev);


VL53L1_Error VL53L1_set_vhv_config(
	VL53L1_DEV                   Dev,
	uint8_t                      vhv_init_en,
	uint8_t                      vhv_init_value);


VL53L1_Error VL53L1_get_vhv_config(
	VL53L1_DEV                   Dev,
	uint8_t                     *pvhv_init_en,
	uint8_t                     *pvhv_init_value);

VL53L1_Error VL53L1_set_vhv_loopbound(
	VL53L1_DEV                   Dev,
	uint8_t                      vhv_loopbound);


VL53L1_Error VL53L1_get_vhv_loopbound(
	VL53L1_DEV                   Dev,
	uint8_t                     *pvhv_loopbound);


VL53L1_Error VL53L1_get_tuning_parm(
	VL53L1_DEV                     Dev,
	VL53L1_TuningParms             tuning_parm_key,
	int32_t                       *ptuning_parm_value);


VL53L1_Error VL53L1_set_tuning_parm(
	VL53L1_DEV                     Dev,
	VL53L1_TuningParms             tuning_parm_key,
	int32_t                        tuning_parm_value);


#ifdef __cplusplus
}
#endif

#endif /* _VL53L1_API_CORE_H_ */
 /** @} */


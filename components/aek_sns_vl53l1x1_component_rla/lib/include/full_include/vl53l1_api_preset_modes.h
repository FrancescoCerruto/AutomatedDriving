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
 * @file    vl53l1_api_preset_modes.h
 * @brief   SPC5xx vl53l1_api_preset_modes.h header file.
 *
 * @addtogroup vl53l1_api_preset_modes
 * @{
 */

#ifndef _VL53L1_API_PRESET_MODES_H_
#define _VL53L1_API_PRESET_MODES_H_


#include "vl53l1_ll_def.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Initializes Ref SPAD Char Configuration Parameters
 *
 * @param[out]   pdata    : pointer to VL53L1_refspadchar_config_t  data structure
 *
 * @return  VL53L1_ERROR_NONE     Success
 * @return  "Other error code"    See ::VL53L1_Error
 */

#ifndef VL53L1_NOCALIB
VL53L1_Error VL53L1_init_refspadchar_config_struct(
	VL53L1_refspadchar_config_t     *pdata);
#endif


/**
 * @brief Initializes SPAD Self Check (SSC) Configuration Parameters
 *
 * @param[out]   pdata    : pointer to VL53L1_ssc_config_t  data structure
 *
 * @return  VL53L1_ERROR_NONE     Success
 * @return  "Other error code"    See ::VL53L1_Error
 */

VL53L1_Error VL53L1_init_ssc_config_struct(
	VL53L1_ssc_config_t     *pdata);

/**
 * @brief Initializes Xtalk Configuration Parameters
 *
 * @param[in]    pnvm     : pointer to VL53L1_customer_nvm_managed_t data structure
 * @param[out]   pdata    : pointer to VL53L1_xtalk_config_t  data structure
 *
 * @return  VL53L1_ERROR_NONE     Success
 * @return  "Other error code"    See ::VL53L1_Error
 */

VL53L1_Error VL53L1_init_xtalk_config_struct(
		VL53L1_customer_nvm_managed_t *pnvm,
		VL53L1_xtalk_config_t   *pdata);

/**
 * @brief Initializes Offset Calibration Configuration Parameters
 *
 * @param[out]   pdata    : pointer to VL53L1_offsetcal_config_t  data structure
 *
 * @return  VL53L1_ERROR_NONE     Success
 * @return  "Other error code"    See ::VL53L1_Error
 */

#ifndef VL53L1_NOCALIB
VL53L1_Error VL53L1_init_offset_cal_config_struct(
	VL53L1_offsetcal_config_t   *pdata);
#endif


/**
 * @brief Initializes Tuning Parameter Storage Values
 *
 * @param[out]   pdata : pointer to VL53L1_tuning_parm_storage_t  data structure
 *
 * @return  VL53L1_ERROR_NONE     Success
 * @return  "Other error code"    See ::VL53L1_Error
 */

VL53L1_Error VL53L1_init_tuning_parm_storage_struct(
	VL53L1_tuning_parm_storage_t   *pdata);

/**
 * @brief Initializes static and dynamic configuration settings for
 *        preset mode VL53L1_DEVICEPRESETMODE_STANDARD_RANGING
 *
 * @param[out]   pstatic    : pointer to VL53L1_static_config_t  data structure
 * @param[out]   pgeneral   : pointer to VL53L1_general_config_t data structure
 * @param[out]   ptiming    : pointer to VL53L1_timing_config_t data structure
 * @param[out]   pdynamic   : pointer to VL53L1_dynamic_config_t data structure
 * @param[out]   psystem    : pointer to VL53L1_system_control_t data structure
 * @param[out]   ptuning_parms : pointer to VL53L1_tuning_parms_storage_t structure
 *
 * @return  VL53L1_ERROR_NONE     Success
 * @return  "Other error code"    See ::VL53L1_Error
 */

VL53L1_Error VL53L1_preset_mode_standard_ranging(
	VL53L1_static_config_t     *pstatic,
	VL53L1_general_config_t    *pgeneral,
	VL53L1_timing_config_t     *ptiming,
	VL53L1_dynamic_config_t    *pdynamic,
	VL53L1_system_control_t    *psystem,
	VL53L1_tuning_parm_storage_t *ptuning_parms);

/**
 * @brief Initializes static and dynamic configuration settings for
 *        preset mode VL53L1_DEVICEPRESETMODE_STANDARD_RANGING_SHORT_RANGE
 *
 * @param[out]   pstatic    : pointer to VL53L1_static_config_t  data structure
 * @param[out]   pgeneral   : pointer to VL53L1_general_config_t data structure
 * @param[out]   ptiming    : pointer to VL53L1_timing_config_t data structure
 * @param[out]   pdynamic   : pointer to VL53L1_dynamic_config_t data structure
 * @param[out]   psystem    : pointer to VL53L1_system_control_t data structure
 * @param[out]   ptuning_parms : pointer to VL53L1_tuning_parms_storage_t structure
 *
 * @return  VL53L1_ERROR_NONE     Success
 * @return  "Other error code"    See ::VL53L1_Error
 */

VL53L1_Error VL53L1_preset_mode_standard_ranging_short_range(
	VL53L1_static_config_t     *pstatic,
	VL53L1_general_config_t    *pgeneral,
	VL53L1_timing_config_t     *ptiming,
	VL53L1_dynamic_config_t    *pdynamic,
	VL53L1_system_control_t    *psystem,
	VL53L1_tuning_parm_storage_t *ptuning_parms);


/**
 * @brief Initializes static and dynamic configuration settings for
 *        preset mode VL53L1_DEVICEPRESETMODE_STANDARD_RANGING_LONG_RANGE
 *
 * @param[out]   pstatic    : pointer to VL53L1_static_config_t  data structure
 * @param[out]   pgeneral   : pointer to VL53L1_general_config_t data structure
 * @param[out]   ptiming    : pointer to VL53L1_timing_config_t data structure
 * @param[out]   pdynamic   : pointer to VL53L1_dynamic_config_t data structure
 * @param[out]   psystem    : pointer to VL53L1_system_control_t data structure
 * @param[out]   ptuning_parms : pointer to VL53L1_tuning_parms_storage_t structure
 *
 * @return  VL53L1_ERROR_NONE     Success
 * @return  "Other error code"    See ::VL53L1_Error
 */

VL53L1_Error VL53L1_preset_mode_standard_ranging_long_range(
	VL53L1_static_config_t     *pstatic,
	VL53L1_general_config_t    *pgeneral,
	VL53L1_timing_config_t     *ptiming,
	VL53L1_dynamic_config_t    *pdynamic,
	VL53L1_system_control_t    *psystem,
	VL53L1_tuning_parm_storage_t *ptuning_parms);

/**
 * @brief Initializes static and dynamic configuration settings for
 *        preset mode VL53L1_DEVICEPRESETMODE_STANDARD_RANGING_MM1_CAL
 *
 * @param[out]   pstatic    : pointer to VL53L1_static_config_t  data structure
 * @param[out]   pgeneral   : pointer to VL53L1_general_config_t data structure
 * @param[out]   ptiming    : pointer to VL53L1_timing_config_t data structure
 * @param[out]   pdynamic   : pointer to VL53L1_dynamic_config_t data structure
 * @param[out]   psystem    : pointer to VL53L1_system_control_t data structure
 * @param[out]   ptuning_parms : pointer to VL53L1_tuning_parms_storage_t structure
 *
 * @return  VL53L1_ERROR_NONE     Success
 * @return  "Other error code"    See ::VL53L1_Error
 */

#ifndef VL53L1_NOCALIB
VL53L1_Error VL53L1_preset_mode_standard_ranging_mm1_cal(
	VL53L1_static_config_t     *pstatic,
	VL53L1_general_config_t    *pgeneral,
	VL53L1_timing_config_t     *ptiming,
	VL53L1_dynamic_config_t    *pdynamic,
	VL53L1_system_control_t    *psystem,
	VL53L1_tuning_parm_storage_t *ptuning_parms);
#endif


/**
 * @brief Initializes static and dynamic configuration settings for
 *        preset mode VL53L1_DEVICEPRESETMODE_STANDARD_RANGING_MM2_CAL
 *
 * @param[out]   pstatic    : pointer to VL53L1_static_config_t  data structure
 * @param[out]   pgeneral   : pointer to VL53L1_general_config_t data structure
 * @param[out]   ptiming    : pointer to VL53L1_timing_config_t data structure
 * @param[out]   pdynamic   : pointer to VL53L1_dynamic_config_t data structure
 * @param[out]   psystem    : pointer to VL53L1_system_control_t data structure
 * @param[out]   ptuning_parms : pointer to VL53L1_tuning_parms_storage_t structure
 *
 * @return  VL53L1_ERROR_NONE     Success
 * @return  "Other error code"    See ::VL53L1_Error
 */

#ifndef VL53L1_NOCALIB
VL53L1_Error VL53L1_preset_mode_standard_ranging_mm2_cal(
	VL53L1_static_config_t     *pstatic,
	VL53L1_general_config_t    *pgeneral,
	VL53L1_timing_config_t     *ptiming,
	VL53L1_dynamic_config_t    *pdynamic,
	VL53L1_system_control_t    *psystem,
	VL53L1_tuning_parm_storage_t *ptuning_parms);
#endif


/**
 * @brief Initializes static and dynamic configuration settings for
 *        preset mode VL53L1_DEVICEPRESETMODE_TIMED_RANGING
 *
 * @param[out]   pstatic    : pointer to VL53L1_static_config_t  data structure
 * @param[out]   pgeneral   : pointer to VL53L1_general_config_t data structure
 * @param[out]   ptiming    : pointer to VL53L1_timing_config_t data structure
 * @param[out]   pdynamic   : pointer to VL53L1_dynamic_config_t data structure
 * @param[out]   psystem    : pointer to VL53L1_system_control_t data structure
 * @param[out]   ptuning_parms : pointer to VL53L1_tuning_parms_storage_t structure
 *
 * @return  VL53L1_ERROR_NONE     Success
 * @return  "Other error code"    See ::VL53L1_Error
 */

VL53L1_Error VL53L1_preset_mode_timed_ranging(

	VL53L1_static_config_t    *pstatic,
	VL53L1_general_config_t   *pgeneral,
	VL53L1_timing_config_t    *ptiming,
	VL53L1_dynamic_config_t   *pdynamic,
	VL53L1_system_control_t   *psystem,
	VL53L1_tuning_parm_storage_t *ptuning_parms);

/**
 * @brief Initializes static and dynamic configuration settings for
 *        preset mode VL53L1_DEVICEPRESETMODE_TIMED_RANGING_SHORT_RANGE
 *
 * @param[out]   pstatic    : pointer to VL53L1_static_config_t  data structure
 * @param[out]   pgeneral   : pointer to VL53L1_general_config_t data structure
 * @param[out]   ptiming    : pointer to VL53L1_timing_config_t data structure
 * @param[out]   pdynamic   : pointer to VL53L1_dynamic_config_t data structure
 * @param[out]   psystem    : pointer to VL53L1_system_control_t data structure
 * @param[out]   ptuning_parms : pointer to VL53L1_tuning_parms_storage_t structure
 *
 * @return  VL53L1_ERROR_NONE     Success
 * @return  "Other error code"    See ::VL53L1_Error
 */

VL53L1_Error VL53L1_preset_mode_timed_ranging_short_range(

	VL53L1_static_config_t    *pstatic,
	VL53L1_general_config_t   *pgeneral,
	VL53L1_timing_config_t    *ptiming,
	VL53L1_dynamic_config_t   *pdynamic,
	VL53L1_system_control_t   *psystem,
	VL53L1_tuning_parm_storage_t *ptuning_parms);

/**
 * @brief Initializes static and dynamic configuration settings for
 *        preset mode VL53L1_DEVICEPRESETMODE_TIMED_RANGING_LONG_RANGE
 *
 * @param[out]   pstatic    : pointer to VL53L1_static_config_t  data structure
 * @param[out]   pgeneral   : pointer to VL53L1_general_config_t data structure
 * @param[out]   ptiming    : pointer to VL53L1_timing_config_t data structure
 * @param[out]   pdynamic   : pointer to VL53L1_dynamic_config_t data structure
 * @param[out]   psystem    : pointer to VL53L1_system_control_t data structure
 * @param[out]   ptuning_parms : pointer to VL53L1_tuning_parms_storage_t structure
 *
 * @return  VL53L1_ERROR_NONE     Success
 * @return  "Other error code"    See ::VL53L1_Error
 */

VL53L1_Error VL53L1_preset_mode_timed_ranging_long_range(

	VL53L1_static_config_t    *pstatic,
	VL53L1_general_config_t   *pgeneral,
	VL53L1_timing_config_t    *ptiming,
	VL53L1_dynamic_config_t   *pdynamic,
	VL53L1_system_control_t   *psystem,
	VL53L1_tuning_parm_storage_t *ptuning_parms);

/**
 * @brief Initializes static and dynamic configuration settings for
 *        preset mode VL53L1_DEVICEPRESETMODE_LOWPOWERAUTO_MEDIUM_RANGE
 *
 * @param[out]   pstatic    : pointer to VL53L1_static_config_t  data structure
 * @param[out]   pgeneral   : pointer to VL53L1_general_config_t data structure
 * @param[out]   ptiming    : pointer to VL53L1_timing_config_t data structure
 * @param[out]   pdynamic   : pointer to VL53L1_dynamic_config_t data structure
 * @param[out]   psystem    : pointer to VL53L1_system_control_t data structure
 * @param[out]   ptuning_parms : pointer to VL53L1_tuning_parms_storage_t structure
 * @param[out]   plpadata   : pointer to VL53L1_low_power_auto_data_t data structure
 *
 * @return  VL53L1_ERROR_NONE     Success
 * @return  "Other error code"    See ::VL53L1_Error
 */

VL53L1_Error VL53L1_preset_mode_low_power_auto_ranging(

	VL53L1_static_config_t    *pstatic,
	VL53L1_general_config_t   *pgeneral,
	VL53L1_timing_config_t    *ptiming,
	VL53L1_dynamic_config_t   *pdynamic,
	VL53L1_system_control_t   *psystem,
	VL53L1_tuning_parm_storage_t *ptuning_parms,
	VL53L1_low_power_auto_data_t *plpadata);

/**
 * @brief Initializes static and dynamic configuration settings for
 *        preset mode VL53L1_DEVICEPRESETMODE_LOWPOWERAUTO_SHORT_RANGE
 *
 * @param[out]   pstatic    : pointer to VL53L1_static_config_t  data structure
 * @param[out]   pgeneral   : pointer to VL53L1_general_config_t data structure
 * @param[out]   ptiming    : pointer to VL53L1_timing_config_t data structure
 * @param[out]   pdynamic   : pointer to VL53L1_dynamic_config_t data structure
 * @param[out]   psystem    : pointer to VL53L1_system_control_t data structure
 * @param[out]   ptuning_parms : pointer to VL53L1_tuning_parms_storage_t structure
 * @param[out]   plpadata   : pointer to VL53L1_low_power_auto_data_t data structure
 *
 * @return  VL53L1_ERROR_NONE     Success
 * @return  "Other error code"    See ::VL53L1_Error
 */

VL53L1_Error VL53L1_preset_mode_low_power_auto_short_ranging(

	VL53L1_static_config_t    *pstatic,
	VL53L1_general_config_t   *pgeneral,
	VL53L1_timing_config_t    *ptiming,
	VL53L1_dynamic_config_t   *pdynamic,
	VL53L1_system_control_t   *psystem,
	VL53L1_tuning_parm_storage_t *ptuning_parms,
	VL53L1_low_power_auto_data_t *plpadata);

/**
 * @brief Initializes static and dynamic configuration settings for
 *        preset mode VL53L1_DEVICEPRESETMODE_LOWPOWERAUTO_LONG_RANGE
 *
 * @param[out]   pstatic    : pointer to VL53L1_static_config_t  data structure
 * @param[out]   pgeneral   : pointer to VL53L1_general_config_t data structure
 * @param[out]   ptiming    : pointer to VL53L1_timing_config_t data structure
 * @param[out]   pdynamic   : pointer to VL53L1_dynamic_config_t data structure
 * @param[out]   psystem    : pointer to VL53L1_system_control_t data structure
 * @param[out]   ptuning_parms : pointer to VL53L1_tuning_parms_storage_t structure
 * @param[out]   plpadata   : pointer to VL53L1_low_power_auto_data_t data structure
 *
 * @return  VL53L1_ERROR_NONE     Success
 * @return  "Other error code"    See ::VL53L1_Error
 */

VL53L1_Error VL53L1_preset_mode_low_power_auto_long_ranging(

	VL53L1_static_config_t    *pstatic,
	VL53L1_general_config_t   *pgeneral,
	VL53L1_timing_config_t    *ptiming,
	VL53L1_dynamic_config_t   *pdynamic,
	VL53L1_system_control_t   *psystem,
	VL53L1_tuning_parm_storage_t *ptuning_parms,
	VL53L1_low_power_auto_data_t *plpadata);


/**
 * @brief Initializes static and dynamic configuration settings for
 *        preset mode VL53L1_DEVICEPRESETMODE_OLT
 *
 * @param[out]   pstatic    : pointer to VL53L1_static_config_t  data structure
 * @param[out]   pgeneral   : pointer to VL53L1_general_config_t data structure
 * @param[out]   ptiming    : pointer to VL53L1_timing_config_t data structure
 * @param[out]   pdynamic   : pointer to VL53L1_dynamic_config_t data structure
 * @param[out]   psystem    : pointer to VL53L1_system_control_t data structure
 * @param[out]   ptuning_parms : pointer to VL53L1_tuning_parms_storage_t structure
 *
 * @return  VL53L1_ERROR_NONE     Success
 * @return  "Other error code"    See ::VL53L1_Error
 */

VL53L1_Error VL53L1_preset_mode_olt(
	VL53L1_static_config_t     *pstatic,
	VL53L1_general_config_t    *pgeneral,
	VL53L1_timing_config_t     *ptiming,
	VL53L1_dynamic_config_t    *pdynamic,
	VL53L1_system_control_t    *psystem,
	VL53L1_tuning_parm_storage_t *ptuning_parms);

/**
 * @brief Initializes static and dynamic configuration settings for
 *        preset mode VL53L1_DEVICEPRESETMODE_SINGLESHOT_RANGING
 *
 * @param[out]   pstatic    : pointer to VL53L1_static_config_t  data structure
 * @param[out]   pgeneral   : pointer to VL53L1_general_config_t data structure
 * @param[out]   ptiming    : pointer to VL53L1_timing_config_t data structure
 * @param[out]   pdynamic   : pointer to VL53L1_dynamic_config_t data structure
 * @param[out]   psystem    : pointer to VL53L1_system_control_t data structure
 * @param[out]   ptuning_parms : pointer to VL53L1_tuning_parms_storage_t structure
 *
 * @return  VL53L1_ERROR_NONE     Success
 * @return  "Other error code"    See ::VL53L1_Error
 */

VL53L1_Error VL53L1_preset_mode_singleshot_ranging(

	VL53L1_static_config_t    *pstatic,
	VL53L1_general_config_t   *pgeneral,
	VL53L1_timing_config_t    *ptiming,
	VL53L1_dynamic_config_t   *pdynamic,
	VL53L1_system_control_t   *psystem,
	VL53L1_tuning_parm_storage_t *ptuning_parms);

#ifdef __cplusplus
}
#endif

#endif /* _VL53L1_API_CORE_H_ */
 /** @} */


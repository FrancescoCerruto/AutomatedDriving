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
 * @file    vl53l1_api_strings.h
 * @brief   SPC5xx vl53l1_api_strings.h header file.
 *
 * @addtogroup vl53l1_api_strings
 * @{
 */


#ifndef _VL53L1_API_STRINGS_H_
#define _VL53L1_API_STRINGS_H_

#include "vl53l1_def.h"

#ifdef __cplusplus
extern "C" {
#endif



VL53L1_Error VL53L1_get_range_status_string(
	uint8_t   RangeStatus,
	char    *pRangeStatusString);

VL53L1_Error VL53L1_get_pal_error_string(
	VL53L1_Error   PalErrorCode,
	char         *pPalErrorString);


VL53L1_Error VL53L1_get_pal_state_string(
	VL53L1_State   PalStateCode,
	char         *pPalStateString);


VL53L1_Error VL53L1_get_sequence_steps_info(
		VL53L1_SequenceStepId SequenceStepId,
		char *pSequenceStepsString);


VL53L1_Error VL53L1_get_limit_check_info(uint16_t LimitCheckId,
	char *pLimitCheckString);

#ifndef VL53L1_USE_EMPTY_STRING
	#define  VL53L1_STRING_DEVICE_INFO_NAME0          "VL53L1 cut1.0"
	#define  VL53L1_STRING_DEVICE_INFO_NAME1          "VL53L1 cut1.1"
	#define  VL53L1_STRING_DEVICE_INFO_TYPE          "VL53L1"

	/* Range Status */
	#define  VL53L1_STRING_RANGESTATUS_NONE                 "No Update"
	#define  VL53L1_STRING_RANGESTATUS_RANGEVALID           "Range Valid"
	#define  VL53L1_STRING_RANGESTATUS_SIGMA                "Sigma Fail"
	#define  VL53L1_STRING_RANGESTATUS_SIGNAL               "Signal Fail"
	#define  VL53L1_STRING_RANGESTATUS_MINRANGE             "Min Range Fail"
	#define  VL53L1_STRING_RANGESTATUS_PHASE                "Phase Fail"
	#define  VL53L1_STRING_RANGESTATUS_HW                   "Hardware Fail"


	/* Range Status */
	#define  VL53L1_STRING_STATE_POWERDOWN               "POWERDOWN State"
	#define  VL53L1_STRING_STATE_WAIT_STATICINIT \
			"Wait for staticinit State"
	#define  VL53L1_STRING_STATE_STANDBY                 "STANDBY State"
	#define  VL53L1_STRING_STATE_IDLE                    "IDLE State"
	#define  VL53L1_STRING_STATE_RUNNING                 "RUNNING State"
	#define  VL53L1_STRING_STATE_RESET                   "RESET State"
	#define  VL53L1_STRING_STATE_UNKNOWN                 "UNKNOWN State"
	#define  VL53L1_STRING_STATE_ERROR                   "ERROR State"



	/* Check Enable */
	#define  VL53L1_STRING_CHECKENABLE_SIGMA_FINAL_RANGE \
			"SIGMA FINAL RANGE"
	#define  VL53L1_STRING_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE \
			"SIGNAL RATE FINAL RANGE"
	#define  VL53L1_STRING_CHECKENABLE_SIGNAL_MIN_CLIP \
			"SIGNAL MIN CLIP"
	#define  VL53L1_STRING_CHECKENABLE_RANGE_IGNORE_THRESHOLD \
			"RANGE IGNORE THRESHOLD"
	#define  VL53L1_STRING_CHECKENABLE_RANGE_PHASE_HIGH \
			"RANGE PHASE HIGH"
	#define  VL53L1_STRING_CHECKENABLE_RANGE_PHASE_LOW \
			"RANGE PHASE LOW"
	#define  VL53L1_STRING_CHECKENABLE_RANGE_PHASE_CONSISTENCY \
			"RANGE PHASE CONSISTENCY"

	/* Sequence Step */
	#define  VL53L1_STRING_SEQUENCESTEP_VHV         "VHV"
	#define  VL53L1_STRING_SEQUENCESTEP_PHASECAL    "PHASE CAL"
	#define  VL53L1_STRING_SEQUENCESTEP_REFPHASE    "REF PHASE"
	#define  VL53L1_STRING_SEQUENCESTEP_DSS1        "DSS1"
	#define  VL53L1_STRING_SEQUENCESTEP_DSS2        "DSS2"
	#define  VL53L1_STRING_SEQUENCESTEP_MM1         "MM1"
	#define  VL53L1_STRING_SEQUENCESTEP_MM2         "MM2"
	#define  VL53L1_STRING_SEQUENCESTEP_RANGE       "RANGE"
#endif /* VL53L1_USE_EMPTY_STRING */


#ifdef __cplusplus
}
#endif

#endif /*_VL53L1_API_STRINGS_H_*/


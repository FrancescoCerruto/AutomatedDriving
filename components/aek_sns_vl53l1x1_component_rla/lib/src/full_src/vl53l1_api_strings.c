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
* EVALUATION ONLY - NOT FOR USE IN PRODUCTION
*****************************************************************************/

/**
 * @file     vl53l1_api_strings.c

 * @brief   SPC5xx  vl53l1_api_strings.c driver code.
 *
 * @addtogroup  vl53l1_api_strings
 * @{
 */

#include "full_include/vl53l1_api_core.h"
#include "full_include/vl53l1_api_strings.h"
#include "full_include/vl53l1_error_codes.h"
#include "full_include/vl53l1_error_strings.h"


/**
 * @brief Generates a string for the input device range status code
 *
 * @param[in]   RangeStatus           : Device Range AStatus Code
 * @param[out]  pRangeStatusString    : pointer to character buffer
 *
 * @return  VL53L1_ERROR_NONE     Success
 * @return  "Other error code"    See ::VL53L1_Error
 */

VL53L1_Error VL53L1_get_range_status_string(
	uint8_t   RangeStatus,
	char    *pRangeStatusString)
{
	VL53L1_Error status = VL53L1_ERROR_NONE;

//	LOG_FUNCTION_START("");

#ifdef VL53L1_USE_EMPTY_STRING
	VL53L1_COPYSTRING(pRangeStatusString, "");
#else
	switch (RangeStatus) {
	case 0:
		VL53L1_COPYSTRING(pRangeStatusString,
			VL53L1_STRING_RANGESTATUS_RANGEVALID);
	break;
	case 1:
		VL53L1_COPYSTRING(pRangeStatusString,
			VL53L1_STRING_RANGESTATUS_SIGMA);
	break;
	case 2:
		VL53L1_COPYSTRING(pRangeStatusString,
			VL53L1_STRING_RANGESTATUS_SIGNAL);
	break;
	case 3:
		VL53L1_COPYSTRING(pRangeStatusString,
			VL53L1_STRING_RANGESTATUS_MINRANGE);
	break;
	case 4:
		VL53L1_COPYSTRING(pRangeStatusString,
			VL53L1_STRING_RANGESTATUS_PHASE);
	break;
	case 5:
		VL53L1_COPYSTRING(pRangeStatusString,
			VL53L1_STRING_RANGESTATUS_HW);
	break;

	default: /**/
		VL53L1_COPYSTRING(pRangeStatusString,
			VL53L1_STRING_RANGESTATUS_NONE);
	}
#endif

//	LOG_FUNCTION_END(status);
	return status;
}

/**
 * @brief Generates an error string for the input PAL error code
 *
 * @param[in]   PalErrorCode         : PAL Error Code
 * @param[out]  pPalErrorString      : pointer to character buffer
 *
 * @return  VL53L1_ERROR_NONE     Success
 * @return  "Other error code"    See ::VL53L1_Error
 */

VL53L1_Error VL53L1_get_pal_error_string(
	VL53L1_Error   PalErrorCode,
	char         *pPalErrorString);

/**
 * @brief Generates a string for the input PAL State code
 *
 * @param[in]   PalStateCode         : PAL State Code
 * @param[out]  pPalStateString      : pointer to character buffer
 *
 * @return  VL53L1_ERROR_NONE     Success
 * @return  "Other error code"    See ::VL53L1_Error
 */
VL53L1_Error VL53L1_get_pal_state_string(
	VL53L1_State PalStateCode,
	char *pPalStateString)
{
	VL53L1_Error status = VL53L1_ERROR_NONE;

//	LOG_FUNCTION_START("");

#ifdef VL53L1_USE_EMPTY_STRING
	VL53L1_COPYSTRING(pPalStateString, "");
#else
	switch (PalStateCode) {
	case VL53L1_STATE_POWERDOWN:
		VL53L1_COPYSTRING(pPalStateString,
			VL53L1_STRING_STATE_POWERDOWN);
	break;
	case VL53L1_STATE_WAIT_STATICINIT:
		VL53L1_COPYSTRING(pPalStateString,
			VL53L1_STRING_STATE_WAIT_STATICINIT);
	break;
	case VL53L1_STATE_STANDBY:
		VL53L1_COPYSTRING(pPalStateString,
			VL53L1_STRING_STATE_STANDBY);
	break;
	case VL53L1_STATE_IDLE:
		VL53L1_COPYSTRING(pPalStateString,
			VL53L1_STRING_STATE_IDLE);
	break;
	case VL53L1_STATE_RUNNING:
		VL53L1_COPYSTRING(pPalStateString,
			VL53L1_STRING_STATE_RUNNING);
	break;
	case VL53L1_STATE_RESET:
		VL53L1_COPYSTRING(pPalStateString,
			VL53L1_STRING_STATE_RESET);
	break;
	case VL53L1_STATE_UNKNOWN:
		VL53L1_COPYSTRING(pPalStateString,
			VL53L1_STRING_STATE_UNKNOWN);
	break;
	case VL53L1_STATE_ERROR:
		VL53L1_COPYSTRING(pPalStateString,
			VL53L1_STRING_STATE_ERROR);
	break;

	default:
		VL53L1_COPYSTRING(pPalStateString,
			VL53L1_STRING_STATE_UNKNOWN);
	}
#endif

//	LOG_FUNCTION_END(status);
	return status;
}

/**
 * @brief Generates a string for the sequence step Id
 *
 * @param[in]   SequenceStepId            : Sequence Step Id
 * @param[out]  pSequenceStepsString      : pointer to character buffer
 *
 * @return  VL53L1_ERROR_NONE     Success
 * @return  "Other error code"    See ::VL53L1_Error
 */
VL53L1_Error VL53L1_get_sequence_steps_info(
		VL53L1_SequenceStepId SequenceStepId,
		char *pSequenceStepsString)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;

//	LOG_FUNCTION_START("");

#ifdef VL53L1_USE_EMPTY_STRING
	VL53L1_COPYSTRING(pSequenceStepsString, "");
#else
	switch (SequenceStepId) {
	case VL53L1_SEQUENCESTEP_VHV:
		VL53L1_COPYSTRING(pSequenceStepsString,
				VL53L1_STRING_SEQUENCESTEP_VHV);
	break;
	case VL53L1_SEQUENCESTEP_PHASECAL:
		VL53L1_COPYSTRING(pSequenceStepsString,
				VL53L1_STRING_SEQUENCESTEP_PHASECAL);
	break;
	case VL53L1_SEQUENCESTEP_REFPHASE:
		VL53L1_COPYSTRING(pSequenceStepsString,
				VL53L1_STRING_SEQUENCESTEP_DSS1);
	break;
	case VL53L1_SEQUENCESTEP_DSS1:
		VL53L1_COPYSTRING(pSequenceStepsString,
				VL53L1_STRING_SEQUENCESTEP_DSS1);
	break;
	case VL53L1_SEQUENCESTEP_DSS2:
		VL53L1_COPYSTRING(pSequenceStepsString,
				VL53L1_STRING_SEQUENCESTEP_DSS2);
	break;
	case VL53L1_SEQUENCESTEP_MM1:
		VL53L1_COPYSTRING(pSequenceStepsString,
				VL53L1_STRING_SEQUENCESTEP_MM1);
	break;
	case VL53L1_SEQUENCESTEP_MM2:
		VL53L1_COPYSTRING(pSequenceStepsString,
				VL53L1_STRING_SEQUENCESTEP_MM2);
	break;
	case VL53L1_SEQUENCESTEP_RANGE:
		VL53L1_COPYSTRING(pSequenceStepsString,
				VL53L1_STRING_SEQUENCESTEP_RANGE);
	break;
	default:
		Status = VL53L1_ERROR_INVALID_PARAMS;
	}
#endif

//	LOG_FUNCTION_END(Status);

	return Status;
}

/**
 * @brief Generates a string for the limit check Id
 *
 * @param[in]   LimitCheckId            : Limit check Id
 * @param[out]  pLimitCheckString       : pointer to character buffer
 *
 * @return  VL53L1_ERROR_NONE     Success
 * @return  "Other error code"    See ::VL53L1_Error
 */
VL53L1_Error VL53L1_get_limit_check_info(uint16_t LimitCheckId,
	char *pLimitCheckString)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;

//	LOG_FUNCTION_START("");

#ifdef VL53L1_USE_EMPTY_STRING
	VL53L1_COPYSTRING(pLimitCheckString, "");
#else
	switch (LimitCheckId) {
	case VL53L1_CHECKENABLE_SIGMA_FINAL_RANGE:
		VL53L1_COPYSTRING(pLimitCheckString,
			VL53L1_STRING_CHECKENABLE_SIGMA_FINAL_RANGE);
	break;
	case VL53L1_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE:
		VL53L1_COPYSTRING(pLimitCheckString,
			VL53L1_STRING_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE);
	break;
	default:
		VL53L1_COPYSTRING(pLimitCheckString,
			VL53L1_STRING_UNKNOW_ERROR_CODE);
	}
#endif

//	LOG_FUNCTION_END(Status);
	return Status;
}

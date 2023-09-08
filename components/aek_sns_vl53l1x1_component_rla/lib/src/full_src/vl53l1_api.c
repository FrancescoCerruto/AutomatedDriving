
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
 * @file    vl53l1_api.c
 * @brief   SPC5xx vl53l1_api.c driver code.
 *
 * ddtogroup vl53l1_api
 * @{
 */

#include "full_include/vl53l1_api.h"
#include "full_include/vl53l1_api_strings.h"
#include "full_include/vl53l1_register_settings.h"
#include "full_include/vl53l1_register_funcs.h"
#include "full_include/vl53l1_core.h"
#include "full_include/vl53l1_api_calibration.h"
#include "full_include/vl53l1_wait.h"
#include "full_include/vl53l1_preset_setup.h"
#include "full_include/vl53l1_api_debug.h"
#include "full_include/vl53l1_api_core.h"

#include "full_include/vl53l1_register_map.h"
/* Check for minimum user zone requested by Xtalk calibration */
/* no need for VL53L1_MAX_USER_ZONES check, set 5 to pass the test */
#define ZONE_CHECK 5

#if ZONE_CHECK < 5
#error Must define at least 5 zones in MAX_USER_ZONES constant
#endif


#ifdef VL53L1_NOCALIB
#define OFFSET_CALIB_EMPTY
#endif

#ifndef VL53L1_NOCALIB
#define OFFSET_CALIB
#endif

#define LOG_FUNCTION_START(fmt, ...) \
	_LOG_FUNCTION_START(VL53L1_TRACE_MODULE_API, fmt, ##__VA_ARGS__)
#define LOG_FUNCTION_END(status, ...) \
	_LOG_FUNCTION_END(VL53L1_TRACE_MODULE_API, status, ##__VA_ARGS__)
#define LOG_FUNCTION_END_FMT(status, fmt, ...) \
	_LOG_FUNCTION_END_FMT(VL53L1_TRACE_MODULE_API, status, \
			fmt, ##__VA_ARGS__)

#ifdef VL53L1_LOG_ENABLE
#define trace_print(level, ...) trace_print_module_function(\
		VL53L1_TRACE_MODULE_API, level, VL53L1_TRACE_FUNCTION_NONE, \
		##__VA_ARGS__)
#endif

#ifndef MIN
#define MIN(v1, v2) ((v1) < (v2) ? (v1) : (v2))
#endif
#ifndef MAX
#define MAX(v1, v2) ((v1) < (v2) ? (v2) : (v1))
#endif

#define DMAX_REFLECTANCE_IDX 2
/* Use Dmax index 2 because it's the 50% reflectance case by default */

/* Following LOWPOWER_AUTO figures have been measured observing vcsel
 * emissions on an actual device
 */
#define LOWPOWER_AUTO_VHV_LOOP_DURATION_US 245
#define LOWPOWER_AUTO_OVERHEAD_BEFORE_A_RANGING 1448
#define LOWPOWER_AUTO_OVERHEAD_BETWEEN_A_B_RANGING 2100

#define FDA_MAX_TIMING_BUDGET_US 550000
/* Maximum timing budget allowed codex #456189*/


/* local static utilities functions */

/* Bare Driver Tuning parameter table indexed with VL53L1_Tuning_t */
static int32_t BDTable[VL53L1_TUNING_MAX_TUNABLE_KEY] = {
		TUNING_VERSION,
		TUNING_PROXY_MIN,
		TUNING_SINGLE_TARGET_XTALK_TARGET_DISTANCE_MM,
		TUNING_SINGLE_TARGET_XTALK_SAMPLE_NUMBER,
		TUNING_MIN_AMBIENT_DMAX_VALID,
		TUNING_MAX_SIMPLE_OFFSET_CALIBRATION_SAMPLE_NUMBER,
		TUNING_XTALK_FULL_ROI_TARGET_DISTANCE_MM,
		TUNING_SIMPLE_OFFSET_CALIBRATION_REPEAT
};


#define VL53L1_NVM_POWER_UP_DELAY_US             50
#define VL53L1_NVM_READ_TRIGGER_DELAY_US          5

static VL53L1_Error VL53L1_nvm_enable(
	VL53L1_DEV      Dev,
	uint16_t        nvm_ctrl_pulse_width,
	int32_t         nvm_power_up_delay_us)
{
	/*
	 * Sequence below enables NVM for reading
	 *
	 *  - Enable power force
	 *  - Disable firmware
	 *  - Power up NVM
	 *  - Wait for 50us while the NVM powers up
	 *  - Configure for reading and set the pulse width (16-bit)
	 */

	VL53L1_Error status = VL53L1_ERROR_NONE;




	/* Disable Firmware */

	if (status == VL53L1_ERROR_NONE) /*lint !e774 always true*/
		status = VL53L1_disable_firmware(Dev);


	/* Enable Power Force  */

	if (status == VL53L1_ERROR_NONE)
		status = VL53L1_enable_powerforce(Dev);

	/* Wait the required time for the regulators, bandgap,
	 * oscillator to wake up and settle
	 */

	if (status == VL53L1_ERROR_NONE)
		status = VL53L1_WaitUs(
					Dev,
					VL53L1_ENABLE_POWERFORCE_SETTLING_TIME_US);

	/*  Power up NVM */

	if (status == VL53L1_ERROR_NONE)
		status = VL53L1_WrByte(
					Dev,
					VL53L1_RANGING_CORE__NVM_CTRL__PDN,
					0x01);

	/* Enable NVM Clock */

	if (status == VL53L1_ERROR_NONE)
		status = VL53L1_WrByte(
					Dev,
					VL53L1_RANGING_CORE__CLK_CTRL1,
					0x05);

	/* Wait the required time for NVM to power up*/

	if (status == VL53L1_ERROR_NONE)
		status = VL53L1_WaitUs(
					Dev,
					nvm_power_up_delay_us);

	/* Select read mode and set control pulse width */

	if (status == VL53L1_ERROR_NONE)
		status = VL53L1_WrByte(
					Dev,
					VL53L1_RANGING_CORE__NVM_CTRL__MODE,
					0x01);

	if (status == VL53L1_ERROR_NONE)
		status = VL53L1_WrWord(
					Dev,
					VL53L1_RANGING_CORE__NVM_CTRL__PULSE_WIDTH_MSB,
					nvm_ctrl_pulse_width);



	return status;

}


static VL53L1_Error VL53L1_nvm_read(
	VL53L1_DEV    Dev,
	uint8_t       start_address,
	uint8_t       count,
	uint8_t      *pdata)
{
	/*
	 * Sequence per 32-bit NVM read access:
	 *
	 * - Set up the 5-bit (0-127) NVM Address
	 * - Trigger the read of the NVM data by toggling NVM_CTRL__READN
	 * - Read the NVM data - 4 bytes wide read/write interface
	 * - Increment data byte pointer by 4 ready for the next loop
	 */

	VL53L1_Error status   = VL53L1_ERROR_NONE;
	uint8_t      nvm_addr = 0;



	for (nvm_addr = start_address; nvm_addr < (start_address+count) ; nvm_addr++) {

		/* Step 1 : set address */

		if (status == VL53L1_ERROR_NONE)
			status = VL53L1_WrByte(
						Dev,
						VL53L1_RANGING_CORE__NVM_CTRL__ADDR,
						nvm_addr);

		/* Step 2 : trigger reading of data */

		if (status == VL53L1_ERROR_NONE)
			status = VL53L1_WrByte(
						Dev,
						VL53L1_RANGING_CORE__NVM_CTRL__READN,
						0x00);

		/* Step 3 : wait the required time */

		if (status == VL53L1_ERROR_NONE)
			status = VL53L1_WaitUs(
						Dev,
						VL53L1_NVM_READ_TRIGGER_DELAY_US);

		if (status == VL53L1_ERROR_NONE)
			status = VL53L1_WrByte(
						Dev,
						VL53L1_RANGING_CORE__NVM_CTRL__READN,
						0x01);

		/* Step 3 : read 4-byte wide data register */
		if (status == VL53L1_ERROR_NONE)
			status = VL53L1_ReadMulti(
						Dev,
						VL53L1_RANGING_CORE__NVM_CTRL__DATAOUT_MMM,
						pdata,
						4);

		/* Step 4 : increment byte buffer pointer */

		pdata = pdata + 4;


	}



	return status;
}

static VL53L1_Error VL53L1_nvm_disable(
	VL53L1_DEV    Dev)
{
	/*
	 * Power down NVM (OTP) to extend lifetime
	 */

	VL53L1_Error status = VL53L1_ERROR_NONE;



	if (status == VL53L1_ERROR_NONE) /*lint !e774 always true*/
		status = VL53L1_WrByte(
					Dev,
					VL53L1_RANGING_CORE__NVM_CTRL__READN,
					0x01);

	/* Power down NVM */

	if (status == VL53L1_ERROR_NONE)
		status = VL53L1_WrByte(
					Dev,
					VL53L1_RANGING_CORE__NVM_CTRL__PDN,
					0x00);

	/* Keep power force enabled */

	if (status == VL53L1_ERROR_NONE)
		status = VL53L1_disable_powerforce(Dev);

	/* (Re)Enable Firmware */

	if (status == VL53L1_ERROR_NONE)
		status = VL53L1_enable_firmware(Dev);



	return status;

}

static VL53L1_Error VL53L1_read_nvm_raw_data(
	VL53L1_DEV     Dev,
	uint8_t        start_address,
	uint8_t        count,
	uint8_t       *pnvm_raw_data)
{

	/*
	 * Reads ALL 512 bytes of NVM data
	 */

	VL53L1_Error status = VL53L1_ERROR_NONE;



	/*
	 *   Enable NVM and set control pulse width
	 */

	if (status == VL53L1_ERROR_NONE) /*lint !e774 always true*/
		status = VL53L1_nvm_enable(
					Dev,
					0x0004,
					VL53L1_NVM_POWER_UP_DELAY_US);

	/*
	 *  Read the raw NVM data
	 *        - currently all of 128 * 4 bytes = 512 bytes are read
	 */

	if (status == VL53L1_ERROR_NONE)
		status = VL53L1_nvm_read(
			Dev,
			start_address,
			count,
			pnvm_raw_data);

	/*
	 *   Disable NVM
	 */

	if (status == VL53L1_ERROR_NONE)
		status = VL53L1_nvm_disable(Dev);



	return status;

}

static VL53L1_Error SingleTargetXTalkCalibration(VL53L1_DEV Dev)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;

	uint32_t sum_ranging = 0;
	uint32_t sum_spads = 0;
	FixPoint1616_t sum_signalRate = 0;
	FixPoint1616_t total_count = 0;
	uint8_t xtalk_meas = 0;
	uint8_t xtalk_measmax =
		BDTable[VL53L1_TUNING_SINGLE_TARGET_XTALK_SAMPLE_NUMBER];
	VL53L1_RangingMeasurementData_t RMData;
	FixPoint1616_t xTalkStoredMeanSignalRate;
	FixPoint1616_t xTalkStoredMeanRange;
	FixPoint1616_t xTalkStoredMeanRtnSpads;
	uint32_t xTalkStoredMeanRtnSpadsAsInt;
	uint32_t xTalkCalDistanceAsInt;
	FixPoint1616_t XTalkCompensationRateMegaCps;
	uint32_t signalXTalkTotalPerSpad;
	VL53L1_PresetModes PresetMode;
	VL53L1_CalibrationData_t  CalibrationData;
	VL53L1_CustomerNvmManaged_t *pC;




	/* check if the following are selected
	 * VL53L1_PRESETMODE_AUTONOMOUS,
	 * VL53L1_PRESETMODE_LOWPOWER_AUTONOMOUS
	 * VL53L1_PRESETMODE_LITE_RANGING
	 */
	PresetMode = VL53L1DevDataGet(Dev, CurrentParameters.PresetMode);

	if ((PresetMode != VL53L1_PRESETMODE_AUTONOMOUS) &&
		(PresetMode != VL53L1_PRESETMODE_LOWPOWER_AUTONOMOUS) &&
		(PresetMode != VL53L1_PRESETMODE_LITE_RANGING)) {
		Status = VL53L1_ERROR_MODE_NOT_SUPPORTED;
		goto ENDFUNC;
	}

	/* disable crosstalk calibration */
	Status = VL53L1_disable_xtalk_compensation(Dev);
	CHECK_ERROR_GO_ENDFUNC;

	Status = VL53L1_StartMeasurement(Dev);
	CHECK_ERROR_GO_ENDFUNC;

	sum_ranging = 0;
	sum_spads = 0;
	sum_signalRate = 0;
	total_count = 0;
	for (xtalk_meas = 0; xtalk_meas < xtalk_measmax; xtalk_meas++) {
		VL53L1_WaitMeasurementDataReady(Dev);
		VL53L1_GetRangingMeasurementData(Dev, &RMData);
		VL53L1_ClearInterruptAndStartMeasurement(Dev);
		if (RMData.RangeStatus == VL53L1_RANGESTATUS_RANGE_VALID) {
			sum_ranging += RMData.RangeMilliMeter;
			sum_signalRate += RMData.SignalRateRtnMegaCps;
			sum_spads += RMData.EffectiveSpadRtnCount / 256;
			total_count++;
		}
	}
	Status = VL53L1_StopMeasurement(Dev);

	if (total_count > 0) {
		/* FixPoint1616_t / uint16_t = FixPoint1616_t */
		xTalkStoredMeanSignalRate = sum_signalRate / total_count;
		xTalkStoredMeanRange = (FixPoint1616_t)(sum_ranging << 16);
		xTalkStoredMeanRange /= total_count;
		xTalkStoredMeanRtnSpads = (FixPoint1616_t)(sum_spads << 16);
		xTalkStoredMeanRtnSpads /= total_count;

		/* Round Mean Spads to Whole Number.
		 * Typically the calculated mean SPAD count is a whole number
		 * or very close to a whole
		 * number, therefore any truncation will not result in a
		 * significant loss in accuracy.
		 * Also, for a grey target at a typical distance of around
		 * 400mm, around 220 SPADs will
		 * be enabled, therefore, any truncation will result in a loss
		 * of accuracy of less than
		 * 0.5%.
		 */
		xTalkStoredMeanRtnSpadsAsInt = (xTalkStoredMeanRtnSpads +
			0x8000) >> 16;

		/* Round Cal Distance to Whole Number.
		 * Note that the cal distance is in mm, therefore no resolution
		 * is lost.
		 */
		 xTalkCalDistanceAsInt = ((uint32_t)BDTable[
			VL53L1_TUNING_SINGLE_TARGET_XTALK_TARGET_DISTANCE_MM]);
		if (xTalkStoredMeanRtnSpadsAsInt == 0 ||
		xTalkCalDistanceAsInt == 0 ||
		xTalkStoredMeanRange >= (xTalkCalDistanceAsInt << 16)) {
			XTalkCompensationRateMegaCps = 0;
		} else {
			/* Apply division by mean spad count early in the
			 * calculation to keep the numbers small.
			 * This ensures we can maintain a 32bit calculation.
			 * Fixed1616 / int := Fixed1616
			 */
			signalXTalkTotalPerSpad = (xTalkStoredMeanSignalRate) /
				xTalkStoredMeanRtnSpadsAsInt;

			/* Complete the calculation for total Signal XTalk per
			 * SPAD
			 * Fixed1616 * (Fixed1616 - Fixed1616/int) :=
			 * (2^16 * Fixed1616)
			 */
			signalXTalkTotalPerSpad *= (((uint32_t)1 << 16) -
				(xTalkStoredMeanRange / xTalkCalDistanceAsInt));

			/* Round from 2^16 * Fixed1616, to Fixed1616. */
			XTalkCompensationRateMegaCps = (signalXTalkTotalPerSpad
				+ 0x8000) >> 16;
		}


		Status = VL53L1_GetCalibrationData(Dev, &CalibrationData);
		CHECK_ERROR_GO_ENDFUNC;

		pC = &CalibrationData.customer;

		pC->algo__crosstalk_compensation_plane_offset_kcps =
			(uint32_t)(1000 * ((XTalkCompensationRateMegaCps  +
				((uint32_t)1<<6)) >> (16-9)));

		Status = VL53L1_SetCalibrationData(Dev, &CalibrationData);
		CHECK_ERROR_GO_ENDFUNC;

		Status = VL53L1_enable_xtalk_compensation(Dev);

	} else
		/* return error because no valid data found */
		Status = VL53L1_ERROR_XTALK_EXTRACTION_NO_SAMPLE_FAIL;

ENDFUNC:

	return Status;

}

/* Check Rectangle in user's coordinate system:
 *	15	TL(x,y) o-----*
 *   ^			|     |
 *   |			*-----o BR(x,y)
 *   0------------------------- >15
 *   check Rectangle definition conforms to the (0,15,15) coordinate system
 *   with a minimum of 4x4 size
 */
static VL53L1_Error CheckValidRectRoi(VL53L1_UserRoi_t ROI)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;



	/* Negative check are not necessary because value is unsigned */
	if ((ROI.TopLeftX > 15) || (ROI.TopLeftY > 15) ||
		(ROI.BotRightX > 15) || (ROI.BotRightY > 15))
		Status = VL53L1_ERROR_INVALID_PARAMS;

	if ((ROI.TopLeftX > ROI.BotRightX) || (ROI.TopLeftY < ROI.BotRightY))
		Status = VL53L1_ERROR_INVALID_PARAMS;


	return Status;
}

static VL53L1_GPIO_Interrupt_Mode ConvertModeToLLD(VL53L1_Error *pStatus,
		VL53L1_ThresholdMode CrossMode)
{
	VL53L1_GPIO_Interrupt_Mode Mode;

	switch (CrossMode) {
	case VL53L1_THRESHOLD_CROSSED_LOW:
		Mode = VL53L1_GPIOINTMODE_LEVEL_LOW;
		break;
	case VL53L1_THRESHOLD_CROSSED_HIGH:
		Mode = VL53L1_GPIOINTMODE_LEVEL_HIGH;
		break;
	case VL53L1_THRESHOLD_OUT_OF_WINDOW:
		Mode = VL53L1_GPIOINTMODE_OUT_OF_WINDOW;
		break;
	case VL53L1_THRESHOLD_IN_WINDOW:
		Mode = VL53L1_GPIOINTMODE_IN_WINDOW;
		break;
	default:
		/* define Mode to avoid warning but actual value doesn't mind */
		Mode = VL53L1_GPIOINTMODE_LEVEL_HIGH;
		*pStatus = VL53L1_ERROR_INVALID_PARAMS;
	}
	return Mode;
}

static VL53L1_ThresholdMode ConvertModeFromLLD(VL53L1_Error *pStatus,
		VL53L1_GPIO_Interrupt_Mode CrossMode)
{
	VL53L1_ThresholdMode Mode;

	switch (CrossMode) {
	case VL53L1_GPIOINTMODE_LEVEL_LOW:
		Mode = VL53L1_THRESHOLD_CROSSED_LOW;
		break;
	case VL53L1_GPIOINTMODE_LEVEL_HIGH:
		Mode = VL53L1_THRESHOLD_CROSSED_HIGH;
		break;
	case VL53L1_GPIOINTMODE_OUT_OF_WINDOW:
		Mode = VL53L1_THRESHOLD_OUT_OF_WINDOW;
		break;
	case VL53L1_GPIOINTMODE_IN_WINDOW:
		Mode = VL53L1_THRESHOLD_IN_WINDOW;
		break;
	default:
		/* define Mode to avoid warning but actual value doesn't mind */
		Mode = VL53L1_THRESHOLD_CROSSED_HIGH;
		*pStatus = VL53L1_ERROR_UNDEFINED;
	}
	return Mode;
}

static VL53L1_Error ComputeDevicePresetMode(
		VL53L1_PresetModes PresetMode,
		VL53L1_DistanceModes DistanceMode,
		VL53L1_DevicePresetModes *pDevicePresetMode)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;

	uint8_t DistIdx;
	VL53L1_DevicePresetModes LightModes[3] = {
		VL53L1_DEVICEPRESETMODE_STANDARD_RANGING_SHORT_RANGE,
		VL53L1_DEVICEPRESETMODE_STANDARD_RANGING,
		VL53L1_DEVICEPRESETMODE_STANDARD_RANGING_LONG_RANGE};


	VL53L1_DevicePresetModes TimedModes[3] = {
		VL53L1_DEVICEPRESETMODE_TIMED_RANGING_SHORT_RANGE,
		VL53L1_DEVICEPRESETMODE_TIMED_RANGING,
		VL53L1_DEVICEPRESETMODE_TIMED_RANGING_LONG_RANGE};

	VL53L1_DevicePresetModes LowPowerTimedModes[3] = {
		VL53L1_DEVICEPRESETMODE_LOWPOWERAUTO_SHORT_RANGE,
		VL53L1_DEVICEPRESETMODE_LOWPOWERAUTO_MEDIUM_RANGE,
		VL53L1_DEVICEPRESETMODE_LOWPOWERAUTO_LONG_RANGE};

	*pDevicePresetMode = VL53L1_DEVICEPRESETMODE_STANDARD_RANGING;

	switch (DistanceMode) {
	case VL53L1_DISTANCEMODE_SHORT:
		DistIdx = 0;
		break;
	case VL53L1_DISTANCEMODE_MEDIUM:
		DistIdx = 1;
		break;
	default:
		DistIdx = 2;
	}

	switch (PresetMode) {
	case VL53L1_PRESETMODE_LITE_RANGING:
		*pDevicePresetMode = LightModes[DistIdx];
		break;


	case VL53L1_PRESETMODE_AUTONOMOUS:
		*pDevicePresetMode = TimedModes[DistIdx];
		break;

	case VL53L1_PRESETMODE_LOWPOWER_AUTONOMOUS:
		*pDevicePresetMode = LowPowerTimedModes[DistIdx];
		break;

	default:
		/* Unsupported mode */
		Status = VL53L1_ERROR_MODE_NOT_SUPPORTED;
	}

	return Status;
}

static VL53L1_Error SetPresetMode(VL53L1_DEV Dev,
		VL53L1_PresetModes PresetMode,
		VL53L1_DistanceModes DistanceMode,
		uint32_t inter_measurement_period_ms)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	VL53L1_DevicePresetModes   device_preset_mode;
	uint8_t measurement_mode;
	uint16_t dss_config__target_total_rate_mcps;
	uint32_t phasecal_config_timeout_us;
	uint32_t mm_config_timeout_us;
	uint32_t lld_range_config_timeout_us;

//	LOG_FUNCTION_START("%d", (int)PresetMode);

	if ((PresetMode == VL53L1_PRESETMODE_AUTONOMOUS) ||
		(PresetMode == VL53L1_PRESETMODE_LOWPOWER_AUTONOMOUS))
		measurement_mode  = VL53L1_DEVICEMEASUREMENTMODE_TIMED;
	else
		measurement_mode  = VL53L1_DEVICEMEASUREMENTMODE_BACKTOBACK;


	Status = ComputeDevicePresetMode(PresetMode, DistanceMode,
			&device_preset_mode);

	if (Status == VL53L1_ERROR_NONE)
		Status =  VL53L1_get_preset_mode_timing_cfg(Dev,
				device_preset_mode,
				&dss_config__target_total_rate_mcps,
				&phasecal_config_timeout_us,
				&mm_config_timeout_us,
				&lld_range_config_timeout_us);

	if (Status == VL53L1_ERROR_NONE)
		Status = VL53L1_set_preset_mode(
				Dev,
				device_preset_mode,
				dss_config__target_total_rate_mcps,
				phasecal_config_timeout_us,
				mm_config_timeout_us,
				lld_range_config_timeout_us,
				inter_measurement_period_ms);

	if (Status == VL53L1_ERROR_NONE)
		VL53L1DevDataSet(Dev, LLData.measurement_mode, measurement_mode);

	if (Status == VL53L1_ERROR_NONE)
		VL53L1DevDataSet(Dev, CurrentParameters.PresetMode, PresetMode);


	return Status;
}


/* Group PAL General Functions */

/** @defgroup VL53L1_cut11_group VL53L1 cut1.1 Function Definition
 *  @brief    VL53L1 cut1.1 Function Definition
 *  @{
 */

/** @defgroup VL53L1_general_group VL53L1 General Functions
 *  @brief    General functions and definitions
 *  @{
 */

/**
 * @brief Return the VL53L1 driver Version
 *
 * @note This function doesn't access to the device
 *
 * @param   pVersion              Rer to current driver Version
 * @return  VL53L1_ERROR_NONE     Success
 * @return  "Other error code"    See ::VL53L1_Error
 */
VL53L1_Error VL53L1_GetVersion(VL53L1_Version_t *pVersion)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;



	pVersion->major = VL53L1_IMPLEMENTATION_VER_MAJOR;
	pVersion->minor = VL53L1_IMPLEMENTATION_VER_MINOR;
	pVersion->build = VL53L1_IMPLEMENTATION_VER_SUB;

	pVersion->revision = VL53L1_IMPLEMENTATION_VER_REVISION;


	return Status;
}

/**
 * @brief Reads the Product Revision for a for given Device
 * This function can be used to distinguish cut1.0 from cut1.1.
 *
 * @param   Dev                 Device Handle
 * @param   pProductRevisionMajor  Pointer to Product Revision Major
 * for a given Device
 * @param   pProductRevisionMinor  Pointer to Product Revision Minor
 * for a given Device
 * @return  VL53L1_ERROR_NONE        Success
 * @return  "Other error code"    See ::VL53L1_Error
 */
VL53L1_Error VL53L1_GetProductRevision(VL53L1_DEV Dev,
	uint8_t *pProductRevisionMajor, uint8_t *pProductRevisionMinor)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	uint8_t revision_id;
	VL53L1_LLDriverData_t   *pLLData;



	pLLData =  VL53L1DevStructGetLLDriverHandle(Dev);
	revision_id = pLLData->nvm_copy_data.identification__revision_id;
	*pProductRevisionMajor = 1;
	*pProductRevisionMinor = (revision_id & 0xF0) >> 4;


	return Status;

}
/**
 * @brief Reads the Device information for given Device
 *
 * @note This function Access to the device
 *
 * @param   Dev                 Device Handle
 * @param   pVL53L1_DeviceInfo  Pointer to current device info for a given
 *  Device
 * @return  VL53L1_ERROR_NONE   Success
 * @return  "Other error code"  See ::VL53L1_Error
 */
VL53L1_Error VL53L1_GetDeviceInfo(VL53L1_DEV Dev,
	VL53L1_DeviceInfo_t *pVL53L1_DeviceInfo)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	uint8_t revision_id;
	VL53L1_LLDriverData_t   *pLLData;



	pLLData =  VL53L1DevStructGetLLDriverHandle(Dev);

	strncpy(pVL53L1_DeviceInfo->ProductId, "",
			VL53L1_DEVINFO_STRLEN-1);
	pVL53L1_DeviceInfo->ProductType =
			pLLData->nvm_copy_data.identification__module_type;

	revision_id = pLLData->nvm_copy_data.identification__revision_id;
	pVL53L1_DeviceInfo->ProductRevisionMajor = 1;
	pVL53L1_DeviceInfo->ProductRevisionMinor = (revision_id & 0xF0) >> 4;

#ifndef VL53L1_USE_EMPTY_STRING
	if (pVL53L1_DeviceInfo->ProductRevisionMinor == 0)
		strncpy(pVL53L1_DeviceInfo->Name,
				VL53L1_STRING_DEVICE_INFO_NAME0,
				VL53L1_DEVINFO_STRLEN-1);
	else
		strncpy(pVL53L1_DeviceInfo->Name,
				VL53L1_STRING_DEVICE_INFO_NAME1,
				VL53L1_DEVINFO_STRLEN-1);
	strncpy(pVL53L1_DeviceInfo->Type,
			VL53L1_STRING_DEVICE_INFO_TYPE,
			VL53L1_DEVINFO_STRLEN-1);
#else
	pVL53L1_DeviceInfo->Name[0] = 0;
	pVL53L1_DeviceInfo->Type[0] = 0;
#endif


	return Status;
}

/**
 * @brief Human readable Range Status string for a given RangeStatus
 *
 * @note This function doesn't access to the device
 *
 * @param   RangeStatus         The RangeStatus code as stored on
 *  VL53L1_RangingMeasurementData_t
 * @param   pRangeStatusString  The returned RangeStatus string. Shall be
 * defined as char buf[VL53L1_MAX_STRING_LENGTH]
 * @return  VL53L1_ERROR_NONE   Success
 * @return  "Other error code"  See ::VL53L1_Error
 */

VL53L1_Error VL53L1_GetRangeStatusString(uint8_t RangeStatus,
	char *pRangeStatusString)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;



	Status = VL53L1_get_range_status_string(RangeStatus,
		pRangeStatusString);


	return Status;
}
/**
 * @brief Human readable error string for driver error status
 *
 * @note This function doesn't access to the device
 *
 * @param   PalErrorCode       The error code as stored on  VL53L1_Error
 * @param   pPalErrorString    The error string corresponding to the
 * PalErrorCode. Shall be defined as char buf[VL53L1_MAX_STRING_LENGTH]
 * @return  VL53L1_ERROR_NONE  Success
 * @return  "Other error code" See ::VL53L1_Error
 */
VL53L1_Error VL53L1_GetPalErrorString(VL53L1_Error PalErrorCode,
	char *pPalErrorString)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;



	Status = VL53L1_get_pal_error_string(PalErrorCode, pPalErrorString);


	return Status;
}
/**
 * @brief Human readable driver State string
 *
 * @note This function doesn't access to the device
 *
 * @param   PalStateCode          The State code as stored on  VL53L1_State
 * @param   pPalStateString       The State string corresponding to the
 * PalStateCode. Shall be defined as char buf[VL53L1_MAX_STRING_LENGTH]
 * @return  VL53L1_ERROR_NONE     Success
 * @return  "Other error code"    See ::VL53L1_Error
 */
VL53L1_Error VL53L1_GetPalStateString(VL53L1_State PalStateCode,
	char *pPalStateString)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;



	Status = VL53L1_get_pal_state_string(PalStateCode, pPalStateString);


	return Status;
}

/**
 * @brief Reads the internal state of the driver for a given Device
 *
 * @note This function doesn't access to the device
 *
 * @param   Dev                   Device Handle
 * @param   pPalState             Pointer to current state of the PAL for a
 * given Device
 * @return  VL53L1_ERROR_NONE     Success
 * @return  "Other error code"    See ::VL53L1_Error
 */
VL53L1_Error VL53L1_GetPalState(VL53L1_DEV Dev, VL53L1_State *pPalState)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;



	*pPalState = VL53L1DevDataGet(Dev, PalState);


	return Status;
}
/** @} VL53L1_general_group */

/** @defgroup VL53L1_init_group VL53L1 Init Functions
 *  @brief    VL53L1 Init Functions
 *  @{
 */

/**
 * @brief Set new device address
 *
 * After completion the device will answer to the new address programmed.
 * This function should be called when several devices are used in parallel
 * before start programming the sensor.
 * When a single device us used, there is no need to call this function.
 *
 * When it is requested for multi devices system this function MUST be called
 * prior to VL53L1_DataInit()
 *
 * @note This function Access to the device
 *
 * @param   Dev                   Device Handle
 * @param   DeviceAddress         The new Device address
 * @return  VL53L1_ERROR_NONE     Success
 * @return  "Other error code"    See ::VL53L1_Error
 */
VL53L1_Error VL53L1_SetDeviceAddress(VL53L1_DEV Dev, uint8_t DeviceAddress)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;



	Status = VL53L1_WrByte(Dev, VL53L1_I2C_SLAVE__DEVICE_ADDRESS,
			DeviceAddress / 2);


	return Status;
}

/**
 *
 * @brief One time device initialization
 *
 * To be called once and only once after device is brought out of reset
 * (Chip enable) and booted se VL53L1_WaitDeviceBooted()
 *
 * @par Function Description
 * When not used after a fresh device "power up" or reset, it may return
 VL53L1_ERROR_CALIBRATION_WARNING meaning wrong calibration data
 * may have been fetched from device that can result in ranging offset error\n
 * If application cannot execute device reset or need to run VL53L1_DataInit
 * multiple time then it  must ensure proper offset calibration saving and
 * restore on its own by using  VL53L1_GetOffsetCalibrationData() on first
 * power up and then  VL53L1_SetOffsetCalibrationData() in all subsequent init
 * This function will change the VL53L1_State from VL53L1_STATE_POWERDOWN to
 * VL53L1_STATE_WAIT_STATICINIT.
 *
 * @note This function Access to the device
 *
 * @param   Dev                   Device Handle
 * @return  VL53L1_ERROR_NONE     Success
 * @return  "Other error code"    See ::VL53L1_Error
 */
VL53L1_Error VL53L1_DataInit(VL53L1_DEV Dev)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	uint8_t i;



	/* 2V8 power mode selection codex 447463 */
#ifdef USE_I2C_2V8
	Status = VL53L1_RdByte(Dev, VL53L1_PAD_I2C_HV__EXTSUP_CONFIG, &i);
	if (Status == VL53L1_ERROR_NONE) {
		i = (i & 0xfe) | 0x01;
		Status = VL53L1_WrByte(Dev, VL53L1_PAD_I2C_HV__EXTSUP_CONFIG,
				i);
	}
#endif

	if (Status == VL53L1_ERROR_NONE)
		Status = VL53L1_data_init(Dev, 1);

	if (Status == VL53L1_ERROR_NONE) {
		VL53L1DevDataSet(Dev, PalState, VL53L1_STATE_WAIT_STATICINIT);
		VL53L1DevDataSet(Dev, CurrentParameters.PresetMode,
				VL53L1_PRESETMODE_LOWPOWER_AUTONOMOUS);
	}

	/* Enable all check */
	for (i = 0; i < VL53L1_CHECKENABLE_NUMBER_OF_CHECKS; i++) {
		if (Status == VL53L1_ERROR_NONE)
			Status |= VL53L1_SetLimitCheckEnable(Dev, i, 1);
		else
			break;

	}
	/* Limit default values */
	if (Status == VL53L1_ERROR_NONE) {
		Status = VL53L1_SetLimitCheckValue(Dev,
			VL53L1_CHECKENABLE_SIGMA_FINAL_RANGE,
				(FixPoint1616_t)(18 * 65536));
	}

	if (Status == VL53L1_ERROR_NONE) {
		Status = VL53L1_SetLimitCheckValue(Dev,
			VL53L1_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,
				(FixPoint1616_t)(25 * 65536 / 100));
				/* 0.25 * 65536 */
	}



	return Status;
}

/**
 * @brief Do basic device init (and eventually patch loading)
 * This function will change the VL53L1_State from
 * VL53L1_STATE_WAIT_STATICINIT to VL53L1_STATE_IDLE.
 * In this stage all default setting will be applied.
 *
 * @note This function Access to the device
 *
 * @param   Dev                   Device Handle
 * @return  VL53L1_ERROR_NONE     Success
 * @return  "Other error code"    See ::VL53L1_Error
 */
VL53L1_Error VL53L1_StaticInit(VL53L1_DEV Dev)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	uint8_t  measurement_mode;



	VL53L1DevDataSet(Dev, PalState, VL53L1_STATE_IDLE);

	measurement_mode  = VL53L1_DEVICEMEASUREMENTMODE_BACKTOBACK;
	VL53L1DevDataSet(Dev, LLData.measurement_mode, measurement_mode);

	VL53L1DevDataSet(Dev, CurrentParameters.NewDistanceMode,
			VL53L1_DISTANCEMODE_LONG);

	VL53L1DevDataSet(Dev, CurrentParameters.InternalDistanceMode,
			VL53L1_DISTANCEMODE_LONG);

	VL53L1DevDataSet(Dev, CurrentParameters.DistanceMode,
			VL53L1_DISTANCEMODE_LONG);

	/* ticket 472728 fix */
	Status = VL53L1_SetPresetMode(Dev,
			VL53L1_PRESETMODE_LOWPOWER_AUTONOMOUS);
	/* end of ticket 472728 fix */

	return Status;
}

/**
 * @brief Wait for device booted after chip enable (hardware standby)
 * This function can be run only when VL53L1_State is VL53L1_STATE_POWERDOWN.
 *
 * @param   Dev      Device Handle
 * @return  VL53L1_ERROR_NONE     Success
 * @return  "Other error code"    See ::VL53L1_Error
 *
 */
VL53L1_Error VL53L1_WaitDeviceBooted(VL53L1_DEV Dev)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;



	Status = VL53L1_poll_for_boot_completion(Dev,
			VL53L1_BOOT_COMPLETION_POLLING_TIMEOUT_MS);


	return Status;
}
/** @} VL53L1_init_group */

/** @defgroup VL53L1_parameters_group VL53L1 Parameters Functions
 *  @brief    Functions used to prepare and setup the device
 *  @{
 */

/**
 * @brief  Set a new Preset Mode
 * @par Function Description
 * Set device to a new Operating Mode (High speed ranging, Multi objects ...)
 *
 * @note This function doesn't Access to the device
 *
 * @warning This function change the timing budget to 16 ms and the inter-
 * measurement period to 1000 ms. Also the VL53L1_DISTANCEMODE_LONG is used.
 *
 * @param   Dev                   Device Handle
 * @param   PresetMode            New Preset mode to apply
 * <br>Valid values are:
 */
/**
 * @li VL53L1_PRESETMODE_LITE_RANGING
 * @li VL53L1_PRESETMODE_AUTONOMOUS
 * @li VL53L1_PRESETMODE_LOWPOWER_AUTONOMOUS
 */
/**
 *
 * @return  VL53L1_ERROR_NONE               Success
 * @return  VL53L1_ERROR_MODE_NOT_SUPPORTED This error occurs when PresetMode is
 *                                          not in the supported list
 */

VL53L1_Error VL53L1_SetPresetMode(VL53L1_DEV Dev, VL53L1_PresetModes PresetMode)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	VL53L1_DistanceModes DistanceMode = VL53L1_DISTANCEMODE_LONG;

//	LOG_FUNCTION_START("%d", (int)PresetMode);

	Status = SetPresetMode(Dev,
			PresetMode,
			DistanceMode,
			1000);

	if (Status == VL53L1_ERROR_NONE) {
		VL53L1DevDataSet(Dev, CurrentParameters.InternalDistanceMode,
				DistanceMode);

		VL53L1DevDataSet(Dev, CurrentParameters.NewDistanceMode,
				DistanceMode);

		if ((PresetMode == VL53L1_PRESETMODE_LITE_RANGING) ||
			(PresetMode == VL53L1_PRESETMODE_AUTONOMOUS) ||
			(PresetMode == VL53L1_PRESETMODE_LOWPOWER_AUTONOMOUS))
			Status = VL53L1_SetMeasurementTimingBudgetMicroSeconds(
				Dev, 41000);
		else
			/* Set default timing budget to 30Hz (33.33 ms)*/
			Status = VL53L1_SetMeasurementTimingBudgetMicroSeconds(
				Dev, 33333);
	}

	if (Status == VL53L1_ERROR_NONE) {
		/* Set default intermeasurement period to 1000 ms */
		Status = VL53L1_SetInterMeasurementPeriodMilliSeconds(Dev,
				1000);
	}


	return Status;
}


VL53L1_Error VL53L1_GetPresetMode(VL53L1_DEV Dev,
	VL53L1_PresetModes *pPresetMode)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;



	*pPresetMode = VL53L1DevDataGet(Dev, CurrentParameters.PresetMode);


	return Status;
}

/**
 * @brief  Set the distance mode
 * @par Function Description
 * Set the distance mode to be used for the next ranging.<br>
 * The modes Short, Medium and Long are used to optimize the ranging accuracy
 * in a specific range of distance.<br> The user select one of these modes to
 * select the distance range.
 * @note This function doesn't Access to the device
 *
 * @warning This function should be called after  VL53L1_SetPresetMode().

 * @param   Dev                   Device Handle
 * @param   DistanceMode          Distance mode to apply, valid values are:
 * @li VL53L1_DISTANCEMODE_SHORT
 * @li VL53L1_DISTANCEMODE_MEDIUM
 * @li VL53L1_DISTANCEMODE_LONG
 * @return  VL53L1_ERROR_NONE               Success
 * @return  VL53L1_ERROR_MODE_NOT_SUPPORTED This error occurs when DistanceMode
 *                                          is not in the supported list
 * @return  "Other error code"              See ::VL53L1_Error
 */
VL53L1_Error VL53L1_SetDistanceMode(VL53L1_DEV Dev,
		VL53L1_DistanceModes DistanceMode)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	VL53L1_PresetModes PresetMode;
	VL53L1_DistanceModes InternalDistanceMode;
	uint32_t inter_measurement_period_ms;
	uint32_t TimingBudget;
	uint32_t MmTimeoutUs;
	uint32_t PhaseCalTimeoutUs;
	VL53L1_user_zone_t user_zone;

//	LOG_FUNCTION_START("%d", (int)DistanceMode);

	PresetMode = VL53L1DevDataGet(Dev, CurrentParameters.PresetMode);

	/* when the distance mode is valid:
	 * Manual Mode: all modes
	 * AUTO AUTO_LITE : LITE_RANGING, RANGING
	 */

	if ((DistanceMode != VL53L1_DISTANCEMODE_SHORT) &&
		(DistanceMode != VL53L1_DISTANCEMODE_MEDIUM) &&
		(DistanceMode != VL53L1_DISTANCEMODE_LONG))
		return VL53L1_ERROR_INVALID_PARAMS;

	/* The internal distance mode is limited to Short, Medium or
	 * long only
	*/
	if (Status == VL53L1_ERROR_NONE) {
		if ((DistanceMode == VL53L1_DISTANCEMODE_SHORT) ||
			(DistanceMode == VL53L1_DISTANCEMODE_MEDIUM))
			InternalDistanceMode = DistanceMode;
		else /* (DistanceMode == VL53L1_DISTANCEMODE_LONG) */
			InternalDistanceMode = VL53L1_DISTANCEMODE_LONG;
	}

	if (Status == VL53L1_ERROR_NONE)
		Status = VL53L1_get_user_zone(Dev, &user_zone);

	inter_measurement_period_ms =  VL53L1DevDataGet(Dev,
				LLData.inter_measurement_period_ms);

	if (Status == VL53L1_ERROR_NONE)
		Status = VL53L1_get_timeouts_us(Dev, &PhaseCalTimeoutUs,
			&MmTimeoutUs, &TimingBudget);

	if (Status == VL53L1_ERROR_NONE)
		Status = SetPresetMode(Dev,
				PresetMode,
				InternalDistanceMode,
				inter_measurement_period_ms);

	if (Status == VL53L1_ERROR_NONE) {
		VL53L1DevDataSet(Dev, CurrentParameters.InternalDistanceMode,
				InternalDistanceMode);
		VL53L1DevDataSet(Dev, CurrentParameters.NewDistanceMode,
				InternalDistanceMode);
		VL53L1DevDataSet(Dev, CurrentParameters.DistanceMode,
				DistanceMode);
	}

	if (Status == VL53L1_ERROR_NONE) {
		Status = VL53L1_set_timeouts_us(Dev, PhaseCalTimeoutUs,
			MmTimeoutUs, TimingBudget);

		if (Status == VL53L1_ERROR_NONE)
			VL53L1DevDataSet(Dev, LLData.range_config_timeout_us,
				TimingBudget);
	}

	if (Status == VL53L1_ERROR_NONE)
		Status = VL53L1_set_user_zone(Dev, &user_zone);


	return Status;
}
/**
 * @brief  Get the distance mode
 * @par Function Description
 * Get the distance mode used for the next ranging.
 *
 * @param   Dev                   Device Handle
 * @param   *pDistanceMode        Pointer to Distance mode
 * @return  VL53L1_ERROR_NONE            Success
 * @return  "Other error code"           See ::VL53L1_Error
 */
VL53L1_Error VL53L1_GetDistanceMode(VL53L1_DEV Dev,
	VL53L1_DistanceModes *pDistanceMode)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;



	*pDistanceMode = VL53L1DevDataGet(Dev, CurrentParameters.DistanceMode);


	return Status;
}


/**
 * @brief Set Ranging Timing Budget in microseconds
 *
 * @par Function Description
 * Defines the maximum time allowed by the user to the device to run a
 * full ranging sequence for the current mode (ranging, histogram, ASL ...)
 *
 * @param   Dev                                Device Handle
 * @param MeasurementTimingBudgetMicroSeconds  Max measurement time in
 * microseconds.
 * @return  VL53L1_ERROR_NONE            Success
 * @return  VL53L1_ERROR_INVALID_PARAMS  Error timing parameter not
 *                                       supported.
 *                                       The maximum accepted value for the
 *                                       computed timing budget is 10 seconds
 *                                       the minimum value depends on the preset
 *                                       mode selected.
 * @return  "Other error code"           See ::VL53L1_Error
 */

VL53L1_Error VL53L1_SetMeasurementTimingBudgetMicroSeconds(VL53L1_DEV Dev,
	uint32_t MeasurementTimingBudgetMicroSeconds)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	uint8_t Mm1Enabled;
	uint8_t Mm2Enabled;
	uint32_t TimingGuard;
	uint32_t divisor;
	uint32_t TimingBudget;
	uint32_t MmTimeoutUs;
	VL53L1_PresetModes PresetMode;
	uint32_t PhaseCalTimeoutUs;
	uint32_t vhv;
	int32_t vhv_loops;
	uint32_t FDAMaxTimingBudgetUs = FDA_MAX_TIMING_BUDGET_US;



	/* Timing budget is limited to 10 seconds */
	if (MeasurementTimingBudgetMicroSeconds > 10000000)
		Status = VL53L1_ERROR_INVALID_PARAMS;

	if (Status == VL53L1_ERROR_NONE) {
		Status = VL53L1_GetSequenceStepEnable(Dev,
			VL53L1_SEQUENCESTEP_MM1, &Mm1Enabled);
	}

	if (Status == VL53L1_ERROR_NONE) {
		Status = VL53L1_GetSequenceStepEnable(Dev,
			VL53L1_SEQUENCESTEP_MM2, &Mm2Enabled);
	}

	if (Status == VL53L1_ERROR_NONE)
		Status = VL53L1_get_timeouts_us(Dev,
			&PhaseCalTimeoutUs,
			&MmTimeoutUs,
			&TimingBudget);

	if (Status == VL53L1_ERROR_NONE) {
		PresetMode = VL53L1DevDataGet(Dev, CurrentParameters.PresetMode);

		TimingGuard = 0;
		divisor = 1;
		switch (PresetMode) {
		case VL53L1_PRESETMODE_LITE_RANGING:
			if ((Mm1Enabled == 1) || (Mm2Enabled == 1))
				TimingGuard = 5000;
			else
				TimingGuard = 1000;
		break;

		case VL53L1_PRESETMODE_AUTONOMOUS:
			FDAMaxTimingBudgetUs *= 2;
			if ((Mm1Enabled == 1) || (Mm2Enabled == 1))
				TimingGuard = 26600;
			else
				TimingGuard = 21600;
			divisor = 2;
		break;

		case VL53L1_PRESETMODE_LOWPOWER_AUTONOMOUS:
			FDAMaxTimingBudgetUs *= 2;
			vhv = LOWPOWER_AUTO_VHV_LOOP_DURATION_US;
			VL53L1_get_tuning_parm(Dev,
				VL53L1_TUNINGPARM_LOWPOWERAUTO_VHV_LOOP_BOUND,
				&vhv_loops);
			if (vhv_loops > 0) {
				vhv += vhv_loops *
					LOWPOWER_AUTO_VHV_LOOP_DURATION_US;
			}
			TimingGuard = LOWPOWER_AUTO_OVERHEAD_BEFORE_A_RANGING +
				LOWPOWER_AUTO_OVERHEAD_BETWEEN_A_B_RANGING +
				vhv;
			divisor = 2;
		break;

		default:
			/* Unsupported mode */
			Status = VL53L1_ERROR_MODE_NOT_SUPPORTED;
		}

		if (MeasurementTimingBudgetMicroSeconds <= TimingGuard)
			Status = VL53L1_ERROR_INVALID_PARAMS;
		else {
			TimingBudget = (MeasurementTimingBudgetMicroSeconds
					- TimingGuard);
		}

		if (Status == VL53L1_ERROR_NONE) {
			if (TimingBudget > FDAMaxTimingBudgetUs)
				Status = VL53L1_ERROR_INVALID_PARAMS;
			else {
				TimingBudget /= divisor;
				Status = VL53L1_set_timeouts_us(
					Dev,
					PhaseCalTimeoutUs,
					MmTimeoutUs,
					TimingBudget);
			}

			if (Status == VL53L1_ERROR_NONE)
				VL53L1DevDataSet(Dev,
					LLData.range_config_timeout_us,
					TimingBudget);
		}
	}
	if (Status == VL53L1_ERROR_NONE) {
		VL53L1DevDataSet(Dev,
			CurrentParameters.MeasurementTimingBudgetMicroSeconds,
			MeasurementTimingBudgetMicroSeconds);
	}


	return Status;
}
/**
 * @brief Get Ranging Timing Budget in microseconds
 *
 * @par Function Description
 * Returns the programmed the maximum time allowed by the user to the
 * device to run a full ranging sequence for the current mode
 * (ranging, histogram, ASL ...)
 *
 * @param   Dev                                    Device Handle
 * @param   pMeasurementTimingBudgetMicroSeconds   Max measurement time in
 * microseconds.
 * @return  VL53L1_ERROR_NONE            Success
 * @return  "Other error code"           See ::VL53L1_Error
 */
VL53L1_Error VL53L1_GetMeasurementTimingBudgetMicroSeconds(VL53L1_DEV Dev,
	uint32_t *pMeasurementTimingBudgetMicroSeconds)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	uint8_t Mm1Enabled = 0;
	uint8_t Mm2Enabled = 0;
	uint32_t  MmTimeoutUs = 0;
	uint32_t  RangeTimeoutUs = 0;
	uint32_t  MeasTimingBdg = 0;
	uint32_t PhaseCalTimeoutUs = 0;
	VL53L1_PresetModes PresetMode;
	uint32_t TimingGuard;
	uint32_t vhv;
	int32_t vhv_loops;



	*pMeasurementTimingBudgetMicroSeconds = 0;

	if (Status == VL53L1_ERROR_NONE)
		Status = VL53L1_GetSequenceStepEnable(Dev,
			VL53L1_SEQUENCESTEP_MM1, &Mm1Enabled);

	if (Status == VL53L1_ERROR_NONE)
		Status = VL53L1_GetSequenceStepEnable(Dev,
			VL53L1_SEQUENCESTEP_MM2, &Mm2Enabled);

	if (Status == VL53L1_ERROR_NONE)
		Status = VL53L1_get_timeouts_us(Dev,
			&PhaseCalTimeoutUs,
			&MmTimeoutUs,
			&RangeTimeoutUs);

	if (Status == VL53L1_ERROR_NONE) {
		PresetMode = VL53L1DevDataGet(Dev, CurrentParameters.PresetMode);

		switch (PresetMode) {
		case VL53L1_PRESETMODE_LITE_RANGING:
			if ((Mm1Enabled == 1) || (Mm2Enabled == 1))
				MeasTimingBdg = RangeTimeoutUs + 5000;
			else
				MeasTimingBdg = RangeTimeoutUs + 1000;

		break;

		case VL53L1_PRESETMODE_AUTONOMOUS:
			if ((Mm1Enabled == 1) || (Mm2Enabled == 1))
				MeasTimingBdg = 2 * RangeTimeoutUs + 26600;
			else
				MeasTimingBdg = 2 * RangeTimeoutUs + 21600;

		break;

		case VL53L1_PRESETMODE_LOWPOWER_AUTONOMOUS:
			vhv = LOWPOWER_AUTO_VHV_LOOP_DURATION_US;
			VL53L1_get_tuning_parm(Dev,
				VL53L1_TUNINGPARM_LOWPOWERAUTO_VHV_LOOP_BOUND,
				&vhv_loops);
			if (vhv_loops > 0) {
				vhv += vhv_loops *
					LOWPOWER_AUTO_VHV_LOOP_DURATION_US;
			}
			TimingGuard = LOWPOWER_AUTO_OVERHEAD_BEFORE_A_RANGING +
				LOWPOWER_AUTO_OVERHEAD_BETWEEN_A_B_RANGING +
				vhv;
			MeasTimingBdg = 2 * RangeTimeoutUs + TimingGuard;
		break;

		default:
			/* Unsupported mode */
			Status = VL53L1_ERROR_MODE_NOT_SUPPORTED;
		}
	}
	if (Status == VL53L1_ERROR_NONE)
		*pMeasurementTimingBudgetMicroSeconds = MeasTimingBdg;


	return Status;
}

/**
 * Program continuous mode Inter-Measurement period in milliseconds
 *
 * @par Function Description
 * When trying to set too short time return  INVALID_PARAMS minimal value
 *
 * @param   Dev                                  Device Handle
 * @param   InterMeasurementPeriodMilliSeconds   Inter-Measurement Period in ms.
 *  this value should be greater than the duration set in
 *   VL53L1_SetMeasurementTimingBudgetMicroSeconds() to ensure smooth ranging
 *  operation.
 * @return  VL53L1_ERROR_NONE            Success
 * @return  "Other error code"           See ::VL53L1_Error
 */
VL53L1_Error VL53L1_SetInterMeasurementPeriodMilliSeconds(VL53L1_DEV Dev,
	uint32_t InterMeasurementPeriodMilliSeconds)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	uint32_t adjustedIMP;



	/* Fix for Ticket 468205 actual measurement period shorter than set */
	adjustedIMP = InterMeasurementPeriodMilliSeconds;
	adjustedIMP += (adjustedIMP * 64) / 1000;
	/* End of fix for Ticket 468205 */
	Status = VL53L1_set_inter_measurement_period_ms(Dev,
			adjustedIMP);


	return Status;
}

/**
 * Get continuous mode Inter-Measurement period in milliseconds
 *
 * @par Function Description
 *
 * @param   Dev                                  Device Handle
 * @param   pInterMeasurementPeriodMilliSeconds  Pointer to programmed
 *  Inter-Measurement Period in milliseconds.
 * @return  VL53L1_ERROR_NONE
 */
VL53L1_Error VL53L1_GetInterMeasurementPeriodMilliSeconds(VL53L1_DEV Dev,
	uint32_t *pInterMeasurementPeriodMilliSeconds)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	uint32_t adjustedIMP;



	Status = VL53L1_get_inter_measurement_period_ms(Dev, &adjustedIMP);
	/* Fix for Ticket 468205 actual measurement period shorter than set */
	adjustedIMP -= (adjustedIMP * 64) / 1000;
	*pInterMeasurementPeriodMilliSeconds = adjustedIMP;
	/* End of fix for Ticket 468205 */


	return Status;
}


/* End Group PAL Parameters Functions */


/* Group Limit check Functions */

VL53L1_Error VL53L1_GetNumberOfLimitCheck(uint16_t *pNumberOfLimitCheck)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;



	*pNumberOfLimitCheck = VL53L1_CHECKENABLE_NUMBER_OF_CHECKS;


	return Status;
}

/**
 * @brief  Return a description string for a given limit check number
 *
 * @par Function Description
 * This function returns a description string for a given limit check number.
 * The limit check is identified with the LimitCheckId.
 *
 * @param   LimitCheckId                  Limit Check ID
 (0<= LimitCheckId < VL53L1_GetNumberOfLimitCheck() ).
 * @param   pLimitCheckString             Pointer to the description string of
 * the given check limit. Shall be defined as char buf[VL53L1_MAX_STRING_LENGTH]
 * @return  VL53L1_ERROR_NONE            Success
 * @return  "Other error code"           See ::VL53L1_Error
 */
VL53L1_Error VL53L1_GetLimitCheckInfo(uint16_t LimitCheckId,
	char *pLimitCheckString)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;



	Status = VL53L1_get_limit_check_info(LimitCheckId,
		pLimitCheckString);


	return Status;
}

/**
 * @brief  Return a the Status of the specified check limit
 *
 * @par Function Description
 * This function returns the Status of the specified check limit.
 * The value indicate if the check is fail or not.
 * The limit check is identified with the LimitCheckId.
 *
 * @param   Dev                           Device Handle
 * @param   LimitCheckId                  Limit Check ID
 (0<= LimitCheckId < VL53L1_GetNumberOfLimitCheck() ).
 * @param   pLimitCheckStatus             Pointer to the
 Limit Check Status of the given check limit.
 * LimitCheckStatus :
 * 0 the check is not fail or not enabled
 * 1 the check if fail
 *
 * <p><ul>
 *    <li>VL53L1_CHECKENABLE_SIGMA_FINAL_RANGE: the sigma indicate the quality
 *    of the measure. The more it is little the better it is.
 *    The status is 1 when current sigma is greater then the limit.</li>
 *    <li>VL53L1_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE: the signal rate indicate
 *    the strength of the returned signal. The more it is big the better it is.
 *    The status is 1 when current signal is lower then the limit.</li>
 * </ul></p>
 *
 *
 * @return  VL53L1_ERROR_NONE            Success
 * @return  "Other error code"           See ::VL53L1_Error
 */

VL53L1_Error VL53L1_GetLimitCheckStatus(VL53L1_DEV Dev, uint16_t LimitCheckId,
	uint8_t *pLimitCheckStatus)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	uint8_t Temp8;



	if (LimitCheckId >= VL53L1_CHECKENABLE_NUMBER_OF_CHECKS) {
		Status = VL53L1_ERROR_INVALID_PARAMS;
	} else {
		VL53L1_GETARRAYPARAMETERFIELD(Dev, LimitChecksStatus,
			LimitCheckId, Temp8);
		*pLimitCheckStatus = Temp8;
	}


	return Status;
}

static VL53L1_Error SetLimitValue(VL53L1_DEV Dev, uint16_t LimitCheckId,
		FixPoint1616_t value)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	uint16_t tmpuint16; /* temporary variable */



	switch (LimitCheckId) {
	case VL53L1_CHECKENABLE_SIGMA_FINAL_RANGE:
		tmpuint16 = VL53L1_FIXPOINT1616TOFIXPOINT142(value);
		VL53L1_set_lite_sigma_threshold(Dev, tmpuint16);
		break;
	case VL53L1_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE:
		tmpuint16 = VL53L1_FIXPOINT1616TOFIXPOINT97(value);
		VL53L1_set_lite_min_count_rate(Dev, tmpuint16);
		break;
	default:
		Status = VL53L1_ERROR_INVALID_PARAMS;
	}


	return Status;
}

/**
 * @brief  Enable/Disable a specific limit check
 *
 * @par Function Description
 * This function Enable/Disable a specific limit check.
 * The limit check is identified with the LimitCheckId.
 *
 * @note This function doesn't Access to the device
 *
 * @param   Dev                           Device Handle
 * @param   LimitCheckId                  Limit Check ID
 *  (0<= LimitCheckId < VL53L1_GetNumberOfLimitCheck() ).
 * @param   LimitCheckEnable
 * @li set LimitCheckEnable=1 enables the LimitCheckId limit
 * @li set LimitCheckEnable=0 disables the LimitCheckId limit
 * @return  VL53L1_ERROR_NONE            Success
 * @return  VL53L1_ERROR_INVALID_PARAMS   This error is returned
 *  when LimitCheckId value is out of range.
 * @return  "Other error code"           See ::VL53L1_Error
 */

VL53L1_Error VL53L1_SetLimitCheckEnable(VL53L1_DEV Dev, uint16_t LimitCheckId,
	uint8_t LimitCheckEnable)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	FixPoint1616_t TempFix1616 = 0;




	if (LimitCheckId >= VL53L1_CHECKENABLE_NUMBER_OF_CHECKS) {
		Status = VL53L1_ERROR_INVALID_PARAMS;
	} else {
		/* TempFix1616 contains either 0 or the limit value */
		if (LimitCheckEnable == 0)
			TempFix1616 = 0;
		else
			VL53L1_GETARRAYPARAMETERFIELD(Dev, LimitChecksValue,
				LimitCheckId, TempFix1616);

		Status = SetLimitValue(Dev, LimitCheckId, TempFix1616);
	}

	if (Status == VL53L1_ERROR_NONE)
		VL53L1_SETARRAYPARAMETERFIELD(Dev,
			LimitChecksEnable,
			LimitCheckId,
			((LimitCheckEnable == 0) ? 0 : 1));




	return Status;
}
/**
 * @brief  Get specific limit check enable state
 *
 * @par Function Description
 * This function get the enable state of a specific limit check.
 * The limit check is identified with the LimitCheckId.
 *
 * @note This function Access to the device
 *
 * @param   Dev                           Device Handle
 * @param   LimitCheckId                  Limit Check ID
 *  (0<= LimitCheckId < VL53L1_GetNumberOfLimitCheck() ).
 * @param   pLimitCheckEnable             Pointer to the check limit enable
 * value.
 * @li if 1 the check limit corresponding to LimitCheckId is Enabled
 * @li if 0 the check limit corresponding to LimitCheckId is disabled
 * @return  VL53L1_ERROR_NONE            Success
 * @return  VL53L1_ERROR_INVALID_PARAMS   This error is returned
 *  when LimitCheckId value is out of range.
 * @return  "Other error code"           See ::VL53L1_Error
 */
VL53L1_Error VL53L1_GetLimitCheckEnable(VL53L1_DEV Dev, uint16_t LimitCheckId,
	uint8_t *pLimitCheckEnable)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	uint8_t Temp8;



	if (LimitCheckId >= VL53L1_CHECKENABLE_NUMBER_OF_CHECKS) {
		Status = VL53L1_ERROR_INVALID_PARAMS;
		*pLimitCheckEnable = 0;
	} else {
		VL53L1_GETARRAYPARAMETERFIELD(Dev, LimitChecksEnable,
			LimitCheckId, Temp8);
		*pLimitCheckEnable = Temp8;
	}



	return Status;
}

/**
 * @brief  Set a specific limit check value
 *
 * @par Function Description
 * This function set a specific limit check value.
 * The limit check is identified with the LimitCheckId.
 *
 * @note Note that the value written with that function will not be applied if
 * the limit is not enabled. In other words this function will not enable the
 * limit but change only the value. In case the limit is not enabled the value
 * is saved internally and applied with VL53L1_SetLimitCheckEnable.
 *
 * @param   Dev                           Device Handle
 * @param   LimitCheckId                  Limit Check ID
 *  (0<= LimitCheckId < VL53L1_GetNumberOfLimitCheck() ).
 * @param   LimitCheckValue               Limit check Value for a given
 * LimitCheckId
 * @return  VL53L1_ERROR_NONE            Success
 * @return  "Other error code"           See ::VL53L1_Error
 */
VL53L1_Error VL53L1_SetLimitCheckValue(VL53L1_DEV Dev, uint16_t LimitCheckId,
	FixPoint1616_t LimitCheckValue)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	uint8_t LimitChecksEnable;



	if (LimitCheckId >= VL53L1_CHECKENABLE_NUMBER_OF_CHECKS) {
		Status = VL53L1_ERROR_INVALID_PARAMS;
	} else {

		VL53L1_GETARRAYPARAMETERFIELD(Dev, LimitChecksEnable,
				LimitCheckId,
				LimitChecksEnable);

		if (LimitChecksEnable == 0) {
			/* disabled write only internal value */
			VL53L1_SETARRAYPARAMETERFIELD(Dev, LimitChecksValue,
				LimitCheckId, LimitCheckValue);
		} else {

			Status = SetLimitValue(Dev, LimitCheckId,
					LimitCheckValue);

			if (Status == VL53L1_ERROR_NONE) {
				VL53L1_SETARRAYPARAMETERFIELD(Dev,
					LimitChecksValue,
					LimitCheckId, LimitCheckValue);
			}
		}
	}


	return Status;
}

/**
 * @brief  Get a specific limit check value
 *
 * @par Function Description
 * This function get a specific limit check value from device then it updates
 * internal values and check enables.
 * The limit check is identified with the LimitCheckId.
 *
 * @note This function get the current value from device if zero then the value
 * returned is the one stored by the user, but in that case the check is store
 * as disabled. If the value from device is not zero, this is returned and set
 * into the memory at the same way that user call VL53L1_SetLimitCheckValue()
 *
 * @param   Dev                           Device Handle
 * @param   LimitCheckId                  Limit Check ID
 *  (0<= LimitCheckId < VL53L1_GetNumberOfLimitCheck() ).
 * @param   pLimitCheckValue              Pointer to Limit
 *  check Value for a given LimitCheckId.
 * @return  VL53L1_ERROR_NONE            Success
 * @return  "Other error code"           See ::VL53L1_Error
 */

VL53L1_Error VL53L1_GetLimitCheckValue(VL53L1_DEV Dev, uint16_t LimitCheckId,
	FixPoint1616_t *pLimitCheckValue)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	uint16_t MinCountRate;
	FixPoint1616_t TempFix1616;
	uint16_t SigmaThresh;



	switch (LimitCheckId) {
	case VL53L1_CHECKENABLE_SIGMA_FINAL_RANGE:
		Status = VL53L1_get_lite_sigma_threshold(Dev, &SigmaThresh);
		TempFix1616 = VL53L1_FIXPOINT142TOFIXPOINT1616(SigmaThresh);
		break;
	case VL53L1_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE:
		Status = VL53L1_get_lite_min_count_rate(Dev, &MinCountRate);
		TempFix1616 = VL53L1_FIXPOINT97TOFIXPOINT1616(MinCountRate);
		break;
	default:
		Status = VL53L1_ERROR_INVALID_PARAMS;
	}

	if (Status == VL53L1_ERROR_NONE) {

		if (TempFix1616 == 0) {
			/* disabled: return value from memory */
			VL53L1_GETARRAYPARAMETERFIELD(Dev,
				LimitChecksValue, LimitCheckId,
				TempFix1616);
			*pLimitCheckValue = TempFix1616;
			VL53L1_SETARRAYPARAMETERFIELD(Dev,
				LimitChecksEnable, LimitCheckId, 0);
		} else {
			*pLimitCheckValue = TempFix1616;
			VL53L1_SETARRAYPARAMETERFIELD(Dev,
				LimitChecksValue, LimitCheckId,
				TempFix1616);
			VL53L1_SETARRAYPARAMETERFIELD(Dev,
				LimitChecksEnable, LimitCheckId, 1);
		}
	}

	return Status;

}
/**
 * @brief  Get the current value of the signal used for the limit check
 *
 * @par Function Description
 * This function get a the current value of the signal used for the limit check.
 * To obtain the latest value you should run a valid ranging before.
 * The value reported is linked to the limit check identified with the
 * LimitCheckId.
 *
 * @param   Dev                           Device Handle
 * @param   LimitCheckId                  Limit Check ID
 *  (0<= LimitCheckId < VL53L1_GetNumberOfLimitCheck() ).
 * @param   pLimitCheckCurrent            Pointer to current Value for a
 * given LimitCheckId.
 * @return  VL53L1_ERROR_NONE            Success
 * @return  "Other error code"           See ::VL53L1_Error
 */

VL53L1_Error VL53L1_GetLimitCheckCurrent(VL53L1_DEV Dev, uint16_t LimitCheckId,
	FixPoint1616_t *pLimitCheckCurrent)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	FixPoint1616_t TempFix1616 = 0;



	if (LimitCheckId >= VL53L1_CHECKENABLE_NUMBER_OF_CHECKS) {
		Status = VL53L1_ERROR_INVALID_PARAMS;
	} else {
		VL53L1_GETARRAYPARAMETERFIELD(Dev, LimitChecksCurrent,
			LimitCheckId, TempFix1616);
		*pLimitCheckCurrent = TempFix1616;
	}


	return Status;

}

/** @} VL53L1_limitcheck_group */



/** @defgroup VL53L1_ROI_group VL53L1 ROI Functions
 *  @brief    Functions used to select ROIs
 *  @{
 */

/**
 * @brief Set the ROI  to be used for ranging
 *
 * @par Function Description
 * The user defined ROI is a rectangle described as per the following system
 * from the Top Left corner to the Bottom Right corner.
 * <br>Minimal ROI size is 4x4 spads
 * @image html roi_coord.png
 *
 * @param   Dev                      Device Handle
 * @param   pUserROi                 Pointer to the Structure definining the ROI
 * @return  VL53L1_ERROR_NONE            Success
 * @return  "Other error code"           See ::VL53L1_Error
 */
VL53L1_Error VL53L1_SetUserROI(VL53L1_DEV Dev,
		VL53L1_UserRoi_t *pRoi)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	VL53L1_user_zone_t user_zone;

	Status = CheckValidRectRoi(*pRoi);
	if (Status != VL53L1_ERROR_NONE)
		return VL53L1_ERROR_INVALID_PARAMS;

	user_zone.x_centre = (pRoi->BotRightX + pRoi->TopLeftX  + 1) / 2;
	user_zone.y_centre = (pRoi->TopLeftY  + pRoi->BotRightY + 1) / 2;
	user_zone.width =    (pRoi->BotRightX - pRoi->TopLeftX);
	user_zone.height =   (pRoi->TopLeftY  - pRoi->BotRightY);
	if ((user_zone.width < 3) || (user_zone.height < 3))
		Status = VL53L1_ERROR_INVALID_PARAMS;
	else
		Status =  VL53L1_set_user_zone(Dev, &user_zone);


	return Status;
}

/**
 * @brief Get the ROI managed by the Device
 *
 * @par Function Description
 * Get the ROI managed by the Device
 *
 * @param   Dev                   Device Handle
 * @param   pUserROi                 Pointer to the Structure definining the ROI
 * @return  VL53L1_ERROR_NONE            Success
 * @return  "Other error code"           See ::VL53L1_Error
 */
VL53L1_Error VL53L1_GetUserROI(VL53L1_DEV Dev,
		VL53L1_UserRoi_t *pRoi)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	VL53L1_user_zone_t	user_zone;

	Status = VL53L1_get_user_zone(Dev, &user_zone);

	pRoi->TopLeftX =  (2 * user_zone.x_centre - user_zone.width) >> 1;
	pRoi->TopLeftY =  (2 * user_zone.y_centre + user_zone.height) >> 1;
	pRoi->BotRightX = (2 * user_zone.x_centre + user_zone.width) >> 1;
	pRoi->BotRightY = (2 * user_zone.y_centre - user_zone.height) >> 1;


	return Status;
}

/** @} VL53L1_ROI_group */

/* \internal */
/** @defgroup VL53L1_sequencestep_group VL53L1 Sequence Step Functions
 *  @brief    Functions used to select Steps done on each ranging
 *  @{
 */

/**
 * @brief Gets number of sequence steps managed by the API.
 *
 * @par Function Description
 * This function retrieves the number of sequence steps currently managed
 * by the API
 *
 * @note This function Accesses the device
 *
 * @param   Dev                          Device Handle
 * @param   pNumberOfSequenceSteps       Out parameter reporting the number of
 *                                       sequence steps.
 * @return  VL53L1_ERROR_NONE            Success
 * @return  "Other error code"           See ::VL53L1_Error
 */
VL53L1_Error VL53L1_GetNumberOfSequenceSteps(VL53L1_DEV Dev,
	uint8_t *pNumberOfSequenceSteps)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;

	SUPPRESS_UNUSED_WARNING(Dev);



	*pNumberOfSequenceSteps = VL53L1_SEQUENCESTEP_NUMBER_OF_ITEMS;


	return Status;
}

/**
 * @brief Gets the name of a given sequence step.
 *
 * @par Function Description
 * This function retrieves the name of sequence steps corresponding to
 * SequenceStepId.
 *
 * @note This function doesn't Accesses the device
 *
 * @param   SequenceStepId               Sequence step identifier.
 * @param   pSequenceStepsString         Pointer to Info string. Shall be
 * defined as char buf[VL53L1_MAX_STRING_LENGTH]
 * @return  VL53L1_ERROR_NONE            Success
 * @return  "Other error code"           See ::VL53L1_Error
 */
VL53L1_Error VL53L1_GetSequenceStepsInfo(VL53L1_SequenceStepId SequenceStepId,
	char *pSequenceStepsString)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;



	Status = VL53L1_get_sequence_steps_info(
			SequenceStepId,
			pSequenceStepsString);



	return Status;
}

/**
 * @brief Sets the (on/off) state of a requested sequence step.
 *
 * @par Function Description
 * This function enables/disables a requested sequence step.
 *
 * @note This function Accesses the device
 *
 * @param   Dev                          Device Handle
 * @param   SequenceStepId	         Sequence step identifier.
 * @param   SequenceStepEnabled          Demanded state {0=Off,1=On}
 *                                       is enabled.
 * @return  VL53L1_ERROR_NONE            Success
 * @return  VL53L1_ERROR_INVALID_PARAMS  Error SequenceStepId parameter not
 *                                       supported.
 * @return  "Other error code"           See ::VL53L1_Error
 */
VL53L1_Error VL53L1_SetSequenceStepEnable(VL53L1_DEV Dev,
	VL53L1_SequenceStepId SequenceStepId, uint8_t SequenceStepEnabled)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	uint32_t MeasurementTimingBudgetMicroSeconds;



	/* the VL53L1_SequenceStepId correspond to the LLD
	 * VL53L1_DeviceSequenceConfig
	 */

	Status = VL53L1_set_sequence_config_bit(Dev,
		(VL53L1_DeviceSequenceConfig)SequenceStepId,
		SequenceStepEnabled);

	/* Apply New Setting */
	if (Status == VL53L1_ERROR_NONE) {

		/* Recalculate timing budget */
		MeasurementTimingBudgetMicroSeconds = VL53L1DevDataGet(Dev,
			CurrentParameters.MeasurementTimingBudgetMicroSeconds);

		VL53L1_SetMeasurementTimingBudgetMicroSeconds(Dev,
			MeasurementTimingBudgetMicroSeconds);
	}



	return Status;
}

/**
 * @brief Gets the (on/off) state of a requested sequence step.
 *
 * @par Function Description
 * This function retrieves the state of a requested sequence step, i.e. on/off.
 *
 * @note This function Accesses the device
 *
 * @param   Dev                    Device Handle
 * @param   SequenceStepId         Sequence step identifier.
 * @param   pSequenceStepEnabled   Out parameter reporting if the sequence step
 *                                 is enabled {0=Off,1=On}.
 * @return  VL53L1_ERROR_NONE            Success
 * @return  VL53L1_ERROR_INVALID_PARAMS  Error SequenceStepId parameter not
 *                                       supported.
 * @return  "Other error code"           See ::VL53L1_Error
 */
VL53L1_Error VL53L1_GetSequenceStepEnable(VL53L1_DEV Dev,
	VL53L1_SequenceStepId SequenceStepId, uint8_t *pSequenceStepEnabled)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;



	Status = VL53L1_get_sequence_config_bit(Dev,
		(VL53L1_DeviceSequenceConfig)SequenceStepId,
		pSequenceStepEnabled);


	return Status;
}


/** @} VL53L1_sequencestep_group */
/* \endinternal */



/** @defgroup VL53L1_measurement_group VL53L1 Measurement Functions
 *  @brief    Functions used for the measurements
 *  @{
 */

/**
 * @brief Start device measurement
 *
 * @details Started measurement will depend on preset parameters set through
 *  VL53L1_SetPreseMode()
 * This function will change the VL53L1_State from VL53L1_STATE_IDLE to
 * VL53L1_STATE_RUNNING.
 *
 * @note This function Access to the device
 *
 * @param   Dev                  Device Handle
 * @return  VL53L1_ERROR_NONE                  Success
 * @return  VL53L1_ERROR_MODE_NOT_SUPPORTED    This error occurs when
 * PresetMode programmed with  VL53L1_SetPresetMode
 * @return  VL53L1_ERROR_TIME_OUT    Time out on start measurement
 * @return  VL53L1_ERROR_INVALID_PARAMS This error might occur in timed mode
 * when inter measurement period is smaller or too close to the timing budget.
 * In such case measurements are not started and user must correct the timings
 * passed to  VL53L1_SetMeasurementTimingBudgetMicroSeconds() and
 *  VL53L1_SetInterMeasurementPeriodMilliSeconds() functions.
 * @return  "Other error code"   See ::VL53L1_Error
 */
VL53L1_Error VL53L1_StartMeasurement(VL53L1_DEV Dev)
{
#define TIMED_MODE_TIMING_GUARD_MILLISECONDS 4
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	uint8_t DeviceMeasurementMode;
	VL53L1_State CurrPalState;
	VL53L1_Error lStatus;
	uint32_t MTBus, IMPms;



	CurrPalState = VL53L1DevDataGet(Dev, PalState);
	switch (CurrPalState) {
	case VL53L1_STATE_IDLE:
		Status = VL53L1_ERROR_NONE;
		break;
	case VL53L1_STATE_POWERDOWN:
	case VL53L1_STATE_WAIT_STATICINIT:
	case VL53L1_STATE_STANDBY:
	case VL53L1_STATE_RUNNING:
	case VL53L1_STATE_RESET:
	case VL53L1_STATE_UNKNOWN:
	case VL53L1_STATE_ERROR:
		Status = VL53L1_ERROR_INVALID_COMMAND;
		break;
	default:
		Status = VL53L1_ERROR_UNDEFINED;
	}

	DeviceMeasurementMode = VL53L1DevDataGet(Dev, LLData.measurement_mode);

	/* Check timing configuration between timing budget and
	* inter measurement period */
	if ((Status == VL53L1_ERROR_NONE) &&
		(DeviceMeasurementMode == VL53L1_DEVICEMEASUREMENTMODE_TIMED)) {
		lStatus = VL53L1_GetMeasurementTimingBudgetMicroSeconds(Dev,
				&MTBus);
		/* convert timing budget in ms */
		MTBus /= 1000;
		lStatus = VL53L1_GetInterMeasurementPeriodMilliSeconds(Dev,
				&IMPms);
		/* trick to get rid of compiler "set but not used" warning */
		SUPPRESS_UNUSED_WARNING(lStatus);
		if (IMPms < MTBus + TIMED_MODE_TIMING_GUARD_MILLISECONDS)
			Status = VL53L1_ERROR_INVALID_PARAMS;
	}

	if (Status == VL53L1_ERROR_NONE)
		Status = VL53L1_init_and_start_range(
				Dev,
				DeviceMeasurementMode,
				VL53L1_DEVICECONFIGLEVEL_FULL);

	/* Set PAL State to Running */
	if (Status == VL53L1_ERROR_NONE)
		VL53L1DevDataSet(Dev, PalState, VL53L1_STATE_RUNNING);



	return Status;
}

/**
 * @brief Stop device measurement
 *
 * @details Will set the device in standby mode at end of current measurement\n
 *          Not necessary in single mode as device shall return automatically
 *          in standby mode at end of measurement.
 *          This function will change the VL53L1_State from VL53L1_STATE_RUNNING
 *          to VL53L1_STATE_IDLE.
 *
 * @note This function Access to the device
 *
 * @param   Dev                  Device Handle
 * @return  VL53L1_ERROR_NONE    Success
 * @return  "Other error code"   See ::VL53L1_Error
 */
VL53L1_Error VL53L1_StopMeasurement(VL53L1_DEV Dev)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;



	Status = VL53L1_stop_range(Dev);

	/* Set PAL State to Idle */
	if (Status == VL53L1_ERROR_NONE)
		VL53L1DevDataSet(Dev, PalState, VL53L1_STATE_IDLE);


	return Status;
}

static VL53L1_Error ChangePresetMode(VL53L1_DEV Dev)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	VL53L1_PresetModes PresetMode;
	VL53L1_DistanceModes NewDistanceMode;
	VL53L1_user_zone_t	user_zone;
	uint32_t TimingBudget;
	uint32_t MmTimeoutUs;
	uint32_t PhaseCalTimeoutUs;
	uint8_t DeviceMeasurementMode;
	uint32_t inter_measurement_period_ms;



	Status = VL53L1_get_user_zone(Dev, &user_zone);
	/*  Initialize variables fix ticket EwokP #475395 */
	PresetMode = VL53L1DevDataGet(Dev,
			CurrentParameters.PresetMode);
	NewDistanceMode = VL53L1DevDataGet(Dev,
			CurrentParameters.NewDistanceMode);
	/*  End of Initialize variables fix ticket EwokP #475395 */
	if (Status == VL53L1_ERROR_NONE)
		Status = VL53L1_get_timeouts_us(Dev, &PhaseCalTimeoutUs,
			&MmTimeoutUs, &TimingBudget);

	if (Status == VL53L1_ERROR_NONE)
		Status = VL53L1_stop_range(Dev);

	if (Status == VL53L1_ERROR_NONE)
		Status = VL53L1_WaitUs(Dev, 500);

	if (Status == VL53L1_ERROR_NONE) {
		inter_measurement_period_ms =  VL53L1DevDataGet(Dev,
					LLData.inter_measurement_period_ms);

		Status = SetPresetMode(Dev,
				PresetMode,
				NewDistanceMode,
				inter_measurement_period_ms);
	}

	if (Status == VL53L1_ERROR_NONE) {
		Status = VL53L1_set_timeouts_us(Dev, PhaseCalTimeoutUs,
			MmTimeoutUs, TimingBudget);

		if (Status == VL53L1_ERROR_NONE)
			VL53L1DevDataSet(Dev, LLData.range_config_timeout_us,
				TimingBudget);
	}

	if (Status == VL53L1_ERROR_NONE)
		Status = VL53L1_set_user_zone(Dev, &user_zone);

	if (Status == VL53L1_ERROR_NONE) {
		DeviceMeasurementMode = VL53L1DevDataGet(Dev,
				LLData.measurement_mode);

		Status = VL53L1_init_and_start_range(
				Dev,
				DeviceMeasurementMode,
				VL53L1_DEVICECONFIGLEVEL_FULL);
	}

	if (Status == VL53L1_ERROR_NONE)
		VL53L1DevDataSet(Dev,
			CurrentParameters.InternalDistanceMode,
			NewDistanceMode);


	return Status;
}

/**
 * @brief Clear the Interrupt flag and start new measurement
 * *
 * @note This function Access to the device
 *
 * @param   Dev                  Device Handle
 * @return  VL53L1_ERROR_NONE    Success
 * @return  "Other error code"   See ::VL53L1_Error
 */
VL53L1_Error VL53L1_ClearInterruptAndStartMeasurement(VL53L1_DEV Dev)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	uint8_t DeviceMeasurementMode;
	VL53L1_DistanceModes InternalDistanceMode;
	VL53L1_DistanceModes NewDistanceMode;



	DeviceMeasurementMode = VL53L1DevDataGet(Dev, LLData.measurement_mode);
	InternalDistanceMode = VL53L1DevDataGet(Dev,
			CurrentParameters.InternalDistanceMode);
	NewDistanceMode = VL53L1DevDataGet(Dev,
			CurrentParameters.NewDistanceMode);

	if (NewDistanceMode != InternalDistanceMode)
		Status = ChangePresetMode(Dev);
	else
		Status = VL53L1_clear_interrupt_and_enable_next_range(
						Dev,
						DeviceMeasurementMode);


	return Status;
}

/**
 * @brief Return Measurement Data Ready
 *
 * @par Function Description
 * This function indicate that a measurement data is ready.
 * This function is used for non-blocking capture.
 *
 * @note This function Access to the device
 *
 * @param   Dev                    Device Handle
 * @param   pMeasurementDataReady  Pointer to Measurement Data Ready.
 * 0 = data not ready, 1 = data ready
 * @return  VL53L1_ERROR_NONE      Success
 * @return  "Other error code"     See ::VL53L1_Error
 */
VL53L1_Error VL53L1_GetMeasurementDataReady(VL53L1_DEV Dev,
	uint8_t *pMeasurementDataReady)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;



	Status = VL53L1_is_new_data_ready(Dev, pMeasurementDataReady);


	return Status;
}

/**
 * @brief Wait for measurement data ready.
 * Blocking function.
 * Note that the timeout is given by:
 * VL53L1_RANGE_COMPLETION_POLLING_TIMEOUT_MS defined in def.h
 *
 *
 * @note This function Access to the device
 *
 * @param   Dev      Device Handle
 * @return  VL53L1_ERROR_NONE        Success
 * @return  VL53L1_ERROR_TIME_OUT In case of timeout
 */
VL53L1_Error VL53L1_WaitMeasurementDataReady(VL53L1_DEV Dev)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;



	/* Note that the timeout is given by:
	* VL53L1_RANGE_COMPLETION_POLLING_TIMEOUT_MS defined in def.h
	*/

	Status = VL53L1_poll_for_range_completion(Dev,
			VL53L1_RANGE_COMPLETION_POLLING_TIMEOUT_MS);


	return Status;
}



static uint8_t ComputeRQL(uint8_t active_results,
		uint8_t FilteredRangeStatus,
		VL53L1_range_data_t *presults_data)
{
	int16_t SRL = 300;
	uint16_t SRAS = 30;
	FixPoint1616_t RAS;
	FixPoint1616_t SRQL;
	FixPoint1616_t GI =   7713587; /* 117.7 * 65536 */
	FixPoint1616_t GGm =  3198157; /* 48.8 * 65536 */
	FixPoint1616_t LRAP = 6554;    /* 0.1 * 65536 */
	FixPoint1616_t partial;
	uint8_t finalvalue;
	uint8_t returnvalue;

	if (active_results == 0)
		returnvalue = 0;
	else if (FilteredRangeStatus == VL53L1_DEVICEERROR_PHASECONSISTENCY)
		returnvalue = 50;
	else {
		if (presults_data->median_range_mm < SRL)
			RAS = SRAS * 65536;
		else
			RAS = LRAP * presults_data->median_range_mm;

		/* Fix1616 + (fix1616 * uint16_t / fix1616) * 65536 = fix1616 */
		if (RAS != 0) {
			partial = (GGm * presults_data->sigma_mm);
			partial = partial + (RAS >> 1);
			partial = partial / RAS;
			partial = partial * 65536;
			if (partial <= GI)
				SRQL = GI - partial;
			else
				SRQL = 50 * 65536;
		} else
			SRQL = 100 * 65536;

		finalvalue = (uint8_t)(SRQL >> 16);
		returnvalue = MAX(50, MIN(100, finalvalue));
	}

	return returnvalue;
}


static uint8_t ConvertStatusLite(uint8_t FilteredRangeStatus)
{
	uint8_t RangeStatus;

	switch (FilteredRangeStatus) {
	case VL53L1_DEVICEERROR_GPHSTREAMCOUNT0READY:
		RangeStatus = VL53L1_RANGESTATUS_SYNCRONISATION_INT;
		break;
	case VL53L1_DEVICEERROR_RANGECOMPLETE_NO_WRAP_CHECK:
		RangeStatus = VL53L1_RANGESTATUS_RANGE_VALID_NO_WRAP_CHECK_FAIL;
		break;
	case VL53L1_DEVICEERROR_RANGEPHASECHECK:
		RangeStatus = VL53L1_RANGESTATUS_OUTOFBOUNDS_FAIL;
		break;
	case VL53L1_DEVICEERROR_MSRCNOTARGET:
		RangeStatus = VL53L1_RANGESTATUS_SIGNAL_FAIL;
		break;
	case VL53L1_DEVICEERROR_SIGMATHRESHOLDCHECK:
		RangeStatus = VL53L1_RANGESTATUS_SIGMA_FAIL;
		break;
	case VL53L1_DEVICEERROR_PHASECONSISTENCY:
		RangeStatus = VL53L1_RANGESTATUS_WRAP_TARGET_FAIL;
		break;
	case VL53L1_DEVICEERROR_RANGEIGNORETHRESHOLD:
		RangeStatus = VL53L1_RANGESTATUS_XTALK_SIGNAL_FAIL;
		break;
	case VL53L1_DEVICEERROR_MINCLIP:
		RangeStatus = VL53L1_RANGESTATUS_RANGE_VALID_MIN_RANGE_CLIPPED;
		break;
	case VL53L1_DEVICEERROR_RANGECOMPLETE:
		RangeStatus = VL53L1_RANGESTATUS_RANGE_VALID;
		break;
	default:
		RangeStatus = VL53L1_RANGESTATUS_NONE;
	}

	return RangeStatus;
}



static VL53L1_Error SetSimpleData(VL53L1_DEV Dev,
	uint8_t active_results, uint8_t device_status,
	VL53L1_range_data_t *presults_data,
	VL53L1_RangingMeasurementData_t *pRangeData)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	uint8_t FilteredRangeStatus;
	uint8_t SigmaLimitflag;
	uint8_t SignalLimitflag;
	uint8_t Temp8Enable;
	uint8_t Temp8;
	FixPoint1616_t AmbientRate;
	FixPoint1616_t SignalRate;
	FixPoint1616_t TempFix1616;
	FixPoint1616_t LimitCheckValue;
	int16_t Range;

	pRangeData->TimeStamp = presults_data->time_stamp;

	FilteredRangeStatus = presults_data->range_status & 0x1F;

	pRangeData->RangeQualityLevel = ComputeRQL(active_results,
					FilteredRangeStatus,
					presults_data);

	SignalRate = VL53L1_FIXPOINT97TOFIXPOINT1616(
		presults_data->peak_signal_count_rate_mcps);
	pRangeData->SignalRateRtnMegaCps
		= SignalRate;

	AmbientRate = VL53L1_FIXPOINT97TOFIXPOINT1616(
		presults_data->ambient_count_rate_mcps);
	pRangeData->AmbientRateRtnMegaCps = AmbientRate;

	pRangeData->EffectiveSpadRtnCount =
		presults_data->actual_effective_spads;

	TempFix1616 = VL53L1_FIXPOINT97TOFIXPOINT1616(
			presults_data->sigma_mm);

	pRangeData->SigmaMilliMeter = TempFix1616;

	pRangeData->RangeMilliMeter = presults_data->median_range_mm;

	pRangeData->RangeFractionalPart = 0;

	/* Treat device error status first */
	switch (device_status) {
	case VL53L1_DEVICEERROR_MULTCLIPFAIL:
	case VL53L1_DEVICEERROR_VCSELWATCHDOGTESTFAILURE:
	case VL53L1_DEVICEERROR_VCSELCONTINUITYTESTFAILURE:
	case VL53L1_DEVICEERROR_NOVHVVALUEFOUND:
		pRangeData->RangeStatus = VL53L1_RANGESTATUS_HARDWARE_FAIL;
		break;
	case VL53L1_DEVICEERROR_USERROICLIP:
		pRangeData->RangeStatus = VL53L1_RANGESTATUS_MIN_RANGE_FAIL;
		break;
	default:
		pRangeData->RangeStatus = VL53L1_RANGESTATUS_RANGE_VALID;
	}

	/* Now deal with range status according to the ranging preset */
	if (pRangeData->RangeStatus == VL53L1_RANGESTATUS_RANGE_VALID) {
			pRangeData->RangeStatus =
				ConvertStatusLite(FilteredRangeStatus);
	}

	/* Update current Limit Check */
	TempFix1616 = VL53L1_FIXPOINT97TOFIXPOINT1616(
			presults_data->sigma_mm);
	VL53L1_SETARRAYPARAMETERFIELD(Dev,
		LimitChecksCurrent, VL53L1_CHECKENABLE_SIGMA_FINAL_RANGE,
		TempFix1616);

	TempFix1616 = VL53L1_FIXPOINT97TOFIXPOINT1616(
			presults_data->peak_signal_count_rate_mcps);
	VL53L1_SETARRAYPARAMETERFIELD(Dev,
		LimitChecksCurrent, VL53L1_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,
		TempFix1616);

	/* Update Limit Check Status */
	/* Sigma */
	VL53L1_GetLimitCheckValue(Dev,
			VL53L1_CHECKENABLE_SIGMA_FINAL_RANGE,
			&LimitCheckValue);

	SigmaLimitflag = (FilteredRangeStatus ==
			VL53L1_DEVICEERROR_SIGMATHRESHOLDCHECK)
			? 1 : 0;

	VL53L1_GetLimitCheckEnable(Dev,
			VL53L1_CHECKENABLE_SIGMA_FINAL_RANGE,
			&Temp8Enable);

	Temp8 = ((Temp8Enable == 1) && (SigmaLimitflag == 1)) ? 1 : 0;
	VL53L1_SETARRAYPARAMETERFIELD(Dev, LimitChecksStatus,
			VL53L1_CHECKENABLE_SIGMA_FINAL_RANGE, Temp8);

	/* Signal Rate */
	VL53L1_GetLimitCheckValue(Dev,
			VL53L1_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,
			&LimitCheckValue);

	SignalLimitflag = (FilteredRangeStatus ==
			VL53L1_DEVICEERROR_MSRCNOTARGET)
			? 1 : 0;

	VL53L1_GetLimitCheckEnable(Dev,
			VL53L1_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,
			&Temp8Enable);

	Temp8 = ((Temp8Enable == 1) && (SignalLimitflag == 1)) ? 1 : 0;
	VL53L1_SETARRAYPARAMETERFIELD(Dev, LimitChecksStatus,
			VL53L1_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, Temp8);

	Range = pRangeData->RangeMilliMeter;
	if ((pRangeData->RangeStatus == VL53L1_RANGESTATUS_RANGE_VALID) &&
		(Range < 0)) {
		if (Range < BDTable[VL53L1_TUNING_PROXY_MIN])
			pRangeData->RangeStatus =
					VL53L1_RANGESTATUS_RANGE_INVALID;
		else
			pRangeData->RangeMilliMeter = 0;
	}

	return Status;
}

/**
 * @brief Retrieve the measurements from device for a given setup
 *
 * @par Function Description
 * Get data from last successful Ranging measurement
 */
/**
 *
 * @warning USER must call  VL53L1_ClearInterruptAndStartMeasurement() prior
 * to call again this function
 *
 * @note This function Access to the device
 *
 * @note The first valid value returned by this function will have a range
 * status equal to VL53L1_RANGESTATUS_RANGE_VALID_NO_WRAP_CHECK which means that
 * the data is valid but no wrap around check have been done. User should take
 * care about that.
 *
 * @param   Dev                      Device Handle
 * @param   pRangingMeasurementData  Pointer to the data structure to fill up.
 * @return  VL53L1_ERROR_NONE        Success
 * @return  VL53L1_ERROR_MODE_NOT_SUPPORTED    in case of MULTIZONES_SCANNING
 * @return  "Other error code"       See ::VL53L1_Error
 */
VL53L1_Error VL53L1_GetRangingMeasurementData(VL53L1_DEV Dev,
	VL53L1_RangingMeasurementData_t *pRangingMeasurementData)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	VL53L1_range_results_t       results;
	VL53L1_range_results_t       *presults = &results;
	VL53L1_range_data_t *presults_data;




	/* Clear Ranging Data */
	memset(pRangingMeasurementData, 0xFF,
		sizeof(VL53L1_RangingMeasurementData_t));

	/* Get Ranging Data */
	Status = VL53L1_get_device_results(
			Dev,
			VL53L1_DEVICERESULTSLEVEL_FULL,
			presults);

	if (Status == VL53L1_ERROR_NONE) {
		pRangingMeasurementData->StreamCount = presults->stream_count;

		/* in case of lite ranging or autonomous the following function
		 * returns index = 0
		 */
		presults_data = &(presults->data[0]);
		Status = SetSimpleData(Dev, 1,
				presults->device_status,
				presults_data,
				pRangingMeasurementData);
	}


	return Status;
}


/** @} VL53L1_measurement_group */

/** @defgroup VL53L1_Calibration_group VL53L1 Calibration Functions
 *  @brief    Functions used for Calibration
 *  @{
 */


/**
 * @brief Set Tuning Parameter value for a given parameter ID
 *
 * @par Function Description
 * This function is used to improve the performance of the device. It permit to
 * change a particular value used for a timeout or a threshold or a constant
 * in an algorithm. The function will change the value of the parameter
 * identified by an unique ID.
 *
 * @note This function doesn't Access to the device
 *
 * @param   Dev                          Device Handle
 * @param   TuningParameterId            Tuning Parameter ID
 * @param   TuningParameterValue         Tuning Parameter Value
 * @return  VL53L1_ERROR_NONE        Success
 * @return  "Other error code"       See ::VL53L1_Error
 */VL53L1_Error VL53L1_SetTuningParameter(VL53L1_DEV Dev,
		uint16_t TuningParameterId, int32_t TuningParameterValue)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;


	if (TuningParameterId >= 32768)
		Status = VL53L1_set_tuning_parm(Dev,
			TuningParameterId,
			TuningParameterValue);
	else {
		if (TuningParameterId < VL53L1_TUNING_MAX_TUNABLE_KEY)
			BDTable[TuningParameterId] = TuningParameterValue;
		else
			Status = VL53L1_ERROR_INVALID_PARAMS;
	}


	return Status;
}

 /**
  * @brief Get Tuning Parameter value for a given parameter ID
  *
  * @par Function Description
  * This function is used to get the value of the parameter
  * identified by an unique ID.
  *
  * @note This function doesn't Access to the device
  *
  * @param   Dev                          Device Handle
  * @param   TuningParameterId            Tuning Parameter ID
  * @param   pTuningParameterValue        Pointer to Tuning Parameter Value
  * for a given TuningParameterId.
  * @return  VL53L1_ERROR_NONE        Success
  * @return  "Other error code"       See ::VL53L1_Error
  */
VL53L1_Error VL53L1_GetTuningParameter(VL53L1_DEV Dev,
		uint16_t TuningParameterId, int32_t *pTuningParameterValue)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;



	if (TuningParameterId >= 32768)
		Status = VL53L1_get_tuning_parm(Dev,
			TuningParameterId,
			pTuningParameterValue);
	else {
		if (TuningParameterId < VL53L1_TUNING_MAX_TUNABLE_KEY)
			*pTuningParameterValue = BDTable[TuningParameterId];
		else
			Status = VL53L1_ERROR_INVALID_PARAMS;
	}


	return Status;
}

/**
 * @brief Performs Reference Spad Management
 *
 * @par Function Description
 * The reference SPAD initialization procedure determines the minimum amount
 * of reference spads to be enables to achieve a target reference signal rate
 * and should be performed once during initialization.
 *
 * @note This function Access to the device
 *
 * @param   Dev                          Device Handle
 * @return  VL53L1_ERROR_NONE        Success
 * @return  "Other error code"       See ::VL53L1_Error
 */
VL53L1_Error VL53L1_PerformRefSpadManagement(VL53L1_DEV Dev)
{
#ifdef VL53L1_NOCALIB
	VL53L1_Error Status = VL53L1_ERROR_NOT_SUPPORTED;

	SUPPRESS_UNUSED_WARNING(Dev);


#else
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	VL53L1_Error RawStatus;
	uint8_t dcrbuffer[24];
	uint8_t *comms_buffer;
	uint8_t numloc[2] = {5,3};
	VL53L1_LLDriverData_t *pdev;
	VL53L1_customer_nvm_managed_t *pc;
	VL53L1_PresetModes PresetMode;



	pdev = VL53L1DevStructGetLLDriverHandle(Dev);
	pc = &pdev->customer;

	if (Status == VL53L1_ERROR_NONE)
	{
		PresetMode = VL53L1DevDataGet(Dev, CurrentParameters.PresetMode);
		Status = VL53L1_run_ref_spad_char(Dev, &RawStatus);
		/* We discovered RefSpad mngt badly breaks some preset mode
		 * The WA is to apply again the current one
		 */
		if (Status == VL53L1_ERROR_NONE)
			Status = VL53L1_SetPresetMode(Dev, PresetMode);
	}

	if (Status == VL53L1_WARNING_REF_SPAD_CHAR_RATE_TOO_HIGH) {
		/* Fix ticket  #466282 RefSpad management error/warning -29
		 * force usage of location 3 and 5 refspads in registers
		*/
		Status = VL53L1_read_nvm_raw_data(Dev,
				(uint8_t)(0xA0 >> 2),
				(uint8_t)(24 >> 2),
				dcrbuffer);

		if (Status == VL53L1_ERROR_NONE)
			Status = VL53L1_WriteMulti( Dev,
				VL53L1_REF_SPAD_MAN__NUM_REQUESTED_REF_SPADS,
				numloc, 2);

		if (Status == VL53L1_ERROR_NONE) {
			pc->ref_spad_man__num_requested_ref_spads = numloc[0];
			pc->ref_spad_man__ref_location = numloc[1];
		}

		if (Status == VL53L1_ERROR_NONE)
			comms_buffer = &dcrbuffer[16];

		/*
		 * update & copy reference SPAD enables to customer nvm managed
		 */

		if (Status == VL53L1_ERROR_NONE)
			Status = VL53L1_WriteMulti(Dev,
				VL53L1_GLOBAL_CONFIG__SPAD_ENABLES_REF_0,
				comms_buffer, 6);

		if (Status == VL53L1_ERROR_NONE) {
			pc->global_config__spad_enables_ref_0 = comms_buffer[0];
			pc->global_config__spad_enables_ref_1 = comms_buffer[1];
			pc->global_config__spad_enables_ref_2 = comms_buffer[2];
			pc->global_config__spad_enables_ref_3 = comms_buffer[3];
			pc->global_config__spad_enables_ref_4 = comms_buffer[4];
			pc->global_config__spad_enables_ref_5 = comms_buffer[5];
		}
		/* End of fix  ticket  #466282 */
	}

#endif


	return Status;
}

/**
 * @brief Enable/Disable Cross talk compensation feature
 *
 * Enable/Disable Cross Talk correction.
 *
 * @param   Dev                       Device Handle
 * @param   XTalkCompensationEnable   Cross talk compensation
 *  to be set 0 = disabled or 1 = enabled.
 * @return  VL53L1_ERROR_NONE        Success
 * @return  "Other error code"       See ::VL53L1_Error
*/
VL53L1_Error VL53L1_SetXTalkCompensationEnable(VL53L1_DEV Dev,
	uint8_t XTalkCompensationEnable)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;



	if (XTalkCompensationEnable == 0)
		Status = VL53L1_disable_xtalk_compensation(Dev);
	else
		Status = VL53L1_enable_xtalk_compensation(Dev);


	return Status;
}

/**
 * @brief Get Cross talk compensation rate enable
 *
 * Get if the Cross Talk is Enabled or Disabled.
 *
 * @note This function doesn't access to the device
 *
 * @param   Dev                        Device Handle
 * @param   pXTalkCompensationEnable   Pointer to the Cross talk compensation
 *  state 0=disabled or 1 = enabled
 * @return  VL53L1_ERROR_NONE        Success
 * @return  "Other error code"       See ::VL53L1_Error
 */
VL53L1_Error VL53L1_GetXTalkCompensationEnable(VL53L1_DEV Dev,
	uint8_t *pXTalkCompensationEnable)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;



	VL53L1_get_xtalk_compensation_enable(
		Dev,
		pXTalkCompensationEnable);


	return Status;
}

/**
 * @brief Perform XTalk Calibration
 *
 * @details Perform a XTalk calibration of the Device.
 * This function will launch a ranging measurement, if interrupts
 * are enabled an interrupt will be done.
 * This function will clear the interrupt generated automatically.
 * This function will program a new value for the XTalk compensation
 * and it will enable the cross talk before exit.
 *
 * @warning This function is a blocking function
 *
 * @note This function Access to the device
 *
 * @param   Dev                   Device Handle
 * @param   CalDistanceMilliMeter Target distance in mm
 * The calibration uses current preset and distance mode without altering them.
 * <br>User must call  VL53L1_SetPresetMode() with
 * VL53L1_PRESETMODE_AUTONOMOUS, VL53L1_PRESETMODE_LITE_RANGING or
 * VL53L1_PRESETMODE_LOWPOWER_AUTONOMOUS parameter prior to launch calibration
 *
 * @return  VL53L1_ERROR_NONE    Success
 * @return  "Other error code"   See ::VL53L1_Error
 */
VL53L1_Error VL53L1_PerformSingleTargetXTalkCalibration(VL53L1_DEV Dev,
		int32_t CalDistanceMilliMeter)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;



	if (CalDistanceMilliMeter > 0) {
		BDTable[VL53L1_TUNING_SINGLE_TARGET_XTALK_TARGET_DISTANCE_MM] =
				CalDistanceMilliMeter;
		Status = SingleTargetXTalkCalibration(Dev);
	} else
		Status = VL53L1_ERROR_INVALID_PARAMS;


	return Status;
}

/**
 * @brief Define the mode to be used for the offset calibration
 *
 * Define the mode to be used for the offset calibration. This function should
 * be called before run the  VL53L1_PerformOffsetCalibration()
 *
 * @param   Dev                       Device Handle
 * @param   OffsetCalibrationMode     Offset Calibration Mode valid values are:
 * @li                                VL53L1_OFFSETCALIBRATIONMODE_STANDARD
 * @li                                VL53L1_OFFSETCALIBRATIONMODE_PRERANGE_ONLY
 */
/**
 *
 * @return  VL53L1_ERROR_NONE         Success
 * @return  "Other error code"        See ::VL53L1_Error
 */
VL53L1_Error VL53L1_SetOffsetCalibrationMode(VL53L1_DEV Dev,
		VL53L1_OffsetCalibrationModes OffsetCalibrationMode)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	VL53L1_OffsetCalibrationMode   offset_cal_mode;



	if (OffsetCalibrationMode == VL53L1_OFFSETCALIBRATIONMODE_STANDARD) {
		offset_cal_mode = VL53L1_DEVICEPRESETMODE_STANDARD_RANGING;
	} else if (OffsetCalibrationMode ==
			VL53L1_OFFSETCALIBRATIONMODE_PRERANGE_ONLY) {
		offset_cal_mode =
		VL53L1_OFFSETCALIBRATIONMODE__MM1_MM2__STANDARD_PRE_RANGE_ONLY;
	} else {
		Status = VL53L1_ERROR_INVALID_PARAMS;
	}

	if (Status == VL53L1_ERROR_NONE)
		Status =  VL53L1_set_offset_calibration_mode(Dev,
				offset_cal_mode);


	return Status;
}

#ifdef OFFSET_CALIB

/**
 * @brief Perform Offset Calibration
 *
 * @details Perform a Offset calibration of the Device.
 * This function will launch a ranging measurement, if interrupts are
 * enabled interrupts will be done.
 * This function will program a new value for the Offset calibration value
 *
 * @warning This function is a blocking function
 *
 * @note This function Access to the device
 *
 * @param   Dev                  Device Handle
 * @param   CalDistanceMilliMeter     Calibration distance value used for the
 * offset compensation.
 *
 * @return  VL53L1_ERROR_NONE
 * @return  "Other error code"   See ::VL53L1_Error
 */
VL53L1_Error VL53L1_PerformOffsetCalibration(VL53L1_DEV Dev,
	int32_t CalDistanceMilliMeter)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	VL53L1_Error UnfilteredStatus;
	VL53L1_OffsetCalibrationMode   offset_cal_mode;



	if (Status == VL53L1_ERROR_NONE)
		Status =  VL53L1_get_offset_calibration_mode(Dev,
				&offset_cal_mode);

	if (Status != VL53L1_ERROR_NONE) {

		return Status;
	}

	if ((offset_cal_mode ==
		VL53L1_OFFSETCALIBRATIONMODE__MM1_MM2__STANDARD) ||
		(offset_cal_mode ==
		VL53L1_OFFSETCALIBRATIONMODE__MM1_MM2__STANDARD_PRE_RANGE_ONLY
		)) {
		if (Status == VL53L1_ERROR_NONE)
		Status = VL53L1_run_offset_calibration(
				Dev,
				(int16_t)CalDistanceMilliMeter,
				&UnfilteredStatus);
	} else {
		Status = VL53L1_ERROR_INVALID_PARAMS;
	}

	return Status;
}
#endif
#ifdef OFFSET_CALIB_EMPTY
VL53L1_Error VL53L1_PerformOffsetCalibration(VL53L1_DEV Dev,
	int32_t CalDistanceMilliMeter)
{
	VL53L1_Error Status = VL53L1_ERROR_NOT_SUPPORTED;
	SUPPRESS_UNUSED_WARNING(Dev);
	SUPPRESS_UNUSED_WARNING(CalDistanceMilliMeter);
	return Status;
}
#endif
/**
 * @brief Perform Offset simple Calibration
 *
 * @details Perform a very simple offset calibration of the Device.
 * This function will launch few ranging measurements and computes offset
 * calibration. The preset mode and the distance mode MUST be set by the
 * application before to call this function.
 *
 * @warning This function is a blocking function
 *
 * @note This function Access to the device
 *
 * @param   Dev                  Device Handle
 * @param   CalDistanceMilliMeter     Calibration distance value used for the
 * offset compensation.
 *
 * @return  VL53L1_ERROR_NONE
 * @return  VL53L1_ERROR_OFFSET_CAL_NO_SAMPLE_FAIL the calibration failed by
 * lack of valid measurements
 * @return  VL53L1_WARNING_OFFSET_CAL_SIGMA_TOO_HIGH means that the target
 * distance combined to the number of loops performed in the calibration lead to
 * an internal overflow. Try to reduce the distance of the target (140 mm)
 * @return  "Other error code"   See ::VL53L1_Error
 */
VL53L1_Error VL53L1_PerformOffsetSimpleCalibration(VL53L1_DEV Dev,
	int32_t CalDistanceMilliMeter)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	int32_t sum_ranging;
	uint8_t offset_meas;
	int16_t Max, UnderMax, OverMax, Repeat;
	int32_t total_count, inloopcount;
	int32_t IncRounding;
	int16_t meanDistance_mm;
	int16_t offset;
	VL53L1_RangingMeasurementData_t RangingMeasurementData;
	VL53L1_LLDriverData_t *pdev;
	uint8_t goodmeas;



	pdev = VL53L1DevStructGetLLDriverHandle(Dev);
	/* Disable any offsets */
	pdev->customer.algo__part_to_part_range_offset_mm = 0;
	pdev->customer.mm_config__inner_offset_mm = 0;
	pdev->customer.mm_config__outer_offset_mm = 0;

	Repeat=BDTable[VL53L1_TUNING_SIMPLE_OFFSET_CALIBRATION_REPEAT];
	Max=BDTable[VL53L1_TUNING_MAX_SIMPLE_OFFSET_CALIBRATION_SAMPLE_NUMBER];
	UnderMax = 1 + (Max / 2);
	OverMax = Max + (Max / 2);
	sum_ranging = 0;
	total_count = 0;

	while ((Repeat > 0) && (Status == VL53L1_ERROR_NONE)) {
		Status = VL53L1_StartMeasurement(Dev);
		/* Very first ranging completion interrupt must be ignored */
		if (Status == VL53L1_ERROR_NONE)
			Status = VL53L1_WaitMeasurementDataReady(Dev);
		if (Status == VL53L1_ERROR_NONE)
			Status = VL53L1_GetRangingMeasurementData(Dev,
						&RangingMeasurementData);
		if (Status == VL53L1_ERROR_NONE)
			Status = VL53L1_ClearInterruptAndStartMeasurement(Dev);
		/* offset calibration main loop */
		inloopcount = 0;
		offset_meas = 0;
		while ((Status == VL53L1_ERROR_NONE) && (inloopcount < Max) &&
				(offset_meas < OverMax)) {
			Status = VL53L1_WaitMeasurementDataReady(Dev);
			if (Status == VL53L1_ERROR_NONE)
				Status = VL53L1_GetRangingMeasurementData(Dev,
						&RangingMeasurementData);
			goodmeas = (RangingMeasurementData.RangeStatus ==
					VL53L1_RANGESTATUS_RANGE_VALID);
			if ((Status == VL53L1_ERROR_NONE) && goodmeas) {
				sum_ranging = sum_ranging +
					RangingMeasurementData.RangeMilliMeter;
				inloopcount++;
			}
			if (Status == VL53L1_ERROR_NONE) {
				Status = VL53L1_ClearInterruptAndStartMeasurement(
					Dev);
			}
			offset_meas++;
		}
		total_count += inloopcount;

		/* no enough valid values found */
		if (inloopcount < UnderMax) {
			Status = VL53L1_ERROR_OFFSET_CAL_NO_SAMPLE_FAIL;
		}

		VL53L1_StopMeasurement(Dev);

		Repeat--;

	}
	/* check overflow (unlikely if target is near to the device) */
	if ((sum_ranging < 0) ||
		(sum_ranging > ((int32_t) total_count * 0xffff))) {
		Status = VL53L1_WARNING_OFFSET_CAL_SIGMA_TOO_HIGH;
	}

	if ((Status == VL53L1_ERROR_NONE) && (total_count > 0)) {
		IncRounding = total_count / 2;
		meanDistance_mm = (int16_t)((sum_ranging + IncRounding)
				/ total_count);
		offset = (int16_t)CalDistanceMilliMeter - meanDistance_mm;
		pdev->customer.algo__part_to_part_range_offset_mm = 0;
		pdev->customer.mm_config__inner_offset_mm = offset;
		pdev->customer.mm_config__outer_offset_mm = offset;

		Status = VL53L1_set_customer_nvm_managed(Dev,
				&(pdev->customer));
	}


	return Status;
}

/**
 * @brief Sets the Calibration Data.
 *
 * @par Function Description
 * This function set all the Calibration Data issued from the functions
 *  VL53L1_PerformRefSpadManagement(),  VL53L1_PerformXTalkCalibration,
 *  VL53L1_PerformOffsetCalibration()
 *
 * @note This function doesn't Accesses the device
 *
 * @param   Dev                          Device Handle
 * @param   *pCalibrationData            Pointer to Calibration data to be set.
 * @return  VL53L1_ERROR_NONE            Success
 * @return  VL53L1_ERROR_INVALID_PARAMS  pCalibrationData points to an older
 * version of the inner structure. Need for support to convert its content.
 * @return  "Other error code"           See ::VL53L1_Error
 */
VL53L1_Error VL53L1_SetCalibrationData(VL53L1_DEV Dev,
		VL53L1_CalibrationData_t *pCalibrationData)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	VL53L1_CustomerNvmManaged_t          *pC;
	VL53L1_calibration_data_t            cal_data;
	uint32_t x;



	cal_data.struct_version = pCalibrationData->struct_version -
			VL53L1_ADDITIONAL_CALIBRATION_DATA_STRUCT_VERSION;



	/* memcpy(DEST, SRC, N)  */
	memcpy(
		&(cal_data.add_off_cal_data),
		&(pCalibrationData->add_off_cal_data),
		sizeof(VL53L1_additional_offset_cal_data_t));

	/* memcpy (DEST, SRC, N) */
	memcpy(
		&(cal_data.optical_centre),
		&(pCalibrationData->optical_centre),
		sizeof(VL53L1_optical_centre_t));

	/* memcpy (DEST, SRC, N) */
	memcpy(
		&(cal_data.gain_cal),
		&(pCalibrationData->gain_cal),
		sizeof(VL53L1_gain_calibration_data_t));

	/* memcpy (DEST, SRC, N) */
	memcpy(
		&(cal_data.cal_peak_rate_map),
		&(pCalibrationData->cal_peak_rate_map),
		sizeof(VL53L1_cal_peak_rate_map_t));

	pC = &pCalibrationData->customer;
	x = pC->algo__crosstalk_compensation_plane_offset_kcps;
	cal_data.customer.algo__crosstalk_compensation_plane_offset_kcps =
		(uint16_t)(x&0x0000FFFF);

	cal_data.customer.global_config__spad_enables_ref_0 =
		pC->global_config__spad_enables_ref_0;
	cal_data.customer.global_config__spad_enables_ref_1 =
		pC->global_config__spad_enables_ref_1;
	cal_data.customer.global_config__spad_enables_ref_2 =
		pC->global_config__spad_enables_ref_2;
	cal_data.customer.global_config__spad_enables_ref_3 =
		pC->global_config__spad_enables_ref_3;
	cal_data.customer.global_config__spad_enables_ref_4 =
		pC->global_config__spad_enables_ref_4;
	cal_data.customer.global_config__spad_enables_ref_5 =
		pC->global_config__spad_enables_ref_5;
	cal_data.customer.global_config__ref_en_start_select =
		pC->global_config__ref_en_start_select;
	cal_data.customer.ref_spad_man__num_requested_ref_spads =
		pC->ref_spad_man__num_requested_ref_spads;
	cal_data.customer.ref_spad_man__ref_location =
		pC->ref_spad_man__ref_location;
	cal_data.customer.algo__crosstalk_compensation_x_plane_gradient_kcps =
		pC->algo__crosstalk_compensation_x_plane_gradient_kcps;
	cal_data.customer.algo__crosstalk_compensation_y_plane_gradient_kcps =
		pC->algo__crosstalk_compensation_y_plane_gradient_kcps;
	cal_data.customer.ref_spad_char__total_rate_target_mcps =
		pC->ref_spad_char__total_rate_target_mcps;
	cal_data.customer.algo__part_to_part_range_offset_mm =
		pC->algo__part_to_part_range_offset_mm;
	cal_data.customer.mm_config__inner_offset_mm =
		pC->mm_config__inner_offset_mm;
	cal_data.customer.mm_config__outer_offset_mm =
		pC->mm_config__outer_offset_mm;

	Status = VL53L1_set_part_to_part_data(Dev, &cal_data);

	return Status;

}

/**
 * @brief Gets the Calibration Data.
 *
 * @par Function Description
 * This function get all the Calibration Data issued from the functions
 *  VL53L1_PerformRefSpadManagement(),  VL53L1_PerformXTalkCalibration,
 *  VL53L1_PerformOffsetCalibration()
 *
 * @note This function doesn't Accesses the device
 *
 * @param   Dev                          Device Handle
 * @param   *pCalibrationData            pointer where to store Calibration
 *  data.
 * @return  VL53L1_ERROR_NONE            Success
 * @return  "Other error code"           See ::VL53L1_Error
 */
VL53L1_Error VL53L1_GetCalibrationData(VL53L1_DEV Dev,
		VL53L1_CalibrationData_t  *pCalibrationData)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	VL53L1_calibration_data_t      cal_data;
	VL53L1_CustomerNvmManaged_t         *pC;
	VL53L1_customer_nvm_managed_t       *pC2;



	/* struct_version is filled inside get part to part function */
	Status = VL53L1_get_part_to_part_data(Dev, &cal_data);

	pCalibrationData->struct_version = cal_data.struct_version +
			VL53L1_ADDITIONAL_CALIBRATION_DATA_STRUCT_VERSION;


	/* memcpy(DEST, SRC, N)  */
	memcpy(
		&(pCalibrationData->add_off_cal_data),
		&(cal_data.add_off_cal_data),
		sizeof(VL53L1_additional_offset_cal_data_t));

	/* memcpy (DEST, SRC, N) */
	memcpy(
		&(pCalibrationData->optical_centre),
		&(cal_data.optical_centre),
		sizeof(VL53L1_optical_centre_t));

	/* memcpy (DEST, SRC, N) */
	memcpy(
		&(pCalibrationData->gain_cal),
		&(cal_data.gain_cal),
		sizeof(VL53L1_gain_calibration_data_t));

	/* memcpy (DEST, SRC, N) */
	memcpy(
		&(pCalibrationData->cal_peak_rate_map),
		&(cal_data.cal_peak_rate_map),
		sizeof(VL53L1_cal_peak_rate_map_t));


	pC = &pCalibrationData->customer;
	pC2 = &cal_data.customer;
	pC->global_config__spad_enables_ref_0 =
		pC2->global_config__spad_enables_ref_0;
	pC->global_config__spad_enables_ref_1 =
		pC2->global_config__spad_enables_ref_1;
	pC->global_config__spad_enables_ref_2 =
		pC2->global_config__spad_enables_ref_2;
	pC->global_config__spad_enables_ref_3 =
		pC2->global_config__spad_enables_ref_3;
	pC->global_config__spad_enables_ref_4 =
		pC2->global_config__spad_enables_ref_4;
	pC->global_config__spad_enables_ref_5 =
		pC2->global_config__spad_enables_ref_5;
	pC->global_config__ref_en_start_select =
		pC2->global_config__ref_en_start_select;
	pC->ref_spad_man__num_requested_ref_spads =
		pC2->ref_spad_man__num_requested_ref_spads;
	pC->ref_spad_man__ref_location =
		pC2->ref_spad_man__ref_location;
	pC->algo__crosstalk_compensation_x_plane_gradient_kcps =
		pC2->algo__crosstalk_compensation_x_plane_gradient_kcps;
	pC->algo__crosstalk_compensation_y_plane_gradient_kcps =
		pC2->algo__crosstalk_compensation_y_plane_gradient_kcps;
	pC->ref_spad_char__total_rate_target_mcps =
		pC2->ref_spad_char__total_rate_target_mcps;
	pC->algo__part_to_part_range_offset_mm =
		pC2->algo__part_to_part_range_offset_mm;
	pC->mm_config__inner_offset_mm =
		pC2->mm_config__inner_offset_mm;
	pC->mm_config__outer_offset_mm =
		pC2->mm_config__outer_offset_mm;

	pC->algo__crosstalk_compensation_plane_offset_kcps =
		(uint32_t)(
			pC2->algo__crosstalk_compensation_plane_offset_kcps);

	return Status;
}

/**
 * @brief Gets the optical center.
 *
 * @par Function Description
 * This function get the optical center issued from the nvm set at FTM stage
 * expressed in the same coordinate system as the ROI are
 *
 * @note This function doesn't Accesses the device
 *
 * @param   Dev                          Device Handle
 * @param   *pOpticalCentreX             pointer to the X position of center
 * in 16.16 fix point
 * @param   *pOpticalCentreY             pointer to the Y position of center
 * in 16.16 fix point
 * @return  VL53L1_ERROR_NONE            Success
 * @return  "Other error code"           See ::VL53L1_Error
 */
VL53L1_Error VL53L1_GetOpticalCenter(VL53L1_DEV Dev,
		FixPoint1616_t *pOpticalCenterX,
		FixPoint1616_t *pOpticalCenterY)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	VL53L1_calibration_data_t  CalibrationData;



	*pOpticalCenterX = 0;
	*pOpticalCenterY = 0;
	Status = VL53L1_get_part_to_part_data(Dev, &CalibrationData);
	if (Status == VL53L1_ERROR_NONE) {
		*pOpticalCenterX = VL53L1_FIXPOINT44TOFIXPOINT1616(
				CalibrationData.optical_centre.x_centre);
		*pOpticalCenterY = VL53L1_FIXPOINT44TOFIXPOINT1616(
				CalibrationData.optical_centre.y_centre);
	}


	return Status;
}

/** @} VL53L1_Calibration_group */

/** @defgroup VL53L1_Thresholds_group VL53L1 IRQ Triggered events Functions
 *  @brief    Functions used to configure interrupt to be triggered only when
 *  a measurement satisfies some thresholds parameters
 *  @{
 */

/**
* @brief Configure the interrupt config, from the given structure
*
* @param[in]    Dev     : Device Handle
* @param[in]    pConfig : pointer to configuration structure
*/
VL53L1_Error VL53L1_SetThresholdConfig(VL53L1_DEV Dev,
		VL53L1_DetectionConfig_t *pConfig)
{
#define BADTHRESBOUNDS(T) \
	(((T.CrossMode == VL53L1_THRESHOLD_OUT_OF_WINDOW) || \
	(T.CrossMode == VL53L1_THRESHOLD_IN_WINDOW)) && (T.Low > T.High))

	VL53L1_Error Status = VL53L1_ERROR_NONE;
	VL53L1_GPIO_interrupt_config_t Cfg;
	uint16_t g;
	FixPoint1616_t gain, high1616, low1616;
	VL53L1_LLDriverData_t *pdev;



	pdev = VL53L1DevStructGetLLDriverHandle(Dev);

	Status = VL53L1_get_GPIO_interrupt_config(Dev, &Cfg);
	if (Status == VL53L1_ERROR_NONE) {
		if (pConfig->DetectionMode == VL53L1_DETECTION_NORMAL_RUN) {
			Cfg.intr_new_measure_ready = 1;
			Status = VL53L1_set_GPIO_interrupt_config_struct(Dev,
					Cfg);
		} else {
			if (BADTHRESBOUNDS(pConfig->Distance))
				Status = VL53L1_ERROR_INVALID_PARAMS;
			if ((Status == VL53L1_ERROR_NONE) &&
					(BADTHRESBOUNDS(pConfig->Rate)))
				Status = VL53L1_ERROR_INVALID_PARAMS;
			if (Status == VL53L1_ERROR_NONE) {
				Cfg.intr_new_measure_ready = 0;
				Cfg.intr_no_target = pConfig->IntrNoTarget;
				/* fix ticket 466238
				 * Apply invert distance gain to thresholds */
				g = pdev->gain_cal.standard_ranging_gain_factor;
				/* gain is ufix 5.11, convert to 16.16 */
				gain = (FixPoint1616_t) ((uint32_t)g << 5);
				high1616 = (FixPoint1616_t) ((uint32_t)
						pConfig->Distance.High << 16);
				low1616 = (FixPoint1616_t) ((uint32_t)
						pConfig->Distance.Low << 16);
				/* +32768 to round the results*/
				high1616 = (high1616 + 32768) / gain;
				low1616 = (low1616 + 32768) / gain;
				Cfg.threshold_distance_high = (uint16_t)
						(high1616 & 0xFFFF);
				Cfg.threshold_distance_low = (uint16_t)
						(low1616 & 0xFFFF);
				/* end fix ticket 466238 */
				Cfg.threshold_rate_high =
					VL53L1_FIXPOINT1616TOFIXPOINT97(
							pConfig->Rate.High);
				Cfg.threshold_rate_low =
					VL53L1_FIXPOINT1616TOFIXPOINT97(
							pConfig->Rate.Low);

				Cfg.intr_mode_distance = ConvertModeToLLD(
						&Status,
						pConfig->Distance.CrossMode);
				if (Status == VL53L1_ERROR_NONE)
					Cfg.intr_mode_rate = ConvertModeToLLD(
						&Status,
						pConfig->Rate.CrossMode);
			}

			/* Refine thresholds combination now */
			if (Status == VL53L1_ERROR_NONE) {
				Cfg.intr_combined_mode = 1;
				switch (pConfig->DetectionMode) {
				case VL53L1_DETECTION_DISTANCE_ONLY:
					Cfg.threshold_rate_high = 0;
					Cfg.threshold_rate_low = 0;
					break;
				case VL53L1_DETECTION_RATE_ONLY:
					Cfg.threshold_distance_high = 0;
					Cfg.threshold_distance_low = 0;
					break;
				case VL53L1_DETECTION_DISTANCE_OR_RATE:
					/* Nothing to do all is already
					 * in place
					 */
					break;
				case VL53L1_DETECTION_DISTANCE_AND_RATE:
					Cfg.intr_combined_mode = 0;
					break;
				default:
					Status = VL53L1_ERROR_INVALID_PARAMS;
				}
			}

			if (Status == VL53L1_ERROR_NONE)
				Status =
				VL53L1_set_GPIO_interrupt_config_struct(Dev,
						Cfg);

		}
	}


	return Status;
}

/**
* @brief Retrieves the interrupt config structure currently programmed
*                             into the API
*
* @param[in]    Dev     : Device Handle
* @param[out]   pConfig : pointer to configuration structure
*/

VL53L1_Error VL53L1_GetThresholdConfig(VL53L1_DEV Dev,
		VL53L1_DetectionConfig_t *pConfig)
{
	VL53L1_Error Status = VL53L1_ERROR_NONE;
	VL53L1_GPIO_interrupt_config_t Cfg;



	Status = VL53L1_get_GPIO_interrupt_config(Dev, &Cfg);

	if (Status != VL53L1_ERROR_NONE) {

		return Status;
	}

	pConfig->IntrNoTarget = Cfg.intr_no_target;
	pConfig->Distance.High = Cfg.threshold_distance_high;
	pConfig->Distance.Low = Cfg.threshold_distance_low;
	pConfig->Rate.High =
		VL53L1_FIXPOINT97TOFIXPOINT1616(
				Cfg.threshold_rate_high);
	pConfig->Rate.Low =
		VL53L1_FIXPOINT97TOFIXPOINT1616(Cfg.threshold_rate_low);
	pConfig->Distance.CrossMode =
		ConvertModeFromLLD(&Status, Cfg.intr_mode_distance);
	if (Status == VL53L1_ERROR_NONE)
		pConfig->Rate.CrossMode =
			ConvertModeFromLLD(&Status, Cfg.intr_mode_rate);

	if (Cfg.intr_new_measure_ready == 1) {
		pConfig->DetectionMode = VL53L1_DETECTION_NORMAL_RUN;
	} else {
		/* Refine thresholds combination now */
		if (Status == VL53L1_ERROR_NONE) {
			if (Cfg.intr_combined_mode == 0)
				pConfig->DetectionMode =
				VL53L1_DETECTION_DISTANCE_AND_RATE;
			else {
				if ((Cfg.threshold_distance_high == 0) &&
					(Cfg.threshold_distance_low == 0))
					pConfig->DetectionMode =
					VL53L1_DETECTION_RATE_ONLY;
				else if ((Cfg.threshold_rate_high == 0) &&
					(Cfg.threshold_rate_low == 0))
					pConfig->DetectionMode =
					VL53L1_DETECTION_DISTANCE_ONLY;
				else
					pConfig->DetectionMode =
					VL53L1_DETECTION_DISTANCE_OR_RATE;
			}
		}
	}


	return Status;
}


/** @} VL53L1_Thresholds_group */


/** @} VL53L1_cut11_group */

/** @} */

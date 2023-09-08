/****************************************************************************
*
* Copyright ï¿½ 2020 STMicroelectronics - All Rights Reserved
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
 * @file     uld_api.h

 * @brief   SPC5xx  uld_api.h header file.
 *
 * @addtogroup uld_api
 * @{
 */

#ifndef LIB_ULD_API_STRUCTS_H_
#define LIB_ULD_API_STRUCTS_H_

#include "AEK_SNS_VL53L1X1_cfg.h"
#ifdef ULD_enable
#include "VL53L1X_api.h"
#include "VL53L1X_calibration.h"
#include <siul_lld.h>

/*OBJECT ULD*/
/**
 *  @brief defines VL53L1X Object

 *  @attributes:
    1) Dev is Sensor device address
    2) Xshut is struct for turn on and turn off sensors
    3) IRQState is the interrupt state of the sensor value : (0 or 1)
    4) stateSensor is the sensor state value (turn on (1) or turn off (0));
*/
typedef void (*ULD_GetErrorSensorState_ptr)(AEK_TOF_DEV index,AEK_TOF_ERROR *state);

typedef void (*ULD_SetIRQSensorState_ptr)(AEK_TOF_DEV index);

typedef void (*ULD_GetIRQSensorState_ptr)(AEK_TOF_DEV index,volatile uint8_t *state);

typedef void (*ULD_TurnOnSensor_ptr)(AEK_TOF_DEV index);

typedef void (*ULD_TurnOffSensor_ptr)(AEK_TOF_DEV index);

typedef void (*ULD_TurnOnAllSensors_ptr)(void);

typedef void (*ULD_TurnOffAllSensors_ptr)(void);

typedef i2c_result_t (*ULD_GetSWVersion_ptr)(VL53L1X_Version_t *pVersion);

typedef i2c_result_t (*ULD_SetI2CAddress_ptr)(AEK_TOF_DEV index, uint8_t new_address);

typedef i2c_result_t (*ULD_SensorInit_ptr)(AEK_TOF_DEV index);

typedef i2c_result_t (*ULD_ClearInterrupt_ptr)(AEK_TOF_DEV index);

typedef i2c_result_t (*ULD_SetInterruptPolarity_ptr)(AEK_TOF_DEV index, uint8_t IntPol);

typedef i2c_result_t (*ULD_GetInterruptPolarity_ptr)(AEK_TOF_DEV index, uint8_t *pIntPol);

typedef i2c_result_t (*ULD_StartRanging_ptr)(AEK_TOF_DEV index);

typedef i2c_result_t (*ULD_StopRanging_ptr)(AEK_TOF_DEV index);

typedef i2c_result_t (*ULD_CheckForDataReady_ptr)(AEK_TOF_DEV index, uint8_t *isDataReady);

typedef i2c_result_t (*ULD_SetTimingBudgetInMs_ptr)(AEK_TOF_DEV index, uint16_t TimingBudgetInMs);

typedef i2c_result_t (*ULD_GetTimingBudgetInMs_ptr)(AEK_TOF_DEV index, uint16_t *pTimingBudgetInMs);

typedef i2c_result_t (*ULD_SetDistanceMode_ptr)(AEK_TOF_DEV index, uint16_t DistanceMode);

typedef i2c_result_t (*ULD_GetDistanceMode_ptr)(AEK_TOF_DEV index, uint16_t *pDistanceMode);

typedef i2c_result_t (*ULD_SetInterMeasurementInMs_ptr)(AEK_TOF_DEV index, uint32_t InterMeasurementInMs);

typedef i2c_result_t (*ULD_GetInterMeasurementInMs_ptr)(AEK_TOF_DEV index, uint16_t * pIM);

typedef i2c_result_t (*ULD_BootState_ptr)(AEK_TOF_DEV index, uint8_t *state);

typedef i2c_result_t (*ULD_GetSensorId_ptr)(AEK_TOF_DEV index, uint16_t *id);

typedef i2c_result_t (*ULD_GetDistance_ptr)(AEK_TOF_DEV index, uint16_t *distance);

typedef i2c_result_t (*ULD_GetSignalPerSpad_ptr)(AEK_TOF_DEV index, uint16_t *signalPerSp);

typedef i2c_result_t (*ULD_GetAmbientPerSpad_ptr)(AEK_TOF_DEV index, uint16_t *amb);

typedef i2c_result_t (*ULD_GetSignalRate_ptr)(AEK_TOF_DEV index, uint16_t *signalRate);

typedef i2c_result_t (*ULD_GetSpadNb_ptr)(AEK_TOF_DEV index, uint16_t *spNb);

typedef i2c_result_t (*ULD_GetAmbientRate_ptr)(AEK_TOF_DEV index, uint16_t *ambRate);

typedef i2c_result_t (*ULD_GetRangeStatus_ptr)(AEK_TOF_DEV index, uint8_t *rangeStatus);

typedef i2c_result_t (*ULD_GetResult_ptr)(AEK_TOF_DEV index, VL53L1X_Result_t *pResult);

typedef i2c_result_t (*ULD_SetOffset_ptr)(AEK_TOF_DEV index, int16_t OffsetValue);

typedef i2c_result_t (*ULD_GetOffset_ptr)(AEK_TOF_DEV index, int16_t *Offset);

typedef i2c_result_t (*ULD_SetXtalk_ptr)(AEK_TOF_DEV index, uint16_t XtalkValue);

typedef i2c_result_t (*ULD_GetXtalk_ptr)(AEK_TOF_DEV index, uint16_t *Xtalk);

typedef i2c_result_t (*ULD_SetDistanceThreshold_ptr)(AEK_TOF_DEV index, uint16_t ThreshLow,uint16_t ThreshHigh,uint8_t Window,uint8_t IntOnNoTarget);

typedef i2c_result_t (*ULD_GetDistanceThresholdWindow_ptr)(AEK_TOF_DEV index, uint16_t *window);

typedef i2c_result_t (*ULD_GetDistanceThresholdWindow_ptr)(AEK_TOF_DEV index, uint16_t *window);

typedef i2c_result_t (*ULD_GetDistanceThresholdWindow_ptr)(AEK_TOF_DEV index, uint16_t *window);

typedef i2c_result_t (*ULD_GetDistanceThresholdWindow_ptr)(AEK_TOF_DEV index, uint16_t *window);

typedef i2c_result_t (*ULD_GetDistanceThresholdWindow_ptr)(AEK_TOF_DEV index, uint16_t *window);

typedef i2c_result_t (*ULD_GetDistanceThresholdLow_ptr)(AEK_TOF_DEV index, uint16_t *low);

typedef i2c_result_t (*ULD_GetDistanceThresholdHigh_ptr)(AEK_TOF_DEV index, uint16_t *high);

typedef i2c_result_t (*ULD_SetROI_ptr)(AEK_TOF_DEV index, uint16_t X, uint16_t Y);

typedef i2c_result_t (*ULD_GetROI_XY_ptr)(AEK_TOF_DEV index, uint16_t *ROI_X, uint16_t *ROI_Y);

typedef i2c_result_t (*ULD_SetROICenter_ptr)(AEK_TOF_DEV index, uint8_t ROICenter);

typedef i2c_result_t (*ULD_GetROICenter_ptr)(AEK_TOF_DEV index, uint8_t *ROICenter);

typedef i2c_result_t (*ULD_SetSignalThreshold_ptr)(AEK_TOF_DEV index, uint16_t signal);

typedef i2c_result_t (*ULD_GetSignalThreshold_ptr)(AEK_TOF_DEV index, uint16_t *signal);

typedef i2c_result_t (*ULD_SetSigmaThreshold_ptr)(AEK_TOF_DEV index, uint16_t sigma);

typedef i2c_result_t (*ULD_GetSigmaThreshold_ptr)(AEK_TOF_DEV index, uint16_t *signal);

typedef i2c_result_t (*ULD_StartTemperatureUpdate_ptr)(AEK_TOF_DEV index);

typedef i2c_result_t (*ULD_CalibrateOffset_ptr)(AEK_TOF_DEV index, uint16_t TargetDistInMm, int16_t *offset);

typedef i2c_result_t (*ULD_CalibrateXtalk_ptr)(AEK_TOF_DEV index, uint16_t TargetDistInMm, uint16_t *xtalk);

typedef struct
{
	/*
     * 	@brief Get error
	 *
	 * 	@param[in]  AEK_TOF_DEV index ->  AEK_TOF_DEV0 for refer to first sensor
	 *
	 * 	@param[in]	AEK_TOF_ERROR *state -> state = AEK_TOF_NO_ERROR : Success ||  state = AEK_TOF_ERROR_CONFIGURATION : Error
	 */
	ULD_GetErrorSensorState_ptr GetErrorSensorState;
	/*
     * 	@brief Set IRQ variable
	 *
	 * 	@param[in]  AEK_TOF_DEV index -> AEK_TOF_DEV0 for refer to first sensor
	 *
	 */
	ULD_SetIRQSensorState_ptr SetIRQSensorState;
	/*
     * 	@brief Get IRQ variable
	 *
	 * 	@param[in]  AEK_TOF_DEV index -> AEK_TOF_DEV0 for refer to first sensor
	 *
	 *	@param[in]  volatile uint8_t *var-> volatile uint8_t ptr
	 */
	ULD_GetIRQSensorState_ptr GetIRQSensorState;
	/*
     * 	@brief Turn on sensor (Xshut state = 1)
	 *
	 * 	@param[in]  AEK_TOF_DEV index ->  AEK_TOF_DEV0 for refer to first sensor
	 *
	 */
	ULD_TurnOnSensor_ptr TurnOnSensor;
	/*
     * 	@brief Turn on sensor (Xshut state = 0)
	 *
	 * 	@param[in]  AEK_TOF_DEV index ->  AEK_TOF_DEV0 for refer to first sensor
	 *
	 */
    ULD_TurnOffSensor_ptr TurnOffSensor;
	/*
     * 	@brief Turn on all sensor (Xshut state = 1)
	 */
    ULD_TurnOnAllSensors_ptr TurnOnAllSensors;
	/*
     * 	@brief Turn off all sensor (Xshut state = 0)
	 */
	ULD_TurnOffAllSensors_ptr TurnOffAllSensors;
    
    ULD_GetSWVersion_ptr GetSWVersion;

    /**
     *  @brief      Edit the slave address
     *
     *	@param[in]  AEK_TOF_DEV index
     *
     *	@param[in]  uint8_t new_address - 8bit new address value
     *
     *  @return     i2c_result_t
     *
     *
     */
	ULD_SetI2CAddress_ptr SetI2CAddress;
	/**
	 *  @brief      VL53L1X initialization
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_SensorInit_ptr SensorInit;
	/**
	 *  @brief      Clear interrupt
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_ClearInterrupt_ptr ClearInterrupt;
	/**
	 *  @brief      Set interrupt polarity value
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint8_t newPolarity - 8bit polarity value
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_SetInterruptPolarity_ptr SetInterruptPolarity;
	/**
	 *  @brief      Get interrupt polarity value
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint8_t pInterruptPolarity - 8bit pointer to interrupt polarity value
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_GetInterruptPolarity_ptr GetInterruptPolarity;
	/**
	 *  @brief      Start ranging sensor
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_StartRanging_ptr StartRanging;
	/**
	 *  @brief      Stop ranging fuction
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_StopRanging_ptr StopRanging;
	/**
	 *  @brief      Check if the data is ready
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint8_t isDataReady - 8bit the value is 0 or 1 (0 = not ready , 1 = ready)
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_CheckForDataReady_ptr CheckForDataReady;
	/**
	 *  @brief      Set timing budget(ms)
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint16_t timingBudgetInMs - 16bit timing budget value(ms)
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_SetTimingBudgetInMs_ptr SetTimingBudgetInMs;
	/**
	 *  @brief      Get timing budget(ms)
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint16_t *pTimingBudget - 16bit pointer of timing budget value(ms)
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_GetTimingBudgetInMs_ptr GetTimingBudgetInMs;
	/**
	 *  @brief      Set Distance Mode
	 *
	 *	@param[in]  AEK_TOF_DEV index 
	 *
	 *	@param[in]  uint16_t distanceMode - 16bit distance mode value
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_SetDistanceMode_ptr SetDistanceMode;
	/**
	 *  @brief      Get distance mode
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint16_t *distanceMode - 16bit pointer to distance mode value
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_GetDistanceMode_ptr GetDistanceMode;
	/**
	 *  @brief      Set inter measurement(ms)
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint32_t *InterMeasMs - 32bit inter measurement value(ms)
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_SetInterMeasurementInMs_ptr SetInterMeasurementInMs;
	/**
	 *  @brief      Get inter measurement(ms)
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint32_t *InterMeasMs - 32bit pointer to inter measurement value(ms)
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_GetInterMeasurementInMs_ptr GetInterMeasurementInMs;
	/**
	 *  @brief      Check boot state
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint8_t *state - 8bit pointer to boot state value
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 */
	ULD_BootState_ptr BootState;
	/**
	 *  @brief      Get sensor id
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint16_t *sensorId - 16bit pointer to sensor id value
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_GetSensorId_ptr GetSensorId;

	/**
	 *  @brief      Get distance
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint16_t *distance - 16bit pointer to distance value
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_GetDistance_ptr GetDistance;
	/**
	 *  @brief      Get signal per spad
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint16_t *signalRate - 16bit pointer to signal rate value
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_GetSignalPerSpad_ptr GetSignalPerSpad;
	/**
	 *  @brief      Get ambient per spad
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint16_t *ambPerSpad - 16bit pointer to ambient per spad value
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_GetAmbientPerSpad_ptr GetAmbientPerSpad;
	/**
	 *  @brief      Get signal rate
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint16_t *signal - 16bit pointer to signal rate value
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_GetSignalRate_ptr GetSignalRate;
	/**
	 *  @brief      Get number of enabled SPADs
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint16_t *spNB - 16bit pointer to number of enabled SPADs value
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_GetSpadNb_ptr GetSpadNb;

	/**
	 *  @brief      Get ambient rate
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint16_t *ambRate - 16bit pointer to ambient rate value
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_GetAmbientRate_ptr GetAmbientRate;

	/**
	 *  @brief      Get range status
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint16_t *rangeStatus - 16bit pointer to range status value
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_GetRangeStatus_ptr GetRangeStatus;

	/**
	 *  @brief      Get result (measurements and the range status)
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  VL53L1X_Result_t *pResult - pointer to result (measurements and the range status) value
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_GetResult_ptr GetResult;

	/**
	 *  @brief      Set offset
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint16_t offsetValue - 16bit offset value
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_SetOffset_ptr SetOffset;
	/**
	 *  @brief      Get offset
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint16_t *offset - 16bit pointer to offset value
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_GetOffset_ptr GetOffset;
	/**
	 *  @brief      Set Xtalk
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint16_t XtalkValue - 16bit Xtalk value
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_SetXtalk_ptr  SetXtalk;

	/**
	 *  @brief      Get Xtalk
	 *
	 *	@param[in] AEK_TOF_DEV index
	 *
	 *	@param[in]  uint16_t *Xtalk - 16bit pointer to Xtalk value
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_GetXtalk_ptr  GetXtalk;

	/**
	 * @brief This function programs the threshold detection mode
	 * Example:
	 * SetDistanceThreshold(AEK_TOF_DEV0,100,300,0,1): Below 100
	 * SetDistanceThreshold(AEK_TOF_DEV0,100,300,1,1): Above 300
	 * SetDistanceThreshold(AEK_TOF_DEV0,100,300,2,1): Out of window
	 * SetDistanceThreshold(AEK_TOF_DEV0,100,300,3,1): In window
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint16_t threshLow - 16bit thresh low value
	 *
	 *	@param[in]  uint16_t threshHigh - 16bit thresh high value
	 *
	 *	@param[in]  uint8_t window - 8bit window value
	 *
	 *	@param[in]  uint8_t intOnNoTarget - 8bit intOnNoTarget value
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_SetDistanceThreshold_ptr SetDistanceThreshold;

	/**
	 *  @brief      Get distance threshold window
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint8_t *window - 16bit pointer to window value
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_GetDistanceThresholdWindow_ptr GetDistanceThresholdWindow;
	/**
	 *  @brief      Get distance threshold window low
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint16_t *low - 16bit pointer to low value
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_GetDistanceThresholdLow_ptr GetDistanceThresholdLow;

	/**
	 *  @brief      Get distance threshold window high
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint16_t *high - 16bit pointer to high value
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_GetDistanceThresholdHigh_ptr GetDistanceThresholdHigh;

	/**
	 *  @brief     Set ROI center
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint16_t ROICenter - 16bit ROI Center value
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_SetROI_ptr SetROI;

	/**
	 *  @brief     Get ROI center
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint16_t *ROICenter - 16bit pointer to ROI center value
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_GetROI_XY_ptr GetROI_XY;
	/**
	 *  @brief    	Set ROI
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint16_t X - 16bit X axel value
	 *
	 *	@param[in]  uint16_t Y - 16bit Y axel value
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */

	ULD_SetROICenter_ptr SetROICenter;
	/**
	 *  @brief    	Get ROI
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint16_t *ROI_X - 16bit pointer to ROI X axel value
	 *
	 *	@param[in]  uint16_t *ROI_Y - 16bit pointer to ROI Y axel value
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */

	ULD_GetROICenter_ptr GetROICenter;

	/**
	 *  @brief    	Set signal threshold
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint16_t signal - 16bit signal value
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_SetSignalThreshold_ptr SetSignalThreshold;

	/**
	 *  @brief    	Get signal threshold
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint16_t *signal - 16bit pointer to signal value
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_GetSignalThreshold_ptr GetSignalThreshold;
	/**
	 *  @brief    	Set sigma threshold
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint16_t sigma - 16bit sigma value
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_SetSigmaThreshold_ptr SetSigmaThreshold;
	/**
	 *  @brief    	Get sigma threshold
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 *	@param[in]  uint16_t *sigma - 16bit pointer to sigma value
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_GetSigmaThreshold_ptr GetSigmaThreshold;
	/**
	 *  @brief    	Start temperature update
	 *
	 *	@param[in]  AEK_TOF_DEV index
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 *
	 *
	 */
	ULD_StartTemperatureUpdate_ptr StartTemperatureUpdate;
	/**
	 * 	@brief This function performs the offset calibration.
	 * 	The function returns the offset value found and programs the offset compensation into the device.
	 *
	 * 	@param[in] TargetDistInMm target distance in mm, ST recommended 100 mm Target reflectance = grey17%
	 *
	 *	@param[in] uint8_t * offset : offset pointer contains the offset found in mm
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 */
	ULD_CalibrateOffset_ptr CalibrateOffset;
	/**
	 * 	@brief This function performs the xtalk calibration.
	 * 	The function returns the xtalk value found and programs the xtalk compensation to the device
	 *
	 * 	@param[in] TargetDistInMm target distance in mm  The target distance : the distance where the sensor start to "under range"
	 * 	due to the influence of the photons reflected back from the cover glass becoming strong\n
	 * 	It's also called inflection point target reflectance = grey 17%
	 *
	 *	@param[in] uint8_t *xtalk : xtalk pointer contains the xtalk value found in cps (number of photons in count per second)
	 *
	 * 	@return i2c_result_t status : status = 0 success, status !=0 failed
	 */
	ULD_CalibrateXtalk_ptr CalibrateXtalk;
}methods_t;

/**
 * @brief This function returns the SW driver version
 */
i2c_result_t ULD_GetSWVersion(VL53L1X_Version_t *pVersion);

/**
 * @brief This function sets the sensor I2C address used in case multiple devices application, default address 0x52
 */
i2c_result_t ULD_SetI2CAddress(AEK_TOF_DEV index, uint8_t new_address);

/**
 * @brief This function loads the 135 bytes default values to initialize the sensor.
 * @param dev Device address
 * @return 0:success, != 0:failed
 */
i2c_result_t ULD_SensorInit(AEK_TOF_DEV index);

/**
 * @brief This function clears the interrupt, to be called after a ranging data reading
 * to arm the interrupt for the next data ready event.
 */
i2c_result_t ULD_ClearInterrupt(AEK_TOF_DEV index);

/**
 * @brief This function programs the interrupt polarity
 * 1=active high (default), 0=active low
 */
i2c_result_t ULD_SetInterruptPolarity(AEK_TOF_DEV index, uint8_t IntPol);

/**
 * @brief This function returns the current interrupt polarity
 * 1=active high (default), 0=active low
 */
i2c_result_t ULD_GetInterruptPolarity(AEK_TOF_DEV index, uint8_t *pIntPol);

/**
 * @brief This function starts the ranging distance operation
 * The ranging operation is continuous. The clear interrupt has to be done after each get data to allow the interrupt to raise when the next data is ready
 * 1=active high (default), 0=active low, use SetInterruptPolarity() to change the interrupt polarity if required.
 */
i2c_result_t ULD_StartRanging(AEK_TOF_DEV index);

/**
 * @brief This function stops the ranging.
 */
i2c_result_t ULD_StopRanging(AEK_TOF_DEV index);

/**
 * @brief This function checks if the new ranging data is available by polling the dedicated register.
 * @param : isDataReady==0 -> not ready; isDataReady==1 -> ready
 */
i2c_result_t ULD_CheckForDataReady(AEK_TOF_DEV index, uint8_t *isDataReady);

/**
 * @brief This function programs the timing budget in ms.
 * Predefined values = 15, 20, 33, 50, 100(default), 200, 500.
 */
i2c_result_t ULD_SetTimingBudgetInMs(AEK_TOF_DEV index, uint16_t TimingBudgetInMs);

/**
 * @brief This function returns the current timing budget in ms.
 */
i2c_result_t ULD_GetTimingBudgetInMs(AEK_TOF_DEV index, uint16_t *pTimingBudgetInMs);

/**
 * @brief This function programs the distance mode (1=short, 2=long(default)).
 * Short mode max distance is limited to 1.3 m but better ambient immunity.
 * Long mode can range up to 4 m in the dark with 200 ms timing budget.
 */
i2c_result_t ULD_SetDistanceMode(AEK_TOF_DEV index, uint16_t DistanceMode);

/**
 * @brief This function returns the current distance mode (1=short, 2=long).
 */
i2c_result_t ULD_GetDistanceMode(AEK_TOF_DEV index, uint16_t *pDistanceMode);

/**
 * @brief This function programs the Intermeasurement period in ms
 * Intermeasurement period must be >/= timing budget. This condition is not checked by the API,
 * the customer has the duty to check the condition. Default = 100 ms
 */
i2c_result_t ULD_SetInterMeasurementInMs(AEK_TOF_DEV index,
					 uint32_t InterMeasurementInMs);

/**
 * @brief This function returns the Intermeasurement period in ms.
 */
i2c_result_t ULD_GetInterMeasurementInMs(AEK_TOF_DEV index, uint16_t * pIM);

/**
 * @brief This function returns the boot state of the device (1:booted, 0:not booted)
 */
i2c_result_t ULD_BootState(AEK_TOF_DEV index, uint8_t *state);

/**
 * @brief This function returns the sensor id, sensor Id must be 0xEEAC
 */
i2c_result_t ULD_GetSensorId(AEK_TOF_DEV index, uint16_t *id);

/**
 * @brief This function returns the distance measured by the sensor in mm
 */
i2c_result_t ULD_GetDistance(AEK_TOF_DEV index, uint16_t *distance);

/**
 * @brief This function returns the returned signal per SPAD in kcps/SPAD.
 * With kcps stands for Kilo Count Per Second
 */
i2c_result_t ULD_GetSignalPerSpad(AEK_TOF_DEV index, uint16_t *signalPerSp);

/**
 * @brief This function returns the ambient per SPAD in kcps/SPAD
 */
i2c_result_t ULD_GetAmbientPerSpad(AEK_TOF_DEV index, uint16_t *amb);

/**
 * @brief This function returns the returned signal in kcps.
 */
i2c_result_t ULD_GetSignalRate(AEK_TOF_DEV index, uint16_t *signalRate);

/**
 * @brief This function returns the current number of enabled SPADs
 */
i2c_result_t ULD_GetSpadNb(AEK_TOF_DEV index, uint16_t *spNb);

/**
 * @brief This function returns the ambient rate in kcps
 */
i2c_result_t ULD_GetAmbientRate(AEK_TOF_DEV index, uint16_t *ambRate);

/**
 * @brief This function returns the ranging status error
 * (0:no error, 1:sigma failed, 2:signal failed, ..., 7:wrap-around)
 */
i2c_result_t ULD_GetRangeStatus(AEK_TOF_DEV index, uint8_t *rangeStatus);

/**
 * @brief This function returns measurements and the range status in a single read access
 */
i2c_result_t ULD_GetResult(AEK_TOF_DEV index, VL53L1X_Result_t *pResult);

/**
 * @brief This function programs the offset correction in mm
 * @param OffsetValue:the offset correction value to program in mm
 */
i2c_result_t ULD_SetOffset(AEK_TOF_DEV index, int16_t OffsetValue);

/**
 * @brief This function returns the programmed offset correction value in mm
 */
i2c_result_t ULD_GetOffset(AEK_TOF_DEV index, int16_t *Offset);

/**
 * @brief This function programs the xtalk correction value in cps (Count Per Second).
 * This is the number of photons reflected back from the cover glass in cps.
 */
i2c_result_t ULD_SetXtalk(AEK_TOF_DEV index, uint16_t XtalkValue);

/**
 * @brief This function returns the current programmed xtalk correction value in cps
 */
i2c_result_t ULD_GetXtalk(AEK_TOF_DEV index, uint16_t *Xtalk);

/**
 * @brief This function programs the threshold detection mode
 * Example:
 * VL53L1X_SetDistanceThreshold(dev,100,300,0,1): Below 100
 * VL53L1X_SetDistanceThreshold(dev,100,300,1,1): Above 300
 * VL53L1X_SetDistanceThreshold(dev,100,300,2,1): Out of window
 * VL53L1X_SetDistanceThreshold(dev,100,300,3,1): In window
 */
i2c_result_t ULD_SetDistanceThreshold(AEK_TOF_DEV index, uint16_t ThreshLow,uint16_t ThreshHigh,
		uint8_t Window,
		uint8_t IntOnNoTarget);

/**
 * @brief This function returns the window detection mode (0=below; 1=above; 2=out; 3=in)
 */
i2c_result_t ULD_GetDistanceThresholdWindow(AEK_TOF_DEV index, uint16_t *window);

/**
 * @brief This function returns the low threshold in mm
 */
i2c_result_t ULD_GetDistanceThresholdLow(AEK_TOF_DEV index, uint16_t *low);

/**
 * @brief This function returns the high threshold in mm
 */
i2c_result_t ULD_GetDistanceThresholdHigh(AEK_TOF_DEV index, uint16_t *high);

/**
 * @brief This function programs the ROI (Region of Interest)
 * The ROI position is centered, only the ROI size can be reprogrammed.
 * The smallest acceptable ROI size = 4
 */
i2c_result_t ULD_SetROI(AEK_TOF_DEV index, uint16_t X, uint16_t Y);

/**
 *@brief This function returns width X and height Y
 */
i2c_result_t ULD_GetROI_XY(AEK_TOF_DEV index, uint16_t *ROI_X, uint16_t *ROI_Y);

/**
 *@brief This function programs the new user ROI center, please to be aware that there is no check in this function.
 *if the ROI center vs ROI size is out of border the ranging function return error #13
 */
i2c_result_t ULD_SetROICenter(AEK_TOF_DEV index, uint8_t ROICenter);

/**
 *@brief This function returns the current user ROI center
 */
i2c_result_t ULD_GetROICenter(AEK_TOF_DEV index, uint8_t *ROICenter);

/**
 * @brief This function programs a new signal threshold in kcps (default=1024 kcps
 */
i2c_result_t ULD_SetSignalThreshold(AEK_TOF_DEV index, uint16_t signal);

/**
 * @brief This function returns the current signal threshold in kcps
 */
i2c_result_t ULD_GetSignalThreshold(AEK_TOF_DEV index, uint16_t *signal);

/**
 * @brief This function programs a new sigma threshold in mm (default = 15 mm)
 */
i2c_result_t ULD_SetSigmaThreshold(AEK_TOF_DEV index, uint16_t sigma);

/**
 * @brief This function returns the current sigma threshold in mm
 */
i2c_result_t ULD_GetSigmaThreshold(AEK_TOF_DEV index, uint16_t *signal);

/**
 * @brief This function performs the temperature calibration.
 * It is recommended to call this function any time the temperature might have changed by more than 8 deg C
 * without sensor ranging activity for an extended period.
 */
i2c_result_t ULD_StartTemperatureUpdate(AEK_TOF_DEV index);

i2c_result_t ULD_CalibrateOffset(AEK_TOF_DEV index, uint16_t TargetDistInMm, int16_t *offset);

i2c_result_t ULD_CalibrateXtalk(AEK_TOF_DEV index, uint16_t TargetDistInMm, uint16_t *xtalk);

void ULD_TurnOnSensor(AEK_TOF_DEV index);

void ULD_TurnOffSensor(AEK_TOF_DEV index);

void ULD_TurnOnAllSensors(void);

void ULD_TurnOffAllSensors(void);

AEK_TOF_ERROR ULD_CreateSensor(AEK_TOF_DEV index, uint8_t Dev, uint8_t XshutPort, uint8_t XshutPin);

void ULD_SetIRQSensorState(AEK_TOF_DEV index);

void ULD_GetIRQSensorState(AEK_TOF_DEV index, volatile uint8_t *state);

void ULD_GetErrorSensorState(AEK_TOF_DEV index,AEK_TOF_ERROR *state);
#endif
#endif/* LIB_INCLUDE_ULD_API_H_ */
/** @} */

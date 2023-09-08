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
 * @file    AEK_SNS_VL53L1X1.c
 * @brief   SPC5xx AEK_SNS_VL53L1X1 driver code.
 *
 * @addtogroup AEK_SNS_VL53L1X1
 * @{
 */

#include "AEK_SNS_VL53L1X1.h"


uint8_t methods_available;
#ifdef ULD_enable
methods_t AEK_TOF_METHODS;

void fillMethodsStructULD(methods_t *AEK_TOF_METHODS)
{
	AEK_TOF_METHODS->GetIRQSensorState					= ULD_GetIRQSensorState;
	AEK_TOF_METHODS->SetIRQSensorState					= ULD_SetIRQSensorState;
	AEK_TOF_METHODS->TurnOnSensor						= ULD_TurnOnSensor;
	AEK_TOF_METHODS->TurnOffSensor						= ULD_TurnOffSensor;
	AEK_TOF_METHODS->TurnOffAllSensors					= ULD_TurnOffAllSensors;
	AEK_TOF_METHODS->TurnOnAllSensors					= ULD_TurnOnAllSensors;
	AEK_TOF_METHODS->GetSWVersion 						= ULD_GetSWVersion;
	AEK_TOF_METHODS->SetI2CAddress                 		= ULD_SetI2CAddress;
	AEK_TOF_METHODS->SensorInit                       	= ULD_SensorInit;
	AEK_TOF_METHODS->ClearInterrupt               		= ULD_ClearInterrupt;
	AEK_TOF_METHODS->SetInterruptPolarity   			= ULD_SetInterruptPolarity;
	AEK_TOF_METHODS->GetInterruptPolarity   			= ULD_GetInterruptPolarity;
	AEK_TOF_METHODS->StartRanging                   	= ULD_StartRanging;
	AEK_TOF_METHODS->StopRanging                     	= ULD_StopRanging;
	AEK_TOF_METHODS->CheckForDataReady         			= ULD_CheckForDataReady;
	AEK_TOF_METHODS->SetTimingBudgetInMs     			= ULD_SetTimingBudgetInMs;
	AEK_TOF_METHODS->GetTimingBudgetInMs     			= ULD_GetTimingBudgetInMs;
	AEK_TOF_METHODS->SetDistanceMode             		= ULD_SetDistanceMode;
	AEK_TOF_METHODS->GetDistanceMode             		= ULD_GetDistanceMode;
	AEK_TOF_METHODS-> SetInterMeasurementInMs 			= ULD_SetInterMeasurementInMs;
	AEK_TOF_METHODS-> GetInterMeasurementInMs 			= ULD_GetInterMeasurementInMs;
	AEK_TOF_METHODS-> BootState                     	= ULD_BootState;
	AEK_TOF_METHODS-> GetSensorId                   	= ULD_GetSensorId;
	AEK_TOF_METHODS-> GetDistance                   	= ULD_GetDistance;
	AEK_TOF_METHODS-> GetSignalPerSpad              	= ULD_GetSignalPerSpad;
	AEK_TOF_METHODS-> GetAmbientPerSpad             	= ULD_GetAmbientPerSpad;
	AEK_TOF_METHODS-> GetSignalRate                 	= ULD_GetSignalRate;
	AEK_TOF_METHODS-> GetSpadNb                     	= ULD_GetSpadNb;
	AEK_TOF_METHODS-> GetAmbientRate                	= ULD_GetAmbientRate;
	AEK_TOF_METHODS-> GetRangeStatus                	= ULD_GetRangeStatus;
	AEK_TOF_METHODS-> GetResult                     	= ULD_GetResult;
	AEK_TOF_METHODS-> SetOffset                     	= ULD_SetOffset;
	AEK_TOF_METHODS-> GetOffset                     	= ULD_GetOffset;
	AEK_TOF_METHODS->  SetXtalk                     	= ULD_SetXtalk;
	AEK_TOF_METHODS->  GetXtalk                     	= ULD_GetXtalk;
	AEK_TOF_METHODS-> SetDistanceThreshold       		= ULD_SetDistanceThreshold;
	AEK_TOF_METHODS-> GetDistanceThresholdWindow 		= ULD_GetDistanceThresholdWindow;
	AEK_TOF_METHODS-> GetDistanceThresholdLow       	= ULD_GetDistanceThresholdLow;
	AEK_TOF_METHODS-> GetDistanceThresholdHigh      	= ULD_GetDistanceThresholdHigh;
	AEK_TOF_METHODS-> SetROI                        	= ULD_SetROI;
	AEK_TOF_METHODS-> GetROI_XY                     	= ULD_GetROI_XY;
	AEK_TOF_METHODS-> SetROICenter                  	= ULD_SetROICenter;
	AEK_TOF_METHODS-> GetROICenter                  	= ULD_GetROICenter;
	AEK_TOF_METHODS-> SetSignalThreshold            	= ULD_SetSignalThreshold;
	AEK_TOF_METHODS-> GetSignalThreshold            	= ULD_GetSignalThreshold;
	AEK_TOF_METHODS-> SetSigmaThreshold             	= ULD_SetSigmaThreshold;
	AEK_TOF_METHODS-> GetSigmaThreshold             	= ULD_GetSigmaThreshold;
	AEK_TOF_METHODS->StartTemperatureUpdate         	= ULD_StartTemperatureUpdate;
	AEK_TOF_METHODS->CalibrateOffset					= ULD_CalibrateOffset;
	AEK_TOF_METHODS->CalibrateXtalk						= ULD_CalibrateXtalk;
}

#endif
#ifdef FULL_enable
methods_t AEK_TOF_METHODS;
void fillMethodsStructFULL(methods_t *AEK_TOF_METHODS)
{
	AEK_TOF_METHODS->GetIRQSensorState						= FULL_GetIRQSensorState;
	AEK_TOF_METHODS->SetIRQSensorState						= FULL_SetIRQSensorState;
	AEK_TOF_METHODS->TurnOnSensor							= FULL_TurnOnSensor;
	AEK_TOF_METHODS->TurnOffSensor							= FULL_TurnOffSensor;
	AEK_TOF_METHODS->TurnOffAllSensors						= FULL_TurnOffAllSensors;
	AEK_TOF_METHODS->TurnOnAllSensors						= FULL_TurnOnAllSensors;
    AEK_TOF_METHODS->GetVersion 							= FULL_GetVersion;
    AEK_TOF_METHODS->GetProductRevision 					= FULL_GetProductRevision;
    AEK_TOF_METHODS->GetDeviceInfo 							= FULL_GetDeviceInfo;
    AEK_TOF_METHODS->GetRangeStatusString 					= FULL_GetRangeStatusString;
    AEK_TOF_METHODS->GetPalErrorString 						= FULL_GetPalErrorString;
    AEK_TOF_METHODS->GetPalStateString 						= FULL_GetPalStateString;
    AEK_TOF_METHODS->GetPalState 							= FULL_GetPalState;
    AEK_TOF_METHODS->SetDeviceAddress 						= FULL_SetDeviceAddress;
    AEK_TOF_METHODS->DataInit 								= FULL_DataInit;
    AEK_TOF_METHODS->StaticInit 							= FULL_StaticInit;
    AEK_TOF_METHODS->WaitDeviceBooted 						= FULL_WaitDeviceBooted;
    AEK_TOF_METHODS->SetPresetMode 							= FULL_SetPresetMode;
    AEK_TOF_METHODS->GetPresetMode							= FULL_GetPresetMode;
    AEK_TOF_METHODS->SetDistanceMode 						= FULL_SetDistanceMode;
    AEK_TOF_METHODS->GetDistanceMode 						= FULL_GetDistanceMode;
    AEK_TOF_METHODS->SetMeasurementTimingBudgetMicroSeconds = FULL_SetMeasurementTimingBudgetMicroSeconds;
    AEK_TOF_METHODS->GetMeasurementTimingBudgetMicroSeconds = FULL_GetMeasurementTimingBudgetMicroSeconds;
    AEK_TOF_METHODS->SetInterMeasurementPeriodMilliSeconds  = FULL_SetInterMeasurementPeriodMilliSeconds;
    AEK_TOF_METHODS->GetInterMeasurementPeriodMilliSeconds  = FULL_GetInterMeasurementPeriodMilliSeconds;
    AEK_TOF_METHODS->GetNumberOfLimitCheck 					= FULL_GetNumberOfLimitCheck;
    AEK_TOF_METHODS->GetLimitCheckInfo 						= FULL_GetLimitCheckInfo;
    AEK_TOF_METHODS->GetLimitCheckStatus 					= FULL_GetLimitCheckStatus;
    AEK_TOF_METHODS->SetLimitCheckEnable 					= FULL_SetLimitCheckEnable;
    AEK_TOF_METHODS->GetLimitCheckEnable 					= FULL_GetLimitCheckEnable;
    AEK_TOF_METHODS->SetLimitCheckValue 					= FULL_SetLimitCheckValue;
    AEK_TOF_METHODS-> GetLimitCheckValue 					= FULL_GetLimitCheckValue;
    AEK_TOF_METHODS->GetLimitCheckCurrent 					= FULL_GetLimitCheckCurrent;
    AEK_TOF_METHODS->SetUserROI 							= FULL_SetUserROI;
    AEK_TOF_METHODS->GetUserROI 							= FULL_GetUserROI;
    AEK_TOF_METHODS->GetNumberOfSequenceSteps				= FULL_GetNumberOfSequenceSteps;
    AEK_TOF_METHODS->GetSequenceStepsInfo 					= FULL_GetSequenceStepsInfo;
    AEK_TOF_METHODS->SetSequenceStepEnable 					= FULL_SetSequenceStepEnable;
    AEK_TOF_METHODS->GetSequenceStepEnable 					= FULL_GetSequenceStepEnable;
    AEK_TOF_METHODS->StartMeasurement 						= FULL_StartMeasurement;
    AEK_TOF_METHODS->StopMeasurement 						= FULL_StopMeasurement;
    AEK_TOF_METHODS->ClearInterruptAndStartMeasurement 		= FULL_ClearInterruptAndStartMeasurement;
    AEK_TOF_METHODS->GetMeasurementDataReady 				= FULL_GetMeasurementDataReady;
    AEK_TOF_METHODS->WaitMeasurementDataReady 				= FULL_WaitMeasurementDataReady;
    AEK_TOF_METHODS->GetRangingMeasurementData 				= FULL_GetRangingMeasurementData;
    AEK_TOF_METHODS->SetTuningParameter 					= FULL_SetTuningParameter;
    AEK_TOF_METHODS->GetTuningParameter 					= FULL_GetTuningParameter;
    AEK_TOF_METHODS->PerformRefSpadManagement 				= FULL_PerformRefSpadManagement;
    AEK_TOF_METHODS->SetXTalkCompensationEnable		 		= FULL_SetXTalkCompensationEnable;
    AEK_TOF_METHODS->GetXTalkCompensationEnable 			= FULL_GetXTalkCompensationEnable;
    AEK_TOF_METHODS->PerformSingleTargetXTalkCalibration 	= FULL_PerformSingleTargetXTalkCalibration ;
    AEK_TOF_METHODS->SetOffsetCalibrationMode 				= FULL_SetOffsetCalibrationMode;
    AEK_TOF_METHODS->PerformOffsetCalibration 				= FULL_PerformOffsetCalibration;
    AEK_TOF_METHODS->PerformOffsetSimpleCalibration 		= FULL_PerformOffsetSimpleCalibration;
    AEK_TOF_METHODS->SetCalibrationData 					= FULL_SetCalibrationData;
    AEK_TOF_METHODS->GetCalibrationData 					= FULL_GetCalibrationData;
    AEK_TOF_METHODS->GetOpticalCenter 						= FULL_GetOpticalCenter;
    AEK_TOF_METHODS->SetThresholdConfig 					= FULL_SetThresholdConfig;
    AEK_TOF_METHODS->GetThresholdConfig 					= FULL_GetThresholdConfig;
}

#endif

AEK_TOF_ERROR AEK_TOF_CONFIG(void)
{
	AEK_TOF_ERROR status = AEK_TOF_NO_ERROR;
	irqIsrEnable();
    i2c_lld_start(AEK_SNS_VL53L1X_I2C_ARRAY_DRIVER[0],AEK_SNS_VL53L1X_I2C_ARRAY_CONFIGURATION[0]);

	if(!methods_available)
	{
		#ifdef ULD_enable
			fillMethodsStructULD(&AEK_TOF_METHODS);
		#else
			fillMethodsStructFULL(&AEK_TOF_METHODS);
		#endif
	}
	uint8_t newI2CDevAddress = 0x52;
	int i;
	#ifdef ULD_enable
		for(i = 0; i < numberSensors; i++)
		{
			newI2CDevAddress += 2;
			status = ULD_CreateSensor(i,newI2CDevAddress,XshutMatrix[i][0],XshutMatrix[i][1]);
		}
		return status;
	#else
		for(i = 0; i < numberSensors; i++)
		{
			newI2CDevAddress += 2;
			status = FULL_CreateSensor(i,newI2CDevAddress,XshutMatrix[i][0],XshutMatrix[i][1]);
		}
		return status;
	#endif
}

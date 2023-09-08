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
 * @file    uld_api.c
 * @brief   SPC5xx uld_api.c code file.
 *
 * @addtogroup uld_api
 * @{
 */
#ifndef _ULD_API_C_
#define _ULD_API_C_
#include <uld_include/uld_api.h>
#ifdef ULD_enable
i2c_result_t status = I2C_NO_ERROR;
/**
 * @brief This function returns the SW driver version
 */
i2c_result_t ULD_GetSWVersion(VL53L1X_Version_t *pVersion)
{
    status = VL53L1X_GetSWVersion(pVersion);
    return status;
}

/**
 * @brief This function sets the sensor I2C address used in case multiple devices application, default address 0x52
 */

i2c_result_t ULD_SetI2CAddress(AEK_TOF_DEV index, uint8_t new_address)
{
    
    status = VL53L1X_SetI2CAddress(uld_VL53L1X[index].Dev,new_address);

    if(status == I2C_NO_ERROR)
    {
        uld_VL53L1X[index].Dev = new_address;
    }

    return status;
}

/**
 * @brief This function loads the 135 bytes default values to initialize the sensor.
 * @param enum AEK_TOF_DEV : Object Array index
 * @return 0:success, != 0:failed
 */
i2c_result_t ULD_SensorInit(AEK_TOF_DEV index)
{
   status = VL53L1X_SensorInit(uld_VL53L1X[index].Dev);
   return status;
}

/**
 * @brief This function clears the interrupt, to be called after a ranging data reading
 * to arm the interrupt for the next data ready event.
 */
i2c_result_t ULD_ClearInterrupt(AEK_TOF_DEV index)
{
    status = VL53L1X_ClearInterrupt(uld_VL53L1X[index].Dev);
    return status;
}

/**
 * @brief This function programs the interrupt polarity\n
 * 1 = active high (default), 0 = active low
 */
i2c_result_t ULD_SetInterruptPolarity(AEK_TOF_DEV index, uint8_t IntPol)
{
 status = VL53L1X_SetInterruptPolarity(uld_VL53L1X[index].Dev,IntPol);
 return status;
}

/**
 * @brief This function returns the current interrupt polarity\n
 * 1=active high (default), 0=active low
 */
i2c_result_t ULD_GetInterruptPolarity(AEK_TOF_DEV index, uint8_t *pIntPol)
{
    status = VL53L1X_GetInterruptPolarity(uld_VL53L1X[index].Dev,pIntPol);
    return status;
}

/**
 * @brief This function starts the ranging distance operation\n
 * The ranging operation is continuous. The clear interrupt has to be done after each get data to allow the interrupt to raise when the next data is ready\n
 * 1=active high (default), 0=active low, use SetInterruptPolarity() to change the interrupt polarity if required.
 */
i2c_result_t ULD_StartRanging(AEK_TOF_DEV index)
{
    status = VL53L1X_StartRanging(uld_VL53L1X[index].Dev);
    return status;
}

/**
 * @brief This function stops the ranging.
 */
i2c_result_t ULD_StopRanging(AEK_TOF_DEV index)
{
    status = VL53L1X_StopRanging(uld_VL53L1X[index].Dev);
    return status;
}

/**
 * @brief This function checks if the new ranging data is available by polling the dedicated register.
 * @param : isDataReady==0 . not ready; isDataReady==1 . ready
 */
i2c_result_t ULD_CheckForDataReady(AEK_TOF_DEV index, uint8_t *isDataReady)
{
   status = VL53L1X_CheckForDataReady(uld_VL53L1X[index].Dev,isDataReady);
   return status;
}

/**
 * @brief This function programs the timing budget in ms.
 * Predefined values = 15, 20, 33, 50, 100(default), 200, 500.
 */
i2c_result_t ULD_SetTimingBudgetInMs(AEK_TOF_DEV index, uint16_t TimingBudgetInMs)
{
    status = VL53L1X_SetTimingBudgetInMs(uld_VL53L1X[index].Dev,TimingBudgetInMs);
    return status;
}

/**
 * @brief This function returns the current timing budget in ms.
 */
i2c_result_t ULD_GetTimingBudgetInMs(AEK_TOF_DEV index, uint16_t *pTimingBudgetInMs)
{
   status = VL53L1X_GetTimingBudgetInMs(uld_VL53L1X[index].Dev,pTimingBudgetInMs);
   return status;
}

/**
 * @brief This function programs the distance mode (1=short, 2=long(default)).
 * Short mode max distance is limited to 1.3 m but better ambient immunity.\n
 * Long mode can range up to 4 m in the dark with 200 ms timing budget.
 */
i2c_result_t ULD_SetDistanceMode(AEK_TOF_DEV index, uint16_t DistanceMode)
{
    status = VL53L1X_SetDistanceMode(uld_VL53L1X[index].Dev,DistanceMode);
    return status;
}

/**
 * @brief This function returns the current distance mode (1=short, 2=long).
 */
i2c_result_t ULD_GetDistanceMode(AEK_TOF_DEV index, uint16_t *pDistanceMode)
{
   status = VL53L1X_GetDistanceMode(uld_VL53L1X[index].Dev,pDistanceMode);
   return status;
}

/**
 * @brief This function programs the Intermeasurement period in ms\n
 * Intermeasurement period must be >/= timing budget. This condition is not checked by the API,
 * the customer has the duty to check the condition. Default = 100 ms
 */
i2c_result_t ULD_SetInterMeasurementInMs(AEK_TOF_DEV index,uint32_t InterMeasurementInMs)
{
    status = VL53L1X_SetInterMeasurementInMs(uld_VL53L1X[index].Dev,InterMeasurementInMs);
    return status;
}

/**
 * @brief This function returns the Intermeasurement period in ms.
 */
i2c_result_t ULD_GetInterMeasurementInMs(AEK_TOF_DEV index, uint16_t * pIM)
{
    status = VL53L1X_GetInterMeasurementInMs(uld_VL53L1X[index].Dev,pIM);
    return status;
}

/**
 * @brief This function returns the boot state of the device (1:booted, 0:not booted)
 */
i2c_result_t ULD_BootState(AEK_TOF_DEV index, uint8_t *state)
{
	status = VL53L1X_BootState(uld_VL53L1X[index].Dev,state);
    return status;
}

/**
 * @brief This function returns the sensor id, sensor Id must be 0xEEAC
 */
i2c_result_t ULD_GetSensorId(AEK_TOF_DEV index, uint16_t *id)
{
   status = VL53L1X_GetSensorId(uld_VL53L1X[index].Dev,id);
   return status;
}

/**
 * @brief This function returns the distance measured by the sensor in mm
 */
i2c_result_t ULD_GetDistance(AEK_TOF_DEV index, uint16_t *distance)
{
   status = VL53L1X_GetDistance(uld_VL53L1X[index].Dev,distance);
   return status;
}

/**
 * @brief This function returns the returned signal per SPAD in kcps/SPAD.
 * With kcps stands for Kilo Count Per Second
 */
i2c_result_t ULD_GetSignalPerSpad(AEK_TOF_DEV index, uint16_t *signalPerSp)
{
   status = VL53L1X_GetSignalPerSpad(uld_VL53L1X[index].Dev,signalPerSp);
   return status;
}

/**
 * @brief This function returns the ambient per SPAD in kcps/SPAD
 */
i2c_result_t ULD_GetAmbientPerSpad(AEK_TOF_DEV index, uint16_t *amb)
{
   status = VL53L1X_GetAmbientPerSpad(uld_VL53L1X[index].Dev,amb);
   return status;
}

/**
 * @brief This function returns the returned signal in kcps.
 */
i2c_result_t ULD_GetSignalRate(AEK_TOF_DEV index, uint16_t *signalRate)
{
    status = VL53L1X_GetSignalRate(uld_VL53L1X[index].Dev,signalRate);
    return status;
}

/**
 * @brief This function returns the current number of enabled SPADs
 */
i2c_result_t ULD_GetSpadNb(AEK_TOF_DEV index, uint16_t *spNb)
{
    status = VL53L1X_GetSpadNb(uld_VL53L1X[index].Dev,spNb);
    return status;
}

/**
 * @brief This function returns the ambient rate in kcps
 */
i2c_result_t ULD_GetAmbientRate(AEK_TOF_DEV index, uint16_t *ambRate)
{
    status = VL53L1X_GetAmbientRate(uld_VL53L1X[index].Dev,ambRate);
    return status;
}

/**
 * @brief This function returns the ranging status error \n
 * (0:no error, 1:sigma failed, 2:signal failed, ..., 7:wrap-around)
 */
i2c_result_t ULD_GetRangeStatus(AEK_TOF_DEV index, uint8_t *rangeStatus)
{
    status = VL53L1X_GetRangeStatus(uld_VL53L1X[index].Dev,rangeStatus);
    return status;
}

/**
 * @brief This function returns measurements and the range status in a single read access
 */
i2c_result_t ULD_GetResult(AEK_TOF_DEV index, VL53L1X_Result_t *pResult)
{
    status = VL53L1X_GetResult(uld_VL53L1X[index].Dev,pResult);
    return status;
}

/**
 * @brief This function programs the offset correction in mm
 * @param OffsetValue:the offset correction value to program in mm
 */
i2c_result_t ULD_SetOffset(AEK_TOF_DEV index, int16_t OffsetValue)
{
    status = VL53L1X_SetOffset(uld_VL53L1X[index].Dev,OffsetValue);
    return status;
}

/**
 * @brief This function returns the programmed offset correction value in mm
 */
i2c_result_t ULD_GetOffset(AEK_TOF_DEV index, int16_t *Offset){
    status = VL53L1X_GetOffset(uld_VL53L1X[index].Dev, Offset);
    return status;
}

/**
 * @brief This function programs the xtalk correction value in cps (Count Per Second).\n
 * This is the number of photons reflected back from the cover glass in cps.
 */
i2c_result_t ULD_SetXtalk(AEK_TOF_DEV index, uint16_t XtalkValue){
    status = VL53L1X_SetXtalk(uld_VL53L1X[index].Dev, XtalkValue);
    return status;
}

/**
 * @brief This function returns the current programmed xtalk correction value in cps
 */
i2c_result_t ULD_GetXtalk(AEK_TOF_DEV index, uint16_t *Xtalk)
{
    status = VL53L1X_GetXtalk(uld_VL53L1X[index].Dev,Xtalk);
    return status;
}

/**
 * @brief This function programs the threshold detection mode\n
 * Example:\n
 * VL53L1X_SetDistanceThreshold(dev,100,300,0,1): Below 100 \n
 * VL53L1X_SetDistanceThreshold(dev,100,300,1,1): Above 300 \n
 * VL53L1X_SetDistanceThreshold(dev,100,300,2,1): Out of window \n
 * VL53L1X_SetDistanceThreshold(dev,100,300,3,1): In window \n
 * @param   dev : device address
 * @param  	ThreshLow(in mm) : the threshold under which one the device raises an interrupt if Window = 0
 * @param 	ThreshHigh(in mm) :  the threshold above which one the device raises an interrupt if Window = 1
 * @param   Window detection mode : 0=below, 1=above, 2=out, 3=in
 * @param   IntOnNoTarget = 1 (No longer used - just use 1)
 */
i2c_result_t ULD_SetDistanceThreshold(AEK_TOF_DEV index, uint16_t ThreshLow,uint16_t ThreshHigh,uint8_t Window,uint8_t IntOnNoTarget)
{
    status = VL53L1X_SetDistanceThreshold(uld_VL53L1X[index].Dev,ThreshLow,ThreshHigh,Window,IntOnNoTarget);
    return status;
}

/**
 * @brief This function returns the window detection mode (0=below; 1=above; 2=out; 3=in)
 */
i2c_result_t ULD_GetDistanceThresholdWindow(AEK_TOF_DEV index, uint16_t *window)
{
    status = VL53L1X_GetDistanceThresholdWindow(uld_VL53L1X[index].Dev,window);
    return status;
}

/**
 * @brief This function returns the low threshold in mm
 */
i2c_result_t ULD_GetDistanceThresholdLow(AEK_TOF_DEV index, uint16_t *low)
{
    status = VL53L1X_GetDistanceThresholdLow(uld_VL53L1X[index].Dev,low);
    return status;
}

/**
 * @brief This function returns the high threshold in mm
 */
i2c_result_t ULD_GetDistanceThresholdHigh(AEK_TOF_DEV index, uint16_t *high)
{
    status = VL53L1X_GetDistanceThresholdHigh(uld_VL53L1X[index].Dev,high);
    return status;
}

/**
 * @brief This function programs the ROI (Region of Interest)\n
 * The ROI position is centered, only the ROI size can be reprogrammed.\n
 * The smallest acceptable ROI size = 4\n
 * @param X:ROI Width; Y=ROI Height
 */
i2c_result_t ULD_SetROI(AEK_TOF_DEV index, uint16_t X, uint16_t Y)
{
    status = VL53L1X_SetROI(uld_VL53L1X[index].Dev,X,Y);
    return status;
}

/**
 *@brief This function returns width X and height Y
 */
i2c_result_t ULD_GetROI_XY(AEK_TOF_DEV index, uint16_t *ROI_X, uint16_t *ROI_Y)
{
    status = VL53L1X_GetROI_XY(uld_VL53L1X[index].Dev,ROI_X,ROI_Y);
    return status;
}

/**
 *@brief This function programs the new user ROI center, please to be aware that there is no check in this function.
 *if the ROI center vs ROI size is out of border the ranging function return error #13
 */
i2c_result_t ULD_SetROICenter(AEK_TOF_DEV index, uint8_t ROICenter)
{
    status = VL53L1X_SetROICenter(uld_VL53L1X[index].Dev,ROICenter);
    return status;
}

/**
 *@brief This function returns the current user ROI center
 */
i2c_result_t ULD_GetROICenter(AEK_TOF_DEV index, uint8_t *ROICenter)
{
    status = VL53L1X_GetROICenter(uld_VL53L1X[index].Dev,ROICenter);
    return status;
}

/**
 * @brief This function programs a new signal threshold in kcps (default=1024 kcps\n
 */
i2c_result_t ULD_SetSignalThreshold(AEK_TOF_DEV index, uint16_t signal)
{
    status = VL53L1X_SetSignalThreshold(uld_VL53L1X[index].Dev,signal);
    return status;
}

/**
 * @brief This function returns the current signal threshold in kcps
 */
i2c_result_t ULD_GetSignalThreshold(AEK_TOF_DEV index, uint16_t *signal)
{
    status = VL53L1X_GetSignalThreshold(uld_VL53L1X[index].Dev,signal);
    return status;
}

/**
 * @brief This function programs a new sigma threshold in mm (default=15 mm)
 */
i2c_result_t ULD_SetSigmaThreshold(AEK_TOF_DEV index, uint16_t sigma)
{
    status = VL53L1X_SetSigmaThreshold(uld_VL53L1X[index].Dev,sigma);
    return status;
}

/**
 * @brief This function returns the current sigma threshold in mm
 */
i2c_result_t ULD_GetSigmaThreshold(AEK_TOF_DEV index, uint16_t *sigma)
{
    status = VL53L1X_GetSigmaThreshold(uld_VL53L1X[index].Dev, sigma);
    return status;
}

/**
 * @brief This function performs the temperature calibration.
 * It is recommended to call this function any time
 * the temperature might have changed by more than 8 deg C without sensor ranging activity for an extended period.
 */
i2c_result_t ULD_StartTemperatureUpdate(AEK_TOF_DEV index)
{
    status = VL53L1X_StartTemperatureUpdate(uld_VL53L1X[index].Dev);
    return status;
}

i2c_result_t ULD_CalibrateOffset(AEK_TOF_DEV index, uint16_t TargetDistInMm, int16_t *offset)
{
	status = VL53L1X_CalibrateOffset(uld_VL53L1X[index].Dev,TargetDistInMm,offset);
	return status;
}

i2c_result_t ULD_CalibrateXtalk(AEK_TOF_DEV index, uint16_t TargetDistInMm, uint16_t *xtalk)
{
	status = VL53L1X_CalibrateXtalk(uld_VL53L1X[index].Dev,TargetDistInMm,xtalk);
	return status;
}

/*
* 	@brief Create element in sensor array
*
* 	@param[in]  AEK_TOF_DEV index : AEK_TOF_DEV0 for refer to first sensor
*	
*	@param[in] uint8_t Dev : new I2C address slave value
*	
*	@param[in] 	uint8_t XshutPort : Port xshutdown pin value
*
*	@param[in] uint8_t XshutPin : Pin xshutdown pin value
*
*   @return AEK_TOF_ERROR bootStatus : bootStatus = AEK_TOF_NO_ERROR : Success || bootStatus = AEK_TOF_ERROR_CONFIGURATION : Error
*/
AEK_TOF_ERROR ULD_CreateSensor(AEK_TOF_DEV index,uint8_t Dev,uint8_t XshutPort,uint8_t XshutPin)
{
	AEK_TOF_ERROR bootStatus = AEK_TOF_NO_ERROR;
	uld_VL53L1X[index].Dev = 0x52;
	uld_VL53L1X[index].Xshut.pin = XshutPin;
	uld_VL53L1X[index].Xshut.port = XshutPort;
	uint8_t sensorStatus = 0 ;
	ULD_TurnOffSensor(index);
    ULD_TurnOnSensor(index);
    uint32_t startTime = 0;
	startTime = osalThreadGetMilliseconds();
    while(sensorStatus == 0)
	{
        ULD_BootState(index,&sensorStatus);
		if((osalThreadGetMilliseconds() - startTime) > 20)
		{
			bootStatus = AEK_TOF_ERROR_CONFIGURATION;
			sensorStatus = 1;
		}
	}
    ULD_SetI2CAddress(index,Dev);

    uld_VL53L1X[index].IRQState = 0;
    uld_VL53L1X[index].errorState = bootStatus;

    return bootStatus;
}
/*
* 	@brief Turn on sensor (Xshut state = 1)
*
* 	@param[in]  AEK_TOF_DEV index : AEK_TOF_DEV0 for refer to first sensor
*
*/
void ULD_TurnOnSensor(AEK_TOF_DEV index)
{
	pal_lld_setpad(uld_VL53L1X[index].Xshut.port,uld_VL53L1X[index].Xshut.pin);
	uld_VL53L1X[index].stateSensor = 1;
}
/*
* 	@brief Turn off sensor (Xshut state = 0)
*
* 	@param[in]  AEK_TOF_DEV index : AEK_TOF_DEV0 for refer to first sensor
*
*/
void ULD_TurnOffSensor(AEK_TOF_DEV index)
{
	pal_lld_clearpad(uld_VL53L1X[index].Xshut.port,uld_VL53L1X[index].Xshut.pin);
	uld_VL53L1X[index].stateSensor = 0;
}
/*
* 	@brief Turn on all sensor (Xshut state = 1)
*/
void ULD_TurnOnAllSensors(void)
{
    int i ;
    for(i = 0; i < numberSensors; i++)
    {
    	ULD_TurnOnSensor(i);
    }
}
/*
* 	@brief Turn off all sensor (Xshut state = 0)
*/
void ULD_TurnOffAllSensors(void)
{
    int i;
    for(i = 0; i < numberSensors; i++)
	{
    	ULD_TurnOffSensor(i);
	}
}
/*
* 	@brief Set IRQ variable
*
* 	@param[in]  AEK_TOF_DEV index : AEK_TOF_DEV0 for refer to first sensor
*
*/
void ULD_SetIRQSensorState(AEK_TOF_DEV index)
{
	uld_VL53L1X[index].IRQState = 0;
}
/*
* 	@brief Get IRQ variable
*
* 	@param[in]  AEK_TOF_DEV index : AEK_TOF_DEV0 for refer to first sensor
*
*	@param[in]  volatile uint8_t *state : uint8_t ptr
*/
void ULD_GetIRQSensorState(AEK_TOF_DEV index, volatile uint8_t *state)
{
	*state = uld_VL53L1X[index].IRQState;
	(void)state;
}

void ULD_GetErrorSensorState(AEK_TOF_DEV index,AEK_TOF_ERROR *state)
{
	*state = uld_VL53L1X[index].errorState;
	(void)state;
}
#endif
#endif/*_ULD_API_C_*/

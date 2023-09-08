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
 * @file     vl53l1x_calibration.c

 * @brief   SPC5xx  vl53l1x_calibration driver code.
 *
 * @addtogroup vl53l1x_calibration.c
 * @{
 */


#include "uld_include/VL53L1X_api.h"
#include "uld_include/VL53L1X_calibration.h"

#define ALGO__PART_TO_PART_RANGE_OFFSET_MM	0x001E
#define MM_CONFIG__INNER_OFFSET_MM			0x0020
#define MM_CONFIG__OUTER_OFFSET_MM 			0x0022
/**
* 	@brief This function performs the offset calibration.
* 	The function returns the offset value found and programs the offset compensation into the device.
*
* 	@param[in] TargetDistInMm target distance in mm, ST recommended 100 mm Target reflectance = grey17%
*
*	@param[in] uint8_t * offset : offset pointer contains the offset found in mm
*
* 	@return status = 0 : success, status !=0 : failed
*/
i2c_result_t VL53L1X_CalibrateOffset(uint8_t dev, uint16_t TargetDistInMm, int16_t *offset)
{
	uint8_t i, tmp;
	int16_t AverageDistance = 0;
	uint16_t distance;
	i2c_result_t status = 0;

	status = VL53L1X_WrWord(dev, ALGO__PART_TO_PART_RANGE_OFFSET_MM, 0x0);
	status = VL53L1X_WrWord(dev, MM_CONFIG__INNER_OFFSET_MM, 0x0);
	status = VL53L1X_WrWord(dev, MM_CONFIG__OUTER_OFFSET_MM, 0x0);
	status = VL53L1X_StartRanging(dev);	/* Enable VL53L1X sensor */
	for (i = 0; i < 50; i++)
	{
		tmp = 0;
		while (tmp == 0)
		{
			status = VL53L1X_CheckForDataReady(dev, &tmp);
		}
		status = VL53L1X_GetDistance(dev, &distance);
		status = VL53L1X_ClearInterrupt(dev);
		AverageDistance = AverageDistance + distance;
	}
	status = VL53L1X_StopRanging(dev);
	AverageDistance = AverageDistance / 50;
	*offset = TargetDistInMm - AverageDistance;
	status = VL53L1X_WrWord(dev, ALGO__PART_TO_PART_RANGE_OFFSET_MM, *offset*4);
	return status;
}
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
* 	@return status = 0 : success, status !=0 : failed
 */
i2c_result_t VL53L1X_CalibrateXtalk(uint8_t dev, uint16_t TargetDistInMm, uint16_t *xtalk)
{
	uint8_t i, tmp;
	float AverageSignalRate = 0;
	float AverageDistance = 0;
	float AverageSpadNb = 0;
	uint16_t distance = 0, spadNum;
	uint16_t sr;
	i2c_result_t status = 0;
	uint32_t calXtalk;

	status = VL53L1X_WrWord(dev, 0x0016,0);
	status = VL53L1X_StartRanging(dev);
	for (i = 0; i < 50; i++) {
		tmp = 0;
		while (tmp == 0){
			status = VL53L1X_CheckForDataReady(dev, &tmp);
		}
		status= VL53L1X_GetSignalRate(dev, &sr);
		status= VL53L1X_GetDistance(dev, &distance);
		status = VL53L1X_ClearInterrupt(dev);
		AverageDistance = AverageDistance + distance;
		status = VL53L1X_GetSpadNb(dev, &spadNum);
		AverageSpadNb = AverageSpadNb + spadNum;
		AverageSignalRate =
		    AverageSignalRate + sr;
	}
	status = VL53L1X_StopRanging(dev);
	AverageDistance = AverageDistance / 50;
	AverageSpadNb = AverageSpadNb / 50;
	AverageSignalRate = AverageSignalRate / 50;
	/* Calculate Xtalk value */
	calXtalk = (uint16_t)(512*(AverageSignalRate*(1-(AverageDistance/TargetDistInMm)))/AverageSpadNb);
	*xtalk = (uint16_t)(calXtalk*1000)>>9;
	status = VL53L1X_WrWord(dev, 0x0016, calXtalk);
	return status;
}


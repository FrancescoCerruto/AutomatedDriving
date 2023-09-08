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
 * @file     vl53l1x_calibration.h

 * @brief   SPC5xx  vl53l1x_calibration driver code.
 *
 * @addtogroup vl53l1x_calibration.h
 * @{
 */


#ifndef _CALIBRATION_H_
#define _CALIBRATION_H_

i2c_result_t VL53L1X_CalibrateOffset(uint8_t dev, uint16_t TargetDistInMm, int16_t *offset);

i2c_result_t VL53L1X_CalibrateXtalk(uint8_t dev, uint16_t TargetDistInMm, uint16_t *xtalk);
#endif/*_CALIBRATION_H_*/

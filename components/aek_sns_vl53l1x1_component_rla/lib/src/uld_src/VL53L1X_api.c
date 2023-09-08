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
 * @file     VL53L1X_api.c

 * @brief   SPC5xx  VL53L1X_api driver code.
 *
 * @addtogroup VL53L1X_api.c
 * @{
 */


#include "uld_include/VL53L1X_api.h"
#include <string.h>

extern i2c_result_t status;
#if 0
uint8_t VL51L1X_NVM_CONFIGURATION[] = {
0x00, /* 0x00 : not user-modifiable */
0x29, /* 0x01 : 7 bits I2C address (default=0x29), use SetI2CAddress(). Warning: after changing the register value to a new I2C address, the device will only answer to the new address */
0x00, /* 0x02 : not user-modifiable */
0x00, /* 0x03 : not user-modifiable */
0x00, /* 0x04 : not user-modifiable */
0x00, /* 0x05 : not user-modifiable */
0x00, /* 0x06 : not user-modifiable */
0x00, /* 0x07 : not user-modifiable */
0x00, /* 0x08 : not user-modifiable */
0x50, /* 0x09 : not user-modifiable */
0x00, /* 0x0A : not user-modifiable */
0x00, /* 0x0B : not user-modifiable */
0x00, /* 0x0C : not user-modifiable */
0x00, /* 0x0D : not user-modifiable */
0x0a, /* 0x0E : not user-modifiable */
0x00, /* 0x0F : not user-modifiable */
0x00, /* 0x10 : not user-modifiable */
0x00, /* 0x11 : not user-modifiable */
0x00, /* 0x12 : not user-modifiable */
0x00, /* 0x13 : not user-modifiable */
0x00, /* 0x14 : not user-modifiable */
0x00, /* 0x15 : not user-modifiable */
0x00, /* 0x16 : Xtalk calibration value MSB (7.9 format in kcps), use SetXtalk() */
0x00, /* 0x17 : Xtalk calibration value LSB */
0x00, /* 0x18 : not user-modifiable */
0x00, /* 0x19 : not user-modifiable */
0x00, /* 0x1a : not user-modifiable */
0x00, /* 0x1b : not user-modifiable */
0x00, /* 0x1e : Part to Part offset x4 MSB (in mm), use SetOffset() */
0x50, /* 0x1f : Part to Part offset x4 LSB */
0x00, /* 0x20 : not user-modifiable */
0x00, /* 0x21 : not user-modifiable */
0x00, /* 0x22 : not user-modifiable */
0x00, /* 0x23 : not user-modifiable */
}
#endif

const uint8_t VL51L1X_DEFAULT_CONFIGURATION[] = {
0x00, /* 0x2d : set bit 2 and 5 to 1 for fast plus mode (1MHz I2C), else don't touch */
0x01, /* 0x2e : bit 0 if I2C pulled up at 1.8V, else set bit 0 to 1 (pull up at AVDD) */ //default 0x00
0x01, /* 0x2f : bit 0 if GPIO pulled up at 1.8V, else set bit 0 to 1 (pull up at AVDD) */ //default 0x00
0x01, /* 0x30 : set bit 4 to 0 for active high interrupt and 1 for active low (bits 3:0 must be 0x1), use SetInterruptPolarity() */
0x02, /* 0x31 : bit 1 = interrupt depending on the polarity, use CheckForDataReady() */
0x00, /* 0x32 : not user-modifiable */
0x02, /* 0x33 : not user-modifiable */
0x08, /* 0x34 : not user-modifiable */
0x00, /* 0x35 : not user-modifiable */
0x08, /* 0x36 : not user-modifiable */
0x10, /* 0x37 : not user-modifiable */
0x01, /* 0x38 : not user-modifiable */
0x01, /* 0x39 : not user-modifiable */
0x00, /* 0x3a : not user-modifiable */
0x00, /* 0x3b : not user-modifiable */
0x00, /* 0x3c : not user-modifiable */
0x00, /* 0x3d : not user-modifiable */
0xff, /* 0x3e : not user-modifiable */
0x00, /* 0x3f : not user-modifiable */
0x0F, /* 0x40 : not user-modifiable */
0x00, /* 0x41 : not user-modifiable */
0x00, /* 0x42 : not user-modifiable */
0x00, /* 0x43 : not user-modifiable */
0x00, /* 0x44 : not user-modifiable */
0x00, /* 0x45 : not user-modifiable */
0x20, /* 0x46 : interrupt configuration 0->level low detection, 1-> level high, 2-> Out of window, 3->In window, 0x20-> New sample ready , TBC */
0x0b, /* 0x47 : not user-modifiable */
0x00, /* 0x48 : not user-modifiable */
0x00, /* 0x49 : not user-modifiable */
0x02, /* 0x4a : not user-modifiable */
0x0a, /* 0x4b : not user-modifiable */
0x21, /* 0x4c : not user-modifiable */
0x00, /* 0x4d : not user-modifiable */
0x00, /* 0x4e : not user-modifiable */
0x05, /* 0x4f : not user-modifiable */
0x00, /* 0x50 : not user-modifiable */
0x00, /* 0x51 : not user-modifiable */
0x00, /* 0x52 : not user-modifiable */
0x00, /* 0x53 : not user-modifiable */
0xc8, /* 0x54 : not user-modifiable */
0x00, /* 0x55 : not user-modifiable */
0x00, /* 0x56 : not user-modifiable */
0x38, /* 0x57 : not user-modifiable */
0xff, /* 0x58 : not user-modifiable */
0x01, /* 0x59 : not user-modifiable */
0x00, /* 0x5a : not user-modifiable */
0x08, /* 0x5b : not user-modifiable */
0x00, /* 0x5c : not user-modifiable */
0x00, /* 0x5d : not user-modifiable */
0x01, /* 0x5e : not user-modifiable */
0xcc, /* 0x5f : not user-modifiable */
0x0f, /* 0x60 : not user-modifiable */
0x01, /* 0x61 : not user-modifiable */
0xf1, /* 0x62 : not user-modifiable */
0x0d, /* 0x63 : not user-modifiable */
0x01, /* 0x64 : Sigma threshold MSB (mm in 14.2 format for MSB+LSB), use SetSigmaThreshold(), default value 90 mm  */
0x68, /* 0x65 : Sigma threshold LSB */
0x00, /* 0x66 : Min count Rate MSB (MCPS in 9.7 format for MSB+LSB), use SetSignalThreshold() */
0x80, /* 0x67 : Min count Rate LSB */
0x08, /* 0x68 : not user-modifiable */
0xb8, /* 0x69 : not user-modifiable */
0x00, /* 0x6a : not user-modifiable */
0x00, /* 0x6b : not user-modifiable */
0x00, /* 0x6c : Intermeasurement period MSB, 32 bits register, use SetIntermeasurementInMs() */
0x00, /* 0x6d : Intermeasurement period */
0x0f, /* 0x6e : Intermeasurement period */
0x89, /* 0x6f : Intermeasurement period LSB */
0x00, /* 0x70 : not user-modifiable */
0x00, /* 0x71 : not user-modifiable */
0x00, /* 0x72 : distance threshold high MSB (in mm, MSB+LSB), use SetDistanceThreshold() */
0x00, /* 0x73 : distance threshold high LSB */
0x00, /* 0x74 : distance threshold low MSB ( in mm, MSB+LSB), use SetDistanceThreshold() */
0x00, /* 0x75 : distance threshold low LSB */
0x00, /* 0x76 : not user-modifiable */
0x01, /* 0x77 : not user-modifiable */
0x0f, /* 0x78 : not user-modifiable */
0x0d, /* 0x79 : not user-modifiable */
0x0e, /* 0x7a : not user-modifiable */
0x0e, /* 0x7b : not user-modifiable */
0x00, /* 0x7c : not user-modifiable */
0x00, /* 0x7d : not user-modifiable */
0x02, /* 0x7e : not user-modifiable */
0xc7, /* 0x7f : ROI center, use SetROI() */
0xff, /* 0x80 : XY ROI (X=Width, Y=Height), use SetROI() */
0x9B, /* 0x81 : not user-modifiable */
0x00, /* 0x82 : not user-modifiable */
0x00, /* 0x83 : not user-modifiable */
0x00, /* 0x84 : not user-modifiable */
0x01, /* 0x85 : not user-modifiable */
0x00, /* 0x86 : clear interrupt, use ClearInterrupt() */
0x00  /* 0x87 : start ranging, use StartRanging() or StopRanging(), If you want an automatic start after VL53L1X_init() call, put 0x40 in location 0x87 */
};

static const uint8_t status_rtn[24] = { 255, 255, 255, 5, 2, 4, 1, 7, 3, 0,
	255, 255, 9, 13, 255, 255, 255, 255, 10, 6,
	255, 255, 11, 12
};


i2c_result_t VL53L1X_GetSWVersion(VL53L1X_Version_t *pVersion)
{
	pVersion->major = VL53L1X_IMPLEMENTATION_VER_MAJOR;
	pVersion->minor = VL53L1X_IMPLEMENTATION_VER_MINOR;
	pVersion->build = VL53L1X_IMPLEMENTATION_VER_SUB;
	pVersion->revision = VL53L1X_IMPLEMENTATION_VER_REVISION;

	return status;
}

/**
 *  @brief      Edit the slave address
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint8_t new_address - 8bit new address value
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_SetI2CAddress(uint16_t dev, uint8_t new_address)
{

	status = VL53L1X_WrByte(dev, VL53L1_I2C_SLAVE__DEVICE_ADDRESS, new_address >> 1);
	return status;
}

/**
 *  @brief      VL53L1X initialization
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_SensorInit(uint8_t dev)
{
	uint8_t addr = 0x00, tmp;

	for (addr = 0x2D; addr <= 0x87; addr++)
	{
		status = VL53L1X_WrByte(dev, addr, VL51L1X_DEFAULT_CONFIGURATION[addr - 0x2D]);
	}
	status = VL53L1X_StartRanging(dev);
	tmp  = 0;
	while(tmp==0)
	{
		status = VL53L1X_CheckForDataReady(dev, &tmp);
	}
	status = VL53L1X_ClearInterrupt(dev);
	status = VL53L1X_StopRanging(dev);
	status = VL53L1X_WrByte(dev, VL53L1_VHV_CONFIG__TIMEOUT_MACROP_LOOP_BOUND, 0x09); /* two bounds VHV */
	status = VL53L1X_WrByte(dev, 0x0B, 0); /* start VHV from the previous temperature */

	return status;
}

/**
 *  @brief      Clear interrupt
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_ClearInterrupt(uint8_t dev)
{
	status = VL53L1X_WrByte(dev, SYSTEM__INTERRUPT_CLEAR, 0x01);

	return status;
}

/**
 *  @brief      Set interrupt polarity value
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint8_t newPolarity - 8bit polarity value
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_SetInterruptPolarity(uint8_t dev, uint8_t newPolarity)
{
	uint8_t temp;
	status = VL53L1X_RdByte(dev, GPIO_HV_MUX__CTRL, &temp);
	temp = temp & 0xEF;
	status = VL53L1X_WrByte(dev, GPIO_HV_MUX__CTRL, temp | (!(newPolarity & 1)) << 4);

	return status;
}

/**
 *  @brief      Get interrupt polarity value
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint8_t pInterruptPolarity - 8bit pointer to interrupt polarity value
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_GetInterruptPolarity(uint8_t dev, uint8_t *pInterruptPolarity)
{
	uint8_t temp;
	status = VL53L1X_RdByte(dev, GPIO_HV_MUX__CTRL, &temp);
	temp = temp & 0x10;
	*pInterruptPolarity = !(temp>>4);

	return status;
}

/**
 *  @brief      Start ranging sensor
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_StartRanging(uint8_t dev)
{
	status = VL53L1X_WrByte(dev, SYSTEM__MODE_START, 0x40);	/* Enable VL53L1X */

	return status;
}

/**
 *  @brief      Stop ranging fuction
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_StopRanging(uint8_t dev)
{
	status = VL53L1X_WrByte(dev, SYSTEM__MODE_START, 0x00);	/* Disable VL53L1X */
	return status;
}

/**
 *  @brief      Check if the data is ready
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint8_t isDataReady - 8bit the value is 0 or 1 (0 = not ready , 1 = ready)
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_CheckForDataReady(uint8_t dev, uint8_t *isDataReady)
{
	uint8_t temp;
	uint8_t intPol;

	status = VL53L1X_GetInterruptPolarity(dev, &intPol);
	status = VL53L1X_RdByte(dev, GPIO__TIO_HV_STATUS, &temp);
	/* Read in the register to check if a new value is available */
	if (status == 0){
		if ((temp & 1) == intPol)
			*isDataReady = 1;
		else
			*isDataReady = 0;
	}
	return status;
}

/**
 *  @brief      Set timing budget(ms)
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint16_t timingBudgetInMs - 16bit timing budget value(ms)
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_SetTimingBudgetInMs(uint8_t dev, uint16_t timingBudgetInMs)
{
	uint16_t DM;

	status = VL53L1X_GetDistanceMode(dev, &DM);
	if (DM == 0)
		return 1;
	else if (DM == 1) {	/* Short DistanceMode */
		switch (timingBudgetInMs) {
		case 15: /* only available in short distance mode */
			VL53L1X_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_A_HI,
					0x01D);
			VL53L1X_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_B_HI,
					0x0027);
			break;
		case 20:
			VL53L1X_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_A_HI,
					0x0051);
			VL53L1X_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_B_HI,
					0x006E);
			break;
		case 33:
			VL53L1X_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_A_HI,
					0x00D6);
			VL53L1X_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_B_HI,
					0x006E);
			break;
		case 50:
			VL53L1X_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_A_HI,
					0x1AE);
			VL53L1X_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_B_HI,
					0x01E8);
			break;
		case 100:
			VL53L1X_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_A_HI,
					0x02E1);
			VL53L1X_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_B_HI,
					0x0388);
			break;
		case 200:
			VL53L1X_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_A_HI,
					0x03E1);
			VL53L1X_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_B_HI,
					0x0496);
			break;
		case 500:
			VL53L1X_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_A_HI,
					0x0591);
			VL53L1X_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_B_HI,
					0x05C1);
			break;
		default:
			status = 1;
			break;
		}
	} else {
		switch (timingBudgetInMs) {
		case 20:
			VL53L1X_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_A_HI,
					0x001E);
			VL53L1X_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_B_HI,
					0x0022);
			break;
		case 33:
			VL53L1X_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_A_HI,
					0x0060);
			VL53L1X_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_B_HI,
					0x006E);
			break;
		case 50:
			VL53L1X_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_A_HI,
					0x00AD);
			VL53L1X_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_B_HI,
					0x00C6);
			break;
		case 100:
			VL53L1X_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_A_HI,
					0x01CC);
			VL53L1X_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_B_HI,
					0x01EA);
			break;
		case 200:
			VL53L1X_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_A_HI,
					0x02D9);
			VL53L1X_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_B_HI,
					0x02F8);
			break;
		case 500:
			VL53L1X_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_A_HI,
					0x048F);
			VL53L1X_WrWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_B_HI,
					0x04A4);
			break;
		default:
			status = 1;
			break;
		}
	}
	return status;
}

/**
 *  @brief      Get timing budget(ms)
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint16_t *pTimingBudget - 16bit pointer of timing budget value(ms)
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_GetTimingBudgetInMs(uint8_t dev, uint16_t *pTimingBudget)
{
	uint16_t temp;

	status = VL53L1X_RdWord(dev, RANGE_CONFIG__TIMEOUT_MACROP_A_HI, &temp);
	switch (temp) {
		case 0x001D :
			*pTimingBudget = 15;
			break;
		case 0x0051 :
		case 0x001E :
			*pTimingBudget = 20;
			break;
		case 0x00D6 :
		case 0x0060 :
			*pTimingBudget = 33;
			break;
		case 0x1AE :
		case 0x00AD :
			*pTimingBudget = 50;
			break;
		case 0x02E1 :
		case 0x01CC :
			*pTimingBudget = 100;
			break;
		case 0x03E1 :
		case 0x02D9 :
			*pTimingBudget = 200;
			break;
		case 0x0591 :
		case 0x048F :
			*pTimingBudget = 500;
			break;
		default:
			status = 1;
			*pTimingBudget = 0;
	}
	return status;
}

/**
 *  @brief      Set Distance Mode
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint16_t distanceMode - 16bit distance mode value
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_SetDistanceMode(uint8_t dev, uint16_t distanceMode)
{
	uint16_t timingBudget;

	status = VL53L1X_GetTimingBudgetInMs(dev, &timingBudget);
	if (status != 0)
		return 1;
	switch (distanceMode) {
	case 1:
		status = VL53L1X_WrByte(dev, PHASECAL_CONFIG__TIMEOUT_MACROP, 0x14);
		status = VL53L1X_WrByte(dev, RANGE_CONFIG__VCSEL_PERIOD_A, 0x07);
		status = VL53L1X_WrByte(dev, RANGE_CONFIG__VCSEL_PERIOD_B, 0x05);
		status = VL53L1X_WrByte(dev, RANGE_CONFIG__VALID_PHASE_HIGH, 0x38);
		status = VL53L1X_WrWord(dev, SD_CONFIG__WOI_SD0, 0x0705);
		status = VL53L1X_WrWord(dev, SD_CONFIG__INITIAL_PHASE_SD0, 0x0606);
		break;
	case 2:
		status = VL53L1X_WrByte(dev, PHASECAL_CONFIG__TIMEOUT_MACROP, 0x0A);
		status = VL53L1X_WrByte(dev, RANGE_CONFIG__VCSEL_PERIOD_A, 0x0F);
		status = VL53L1X_WrByte(dev, RANGE_CONFIG__VCSEL_PERIOD_B, 0x0D);
		status = VL53L1X_WrByte(dev, RANGE_CONFIG__VALID_PHASE_HIGH, 0xB8);
		status = VL53L1X_WrWord(dev, SD_CONFIG__WOI_SD0, 0x0F0D);
		status = VL53L1X_WrWord(dev, SD_CONFIG__INITIAL_PHASE_SD0, 0x0E0E);
		break;
	default:
		status = 1;
		break;
	}

	if (status == 0)
		status = VL53L1X_SetTimingBudgetInMs(dev, timingBudget);
	return status;
}

/**
 *  @brief      Get distance mode
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint16_t *distanceMode - 16bit pointer to distance mode value
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_GetDistanceMode(uint8_t dev, uint16_t *distanceMode)
{
	uint8_t tempDistanceMode;

	status = VL53L1X_RdByte(dev,PHASECAL_CONFIG__TIMEOUT_MACROP, &tempDistanceMode);
	if (tempDistanceMode == 0x14)
		*distanceMode=1;
	if(tempDistanceMode == 0x0A)
		*distanceMode=2;

	return status;
}

/**
 *  @brief      Set inter measurement(ms)
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint32_t *InterMeasMs - 32bit inter measurement value(ms)
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_SetInterMeasurementInMs(uint8_t dev, uint32_t interMeasMs)
{
	uint16_t clockPLL;

	status = VL53L1X_RdWord(dev, VL53L1_RESULT__OSC_CALIBRATE_VAL, &clockPLL);
	clockPLL = clockPLL&0x3FF;
	VL53L1X_WrDWord(dev, VL53L1_SYSTEM__INTERMEASUREMENT_PERIOD,(uint32_t)(clockPLL * interMeasMs * 1.075));

	return status;

}

/**
 *  @brief      Get inter measurement(ms)
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint32_t *InterMeasMs - 32bit pointer to inter measurement value(ms)
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_GetInterMeasurementInMs(uint8_t dev, uint16_t *pInterMeas)
{
	uint16_t clockPLL;
	uint32_t tmp;

	status = VL53L1X_RdDWord(dev,VL53L1_SYSTEM__INTERMEASUREMENT_PERIOD, &tmp);
	*pInterMeas = (uint16_t)tmp;
	status = VL53L1X_RdWord(dev, VL53L1_RESULT__OSC_CALIBRATE_VAL, &clockPLL);
	clockPLL = clockPLL&0x3FF;
	*pInterMeas= (uint16_t)(*pInterMeas/(clockPLL*1.065));
	return status;
}

/**
 *  @brief      Check boot state
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint8_t *state - 8bit pointer to boot state value
 *
 *  @return     i2c_result_t
//
 *  @api
 */
i2c_result_t VL53L1X_BootState(uint8_t dev, uint8_t *state)
{
	uint8_t tmp = 0;

	status = VL53L1X_RdByte(dev,VL53L1_FIRMWARE__SYSTEM_STATUS, &tmp);

	*state = tmp;

	return status;
}

/**
 *  @brief      Get sensor id
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint16_t *sensorId - 16bit pointer to sensor id value
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_GetSensorId(uint8_t dev, uint16_t *sensorId)
{
	uint16_t tmp = 0;

	status = VL53L1X_RdWord(dev, VL53L1_IDENTIFICATION__MODEL_ID, &tmp);
	*sensorId = tmp;
	return status;
}

/**
 *  @brief      Get distance
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint16_t *distance - 16bit pointer to distance value
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_GetDistance(uint8_t dev, uint16_t *distance)
{
	uint16_t tmp;

	status = (VL53L1X_RdWord(dev,VL53L1_RESULT__FINAL_CROSSTALK_CORRECTED_RANGE_MM_SD0, &tmp));
	*distance = tmp;

	return status;
}

/**
 *  @brief      Get signal per spad
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint16_t *signalRate - 16bit pointer to signal rate value
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_GetSignalPerSpad(uint8_t dev, uint16_t *signalRate)
{
	uint16_t spadNB = 1, signal;

	status = VL53L1X_RdWord(dev,VL53L1_RESULT__PEAK_SIGNAL_COUNT_RATE_CROSSTALK_CORRECTED_MCPS_SD0, &signal);
	status = VL53L1X_RdWord(dev,VL53L1_RESULT__DSS_ACTUAL_EFFECTIVE_SPADS_SD0, &spadNB);
	*signalRate = (uint16_t) (2000.0*signal/spadNB);

	return status;
}

/**
 *  @brief      Get ambient per spad
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint16_t *ambPerSpad - 16bit pointer to ambient per spad value
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_GetAmbientPerSpad(uint8_t dev, uint16_t *ambPerSpad)
{
	uint16_t ambientRate, spadNB = 1;

	status = VL53L1X_RdWord(dev, RESULT__AMBIENT_COUNT_RATE_MCPS_SD, &ambientRate);
	status = VL53L1X_RdWord(dev, VL53L1_RESULT__DSS_ACTUAL_EFFECTIVE_SPADS_SD0, &spadNB);
	*ambPerSpad=(uint16_t) (2000.0 * ambientRate / spadNB);

	return status;
}

/**
 *  @brief      Get signal rate
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint16_t *signal - 16bit pointer to signal rate value
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_GetSignalRate(uint8_t dev, uint16_t *signal)
{
	uint16_t tmp;

	status = VL53L1X_RdWord(dev,VL53L1_RESULT__PEAK_SIGNAL_COUNT_RATE_CROSSTALK_CORRECTED_MCPS_SD0, &tmp);
	*signal = tmp*8;

	return status;
}

/**
 *  @brief      Get number of enabled SPADs
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint16_t *spNB - 16bit pointer to number of enabled SPADs value
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_GetSpadNb(uint8_t dev, uint16_t *spNB)
{
	uint16_t tmp;

	status = VL53L1X_RdWord(dev,VL53L1_RESULT__DSS_ACTUAL_EFFECTIVE_SPADS_SD0, &tmp);
	*spNB = tmp >> 8;

	return status;
}

/**
 *  @brief      Get ambient rate
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint16_t *ambRate - 16bit pointer to ambient rate value
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_GetAmbientRate(uint8_t dev, uint16_t *ambRate)
{
	uint16_t tmp;

	status = VL53L1X_RdWord(dev, RESULT__AMBIENT_COUNT_RATE_MCPS_SD, &tmp);
	*ambRate = tmp*8;

	return status;
}

/**
 *  @brief      Get range status
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint16_t *rangeStatus - 16bit pointer to range status value
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_GetRangeStatus(uint8_t dev, uint8_t *rangeStatus)
{
	uint8_t rgSt;

	*rangeStatus = 255;
	status = VL53L1X_RdByte(dev, VL53L1_RESULT__RANGE_STATUS, &rgSt);
	rgSt = rgSt & 0x1F;
	if (rgSt < 24)
		*rangeStatus = status_rtn[rgSt];
	return status;
}

/**
 *  @brief      Get result (measurements and the range status)
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  VL53L1X_Result_t *pResult - pointer to result (measurements and the range status) value
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_GetResult(uint8_t dev, VL53L1X_Result_t *pResult)
{
	uint8_t temp[17];
	uint8_t rgSt = 255;
	status = i2c_lld_read16(&I2CD1,dev, VL53L1_RESULT__RANGE_STATUS, temp, 17);

	rgSt = temp[0] & 0x1F;
	if (rgSt < 24)
		rgSt = status_rtn[rgSt];
	pResult->Status = rgSt;
	pResult->Ambient = (temp[7] << 8 | temp[8]) * 8;
	pResult->NumSPADs = temp[3];
	pResult->SigPerSPAD = (temp[15] << 8 | temp[16]) * 8;
	pResult->Distance = temp[13] << 8 | temp[14];

	return status;
}

/**
 *  @brief      Set offset
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint16_t offsetValue - 16bit offset value
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_SetOffset(uint8_t dev, int16_t offsetValue)
{
	int16_t temp;

	temp = (offsetValue*4);
	VL53L1X_WrWord(dev, ALGO__PART_TO_PART_RANGE_OFFSET_MM,(uint16_t)temp);
	VL53L1X_WrWord(dev, MM_CONFIG__INNER_OFFSET_MM, 0x0);
	VL53L1X_WrWord(dev, MM_CONFIG__OUTER_OFFSET_MM, 0x0);

	return status;
}

/**
 *  @brief      Get offset
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint16_t *offset - 16bit pointer to offset value
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t  VL53L1X_GetOffset(uint8_t dev, int16_t *offset)
{
	uint16_t temp;

	status = VL53L1X_RdWord(dev,ALGO__PART_TO_PART_RANGE_OFFSET_MM, &temp);
	temp = temp<<3;
	temp = temp>>5;
	*offset = (int16_t)(temp);

	return status;
}

/**
 *  @brief      Set Xtalk
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint16_t XtalkValue - 16bit Xtalk value
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_SetXtalk(uint8_t dev, uint16_t XtalkValue)
{
/* XTalkValue in count per second to avoid float type */

	status = VL53L1X_WrWord(dev,
			ALGO__CROSSTALK_COMPENSATION_X_PLANE_GRADIENT_KCPS,
			0x0000);
	status = VL53L1X_WrWord(dev, ALGO__CROSSTALK_COMPENSATION_Y_PLANE_GRADIENT_KCPS,
			0x0000);
	status = VL53L1X_WrWord(dev, ALGO__CROSSTALK_COMPENSATION_PLANE_OFFSET_KCPS,
			(XtalkValue<<9)/1000); /* * << 9 (7.9 format) and /1000 to convert cps to kpcs */
	return status;
}

/**
 *  @brief      Get Xtalk
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint16_t *Xtalk - 16bit pointer to Xtalk value
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_GetXtalk(uint8_t dev, uint16_t *Xtalk )
{
	uint32_t tmp;

	status = VL53L1X_RdDWord(dev,ALGO__CROSSTALK_COMPENSATION_PLANE_OFFSET_KCPS, &tmp);
	*Xtalk = (uint16_t)(tmp*1000) >> 9; /* * 1000 to convert kcps to cps and >> 9 (7.9 format) */

	return status;
}
/**

 * @param   dev : device address
 * @param  	ThreshLow(in mm) : the threshold under which one the device raises an interrupt if Window = 0
 * @param 	ThreshHigh(in mm) :  the threshold above which one the device raises an interrupt if Window = 1
 * @param   Window detection mode : 0=below, 1=above, 2=out, 3=in
 * @param   IntOnNoTarget = 1 (No longer used - just use 1)
 */
/**
 *  @brief      Set distance threshold
 * This function programs the threshold detection mode\n
 * Example:
 * VL53L1X_SetDistanceThreshold(dev,100,300,0,1): Below 100
 * VL53L1X_SetDistanceThreshold(dev,100,300,1,1): Above 300
 * VL53L1X_SetDistanceThreshold(dev,100,300,2,1): Out of window
 * VL53L1X_SetDistanceThreshold(dev,100,300,3,1): In window
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint16_t threshLow - 16bit thresh low value
 *
 *	@param[in]  uint16_t threshHigh - 16bit thresh high value
 *
 *	@param[in]  uint8_t window - 8bit window value
 *
 *	@param[in]  uint8_t intOnNoTarget - 8bit intOnNoTarget value
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_SetDistanceThreshold(uint8_t dev, uint16_t threshLow, uint16_t threshHigh, uint8_t window,uint8_t intOnNoTarget)
{
	uint8_t temp = 0;

	status = VL53L1X_RdByte(dev, SYSTEM__INTERRUPT_CONFIG_GPIO, &temp);
	temp = temp & 0x47;
	if (intOnNoTarget == 0) {
		status = VL53L1X_WrByte(dev, SYSTEM__INTERRUPT_CONFIG_GPIO,(temp | (window & 0x07)));
	} else {
		status = VL53L1X_WrByte(dev, SYSTEM__INTERRUPT_CONFIG_GPIO,((temp | (window & 0x07)) | 0x40));
	}
	uint16_t tempThreshLow = 0;
	uint16_t tempThreshHigh = 0;
	status = VL53L1X_WrWord(dev, SYSTEM__THRESH_HIGH, threshHigh);

	status = VL53L1X_RdWord(dev,SYSTEM__THRESH_HIGH,&tempThreshHigh);

	status = VL53L1X_WrWord(dev, SYSTEM__THRESH_LOW, threshLow);

	status = VL53L1X_RdWord(dev,SYSTEM__THRESH_LOW,&tempThreshLow);

	return status;
}

/**
 *  @brief      Get distance threshold window
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint8_t window - 16bit pointer to window value
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_GetDistanceThresholdWindow(uint8_t dev, uint16_t *window)
{
	uint8_t tmp;
	status = VL53L1X_RdByte(dev,SYSTEM__INTERRUPT_CONFIG_GPIO, &tmp);
	*window = (uint16_t)(tmp & 0x7);

	return status;
}

/**
 *  @brief      Get distance threshold window low
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint16_t low - 16bit pointer to low value
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_GetDistanceThresholdLow(uint8_t dev, uint16_t *low)
{
	uint16_t tmp;

	status = VL53L1X_RdWord(dev, SYSTEM__THRESH_LOW, &tmp);
	*low = tmp;
	return status;
}

/**
 *  @brief      Get distance threshold window high
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint16_t high - 16bit pointer to high value
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_GetDistanceThresholdHigh(uint8_t dev, uint16_t *high)
{
	uint16_t tmp;

	status = VL53L1X_RdWord(dev,SYSTEM__THRESH_HIGH, &tmp);
	*high = tmp;

	return status;
}

/**
 *  @brief     Set ROI center
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint16_t ROICenter - 16bit ROI Center value
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_SetROICenter(uint8_t dev, uint8_t ROICenter)
{
	status = VL53L1X_WrByte(dev, ROI_CONFIG__USER_ROI_CENTRE_SPAD, ROICenter);
	return status;
}

/**
 *  @brief     Get ROI center
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint16_t *ROICenter - 16bit pointer to ROI center value
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_GetROICenter(uint8_t dev, uint8_t *ROICenter)
{
	uint8_t tmp;
	status = VL53L1X_RdByte(dev, ROI_CONFIG__USER_ROI_CENTRE_SPAD, &tmp);
	*ROICenter = tmp;

	return status;
}
/**
 *  @brief    	Set ROI
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint16_t X - 16bit X axel value
 *
 *	@param[in]  uint16_t Y - 16bit Y axel value
 *
 *  @return     i2c_result_t
 *
 *  @api
 */

i2c_result_t VL53L1X_SetROI(uint8_t dev, uint16_t X, uint16_t Y)
{
	uint8_t opticalCenter;

	status = VL53L1X_RdByte(dev, VL53L1_ROI_CONFIG__MODE_ROI_CENTRE_SPAD, &opticalCenter);
	if (X > 16)
		X = 16;
	if (Y > 16)
		Y = 16;
	if (X > 10 || Y > 10){
		opticalCenter = 199;
	}
	status = VL53L1X_WrByte(dev, ROI_CONFIG__USER_ROI_CENTRE_SPAD, opticalCenter);
	status = VL53L1X_WrByte(dev, ROI_CONFIG__USER_ROI_REQUESTED_GLOBAL_XY_SIZE,(Y - 1) << 4 | (X - 1));

	return status;
}

/**
 *  @brief    	Get ROI
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint16_t *ROI_X - 16bit pointer to ROI X axel value
 *
 *	@param[in]  uint16_t *ROI_Y - 16bit pointer to ROI Y axel value
 *
 *  @return     i2c_result_t
 *
 *  @api
 */

i2c_result_t VL53L1X_GetROI_XY(uint8_t dev, uint16_t *ROI_X, uint16_t *ROI_Y)
{
	uint8_t tmp;

	status = VL53L1X_RdByte(dev,ROI_CONFIG__USER_ROI_REQUESTED_GLOBAL_XY_SIZE, &tmp);
	*ROI_X = ((uint16_t)tmp & 0x0F) + 1;
	*ROI_Y = (((uint16_t)tmp & 0xF0) >> 4) + 1;

	return status;
}

/**
 *  @brief    	Set signal threshold
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint16_t signal - 16bit signal value
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_SetSignalThreshold(uint8_t dev, uint16_t signal)
{

	VL53L1X_WrWord(dev,RANGE_CONFIG__MIN_COUNT_RATE_RTN_LIMIT_MCPS,signal>>3);

	return status;
}

/**
 *  @brief    	Get signal threshold
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint16_t *signal - 16bit pointer to signal value
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_GetSignalThreshold(uint8_t dev, uint16_t *signal)
{
	uint16_t tmp;

	status = VL53L1X_RdWord(dev,RANGE_CONFIG__MIN_COUNT_RATE_RTN_LIMIT_MCPS, &tmp);
	*signal = tmp << 3;

	return status;
}

/**
 *  @brief    	Set sigma threshold
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint16_t sigma - 16bit sigma value
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_SetSigmaThreshold(uint8_t dev, uint16_t sigma)
{
	if(sigma > (0xFFFF>>2))
	{
		return 1;
	}
	/* 16 bits register 14.2 format */
	status = VL53L1X_WrWord(dev,RANGE_CONFIG__SIGMA_THRESH,sigma<<2);
	return status;
}

/**
 *  @brief    	Get sigma threshold
 *
 *	@param[in]  uint8_t dev - 8bit device address
 *
 *	@param[in]  uint16_t *sigma - 16bit pointer to sigma value
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_GetSigmaThreshold(uint8_t dev, uint16_t *sigma)
{
	uint16_t tmp;

	status = VL53L1X_RdWord(dev,RANGE_CONFIG__SIGMA_THRESH, &tmp);
	*sigma = tmp >> 2;
	return status;

}

/**
 *  @brief    	Start temperature update
 *
 *	@param[in]  uint16_t dev - 16bit dev address
 *
 *  @return     i2c_result_t
 *
 *  @api
 */
i2c_result_t VL53L1X_StartTemperatureUpdate(uint8_t dev)
{
	uint8_t tmp=0;

	status = VL53L1X_WrByte(dev,VL53L1_VHV_CONFIG__TIMEOUT_MACROP_LOOP_BOUND,0x81); /* full VHV */
	status = VL53L1X_WrByte(dev,0x0B,0x92);
	status = VL53L1X_StartRanging(dev);
	while(tmp==0){
		status = VL53L1X_CheckForDataReady(dev, &tmp);
	}
	tmp  = 0;
	status = VL53L1X_ClearInterrupt(dev);
	status = VL53L1X_StopRanging(dev);
	status = VL53L1X_WrByte(dev, VL53L1_VHV_CONFIG__TIMEOUT_MACROP_LOOP_BOUND, 0x09); /* two bounds VHV */
	status = VL53L1X_WrByte(dev, 0x0B, 0); /* start VHV from the previous temperature */
	return status;
}
/** @} */



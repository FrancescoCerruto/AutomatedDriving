

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
 * @file     vl53l1_platform.c

 * @brief   SPC5xx  vl53l1_platform driver code.
 *
 * @addtogroup vl53l1_platform
 * @{
 */

#include "full_include/vl53l1_platform.h"
#include "full_include/vl53l1_platform_log.h"
#include "full_include/vl53l1_api.h"
#include <string.h>
#include <time.h>
#include <math.h>
extern I2CDriver* AEK_SNS_VL53L1X_I2C_ARRAY_DRIVER[1];
uint8_t _I2CBuffer[256];

/**
 *  @brief      Write array of byte VL53L1 register
 *
 *  @param[in]  uint8_t dev - 8bit dev address
 *
 *  @param[in]  uint8_t reg_addr - bit 8 address register
 *
 *  @param[in]  uint8_t *data - 8bit pointer to the data where written data will be stored
 *
 *  @param[in]  uint8_t size - number of bytes to write
 *
 *  @return     VL53L1_Error
 *
 *  @api
 */
VL53L1_Error VL53L1_WriteMulti(VL53L1_DEV dev, uint16_t reg_addr, uint8_t *data,uint8_t size)
{
	status = i2c_lld_write16(AEK_SNS_VL53L1X_I2C_ARRAY_DRIVER[0], dev->I2cDevAddr, reg_addr, data, size);

    return status;
}
/**
 *  @brief      Read VL53L1 register
 *
 *  @param[in]  uint8_t dev - 8bit dev address
 *
 *  @param[in]  uint16_t reg_addr - bit 16 address register
 *
 *  @param[in]  uint8_t *data - 8bit pointer to the data where written data will be stored
 *
 *  @param[in]  uint8_t size - number of bytes to write
 *
 *  @return     VL53L1_Error
 *
 *  @api
 */
VL53L1_Error VL53L1_ReadMulti(VL53L1_DEV dev, uint16_t reg_addr, uint8_t *data,uint8_t size)
{
    status = i2c_lld_read16(AEK_SNS_VL53L1X_I2C_ARRAY_DRIVER[0], dev->I2cDevAddr, reg_addr, data, size);

    return status;
}

/**
 *  @brief      Write a byte VL53L1 register
 *
 *  @param[in]  uint8_t dev - 8bit dev address
 *
 *  @param[in]  uint16_t reg_addr - bit 16 address register
 *
 *  @param[in]  uint8_t data - 8bit pointer to the data where written data will be stored <--!!!
 *
 *  @return     VL53L1_Error
 *
 *  @api
 */
VL53L1_Error VL53L1_WrByte(VL53L1_DEV dev, uint16_t reg_addr, uint8_t data)
{
    status = i2c_lld_write16(AEK_SNS_VL53L1X_I2C_ARRAY_DRIVER[0], dev->I2cDevAddr, reg_addr, &data, 1);

    return status;
}

/**
 *  @brief      Write a word in VL53L1 register
 *
 *  @param[in]  uint8_t dev - 8bit dev address
 *
 *  @param[in]  uint16_t reg_addr - bit 16 address register
 *
 *  @param[in]  uint16_t data - 16bit pointer to the data where written data will be stored <--
 *
 *  @return     VL53L1_Error
 *
 *  @api
 */
VL53L1_Error VL53L1_WrWord(VL53L1_DEV dev, uint16_t reg_addr, uint16_t data)
{
    uint8_t _i2cBuffer[2];
    /* MSB first (Big endians) */
    _i2cBuffer[0] = data >> 8;
    _i2cBuffer[1] = data & 0x00FF;

    status = i2c_lld_write16 (AEK_SNS_VL53L1X_I2C_ARRAY_DRIVER[0], dev->I2cDevAddr, reg_addr, _i2cBuffer, 2);

    return status;
}

/**
 *  @brief      Write a DWord in VL53L1 register
 *
 *  @param[in]  uint8_t dev - 8bit dev address
 *
 *  @param[in]  uint16_t reg_addr - bit 16 address register
 *
 *  @param[in]  uint32_t data - 32bit pointer to the data where written data will be stored
 *
 *  @return     VL53L1_Error
 *
 *  @api
 */
VL53L1_Error VL53L1_WrDWord(VL53L1_DEV dev, uint16_t reg_addr, uint32_t data)
{
    uint8_t _i2cBuffer[4];
    /* MSB first (Big endians) */
    _i2cBuffer[0] = (data >> 24) & 0xFF;
    _i2cBuffer[1] = (data >> 16) & 0xFF;
    _i2cBuffer[2] = (data >> 8) & 0xFF;
    _i2cBuffer[3] = data & 0xFF;

    status = i2c_lld_write16 (AEK_SNS_VL53L1X_I2C_ARRAY_DRIVER[0], dev->I2cDevAddr, reg_addr, _i2cBuffer, 4);

    return status;
}

/**
 *  @brief      Update VL53L1 register
 *
 *  @param[in]  uint8_t dev - 8bit dev address
 *
 *  @param[in]  uint16_t reg_addr - bit 16 address register
 *
 *  @param[in]  uint8_t andData -
 *
 *  @param[in]  uint8_t orData -
 *
 *  @return     VL53L1_Error
 *
 *  @api
 */
VL53L1_Error VL53L1_UpdateByte(VL53L1_DEV dev, uint16_t reg_addr, uint8_t andData, uint8_t orData)
{
    uint8_t data;

    status = VL53L1_RdByte(dev, reg_addr, &data);
    if (status)
    {
        goto done;
    }
    data = (data & andData) | orData;
    status = VL53L1_WrByte(dev, reg_addr, data);

    done:
		return status;
}

/**
 *  @brief      Read a byte in VL53L1 register
 *
 *  @param[in]  uint8_t dev - 8bit dev address
 *
 *  @param[in]  uint16_t reg_addr - bit 16 address register
 *
 *  @param[in]  uint8_t *data - 8bit pointer to the data where written data will be stored
 *
 *  @return     VL53L1_Error
 *
 *  @api
 */
VL53L1_Error VL53L1_RdByte(VL53L1_DEV dev, uint16_t reg_addr, uint8_t *data)
{
    status = i2c_lld_read16(AEK_SNS_VL53L1X_I2C_ARRAY_DRIVER[0],dev->I2cDevAddr, reg_addr, data, 1);

    if (status)
    {
        goto done;
    }
    done:
        return status;
}

/**
 *  @brief      Read a word in VL53L1 register
 *
 *  @param[in]  uint8_t dev - 8bit dev address
 *
 *  @param[in]  uint16_t reg_addr - bit 16 address register
 *
 *  @param[in]  uint16_t *data - 16bit pointer to the data where written data will be stored
 *
 *  @return     VL53L1_Error
 *
 *  @api
 */
VL53L1_Error VL53L1_RdWord(VL53L1_DEV dev, uint16_t reg_addr, uint16_t *data)
{

    status = i2c_lld_read16(AEK_SNS_VL53L1X_I2C_ARRAY_DRIVER[0],dev->I2cDevAddr, reg_addr, _I2CBuffer, 2);
    if (status)
    {
        goto done;
    }

    *data = ((uint16_t)_I2CBuffer[0]<<8) + (uint16_t)_I2CBuffer[1];

    done:
        return status;
}

/**
 *  @brief      Read a Dword in VL53L1 register
 *
 *  @param[in]  uint8_t dev - 8bit dev address
 *
 *  @param[in]  uint16_t reg_addr - bit 16 address register
 *
 *  @param[in]  uint32_t *data - 32bit pointer to the data where written data will be stored
 *
 *  @return     VL53L1_Error
 *
 *  @api
 */
VL53L1_Error VL53L1_RdDWord(VL53L1_DEV dev, uint16_t reg_addr, uint32_t *data)
{

	status = i2c_lld_read16(AEK_SNS_VL53L1X_I2C_ARRAY_DRIVER[0],dev->I2cDevAddr, reg_addr, _I2CBuffer, 4);
    if (status)
    {
        goto done;
    }

    *data = ((uint32_t)_I2CBuffer[0]<<24) + ((uint32_t)_I2CBuffer[1]<<16) + ((uint32_t)_I2CBuffer[2]<<8) + (uint32_t)_I2CBuffer[3];

    done:
        return status;
}

/****************************************************
*****  FUNZIONI PER IL VL53L1_WaitDeviceBooted  *****
****************************************************/



VL53L1_Error VL53L1_GetTickCount(uint32_t *ptick_count_ms)
{


	VL53L1_Error status  = VL53L1_ERROR_NONE;

	*ptick_count_ms = 0;
	return status;
}


VL53L1_Error VL53L1_GetTimerFrequency(int32_t *ptimer_freq_hz)
{
	VL53L1_Error status  = VL53L1_ERROR_NONE;
	*ptimer_freq_hz = 0;

	//trace_print(VL53L1_TRACE_LEVEL_INFO, "VL53L1_GetTimerFrequency: Freq : %dHz\n", *ptimer_freq_hz);
	return status;
}

VL53L1_Error VL53L1_WaitMs(VL53L1_Dev_t *pdev, int32_t wait_ms)
{
	(void)pdev;
	VL53L1_Error status  = VL53L1_ERROR_NONE;
	osalThreadDelayMilliseconds(wait_ms);
	return status;
}

/*ORIGINAL LOCATION : vl53l1_platform.c*/
VL53L1_Error VL53L1_WaitUs(VL53L1_Dev_t *pdev, int32_t wait_us)
{
	(void)pdev;
	VL53L1_Error status  = VL53L1_ERROR_NONE;
	osalThreadDelayMilliseconds(wait_us/1000);
	return status;
}

/*ORIGINAL LOCATION : vl53l1_platform.c*/

VL53L1_Error VL53L1_WaitValueMaskEx(
	VL53L1_Dev_t *pdev,
	uint32_t      timeout_ms,
	uint16_t      index,
	uint8_t       value,
	uint8_t       mask,
	uint32_t      poll_delay_ms)
{

	/*
	 * Platform implementation of WaitValueMaskEx V2WReg script command
	 *
	 * WaitValueMaskEx(
	 *          duration_ms,
	 *          index,
	 *          value,
	 *          mask,
	 *          poll_delay_ms);
	 */

	VL53L1_Error status         = VL53L1_ERROR_NONE;
	uint32_t     start_time_ms = 0;
	uint32_t     current_time_ms = 0;
	uint32_t     polling_time_ms = 0;
	uint8_t      byte_value      = 0;
	uint8_t      found           = 0;
#ifdef VL53L1_LOG_ENABLE
	uint8_t      trace_functions = VL53L1_TRACE_FUNCTION_NONE;
#endif

	char   register_name[VL53L1_MAX_STRING_LENGTH];

    /* look up register name */
#ifdef PAL_EXTENDED
	VL53L1_get_register_name(
			index,
			register_name);
#else
	VL53L1_COPYSTRING(register_name, "");
#endif

	/* Output to I2C logger for FMT/DFT  */

    /*trace_i2c("WaitValueMaskEx(%5d, 0x%04X, 0x%02X, 0x%02X, %5d);\n",
    		     timeout_ms, index, value, mask, poll_delay_ms); */
//    trace_i2c("WaitValueMaskEx(%5d, %s, 0x%02X, 0x%02X, %5d);\n",
//    		     timeout_ms, register_name, value, mask, poll_delay_ms);

	/* calculate time limit in absolute time */

	 VL53L1_GetTickCount(&start_time_ms);

	/* remember current trace functions and temporarily disable
	 * function logging
	 */

#ifdef VL53L1_LOG_ENABLE
	trace_functions = VL53L1_get_trace_functions();
	VL53L1_set_trace_functions(VL53L1_TRACE_FUNCTION_NONE);
#endif

	/* wait until value is found, timeout reached on error occurred */

	while ((status == VL53L1_ERROR_NONE) &&
		   (polling_time_ms < timeout_ms) &&
		   (found == 0)) {

		if (status == VL53L1_ERROR_NONE)
			status = VL53L1_RdByte(
							pdev,
							index,
							&byte_value);

		if ((byte_value & mask) == value)
			found = 1;

		if (status == VL53L1_ERROR_NONE  &&
			found == 0 &&
			poll_delay_ms > 0)
			status = VL53L1_WaitMs(
					pdev,
					poll_delay_ms);

		/* Update polling time (Compare difference rather than absolute to
		negate 32bit wrap around issue) */
		VL53L1_GetTickCount(&current_time_ms);
		polling_time_ms = current_time_ms - start_time_ms;

	}

#ifdef VL53L1_LOG_ENABLE
	/* Restore function logging */
	VL53L1_set_trace_functions(trace_functions);
#endif

	if (found == 0 && status == VL53L1_ERROR_NONE)
		status = VL53L1_ERROR_TIME_OUT;

	return status;
}


/** @} */


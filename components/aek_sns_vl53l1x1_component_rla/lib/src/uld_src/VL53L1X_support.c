

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
 * @file     VL53L1X_support.c

 * @brief   SPC5xx  VL53L1X_support driver code.
 *
 * @addtogroup VL53L1X_support
 * @{
 */


#include <string.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <uld_include/VL53L1X_support.h>

extern i2c_result_t status;
uint8_t ULD_I2CBuffer[256];
extern I2CDriver* AEK_SNS_VL53L1X_I2C_ARRAY_DRIVER[1];
/**
 * 	@brief This function write uint8_t data into 16 bit register.
 *
 *	@param[in] uint8_t dev: slave device address
 *
 * 	@param[in] uint16_t reg_addr : register address
 *
 * 	@param[in] uint8_t data : the data to be written
 *
 * 	@return status -> status = 0 success, status != 0 error
 */
i2c_result_t VL53L1X_WrByte(uint8_t dev, uint16_t reg_addr, uint8_t data)
{
    status = i2c_lld_write16(AEK_SNS_VL53L1X_I2C_ARRAY_DRIVER[0], dev, reg_addr, &data, 1);

    return status;
}
/**
 * 	@brief This function write uint16_t data into 16 bit register.
 *
 *	@param[in] uint8_t dev: slave device address
 *
 * 	@param[in] uint16_t reg_addr : register address
 *
 * 	@param[in] uint16_t data : the data to be written
 *
 * 	@return status -> status = 0 success, status != 0 error
 */
i2c_result_t VL53L1X_WrWord(uint8_t dev, uint16_t reg_addr, uint16_t data)
{
    uint8_t _i2cBuffer[2];
    /* MSB first (Big endians) */
    _i2cBuffer[0] = data >> 8;
    _i2cBuffer[1] = data & 0x00FF;

    status = i2c_lld_write16 (AEK_SNS_VL53L1X_I2C_ARRAY_DRIVER[0], dev, reg_addr, _i2cBuffer, 2);

    return status;
}
/**
 * 	@brief This function write uint32_t data into 16 bit register.
 *
 *	@param[in] uint8_t dev: slave device address
 *
 * 	@param[in] uint16_t reg_addr : register address
 *
 * 	@param[in] uint32_t data : the data to be written
 *
 * 	@return status : status = 0 success, status != 0 error
 */
i2c_result_t VL53L1X_WrDWord(uint8_t dev, uint16_t reg_addr, uint32_t data)
{
    uint8_t _i2cBuffer[4];
    /* MSB first (Big endians) */
    _i2cBuffer[0] = (data >> 24) & 0xFF;
    _i2cBuffer[1] = (data >> 16) & 0xFF;
    _i2cBuffer[2] = (data >> 8) & 0xFF;
    _i2cBuffer[3] = data & 0xFF;

    status = i2c_lld_write16 (AEK_SNS_VL53L1X_I2C_ARRAY_DRIVER[0], dev, reg_addr, _i2cBuffer, 4);

    return status;
}
/**
 * 	@brief This function update the data of register.
 *
 *	@param[in] uint8_t dev: slave device address
 *
 * 	@param[in] uint16_t reg_addr : register address
 *
 * 	@param[in] uint8_t andData : the data to be written
 *
 * 	@param[in] uint8_t orData : the data to be written
 *
 * 	@return status -> status = 0 success, status != 0 error
 */
i2c_result_t VL53L1X_UpdateByte(uint8_t dev, uint16_t reg_addr, uint8_t andData, uint8_t orData)
{
    uint8_t data;

    status = VL53L1X_RdByte(dev, reg_addr, &data);
    if (status)
    {
        goto done;
    }
    data = (data & andData) | orData;
    status = VL53L1X_WrByte(dev, reg_addr, data);

    done:
		return status;
}
/**
 * 	@brief This function read uint8_t data from the 16 bit register.
 *
 *	@param[in] uint8_t dev: slave device address
 *
 * 	@param[in] uint16_t reg_addr : register address
 *
 * 	@param[in] uint8_t *data
 *
 * 	@return status : status = 0 success, status != 0 error
 */
i2c_result_t VL53L1X_RdByte(uint8_t dev, uint16_t reg_addr, uint8_t *data)
{
    status = i2c_lld_read16(AEK_SNS_VL53L1X_I2C_ARRAY_DRIVER[0],dev, reg_addr, data, 1);

    if (status)
    {
        goto done;
    }
    done:
        return status;
}
/**
 * 	@brief This function read uint16_t data from the 16 bit register.
 *
 *	@param[in] uint8_t dev: slave device address
 *
 * 	@param[in] uint16_t reg_addr : register address
 *
 * 	@param[in] uint16_t *data
 *
 * 	@return status : status = 0 success, status != 0 error
 */
i2c_result_t VL53L1X_RdWord(uint8_t dev, uint16_t reg_addr, uint16_t *data)
{
    status = i2c_lld_read16(AEK_SNS_VL53L1X_I2C_ARRAY_DRIVER[0],dev, reg_addr, ULD_I2CBuffer, 2);
    if (status)
    {
        goto done;
    }

    *data = ((uint16_t)ULD_I2CBuffer[0]<<8) + (uint16_t)ULD_I2CBuffer[1];

    done:
        return status;
}
/**
 * 	@brief This function read uint32_t data from the 16 bit register.
 *
 *	@param[in] uint8_t dev: slave device address
 *
 * 	@param[in] uint16_t reg_addr : register address
 *
 * 	@param[in] uint32_t *data
 *
 * 	@return status : status = 0 success, status != 0 error
 */
i2c_result_t VL53L1X_RdDWord(uint8_t dev, uint16_t reg_addr, uint32_t *data)
{

	status = i2c_lld_read16(AEK_SNS_VL53L1X_I2C_ARRAY_DRIVER[0],dev, reg_addr, ULD_I2CBuffer, 4);
    if (status)
    {
        goto done;
    }

    *data = ((uint32_t)ULD_I2CBuffer[0]<<24) + ((uint32_t)ULD_I2CBuffer[1]<<16) + ((uint32_t)ULD_I2CBuffer[2]<<8) + (uint32_t)ULD_I2CBuffer[3];

    done:
        return status;
}

/** @} */



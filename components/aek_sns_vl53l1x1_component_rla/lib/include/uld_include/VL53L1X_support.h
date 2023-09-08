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
 * @file    VL53L1X_support.h
 * @brief   SPC5xx VL53L1X_support.h header file.
 *
 * @addtogroup VL53L1X_support
 * @{
 */
#ifndef LIB_INCLUDE_VL53L1X_SUPPORT_H_
#define LIB_INCLUDE_VL53L1X_SUPPORT_H_

#include "components.h"
#include <i2c_lld_cfg.h>

i2c_result_t VL53L1X_WrByte(uint8_t dev,uint16_t reg_addr,uint8_t data);

i2c_result_t VL53L1X_WrWord(uint8_t dev,uint16_t reg_addr,uint16_t data);

i2c_result_t VL53L1X_WrDWord(uint8_t dev,uint16_t reg_addr,uint32_t data);

i2c_result_t VL53L1X_RdByte(uint8_t dev,uint16_t reg_addr,uint8_t *pdata);

i2c_result_t VL53L1X_RdWord(uint8_t dev,uint16_t reg_addr,uint16_t *pdata);

i2c_result_t VL53L1X_RdDWord(uint8_t dev,uint16_t reg_addr,uint32_t *pdata);

i2c_result_t VL53L1X_UpdateByte(uint8_t dev,uint16_t reg_addr,uint8_t andData,uint8_t orData);

#endif /* LIB_INCLUDE_VL53L1X_SUPPORT_H_ */

/** @} */

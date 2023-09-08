/****************************************************************************
*
* Copyright � 2019 STMicroelectronics - All Rights Reserved
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
 * @file    AEK_SNS_VL53L1X1_cfg.h
 * @brief   AEK_SNS_VL53L1X1 Driver configuration macros and structures.
 *
 * @addtogroup AEK_SNS_VL53L1X1
 * @{
 */

#ifndef _AEK_SNS_VL53L1X1_CFG_H_
#define _AEK_SNS_VL53L1X1_CFG_H_

#include "spc5_lld.h"
#include "lldconf.h"
#include "i2s_lld.h"
#include "i2c_lld.h"
#include <i2s_lld_cfg.h>
#include <i2c_lld_cfg.h>
#include <board.h>

// ******************************************************************************* //
// ****************************  Global variables  ******************************* //
// ******************************************************************************* //




typedef enum
{
	AEK_TOF_DEV0
	
}AEK_TOF_DEV;

extern  I2CDriver* AEK_SNS_VL53L1X_I2C_ARRAY_DRIVER[1];
extern  void* AEK_SNS_VL53L1X_I2C_ARRAY_CONFIGURATION[1];

#define Allocation_Executed  
#define ULD_enable
#define numberSensors 1

extern uint8_t XshutMatrix [numberSensors][2];


typedef struct
{
	uint8_t port;
	uint8_t pin;

}xshut_t;


typedef enum
{
	AEK_TOF_NO_ERROR,
	AEK_TOF_ERROR_CONFIGURATION
}AEK_TOF_ERROR;

#ifdef ULD_enable

typedef struct
{
	uint8_t Dev;
	xshut_t Xshut;
	volatile uint8_t IRQState;
	AEK_TOF_ERROR errorState;
	uint8_t stateSensor;
}uld_vl53l1x_t;
extern uld_vl53l1x_t uld_VL53L1X[numberSensors];
#else
#include "full_include/vl53l1_platform_user_data.h"

typedef struct
{
	VL53L1_Dev_t Dev;
	xshut_t Xshut;
	volatile uint8_t IRQState;
	AEK_TOF_ERROR errorState;
	uint8_t stateSensor;
}vl53l1x_t;
extern vl53l1x_t VL53L1X[numberSensors];
#endif

void TOF_Callback0(void); 


#endif /*_AEK_SNS_VL53L1X1_CFG_H_*/
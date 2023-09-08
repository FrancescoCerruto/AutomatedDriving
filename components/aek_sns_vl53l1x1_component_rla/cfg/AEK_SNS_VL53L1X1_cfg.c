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
 * @brief 	AEK_SNS_VL53L1X1 Driver configuration macros and structures.
 *
 * @addtogroup AEK_SNS_VL53L1X1
 * @{
 */

#include "spc5_lld.h"
#include "lldconf.h"
#include "i2s_lld.h"
#include "i2c_lld.h"
#include <i2s_lld_cfg.h>
#include <i2c_lld_cfg.h>
#include <AEK_SNS_VL53L1X1_cfg.h>



#ifdef ULD_enable
uld_vl53l1x_t uld_VL53L1X[numberSensors];
#else
vl53l1x_t VL53L1X[numberSensors];
#endif
				
uint8_t XshutMatrix [numberSensors][2] = { {PORT_D,14}};

I2CDriver* AEK_SNS_VL53L1X_I2C_ARRAY_DRIVER[1]={
&I2CD1
};

void* AEK_SNS_VL53L1X_I2C_ARRAY_CONFIGURATION[1]={
&i2c_hw_config_AEK_SNS_VL53L1X1Board0
};	

	


void TOF_Callback0(void)
{

			uld_VL53L1X[0].IRQState = 1; 

}

/** @} */

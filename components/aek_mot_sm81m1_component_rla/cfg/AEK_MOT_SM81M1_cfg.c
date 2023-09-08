/****************************************************************************
*
* Copyright © 2019 STMicroelectronics - All Rights Reserved
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
 * @file    AEK_MOT_SM81M1_cfg.c
 * @brief   AEK_MOT_SM81M1 Driver configuration macros and structures.
 *
 * @addtogroup AEK_MOT_SM81M1
 * @{
 */

#ifndef _AEK_MOT_SM81M1_CFG_C_
#define _AEK_MOT_SM81M1_CFG_C_

#include "spc5_lld.h"
#include "lldconf.h"
#include "spi_lld.h"
#include "spi_lld_cfg.h"




SPIDriver* AEK_MOT_SM81M1_ARRAY_DRIVER[1]={
&SPID1
};


const int AEK_MOT_SM81M1_ARRAY_GPIO_PORT[1]={
PORT_PIN_AEK_MOT_SM81M1Board0_DIOTP_EN_GPIO8 				
};

const int AEK_MOT_SM81M1_ARRAY_GPIO_PORT_NUMBER[1]={
PIN_AEK_MOT_SM81M1Board0_DIOTP_EN_GPIO8 				
};

SPIConfig* AEK_MOT_SM81M1_ARRAY_CONFIGURATION[1]={
&spi_config_AEK_MOT_SM81M1Board0_SPITP_CS_CS0_0
};

int position =0;
uint8_t previousMode = 0;
uint8_t rotation = 0;


#endif /* _AEK_MOT_SM81M1_CFG_C_ */

/** @} */

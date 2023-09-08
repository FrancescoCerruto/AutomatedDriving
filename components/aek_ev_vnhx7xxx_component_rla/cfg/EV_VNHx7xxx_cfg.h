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
 * @file    EV_VNHx7xxx_cfg.h
 * @brief   EV_VNHx7xxx Driver configuration macros and structures.
 *
 * @addtogroup EV-VNHx7xxx
 * @{
 */

#ifndef _EV_VNHx7xxx_CFG_H_
#define _EV_VNHx7xxx_CFG_H_

#include "spc5_lld.h"
#include "lldconf.h"
#include "saradc_lld.h"
#include "siul_lld.h"
#include "pwm_lld_cfg.h"
#include "saradc_lld_cfg.h"
extern float valueVNH;


#define AEK_VNH_GTM_ACTIVE 0
#define ADC_LOGIC_VOLTAGE 3.3F //Volt
#define AEK_EV_VNH_LIST_ELEMENT 2


#define MULTISENSE_RESPONSE 0










extern const int EV_VNH_ARRAY_BOARD_0SEL_GPIO_PORT[1];
extern const int EV_VNH_ARRAY_BOARD_1SEL_GPIO_PORT[1];



extern int EV_VNH_ARRAY_SEL_TOT_GPIO_PORT[2];


extern const int EV_VNH_ARRAY_BOARD_0SEL_GPIO_PORT_NUMBER[1];
extern const int EV_VNH_ARRAY_BOARD_1SEL_GPIO_PORT_NUMBER[1];



extern int EV_VNH_ARRAY_SEL_TOT_GPIO_PORT_NUMBER[2];


extern const int EV_VNH_ARRAY_BOARD_0IN_GPIO_PORT[2];
extern const int EV_VNH_ARRAY_BOARD_1IN_GPIO_PORT[2];


extern int EV_VNH_ARRAY_IN_TOT_GPIO_PORT[2];


extern const int EV_VNH_ARRAY_BOARD_0IN_GPIO_PORT_NUMBER[2];
extern const int EV_VNH_ARRAY_BOARD_1IN_GPIO_PORT_NUMBER[2];



extern int EV_VNH_ARRAY_IN_TOT_GPIO_PORT_NUMBER[2];



extern const int EV_VNH_IN_NUMBER_VALUE[2];

extern const int EV_VNH_SEL_NUMBER_VALUE[2];

typedef enum {
EV_VNHx7xxx_DEV0=0
	,	
EV_VNHx7xxx_DEV1=1
}EV_VNHx7xxx_DEVICE;







extern PWMDriver* EV_VNH_ARRAY_PWM[2];

extern const int PWMCHANNELS[2];

extern PWMConfig* EV_VNH_ARRAY_PWM_CONFIGURATION[2];

extern const int EV_VNH_ARRAY_PWM_GPIO_PORT[2];

extern const int EV_VNH_ARRAY_PWM_GPIO_PORT_NUMBER[2];


#endif /* _EV_VNHx7xxx_CFG_H_ */

/** @} */

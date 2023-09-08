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

#ifndef _EV_VNHx7xxx_CFG_C_
#define _EV_VNHx7xxx_CFG_C_

#include <EV_VNHx7xxx_cfg.h>
float valueVNH = 0;






/**
 * @name EV-VNHx7xxx
 * @brief EV-VNHx7xxx Variables
 *@{
 */ 

uint16_t value = 0U;




const int EV_VNH_MULTISENSE_BOOL[2]={
0
	,	
0
};


const int EV_VNH_ARRAY_BOARD_0SEL_GPIO_PORT[1]=
{	
PORT_PIN_AEK_EV_VNHx7xxxBoard0_J2_SEL0_GPIO1
};
const int EV_VNH_ARRAY_BOARD_1SEL_GPIO_PORT[1]=
{	
PORT_PIN_AEK_EV_VNHx7xxxBoard1_J2_SEL0_GPIO9
};


int EV_VNH_ARRAY_SEL_TOT_GPIO_PORT[2]={
(int) EV_VNH_ARRAY_BOARD_0SEL_GPIO_PORT
	,	
(int) EV_VNH_ARRAY_BOARD_1SEL_GPIO_PORT
};

const int EV_VNH_ARRAY_BOARD_0SEL_GPIO_PORT_NUMBER[1]=
{	

PIN_AEK_EV_VNHx7xxxBoard0_J2_SEL0_GPIO1

};
const int EV_VNH_ARRAY_BOARD_1SEL_GPIO_PORT_NUMBER[1]=
{	

PIN_AEK_EV_VNHx7xxxBoard1_J2_SEL0_GPIO9

};


int EV_VNH_ARRAY_SEL_TOT_GPIO_PORT_NUMBER[2]={
(int) EV_VNH_ARRAY_BOARD_0SEL_GPIO_PORT_NUMBER
	,	
(int) EV_VNH_ARRAY_BOARD_1SEL_GPIO_PORT_NUMBER
};


const int EV_VNH_ARRAY_BOARD_0IN_GPIO_PORT[2]=
{	
PORT_PIN_AEK_EV_VNHx7xxxBoard0_J1_INB_GPIO8
						
,	
PORT_PIN_AEK_EV_VNHx7xxxBoard0_J1_INA_GPIO2
						
};
const int EV_VNH_ARRAY_BOARD_1IN_GPIO_PORT[2]=
{	
PORT_PIN_AEK_EV_VNHx7xxxBoard1_J1_INA_GPIO10
						
,	
PORT_PIN_AEK_EV_VNHx7xxxBoard1_J1_INB_GPIO11
						
};


int EV_VNH_ARRAY_IN_TOT_GPIO_PORT[2]={
(int) EV_VNH_ARRAY_BOARD_0IN_GPIO_PORT
	,	
(int) EV_VNH_ARRAY_BOARD_1IN_GPIO_PORT
};


const int EV_VNH_ARRAY_BOARD_0IN_GPIO_PORT_NUMBER[2]=
{	
PIN_AEK_EV_VNHx7xxxBoard0_J1_INB_GPIO8
,	
PIN_AEK_EV_VNHx7xxxBoard0_J1_INA_GPIO2
};
const int EV_VNH_ARRAY_BOARD_1IN_GPIO_PORT_NUMBER[2]=
{	
PIN_AEK_EV_VNHx7xxxBoard1_J1_INA_GPIO10
,	
PIN_AEK_EV_VNHx7xxxBoard1_J1_INB_GPIO11
};


int EV_VNH_ARRAY_IN_TOT_GPIO_PORT_NUMBER[2]={
(int) EV_VNH_ARRAY_BOARD_0IN_GPIO_PORT_NUMBER
	,	
(int) EV_VNH_ARRAY_BOARD_1IN_GPIO_PORT_NUMBER
};


const int EV_VNH_IN_NUMBER_VALUE[2]={
2
	,	
2
};

const int EV_VNH_SEL_NUMBER_VALUE[2]={
1
	,	
1
};
PWMDriver* EV_VNH_ARRAY_PWM[2]={
&PWMD9
	,	
&PWMD11
};

const int PWMCHANNELS[2]={
5U
	,	
3U
};

PWMConfig* EV_VNH_ARRAY_PWM_CONFIGURATION[2]={
&pwm_config_AEK_EV_VNHx7xxxBoard0_J2_PWM_14

	,	
&pwm_config_AEK_EV_VNHx7xxxBoard1_J2_PWM_28

};


const int EV_VNH_ARRAY_PWM_GPIO_PORT[2]={
PORT_PIN_AEK_EV_VNHx7xxxBoard0_J2_PWM_UC14 				
	,	
PORT_PIN_AEK_EV_VNHx7xxxBoard1_J2_PWM_UC28 				
};

const int EV_VNH_ARRAY_PWM_GPIO_PORT_NUMBER[2]={
PIN_AEK_EV_VNHx7xxxBoard0_J2_PWM_UC14 				
	,	
PIN_AEK_EV_VNHx7xxxBoard1_J2_PWM_UC28 				
};
/** @} */





#endif /* _EV_VNHx7xxx_CFG_C_ */

/** @} */

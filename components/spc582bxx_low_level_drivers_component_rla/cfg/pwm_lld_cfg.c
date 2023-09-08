/****************************************************************************
*
* Copyright © 2018-2019 STMicroelectronics - All Rights Reserved
*
* This software is licensed under SLA0098 terms that can be found in the
* DM00779817_1_0.pdf file in the licenses directory of this software product.
* 
* THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED, 
* INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
*****************************************************************************/
/**
 * @file    pwm_lld_cfg.c
 * @brief   PWM Driver configuration code.
 *
 * @addtogroup PWM
 * @{
 */

#include "pwm_lld_cfg.h"

#if (LLD_USE_PWM == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   Structure defining the PWM channel configurations.
 */
static PWMChannelConfig pwm_config_AEK_EV_VNHx7xxxBoard0_J2_PWM_14_ch0 = {
  PWM_OUTPUT_DISABLED,
  NULL,
};

static PWMChannelConfig pwm_config_AEK_EV_VNHx7xxxBoard0_J2_PWM_14_ch1 = {
  PWM_OUTPUT_DISABLED,
  NULL,
};

static PWMChannelConfig pwm_config_AEK_EV_VNHx7xxxBoard0_J2_PWM_14_ch2 = {
  PWM_OUTPUT_DISABLED,
  NULL,
};

static PWMChannelConfig pwm_config_AEK_EV_VNHx7xxxBoard0_J2_PWM_14_ch3 = {
  PWM_OUTPUT_DISABLED,
  NULL,
};

static PWMChannelConfig pwm_config_AEK_EV_VNHx7xxxBoard0_J2_PWM_14_ch4 = {
  PWM_OUTPUT_DISABLED,
  NULL,
};

static PWMChannelConfig pwm_config_AEK_EV_VNHx7xxxBoard0_J2_PWM_14_ch5 = {
  PWM_OUTPUT_ACTIVE_HIGH,
  NULL,
};

static PWMChannelConfig pwm_config_AEK_EV_VNHx7xxxBoard0_J2_PWM_14_ch6 = {
  PWM_OUTPUT_DISABLED,
  NULL,
};

/**
 * @brief   Structure defining the PWM channel configurations.
 */
static PWMChannelConfig pwm_config_AEK_EV_VNHx7xxxBoard1_J2_PWM_28_ch0 = {
  PWM_OUTPUT_DISABLED,
  NULL,
};

static PWMChannelConfig pwm_config_AEK_EV_VNHx7xxxBoard1_J2_PWM_28_ch1 = {
  PWM_OUTPUT_DISABLED,
  NULL,
};

static PWMChannelConfig pwm_config_AEK_EV_VNHx7xxxBoard1_J2_PWM_28_ch2 = {
  PWM_OUTPUT_DISABLED,
  NULL,
};

static PWMChannelConfig pwm_config_AEK_EV_VNHx7xxxBoard1_J2_PWM_28_ch3 = {
  PWM_OUTPUT_ACTIVE_HIGH,
  NULL,
};

static PWMChannelConfig pwm_config_AEK_EV_VNHx7xxxBoard1_J2_PWM_28_ch4 = {
  PWM_OUTPUT_DISABLED,
  NULL,
};

static PWMChannelConfig pwm_config_AEK_EV_VNHx7xxxBoard1_J2_PWM_28_ch5 = {
  PWM_OUTPUT_DISABLED,
  NULL,
};

static PWMChannelConfig pwm_config_AEK_EV_VNHx7xxxBoard1_J2_PWM_28_ch6 = {
  PWM_OUTPUT_DISABLED,
  NULL,
};

/**
 * @brief   Structure defining the PWM configuration "AEK_EV_VNHx7xxxBoard0_J2_PWM_14".
 */
PWMConfig pwm_config_AEK_EV_VNHx7xxxBoard0_J2_PWM_14 = {
  FALSE,
  1,
  20000,
  NULL,
  {
    &pwm_config_AEK_EV_VNHx7xxxBoard0_J2_PWM_14_ch0,
    &pwm_config_AEK_EV_VNHx7xxxBoard0_J2_PWM_14_ch1,
    &pwm_config_AEK_EV_VNHx7xxxBoard0_J2_PWM_14_ch2,
    &pwm_config_AEK_EV_VNHx7xxxBoard0_J2_PWM_14_ch3,
    &pwm_config_AEK_EV_VNHx7xxxBoard0_J2_PWM_14_ch4,
    &pwm_config_AEK_EV_VNHx7xxxBoard0_J2_PWM_14_ch5,
    &pwm_config_AEK_EV_VNHx7xxxBoard0_J2_PWM_14_ch6,
  }
};

/**
 * @brief   Structure defining the PWM configuration "AEK_EV_VNHx7xxxBoard1_J2_PWM_28".
 */
PWMConfig pwm_config_AEK_EV_VNHx7xxxBoard1_J2_PWM_28 = {
  FALSE,
  1,
  20000,
  NULL,
  {
    &pwm_config_AEK_EV_VNHx7xxxBoard1_J2_PWM_28_ch0,
    &pwm_config_AEK_EV_VNHx7xxxBoard1_J2_PWM_28_ch1,
    &pwm_config_AEK_EV_VNHx7xxxBoard1_J2_PWM_28_ch2,
    &pwm_config_AEK_EV_VNHx7xxxBoard1_J2_PWM_28_ch3,
    &pwm_config_AEK_EV_VNHx7xxxBoard1_J2_PWM_28_ch4,
    &pwm_config_AEK_EV_VNHx7xxxBoard1_J2_PWM_28_ch5,
    &pwm_config_AEK_EV_VNHx7xxxBoard1_J2_PWM_28_ch6,
  }
};

/*===========================================================================*/
/* Driver local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

#endif /* LLD_USE_PWM */

/** @} */

/****************************************************************************
 *
 * Copyright ï¿½ 2019 STMicroelectronics - All Rights Reserved
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
 * @file    EV_VNHx7xxx.h
 * @brief   SPC5xx EV_VNHx7xxx header file.
 *
 * @addtogroup EV-VNHx7xxx
 * @{
 */

#include "EV_VNHx7xxx_cfg.h"
 
#ifndef EV_VNHx7xxx_H_
#define EV_VNHx7xxx_H_

/*******************************************************************************
 ********************************** Constants ***********************************
 *******************************************************************************/

#define EV_VNHx7xxx_INITIAL_STATUS 0
#define EV_VNHx7xxx_COMPLETE_STATUS 1
#define NOBRAKE 0
#define BRAKING 1
#define CLOCKWISE 0
#define COUNTERCLOCKWISE 1
#define NUMOFCHANNELS 1U
/*Example Motor Rating value*/
#define NO_LOAD_CURRENT 0.111F //mA
#define LOCKED_ROTOR_CURRENT 0.513F //mA
#define NO_LOAD_VOLTAGE 770.0F //mV
#define LOCKED_ROTOR_VOLTAGE 167.0F //mV
#define K_FACTOR 4500.0F
#define ARMATURE_VOLTAGE 12000.0F //mV
#define ARMATURE_RESISTANCE 0.005F //kOhm
#define NO_LOAD_SPEED 3000.0F //r/min
/*Evalutaion board value*/
#define R_SENSE 1.5F //kOhm
#define I_OUT_MAX 0.007F //mA/max
#define VSENS_H 5.0F //V/sat
#define K_FACTOR_NORMAL 1540 //Typical
#define I_SENSE_MAX 0.00455F //A

typedef enum {
	ACTIVE, DEACTIVE
} AEK_EV_VNH_SEL0;

typedef enum {
	SATURATION, HIZ
} AEK_EV_VNH_OFF_STATE;

typedef enum {
	INATYPE, INBTYPE
} EV_VNHx7xxx_TYPE_IN;

typedef enum {
	IN_ACTIVE, IN_DEACTIVE
}
EV_VNHx7xxx_MOTOR_ST_IN_STATUS;

typedef enum {
	HSB, HSA,HSA_OFF, HSB_OFF, LSB_OFFMOTOR,LSA_OFFMOTOR,HSA_BREAK,HSB_BREAK
} EV_VNHx7xxx_TYPE_SENSING;

typedef enum {
	HI_Z_STATUS, HSB_CURRENT, HSA_CURRENT, TEMP_STATUS, VCC_STATUS, HSA_PROBLEM, HSB_PROBLEM,LSA_PROBLEM, LSB_PROBLEM, SHORT_VCC, CHANNEL_PROBLEM
}
EV_VNHx7xxx_STATUS_DEVICE;

#if (AEK_EV_VNH_LIST_ELEMENT!=0)

/*******************************************************************************
 ********************************** Functions ***********************************
 *******************************************************************************/
void delay(uint8_t);
void EV_VNHx7xxx_PWMSetting(EV_VNHx7xxx_DEVICE device, uint8_t pwm);
void EV_VNHx7xxx_IN(EV_VNHx7xxx_DEVICE device, EV_VNHx7xxx_TYPE_IN type, EV_VNHx7xxx_MOTOR_ST_IN_STATUS status);
void EV_VNHx7xxx_SEL(EV_VNHx7xxx_DEVICE device, AEK_EV_VNH_SEL0 status);
void initMotor(EV_VNHx7xxx_DEVICE);
void brakeMotor(EV_VNHx7xxx_DEVICE);
void standByMotor(EV_VNHx7xxx_DEVICE device);
uint8_t setSpeedMotor(EV_VNHx7xxx_DEVICE, uint8_t, uint8_t);
uint8_t readPwmState(EV_VNHx7xxx_DEVICE);

#if (MULTISENSE_RESPONSE == 1)


void startAdc(EV_VNHx7xxx_DEVICE);
float startConversion(EV_VNHx7xxx_DEVICE device);
float getCurrent(void);
float getAngularSpeed(float);
float convertAngularSpeed(float);
void clearAll(EV_VNHx7xxx_DEVICE);
float EV_VNHx7xxx_currentSensingTesting(EV_VNHx7xxx_DEVICE device, EV_VNHx7xxx_TYPE_SENSING type,uint8_t pwm);
float currentSensingON(EV_VNHx7xxx_DEVICE device, uint8_t pwm);
AEK_EV_VNH_OFF_STATE currentSensingOFF(EV_VNHx7xxx_DEVICE device, AEK_EV_VNH_SEL0 sel);
float currentIOUT(EV_VNHx7xxx_DEVICE device, float IoutMax, uint8_t pwm);
#endif
#endif

/** @} */

#endif /* EV_VNHx7xxx_H_ */

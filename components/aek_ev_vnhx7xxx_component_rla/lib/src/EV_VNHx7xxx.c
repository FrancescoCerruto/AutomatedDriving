/****************************************************************************
 *
 * Copyright Ãƒâ€šÃ‚Â© 2020 STMicroelectronics - All Rights Reserved
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
 * @file    EV_VNHx7xxx.c
 * @brief   SPC5xx EV-VNHx7xxx driver code.
 *
 * @addtogroup EV-VNHx7xxx
 * @{
 */

#include "EV_VNHx7xxx.h"
#if (AEK_EV_VNH_LIST_ELEMENT!=0)
PWMDriver* PrevpwmStart;
uint8_t INA = 0;
uint8_t INB = 0;
uint8_t SEL0 = 0;
uint8_t ADCstart = 0;
EV_VNHx7xxx_STATUS_DEVICE STATUS_SENSING_VNH = HI_Z_STATUS;
extern float valueVNH;



/*******************************************************************************
 ***************************** Internal Functions ******************************
 *******************************************************************************/

/**
 * @brief   Delays for the specified number of seconds.
 * @note    The specified time is rounded up to a value allowed by the real
 *          system tick clock.
 * @note    The maximum specifiable value is implementation dependent.
 *
 * @param[in] seconds      time in seconds, must be different from zero
 *
 * @api
 */
void delay(uint8_t seconds) {
	osalThreadDelayMilliseconds(seconds * 1000);
}

/**
 * @brief   Set the frequency of the PWM.
 *
 * @param[in] device     Component of the device selected
 *
 * @param[in] pwm        Frequency of the PWM in Hz
 *
 * @api
 */
void EV_VNHx7xxx_PWMSetting(EV_VNHx7xxx_DEVICE device, uint8_t pwm) {
	if (PrevpwmStart!=EV_VNH_ARRAY_PWM[device]) {
		PrevpwmStart=EV_VNH_ARRAY_PWM[device];
		pwm_lld_start(EV_VNH_ARRAY_PWM[device], EV_VNH_ARRAY_PWM_CONFIGURATION[device]);
	}
#if (AEK_VNH_GTM_ACTIVE==0)
	pwm_lld_enable_channel(EV_VNH_ARRAY_PWM[device], PWMCHANNELS[device], PWM_PERCENTAGE_TO_WIDTH(EV_VNH_ARRAY_PWM[device], pwm * 100), 0, 0, 0);
#endif
#if (AEK_VNH_GTM_ACTIVE==1)
	pwm_lld_enable_channel(EV_VNH_ARRAY_PWM[device], PWMCHANNELS[device], PWM_PERCENTAGE_TO_WIDTH(EV_VNH_ARRAY_PWM[device],PWMCHANNELS[device], pwm * 100));
#endif
}

/**
 * @brief   Activate the INA or INB for the VNH7x device
 *
 * @param[in] device     Component of the device selected
 *
 * @param[in] type       INA or INB Input used
 *
 * @param[in] status     ACTIVE or DEACTIVE the Input used
 *
 * @api
 */
void EV_VNHx7xxx_IN(EV_VNHx7xxx_DEVICE device, EV_VNHx7xxx_TYPE_IN type, EV_VNHx7xxx_MOTOR_ST_IN_STATUS status) {
	int* tempInPort = (int*)EV_VNH_ARRAY_IN_TOT_GPIO_PORT[device];
	int* tempInPortNumber = (int*)EV_VNH_ARRAY_IN_TOT_GPIO_PORT_NUMBER[device];
	if (status == IN_ACTIVE) {
		if (type==INATYPE)
			INA=1;
		else
			INB=1;
		pal_lld_setpad(tempInPort[type],tempInPortNumber[type]);
	} else {
		if (type==INBTYPE)
			INA=0;
		else
			INB=0;
		pal_lld_clearpad(tempInPort[type],tempInPortNumber[type]);
	}
}

/**
 * @brief   Activate the INA or INB for the VNH7x device
 *
 * @param[in] device     Component of the device selected
 *
 * @param[in] status     ACTIVE or DEACTIVE the Input used
 *
 * @api
 */
void EV_VNHx7xxx_SEL(EV_VNHx7xxx_DEVICE device, AEK_EV_VNH_SEL0 status) {
	int* tempSelPort = (int*)EV_VNH_ARRAY_SEL_TOT_GPIO_PORT[device];
	int* tempSelPortNumber = (int*)EV_VNH_ARRAY_SEL_TOT_GPIO_PORT_NUMBER[device];
	if (status == ACTIVE) {
		SEL0=1;
		pal_lld_setpad(tempSelPort[0],tempSelPortNumber[0]);
	} else {
		SEL0=0;
		pal_lld_clearpad(tempSelPort[0],tempSelPortNumber[0]);
	}
}

/**
 * @brief   Initialize the motor through GPIO with the EV-VNHx7xxx device.
 *
 * @param[in] device     Driver of the device selected
 *
 * @api
 */
void initMotor(EV_VNHx7xxx_DEVICE device) {
	EV_VNHx7xxx_PWMSetting(device, 0);
	brakeMotor(device);
}

/**
 * @brief   Stand-by motor: SEL0 & PWM & INA & INB =0 EV-VNHx7xxx device.
 *
 * @param[in] device     Driver of the device selected
 *
 * @api
 */
void standByMotor(EV_VNHx7xxx_DEVICE device) {
	EV_VNHx7xxx_PWMSetting(device, 0);
	EV_VNHx7xxx_IN(device, INATYPE, IN_DEACTIVE);
	EV_VNHx7xxx_IN(device, INBTYPE, IN_DEACTIVE);
	EV_VNHx7xxx_SEL(device, DEACTIVE);
#if (MULTISENSE_RESPONSE == 1)
	pal_lld_clearpad(EV_VNH_ARRAY_EN_GPIO_PORT[device], EV_VNH_ARRAY_EN_GPIO_PORT_NUMBER[device]);
#endif
	osalThreadDelayMicroseconds(550);
}

/**
 * @brief   Braking the motor through GPIO with the EV-VNHx7xxx device.
 *
 * @param[in] device     Driver of the device selected
 *
 * @api
 */
void brakeMotor(EV_VNHx7xxx_DEVICE device) {
	uint8_t duty = 10;
	EV_VNHx7xxx_PWMSetting(device, duty);
	EV_VNHx7xxx_IN(device, INATYPE, IN_ACTIVE);
	EV_VNHx7xxx_IN(device, INBTYPE, IN_ACTIVE);
}

/**
 * @brief   Turn-on the motor through GPIO with the EV-VNHx7xxx device.
 *
 * @param[in] device     Driver of the device selected
 *
 * @param[in] direction      Set rotation direction
 *
 * @param[in] pwm			Set PWM
 *
 * @return uint8_t
 * @api
 */
uint8_t setSpeedMotor(EV_VNHx7xxx_DEVICE device, uint8_t direction, uint8_t pwm) {
	uint8_t status = EV_VNHx7xxx_INITIAL_STATUS;
	EV_VNHx7xxx_PWMSetting(device, pwm);
	if (direction == CLOCKWISE) {
		EV_VNHx7xxx_IN(device, INATYPE, IN_DEACTIVE);
		EV_VNHx7xxx_IN(device, INBTYPE, IN_ACTIVE);
		status = EV_VNHx7xxx_COMPLETE_STATUS;
	} else if (direction == COUNTERCLOCKWISE) {
		EV_VNHx7xxx_IN(device, INATYPE, IN_ACTIVE);
		EV_VNHx7xxx_IN(device, INBTYPE, IN_DEACTIVE);
		status = EV_VNHx7xxx_COMPLETE_STATUS;
	} else {
		brakeMotor(device);
	}
	return status;
}

/**
 * @brief   Read the logical state of the PWM through GPIO.
 *
 * @param[in] device     Driver of the device selected
 *
 * @return uint8_t
 * @api
 */
uint8_t readPwmState(EV_VNHx7xxx_DEVICE device) {
	uint8_t state;
	state = pal_lld_readpad(EV_VNH_ARRAY_PWM_GPIO_PORT[device], EV_VNH_ARRAY_PWM_GPIO_PORT_NUMBER[device]);
	return state;
}

#if (MULTISENSE_RESPONSE == 1)
/**
 * @brief   Initialize the ADC
 *
 * @param[in] device     Driver of the device selected
 *
 * @api
 */
void startAdc(EV_VNHx7xxx_DEVICE device) {
	ADCstart=1;
	SARADCDriver* driverUnderTest;
	driverUnderTest = EV_VNH_ARRAY_ADC[device];
	saradc_lld_start(driverUnderTest, EV_VNH_ARRAY_ADC_CONFIGURATION[device]);
}

/**
 * @brief   Start conversion through the ADC
 *
 * @param[in] device     Driver of the device selected
 *
 * @api
 */
float startConversion(EV_VNHx7xxx_DEVICE device) {
	SARADCDriver* driverUnderTest;
	driverUnderTest = EV_VNH_ARRAY_ADC[device];
	saradc_lld_start_conversion(driverUnderTest);
	//osalThreadDelayMilliseconds(50U);
	saradc_lld_stop_conversion(driverUnderTest);
	return valueVNH;
}

/**
 * @brief   Return the current respect to the measured voltage.
 *          Please Note: Before using this function, make sure to set the employed motor parameters in the EV-VNHx7xxx.h file.  
 *
 * @param[in] voltage 	Measured voltage from ADC
 *
 * @return float current
 * @api
 */
float getCurrent() {

	float current;
	current = (valueVNH)/(R_SENSE);

	return current;
}

/**
 * @brief   Return the angular speed respect to the measured current. 
 *			Please Note: Before using this function, make sure to set the employed motor parameters in the EV-VNHx7xxx.h file.
 *
 * @param[in] current 	Measured current from ADC
 *
 * @return uint8_t
 * @api
 */
float getAngularSpeed(float current) {

	float speed;

	float current_out = current * K_FACTOR;

	speed = (ARMATURE_VOLTAGE - ARMATURE_RESISTANCE * current_out) / (ARMATURE_VOLTAGE / NO_LOAD_SPEED);

	return speed;
}

/**
 * @brief   Convert the angular speed from r/min to rad/s.
 *
 * @param[in] speed 	Speed in r/min
 *
 * @return uint8_t
 * @api
 */
float convertAngularSpeed(float speed) {

	float angular_speed = 0;

	angular_speed = (speed)*(3.14/30);

	return angular_speed;
}

/**
 * @brief   Clear all the GPIO of the EV-VNHx7xxx device.
 *
 * @param[in] device     Driver of the device selected
 *
 * @api
 */
void clearAll(EV_VNHx7xxx_DEVICE device) {
	standByMotor(device);
}

/**
 * @brief   Current Monitoring Test High and Low Side.
 *
 * @param[in] device     Driver of the device selected
 *
 * @param[in] type       Testing a specific part of the VNH or VN
 *
 * @param[in] pwm        Frequency of the PWM in Hz
 *
 *
 * @return float current read.
 * @api
 * */
float EV_VNHx7xxx_currentSensingTesting(EV_VNHx7xxx_DEVICE device, EV_VNHx7xxx_TYPE_SENSING type,
		uint8_t pwm) {
	pal_lld_setpad(EV_VNH_ARRAY_EN_GPIO_PORT[device], EV_VNH_ARRAY_EN_GPIO_PORT_NUMBER[device]);
	EV_VNHx7xxx_PWMSetting(device, pwm);
	if (ADCstart==0) {
		startAdc(device);
	}
	startConversion(device);

	switch (type) {
		case HSA:
		EV_VNHx7xxx_IN(device, INATYPE, IN_ACTIVE);
		EV_VNHx7xxx_IN(device, INBTYPE, IN_DEACTIVE);
		EV_VNHx7xxx_SEL(device, ACTIVE);
		if (valueVNH >= 0 || valueVNH < 0.00360) {
			STATUS_SENSING_VNH = HI_Z_STATUS;
			return 0;
		}
		if ((valueVNH >= 5 && EV_VNH_ARRAY_ACDTYPE[device] == 5) || (valueVNH >= 3.3 && EV_VNH_ARRAY_ACDTYPE[device] == 3.3)) {
			STATUS_SENSING_VNH = HSA_PROBLEM;
			return I_OUT_MAX;
		}
		break;
		case HSB:
		EV_VNHx7xxx_IN(device, INATYPE, IN_DEACTIVE);
		EV_VNHx7xxx_IN(device, INBTYPE, IN_ACTIVE);
		EV_VNHx7xxx_SEL(device, DEACTIVE);
		if (valueVNH >= 0 || valueVNH < 0.00360) {
			STATUS_SENSING_VNH = HI_Z_STATUS;
			return 0;
		}if ((valueVNH >= 5 && EV_VNH_ARRAY_ACDTYPE[device] == 5) || (valueVNH >= 3.3 && EV_VNH_ARRAY_ACDTYPE[device] == 3.3)) {
			STATUS_SENSING_VNH = HSB_PROBLEM;
			return I_OUT_MAX;
		}
		break;
		case LSA_OFFMOTOR:
		EV_VNHx7xxx_IN(device, INATYPE, IN_DEACTIVE);
		EV_VNHx7xxx_IN(device, INBTYPE, IN_DEACTIVE);
		EV_VNHx7xxx_SEL(device, DEACTIVE);
		if (pwm != 0) {
			if (valueVNH >= 0 || valueVNH < 0.00360) {
				STATUS_SENSING_VNH = HI_Z_STATUS;
				return 0;
			}
			if ((valueVNH >= 5 && EV_VNH_ARRAY_ACDTYPE[device] == 5) || (valueVNH >= 3.3 && EV_VNH_ARRAY_ACDTYPE[device] == 3.3)) {
				STATUS_SENSING_VNH = LSA_PROBLEM;
				return I_OUT_MAX;
			}
		}
		break;
		case LSB_OFFMOTOR:
		EV_VNHx7xxx_IN(device, INATYPE, IN_DEACTIVE);
		EV_VNHx7xxx_IN(device, INBTYPE, IN_DEACTIVE);
		EV_VNHx7xxx_SEL(device, ACTIVE);
		if (pwm != 0) {
			if (valueVNH >= 0 || valueVNH < 0.00360) {
				STATUS_SENSING_VNH = HI_Z_STATUS;
				return 0;
			}
			if ((valueVNH >= 5 && EV_VNH_ARRAY_ACDTYPE[device] == 5) || (valueVNH >= 3.3 && EV_VNH_ARRAY_ACDTYPE[device] == 3.3)) {
				STATUS_SENSING_VNH = LSB_PROBLEM;
				return I_OUT_MAX;
			}

		}
		break;
		case HSA_OFF:
		EV_VNHx7xxx_IN(device, INATYPE, IN_DEACTIVE);
		EV_VNHx7xxx_IN(device, INBTYPE, IN_ACTIVE);
		EV_VNHx7xxx_SEL(device, ACTIVE);
		if (valueVNH >= 0 || valueVNH < 0.00360) {
			STATUS_SENSING_VNH = HI_Z_STATUS;
			return 0;
		}
		if ((valueVNH >= 5 && EV_VNH_ARRAY_ACDTYPE[device] == 5) || (valueVNH >= 3.3 && EV_VNH_ARRAY_ACDTYPE[device] == 3.3)) {
			STATUS_SENSING_VNH = HSA_PROBLEM;
			return I_OUT_MAX;
		}
		break;
		case HSB_OFF:
		EV_VNHx7xxx_IN(device, INATYPE, IN_ACTIVE);
		EV_VNHx7xxx_IN(device, INBTYPE, IN_DEACTIVE);
		EV_VNHx7xxx_SEL(device, DEACTIVE);
		if (valueVNH >= 0 || valueVNH < 0.00360) {
			STATUS_SENSING_VNH = HI_Z_STATUS;
			return 0;
		}
		if ((valueVNH >= 5 && EV_VNH_ARRAY_ACDTYPE[device] == 5) || (valueVNH >= 3.3 && EV_VNH_ARRAY_ACDTYPE[device] == 3.3)) {
			STATUS_SENSING_VNH = HSB_PROBLEM;
			return I_OUT_MAX;
		}
		break;
		case HSA_BREAK:
		brakeMotor(device);
		EV_VNHx7xxx_SEL(device, ACTIVE);
		if (((valueVNH >= 5 && EV_VNH_ARRAY_ACDTYPE[device] == 5) || (valueVNH >= 3.3 && EV_VNH_ARRAY_ACDTYPE[device] == 3.3)) && pwm > 0) {
			STATUS_SENSING_VNH = HSA_PROBLEM;
			return I_OUT_MAX;
		}
		break;
		case HSB_BREAK:
		brakeMotor(device);
		EV_VNHx7xxx_SEL(device, DEACTIVE);
		if (((valueVNH >= 5 && EV_VNH_ARRAY_ACDTYPE[device] == 5) || (valueVNH >= 3.3 && EV_VNH_ARRAY_ACDTYPE[device] == 3.3)) && pwm == 0) {
			STATUS_SENSING_VNH = HSB_PROBLEM;
			return I_OUT_MAX;
		}
		break;
	}
	return getCurrent();
}

/**
 * @brief   Current Monitoring HSA/B.
 *
 * @param[in] device     Driver of the device selected
 *
 * @param[in] pwm	     Set PWM
 *
 * @return float current read.
 * @api
 */
float currentSensingON(EV_VNHx7xxx_DEVICE device, uint8_t pwm) {
	pal_lld_setpad(EV_VNH_ARRAY_EN_GPIO_PORT[device], EV_VNH_ARRAY_EN_GPIO_PORT_NUMBER[device]);
	if (ADCstart==0) {
		startAdc(device);
	}
	startConversion(device);
	if (INA == 1 && INB == 0 && SEL0 == 0) {
		if (valueVNH >= 0 || valueVNH < 0.00360) {
			STATUS_SENSING_VNH = HI_Z_STATUS;
			return 0;
		}
		if ((valueVNH >= 5 && EV_VNH_ARRAY_ACDTYPE[device] == 5) || (valueVNH >= 3.3 && EV_VNH_ARRAY_ACDTYPE[device] == 3.3)) {
			STATUS_SENSING_VNH = HSA_PROBLEM;
			return I_OUT_MAX;
		}
	}
	if (INA == 0 && INB == 1 && SEL0 == 0) {
		if (valueVNH >= 0 || valueVNH < 0.00360) {
			STATUS_SENSING_VNH = HI_Z_STATUS;
			return 0;
		}
		if ((valueVNH >= 5 && EV_VNH_ARRAY_ACDTYPE[device] == 5) || (valueVNH >= 3.3 && EV_VNH_ARRAY_ACDTYPE[device] == 3.3)) {
			STATUS_SENSING_VNH = HSB_PROBLEM;
			return I_OUT_MAX;
		}
	}
	if (INA == 0 && INB == 0 && SEL0 == 0) {
		if (pwm != 0) {
			if (valueVNH >= 0 || valueVNH < 0.00360) {
				STATUS_SENSING_VNH = HI_Z_STATUS;
				return 0;
			}
			if ((valueVNH >= 5 && EV_VNH_ARRAY_ACDTYPE[device] == 5) || (valueVNH >= 3.3 && EV_VNH_ARRAY_ACDTYPE[device] == 3.3)) {
				STATUS_SENSING_VNH = LSA_PROBLEM;
				return I_OUT_MAX;
			}

		}
	}
	if (INA == 0 && INB == 0 && SEL0 == 1) {
		if (pwm != 0) {
			if (valueVNH >= 0 || valueVNH < 0.00360) {
				STATUS_SENSING_VNH = HI_Z_STATUS;
				return 0;
			}
			if ((valueVNH >= 5 && EV_VNH_ARRAY_ACDTYPE[device] == 5) || (valueVNH >= 3.3 && EV_VNH_ARRAY_ACDTYPE[device] == 3.3)) {
				STATUS_SENSING_VNH = LSB_PROBLEM;
				return I_OUT_MAX;
			}

		}
	}
	if (INA == 0 && INB == 1 && SEL0 == 1) {
		if (valueVNH >= 0 || valueVNH < 0.00360) {
			STATUS_SENSING_VNH = HI_Z_STATUS;
			return 0;
		}
		if ((valueVNH >= 5 && EV_VNH_ARRAY_ACDTYPE[device] == 5) || (valueVNH >= 3.3 && EV_VNH_ARRAY_ACDTYPE[device] == 3.3)) {
			STATUS_SENSING_VNH = HSA_PROBLEM;
			return I_OUT_MAX;
		}
	}
	if (INA == 1 && INB == 0 && SEL0 == 0) {
		if (valueVNH >= 0 || valueVNH < 0.00360) {
			STATUS_SENSING_VNH = HI_Z_STATUS;
			return 0;
		}
		if ((valueVNH >= 5 && EV_VNH_ARRAY_ACDTYPE[device] == 5) || (valueVNH >= 3.3 && EV_VNH_ARRAY_ACDTYPE[device] == 3.3)) {
			STATUS_SENSING_VNH = HSB_PROBLEM;
			return I_OUT_MAX;
		}
	}
	if (INA == 1 && INB == 1 && SEL0 == 1 && pwm > 0) {
		if (((valueVNH >= 5 && EV_VNH_ARRAY_ACDTYPE[device] == 5) || (valueVNH >= 3.3 && EV_VNH_ARRAY_ACDTYPE[device] == 3.3)) && pwm > 0) {
			STATUS_SENSING_VNH = HSA_PROBLEM;
			return I_OUT_MAX;
		}
	}
	if (INA == 1 && INB == 1 && SEL0 == 0 && pwm == 0) {
		if (((valueVNH >= 5 && EV_VNH_ARRAY_ACDTYPE[device] == 5) || (valueVNH >= 3.3 && EV_VNH_ARRAY_ACDTYPE[device] == 3.3)) && pwm == 0) {
			STATUS_SENSING_VNH = HSB_PROBLEM;
			return I_OUT_MAX;
		}
	}
	return getCurrent();
}

/**
 * @brief   Current Monitoring in OFF-STATE.
 *
 * @param[in] device     Driver of the device selected
 *
 * @param[in] sel	     SEL0 drive the multiplexer sensing
 *
 * @return AEK_EV_VNH_OFF_STATE to understand the actuale state.
 * @api
 */
AEK_EV_VNH_OFF_STATE currentSensingOFF(EV_VNHx7xxx_DEVICE device, AEK_EV_VNH_SEL0 sel) {
	standByMotor(device);
	int* tempSelPort = (int*)EV_VNH_ARRAY_SEL_TOT_GPIO_PORT[device];
	int* tempSelPortNumber = (int*)EV_VNH_ARRAY_SEL_TOT_GPIO_PORT_NUMBER[device];
	if (sel==ACTIVE) {
		pal_lld_setpad(tempSelPort[0], tempSelPortNumber[0]);
		pal_lld_setpad(EV_VNH_ARRAY_EN_GPIO_PORT[device], EV_VNH_ARRAY_EN_GPIO_PORT_NUMBER[device]);
	}
	else {
		pal_lld_clearpad(tempSelPort[0], tempSelPortNumber[0]);
		pal_lld_setpad(EV_VNH_ARRAY_EN_GPIO_PORT[device], EV_VNH_ARRAY_EN_GPIO_PORT_NUMBER[device]);

	}
	if (ADCstart==0) {
		startAdc(device);
	}
	startConversion(device);
	if ((valueVNH >= 5 && EV_VNH_ARRAY_ACDTYPE[device] == 5) || (valueVNH >= 3.3 && EV_VNH_ARRAY_ACDTYPE[device] == 3.3)) {
		return SATURATION;
	}
	else {
		return HIZ;
	}
}

/**
 * @brief       Iout Current calculation in mA.
 *
 * @param[in] device     Driver of the device selected
 *
 * @param[in] IoutMax    compatible with the DC motor used in Amper
 *
 * @param[in] pwm	     Set PWM
 *
 * @return Iout calculated.
 * @api
 */
float currentIOUT(EV_VNHx7xxx_DEVICE device, float IoutMax, uint8_t pwm) {
	float k;
	k=(IoutMax)/(I_SENSE_MAX);
	float Isense=currentSensingON(device, pwm);
	float Iout=k*Isense;
	return Iout;
}
#endif
#endif

/****************************************************************************
*
* Copyright © 2020 STMicroelectronics - All Rights Reserved
*
* License terms: STMicroelectronics Proprietary in accordance with licensing
* terms SLA0098 at www.st.com.
*
* THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
* INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
* EVALUATION ONLY – NOT FOR USE IN PRODUCTION
*
*****************************************************************************/

/**
 * @file    vl53l1_platform_user_define.h
 * @brief   SPC5xx vl53l1_platform_user_define.h header file.
 *
 * @addtogroup vl53l1_platform_user_define
 */


#ifndef _VL53L1_PLATFORM_USER_DEFINES_H_
#define _VL53L1_PLATFORM_USER_DEFINES_H_

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @file   vl53l1_platform_user_defines.h
 *
 * @brief  All end user OS/platform/application definitions
 */


/**
 * @def do_division_u
 * @brief customer supplied division operation - 64-bit unsigned
 *
 * @param dividend      unsigned 64-bit numerator
 * @param divisor       unsigned 64-bit denominator
 */
#define do_division_u(dividend, divisor) (dividend / divisor)


/**
 * @def do_division_s
 * @brief customer supplied division operation - 64-bit signed
 *
 * @param dividend      signed 64-bit numerator
 * @param divisor       signed 64-bit denominator
 */
#define do_division_s(dividend, divisor) (dividend / divisor)


/**
 * @def WARN_OVERRIDE_STATUS
 * @brief customer supplied macro to optionally output info when a specific
	  error has been overridden with success within the EwokPlus driver
 *
 * @param __X__      the macro which enabled the suppression
 */
#define WARN_OVERRIDE_STATUS(__X__)\
	trace_print (VL53L1_TRACE_LEVEL_WARNING, #__X__);


#ifdef _MSC_VER
#define DISABLE_WARNINGS() { \
	__pragma (warning (push)); \
	__pragma (warning (disable:4127)); \
	}
#define ENABLE_WARNINGS() { \
	__pragma (warning (pop)); \
	}
#else
	#define DISABLE_WARNINGS()
	#define ENABLE_WARNINGS()
#endif


#ifdef __cplusplus
}
#endif

#endif // _VL53L1_PLATFORM_USER_DEFINES_H_
 /** @} */

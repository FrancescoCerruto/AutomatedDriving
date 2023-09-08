/**
 * @file    AEK_SNS_VL53L1X1.h
 * @brief   SPC5xx AEK_SNS_VL53L1X1 header file.
 *
 * @addtogroup AEK_SNS_VL53L1X1
 * @{
 */
 
#include "AEK_SNS_VL53L1X1_cfg.h"

#ifdef Allocation_Executed
#ifndef _AEK_SNS_VL53L1X1_H_
/** @{ */
#define _AEK_SNS_VL53L1X1_H_

AEK_TOF_ERROR AEK_TOF_CONFIG(void);
#ifdef ULD_enable
#include "uld_include/uld_api.h"
void fillMethodsStructULD(methods_t *AEK_TOF_METHODS);
#endif
#ifdef FULL_enable
#include <full_include/full_api.h>
void fillMethodsStructFULL(methods_t *AEK_TOF_METHODS);
#endif
extern methods_t AEK_TOF_METHODS;
#endif /*_LIB_ULD_API_STRUCTS_H_*/
/** @} */
#endif /*Allocation_Executed*/

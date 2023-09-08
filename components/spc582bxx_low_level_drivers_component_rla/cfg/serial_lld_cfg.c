/****************************************************************************
*
* Copyright © 2017-2019 STMicroelectronics - All Rights Reserved
*
* This software is licensed under SLA0098 terms that can be found in the
* DM00779817_1_0.pdf file in the licenses directory of this software product.
* 
* THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED, 
* INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
*****************************************************************************/
/**
 * @file    serial_lld_cfg.c
 * @brief   SERIAL Driver configuration code.
 *
 * @addtogroup SERIAL
 * @{
 */

#include "serial_lld_cfg.h"

#if (LLD_USE_SERIAL == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   Structure defining the SERIAL configuration "configuration_name API mode : SYNCHRONOUS".
 */

 
SerialConfig serial_config_configuration_name = {
  115200,
  SD_MODE_8BITS_PARITY_NONE,
  SPC5_LIN_API_MODE_SYNCHRONOUS,
  NULL,
  NULL,
  FALSE,
  NULL,
  0,
  NULL 
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

#endif /* LLD_USE_SERIAL */

/** @} */

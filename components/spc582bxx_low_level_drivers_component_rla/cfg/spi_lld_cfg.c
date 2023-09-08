/****************************************************************************
*
* Copyright © 2015-2019 STMicroelectronics - All Rights Reserved
*
* This software is licensed under SLA0098 terms that can be found in the
* DM00779817_1_0.pdf file in the licenses directory of this software product.
* 
* THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
* INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
*****************************************************************************/
/**
 * @file    spi_lld_cfg.c
 * @brief   SPI Driver configuration code.
 *
 * @addtogroup SPI
 * @{
 */

#include "spi_lld_cfg.h"

#if (LLD_USE_SPI == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   Structure defining the SPI configuration "AEK_MOT_SM81M1Board0_SPITP_CS_CS0_0".
 */
SPIConfig spi_config_AEK_MOT_SM81M1Board0_SPITP_CS_CS0_0 = {
  SPI_MASTER,
  NULL,
  PORT_A,
  0,
  0UL | SPC5_CTAR_FMSZ(8) |
        SPC5_CTAR_PCSSCK_PRE3 | SPC5_CTAR_PASC_PRE3 |
        SPC5_CTAR_PDT_PRE1 | SPC5_CTAR_PBR_PRE5 |
        SPC5_CTAR_CSSCK_DIV32 | SPC5_CTAR_ASC_DIV32 |
        SPC5_CTAR_DT_DIV32 | SPC5_CTAR_BR_DIV16,
  0UL | SPC5_PUSHR_CONT | SPC5_PUSHR_CTAS(0) | SPC5_PUSHR_PCS(0),
  XSPI_OFF
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

#endif /* LLD_USE_SPI */

/** @} */

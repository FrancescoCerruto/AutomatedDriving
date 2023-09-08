/****************************************************************************
*
* Copyright © 2017-2022 STMicroelectronics - All Rights Reserved
*
* This software is licensed under SLA0098 terms that can be found in the
* DM00779817_1_0.pdf file in the licenses directory of this software product.
* 
* THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED, 
* INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
*****************************************************************************/

#include "board.h"
#include "clock.h"

/*
 * Initial setup of MSCR_IO registers, the list is terminated
 * by a {-1, 0, 0}.
 */
static spc_mscr_io_init_t spc_mscr_io_init[] = {
  {(int16_t)MSCR_IO_PIN_RXD_1, (iomode_t)(PAL_SPC5_SSS(0) | PAL_SPC5_OERC(0) | PAL_SPC5_ODC(0) | PAL_SPC5_SMC | PAL_SPC5_ILS(0) | PAL_SPC5_IBE | PAL_SPC5_WPUE), PAL_LOW},
  {(int16_t)MSCR_IO_PIN_AEK_EV_VNHx7xxxBoard1_J1_INA_GPIO10, (iomode_t)(PAL_SPC5_SSS(0) | PAL_SPC5_OERC(1) | PAL_SPC5_ODC(2) | PAL_SPC5_SMC | PAL_SPC5_ILS(0) | PAL_SPC5_IBE), PAL_LOW},
  {(int16_t)MSCR_IO_PIN_AEK_EV_VNHx7xxxBoard1_J1_INB_GPIO11, (iomode_t)(PAL_SPC5_SSS(0) | PAL_SPC5_OERC(1) | PAL_SPC5_ODC(2) | PAL_SPC5_SMC | PAL_SPC5_ILS(0) | PAL_SPC5_IBE), PAL_LOW},
  {(int16_t)MSCR_IO_PIN_TXD_1, (iomode_t)(PAL_SPC5_SSS(1) | PAL_SPC5_OERC(0) | PAL_SPC5_ODC(2) | PAL_SPC5_SMC | PAL_SPC5_ILS(0) | PAL_SPC5_IBE), PAL_LOW},
  {(int16_t)MSCR_IO_PIN_AEK_MOT_SM81M1Board0_SPITP_CS0_CS0_0, (iomode_t)(PAL_SPC5_SSS(8) | PAL_SPC5_OERC(1) | PAL_SPC5_ODC(2) | PAL_SPC5_SMC | PAL_SPC5_ILS(0) | PAL_SPC5_IBE), PAL_HIGH},
  {(int16_t)MSCR_IO_PIN_AEK_SNS_VL53L1X1Board0_J1_SCLexSDA_SCL_SCL, (iomode_t)(PAL_SPC5_SSS(3) | PAL_SPC5_OERC(2) | PAL_SPC5_ODC(1) | PAL_SPC5_SMC | PAL_SPC5_ILS(0) | PAL_SPC5_IBE), PAL_LOW},
  {(int16_t)MSCR_IO_PIN_AEK_SNS_VL53L1X1Board0_J1_SCLexSDA_SDA_SDA, (iomode_t)(PAL_SPC5_SSS(3) | PAL_SPC5_OERC(2) | PAL_SPC5_ODC(1) | PAL_SPC5_SMC | PAL_SPC5_ILS(0) | PAL_SPC5_IBE), PAL_LOW},
  {(int16_t)MSCR_IO_PIN_AEK_EV_VNHx7xxxBoard1_J2_SEL0_GPIO9, (iomode_t)(PAL_SPC5_SSS(0) | PAL_SPC5_OERC(1) | PAL_SPC5_ODC(2) | PAL_SPC5_SMC | PAL_SPC5_ILS(0) | PAL_SPC5_IBE), PAL_LOW},
  {(int16_t)MSCR_IO_PIN_AEK_EV_VNHx7xxxBoard1_J2_PWM_UC28, (iomode_t)(PAL_SPC5_SSS(2) | PAL_SPC5_OERC(1) | PAL_SPC5_ODC(2) | PAL_SPC5_SMC | PAL_SPC5_ILS(0) | PAL_SPC5_IBE), PAL_LOW},
  {(int16_t)MSCR_IO_PIN_AEK_SNS_VL53L1X1Board0_J1_IRQ_EIRQ10, (iomode_t)(PAL_SPC5_SSS(0) | PAL_SPC5_IBE), PAL_LOW},
  {(int16_t)MSCR_IO_PIN_AEK_MOT_SM81M1Board0_DIOTP_EN_GPIO8, (iomode_t)(PAL_SPC5_SSS(0) | PAL_SPC5_OERC(1) | PAL_SPC5_ODC(2) | PAL_SPC5_SMC | PAL_SPC5_ILS(0) | PAL_SPC5_IBE), PAL_LOW},
  {(int16_t)MSCR_IO_PIN_AEK_MOT_SM81M1Board0_SPITP_MISO_0, (iomode_t)(PAL_SPC5_SSS(0) | PAL_SPC5_OERC(1) | PAL_SPC5_ODC(0) | PAL_SPC5_SMC | PAL_SPC5_ILS(0) | PAL_SPC5_IBE), PAL_LOW},
  {(int16_t)MSCR_IO_PIN_AEK_EV_VNHx7xxxBoard0_J2_PWM_UC14, (iomode_t)(PAL_SPC5_SSS(2) | PAL_SPC5_OERC(1) | PAL_SPC5_ODC(2) | PAL_SPC5_SMC | PAL_SPC5_ILS(0) | PAL_SPC5_IBE), PAL_LOW},
  {(int16_t)MSCR_IO_PIN_AEK_SNS_VL53L1X1Board0_J1_XSHUT_XSHUT0_GPIO8, (iomode_t)(PAL_SPC5_SSS(0) | PAL_SPC5_OERC(1) | PAL_SPC5_ODC(2) | PAL_SPC5_SMC | PAL_SPC5_ILS(0) | PAL_SPC5_IBE), PAL_LOW},
  {(int16_t)MSCR_IO_PIN_AEK_EV_VNHx7xxxBoard0_J1_INB_GPIO8, (iomode_t)(PAL_SPC5_SSS(0) | PAL_SPC5_OERC(1) | PAL_SPC5_ODC(2) | PAL_SPC5_SMC | PAL_SPC5_ILS(0) | PAL_SPC5_IBE), PAL_LOW},
  {(int16_t)MSCR_IO_PIN_AEK_MOT_SM81M1Board0_SPITP_MOSI_0, (iomode_t)(PAL_SPC5_SSS(1) | PAL_SPC5_OERC(1) | PAL_SPC5_ODC(2) | PAL_SPC5_SMC | PAL_SPC5_ILS(0) | PAL_SPC5_IBE), PAL_LOW},
  {(int16_t)MSCR_IO_PIN_AEK_EV_VNHx7xxxBoard0_J2_SEL0_GPIO1, (iomode_t)(PAL_SPC5_SSS(0) | PAL_SPC5_OERC(1) | PAL_SPC5_ODC(2) | PAL_SPC5_SMC | PAL_SPC5_ILS(0) | PAL_SPC5_IBE), PAL_LOW},
  {(int16_t)MSCR_IO_PIN_AEK_EV_VNHx7xxxBoard0_J1_INA_GPIO2, (iomode_t)(PAL_SPC5_SSS(0) | PAL_SPC5_OERC(1) | PAL_SPC5_ODC(2) | PAL_SPC5_SMC | PAL_SPC5_ILS(0) | PAL_SPC5_IBE), PAL_LOW},
  {(int16_t)MSCR_IO_PIN_AEK_MOT_SM81M1Board0_SPITP_SCK_0, (iomode_t)(PAL_SPC5_SSS(1) | PAL_SPC5_OERC(1) | PAL_SPC5_ODC(2) | PAL_SPC5_SMC | PAL_SPC5_ILS(0) | PAL_SPC5_IBE), PAL_LOW},
  {-1, 0, 0}
};

/*
 * Initial setup of MSCR_MUX registers, the list is terminated
 * by a {-1, 0}.
 */
static spc_mscr_mux_init_t spc_mscr_mux_init[] = {
  {(int16_t)(MSCR_MUX_PIN_RXD_1), 1U},
  {(int16_t)(MSCR_MUX_PIN_AEK_SNS_VL53L1X1Board0_J1_SCLexSDA_SCL_SCL), 3U},
  {(int16_t)(MSCR_MUX_PIN_AEK_SNS_VL53L1X1Board0_J1_SCLexSDA_SDA_SDA), 3U},
  {(int16_t)(MSCR_MUX_PIN_AEK_MOT_SM81M1Board0_SPITP_MISO_0), 2U},
  {-1, 0}
};

/**
 * @brief   PAL setup.
 */
static PALConfig pal_default_config = {
  spc_mscr_io_init,
  spc_mscr_mux_init
};

/*
 * Board-specific initialization code.
 */
void boardInit(void) {

  pal_init(&pal_default_config);
}

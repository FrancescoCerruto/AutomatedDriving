/****************************************************************************
*
* Copyright © 2019 STMicroelectronics - All Rights Reserved
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
 * @file    comm_commands.h
 * @brief   SPC5xx command header file.
 *
 * @addtogroup AEK_MOT_SM81M1
 * @{
 */

#ifndef COMM_COMMANDS_H
#define COMM_COMMANDS_H

/**
 * @name AEK_MOT_SM81M1
 * @brief AEK_MOT_SM81M1 operation code
 * @{
 */
#define SET_SPI_WR(reg)  (reg & 0x3F)
#define SET_SPI_RD(reg)  ((reg & 0x3F) | 0x40)
#define SET_SPI_CLR(reg) ((reg & 0x3F) | 0x80)    
/** @} */

/*******************************************************************************
 * 	Device Register address
*******************************************************************************/    
/**
 * @name AEK_MOT_SM81M1
 * @brief AEK_MOT_SM81M1 register address
 * @{
 */                        
#define GSR               0x01       // Global Status Register
#define MSR               0x02       // Motor abd driver status register
#define GCR1              0x03       // Global configuration register
#define GCR2              0x04       // Global configuration register 2
#define MCR1              0x05       // Control register 1
#define MCR2              0x06       // Control register 2
#define MCR3              0x07       // Control register 3
#define MCREF             0x08       // Current reference register
#define MCVA              0x09       // Coil Voltage 0
#define MCVB              0x0A       // Coil Voltage 90
#define MCVC              0x0B       // Coil Voltage 180
#define MCVD              0x0C       // Coil Voltage 270
#define MCVLLB            0x0D       // Coil Voltage Low Limit B
#define MCVLLA            0x0E       // Coil Voltage Low Limit A
#define MCVUL             0x0F       // Coil Voltage Upper Limit
/** @} */

/*******************************************************************************
 * 	Device driver commands
*******************************************************************************/    
/**
 * @name AEK_MOT_SM81M1
 * @brief AEK_MOT_SM81M1 driver command
 * @{
 */                        
#define B0_L99LD21_DRV        0x41       // L99LD21 device driver commands
#define B1_INIT               0x00       // Driver initialization
#define B1_DEINIT             0x01       // Driver de-initialization
#define B1_SETMODE            0x02       // Set device mode
#define B3_NORMAL             0x00       // Normnal mode
#define B3_LIMPHOME           0x01       // Limp home
#define B3_STANDBY            0x02       // Standby
#define B1_GETMODE            0x03       // Get device mode
#define B1_BOOSTEN            0x04       // Boost enable
#define B3_BOOSTENABLED       0x01       // Enabled
#define B3_BOOSTDISABLED      0x00       // Disables
#define B7_E_OK                0x01       // Command status
#define B7_E_NOT_OK            0x00       // Command status 
/** @} */
/*******************************************************************************
 * 	Direct ST-SPI access
*******************************************************************************/
/**
 * @name AEK_MOT_SM81M1
 * @brief AEK_MOT_SM81M1 ST-SPI access
 * @{
 */                                
#define B0_STSPI_DEV_0        0x10       // Direct ST-SPI access - Device 0
#define B0_STSPI_DEV_1        0x11       // Direct ST-SPI access - Device 1
#define B1_GET_ROM            0xC0       // Get device ROM
#define B0_STSPI_CUSTOM_DEV_0 0x12       // Custom ST-SPI frame - Device 0
#define B0_STSPI_CUSTOM_DEV_1 0x13       // Custom ST-SPI frame - Device 1
/** @} */

/*******************************************************************************
 * 	ROM
*******************************************************************************/
/**
 * @name AEK_MOT_SM81M1
 * @brief AEK_MOT_SM81M1 ROM
 * @{
 */  
#define COMPANY        0x00 
#define FAMILY         0x01 
#define DEVICE_N1      0x02 
#define DEVICE_N2      0x03
#define DEVICE_N3      0x04 
#define DEVICE_N4      0x05 
#define SILICON_VER    0x0A 
#define SPI_MODE       0x10 
#define WD_TYPE1       0x11 
#define WD_BITPOS1     0x13 
#define WD_BITPOS2     0x14 
#define SPI_CPHA_TEST  0x20 
#define GSB_OPT        0x3E 
#define ADV_OPCODE     0x3F
/** @} */

/*******************************************************************************
 * 	MASK RESET REGISTER
*******************************************************************************/
/**
 * @name AEK_MOT_SM81M1
 * @brief AEK_MOT_SM81M1 MASK TO RESET REGISTER
 * @{
 */  
#define ME_CPWBE_MASK_ALL  0xFF7FFF
#define HOLD_MWBE_MASK_ALL 0xFFBFFF
#define AOUT_MASK_ALL      0xFFE7FF
#define V5VE_MASK_ALL      0xFFFBFF
#define MX1_MASK_ALL       0xFFFFDF
#define MX2_MASK_ALL       0xFFFFF7
#define MX3_MASK_ALL       0xFFFFF9
#define DOUT1_MASK_ALL     0xFFF3FF
#define DOUT2_MASK_ALL     0xFFFE7F
#define ASM_MASK_ALL       0xFFC7FF
#define SM_MASK_ALL        0xFFF8FF
#define DIR_MASK_ALL       0xFFFF7F
#define PH_MASK_ALL        0xFFFF81
#define FREQ_MASK_ALL      0xFF3FFF
#define FTOCE_MASK_ALL     0xFFDFFF
#define TBE_MASK_ALL       0xFFEFFF
#define FT_MASK_ALL        0xFFF3FF
#define SR_MASK_ALL        0xFFFCFF
#define DMR_MASK_ALL       0xFFFF3F
#define SDA_MASK_ALL       0xFFFFDF
#define SDB_MASK_ALL       0xFFFFEF
#define OLDLY_MASK_ALL     0xFFFFF7
#define DMH_MASK_ALL       0xFFFFFB
#define D_MODE_MASK_ALL    0xFFC1FF
#define SD_MODE_MASK_ALL   0xFFFE3F
#define AHMSD_MASK_ALL     0xFFFFF7
#define HC_MASK_ALL        0xFF0FFF
#define CA_MASK_ALL        0xFFFFE1
#define MCV_MASK_ALL       0xFFF801
/** @} */

/*******************************************************************************
 * 	MASK TO SET THE FUNCTION IN THE REGISTER
*******************************************************************************/
/**
 * @name AEK_MOT_SM81M1
 * @brief AEK_MOT_SM81M1 FUNCTION GCR1/MCR1
 * @{
 */  
#define ME_CPWBE_ON        0x008000
#define HOLDM_MWBE_ON      0x004000
#define AOUT_BAND          0x001000
#define AOUT_TEMP          0x000800
#define AOUT_DISA          0x001800
#define V5VE_ON            0x000400
#define MX1_ON             0x000020
#define MX2_ON             0x000008
#define MX3_OFF            0x000006
#define MX3_SMODE          0x000002
#define MX3_HOLD           0x000004
/** @} */

/**
 * @name AEK_MOT_SM81M1
 * @brief AEK_MOT_SM81M1 FUNCTION GCR1/MCR1
 * @{
 */  
#define DOUT1_CVRDY        0x000400
#define DOUT1_CVLL         0x000800
#define DOUT1_CVRUN        0x000C00
#define DOUT2_PWM          0x000080
#define DOUT2_ERR          0x000100
#define DOUT2_EC           0x000180
/** @} */

/**
 * @name AEK_MOT_SM81M1
 * @brief AEK_MOT_SM81M1 FUNCTION GCR1/MCR1
 * @{
 */ 
#define ASM_MICROSTEP1_16  0x003800
#define ASM_MICROSTEP1_8   0x000800
#define ASM_MINISTEP       0x001000
#define ASM_HALFSTEP       0x001800
#define ASM_FULLSTEP       0x002000
#define SM_MICROSTEP1_16   0x000700
#define SM_MICROSTEP1_8    0x000100
#define SM_MINISTEP        0x000200
#define SM_HALFSTEP        0x000300
#define SM_FULLSTEP        0x000400
#define DIRECTION_DECRE    0x000080
/** @} */

/**
 * @name AEK_MOT_SM81M1
 * @brief AEK_MOT_SM81M1 FUNCTION MCR2
 * @{
 */ 
#define FREQ_MIDFREQ       0x004000
#define FREQ_HIGHFREQ      0x00C000
#define FTOCE_ON           0x002000
#define TBE_ON             0x001000
#define FT_REGULATIONTWO   0x000400
#define FT_REGULATIONTHRE  0x000800
#define FT_REGULATIONFOUR  0x000C00
#define SR_REGULATIONTWO   0x000100
#define SR_REGULATIONTHRE  0x000200
#define SR_REGULATIONFOUR  0x000300
#define DMR_AUTO_DECAY_2   0x0000C0
#define DMR_SLOW_DECAY     0x000040
#define DMR_MIXED_DECAY    0x000080
#define SDA_ON             0x000020
#define SDB_ON             0x000010
#define OLDLY_ON           0x000008
#define DMH_ON             0x000004
/** @} */

#endif
/** @} */
/*** (c) 2009  STMicroelectronics ****************** END OF FILE ***/

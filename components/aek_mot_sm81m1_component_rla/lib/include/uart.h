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


void UART_RX_Interrupt(char);
void UART_Init (void);


#define ENABLEMOTSM 1
#define DISABLE 0

#define MICRO_CMD            0x01
#define AEK_MOT_SM81M1_CMD         0x10
#define AEK_MOT_SM81M1_CMD_DIRECT  0x20
#define COMM_TEST            0x7F

#define MOTOR_CMD        0x01
#define DRIVER_STATUS    0x02
#define DRIVER_CTRL      0x03
#define STEPPING_PROFILE 0x04

#define MOTOR_STATE    0x01
#define MOTOR_HOLD     0x03
#define MOTOR_DIR      0x04


#define READ_ANALOG         0x03
#define CLEAR_SPI_ERROR_VAR 0x04

#define IC_ENABLE        0x01
#define IC_ENABLE_HIGH   0x01
#define SET_CTRL2_LEVEL  0x02
#define SET_CTRL3_LEVEL  0x03
#define DIRECTION_CTRL   0x04
#define STEP_CTRL        0x05
#define HOLD_CTRL        0x06
#define DOUT1_SELECT     0x07
#define DOUT2_SELECT     0x08
#define STEP_MODE_CTRL   0x09

#define HIGH_LEVEL       0x01
#define LOW_LEVEL        0x00
#define CW_DIR           0x00
#define CCW_DIR          0x01
#define PIN              0x01
#define SPI              0x00

#define DOUT2_OFF        0x00
#define DOUT1_OFF        0x00

#define MICRO64          0x00
#define MICRO32          0x01
#define MICRO16          0x02
#define HALFSTEP         0x03
#define FULLSTEP         0x04

#define ST_SPI_WR        0x00
#define ST_SPI_RD        0x40
#define ST_SPI_CLR       0x80
#define ST_SPI_ROM       0xC0


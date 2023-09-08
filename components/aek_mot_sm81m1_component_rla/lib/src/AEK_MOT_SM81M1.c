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
 * @file    AEK_MOT_SM81M1.c
 * @brief   SPC5xx AEK_MOT_SM81M1 driver code.
 *
 * @addtogroup AEK_MOT_SM81M1
 * @{
 */


#include "AEK_MOT_SM81M1_Driver.h"
#include "Std_Types.h"
#include "uart.h"
#include "Platform_Types.h"
#include "components.h"

/*******************************************************************************
 ******************* macros for generating debugging messages *******************
 *******************************************************************************/
#define ST_SPI_WR        0x00
#define ST_SPI_RD        0x40
#define ST_SPI_CLR       0x80
#define ST_SPI_ROM       0xC0
#define MCR1_START       0xE43E
#define MCR1_POS1        0xA41E
#define MCR1_POS2        0xA43C
#define MCR1_POS3        0xA460
#define MCR1_POS4        0xA47E
#define MCREF_CURR       0xB016
#define ST_SPI_ADDR_MASK 0x3F
#define ST_SPI_DATA_MASK 0xFFFFFF
#define MCR2_FREG	     0x80C0

#define MCR1_ME	         0x8000
#define MCR1_HOLDM       0x4000
#define MCR1_MICRO2      0x2D00
#define MCR1_MICRO3      0x3600
#define MCR1_MICRO4      0x3F00
#define MCR1_MICRO8      0x0900
#define MCR1_MINI	     0x1200
#define MCR1_HALF	     0x1B00
#define MCR1_FULL        0x2400
#define MCR1_DIR         0x80

typedef enum {
	CCW, CW
} DIRECTION;
/*******************************************************************************
 ************************* Internal Constants and Variables *********************
 *******************************************************************************/

/*******************************************************************************
 ********************************** Variables **********************************
 *******************************************************************************/
typedef enum {
	Idle, Init, Running, Hold, Stopped
} Status;
char SPI_read_MSB, SPI_read_LSB;
extern Status Motor_State;
extern char Step_Phase_Number, Step_Mode_New, Step_Mode, Motor_direction;
uint8 GlobalStatusRegister2, SPI_Errors;
volatile unsigned int GCR1_reg, GCR2_reg, MCR1_reg, MCR2_reg, MCR3_reg,
		MCREF_reg, MCVLLB_reg, MCVLLA_reg, MCVUL_reg;
/*******************************************************************************
 ********************************** Functions ***********************************
 *******************************************************************************/
 
/**
 * @brief   Convert Decimal in Binary.
 *
 * @param[in] n       Decimal number
 *
 * @return Binary number
 *
 * @api
 */
 long decimalToBinary(int n) {
	int remainder;
	int binary = 0, i = 1;

	while (n != 0) {
		remainder = n % 2;
		n = n / 2;
		binary = binary + (remainder * i);
		i = i * 10;
	}
	return binary;
}

/**
 * @brief   Convert 24 bit int in an Array with 3 elements.
 *
 * @param[in] in      Int number with 24 bits
 *
 * @param[in] out     Array pointer with 3 elements 
 *
 * @api
 */
void intToArrayMotor(uint32 in, Spi_DataType *out) {
	//out[0] = (in >> 24 )& 0xff;
	out[0] = (in >> 16) & 0xff;
	out[1] = (in >> 8) & 0xff;
	out[2] = (in) & 0xff;
}

/**
 * @brief   Convert 32 bit int in an Array with 3 elements.
 *
 * @param[in] in      Int number with 24 bits
 *
 * @param[in] out     Array pointer with 3 elements 
 *
 * @api
 */
void ArrayToIntMotor(Spi_DataType *in, uint32 *out) {
	*out = in[0];
	*out <<= 8;
	*out |= in[1];
	*out <<= 8;
	*out |= in[2];
}

/**
 * @brief   Check the parity of the string in input.
 *
 * @param[in] in     Stream of 32bits
 *
 * @return the boolean value true if the parity is ok false otherwise
 *
 * @api
 */
boolean checkParityMotor(uint32 in) {
	//in ^= in >> 16;
	in ^= in >> 8;
	in ^= in >> 4;
	in ^= in >> 2;
	in ^= in >> 1;
	if (in & 0x00000001)
		return TRUE;      // Odd parity
	else
		return FALSE;                     // Even parity
}

/**
 * @brief   Add the positive parity in the last bit of the stream.
 *
 * @param[in] in     Stream of 32bits
 *
 * @return Stream of 32bit with positive parity
 *
 * @api
 */
uint32 updateParityBitMotor(uint32 in) {
	if (checkParityMotor(in)) {     // Odd parity - OK (don't change parity bit)
		return (in);
	} else {                      // Even parity - NOK (invert parity bit)
		return (in ^= 0x00000001);
	}
}

#if (AEK_MOT_LIST_ELEMENT != 0)

/**
 * @brief   Read the register through the SPI peripheral.
 *
 * @param[in] driver         SPI driver pointer
 *
 * @param[in] dev            Device component number
 *
 * @param[in] regAddress     Address of the register
 *
 * @param[in] DataReceived   Data received pointer
 *
 * @api
 */
void AEK_MOT_SM81M1drv_ReadSPIRegister(SPIDriver* driver,AEK_MOT_SM81M1_DEVICE dev , unsigned char regAddress) {
	Spi_DataType rawData[3];
	uint32 tmpData_A;
	uint32 tmpData;
	uint8_t rxbuf[3];

	tmpData = updateParityBitMotor((uint32) (regAddress | ST_SPI_RD) << 16);
	intToArrayMotor(tmpData, rawData);

	spi_lld_start(driver, AEK_MOT_SM81M1_ARRAY_CONFIGURATION[dev]);
	spi_lld_exchange(driver, 3, rawData, rxbuf);
	spi_lld_stop(driver);

	tmpData_A = MCR1_START;
	tmpData = updateParityBitMotor(
			(uint32) (((regAddress & ST_SPI_ADDR_MASK) | ST_SPI_WR) << 16)
					| (tmpData_A & ST_SPI_DATA_MASK));
	intToArrayMotor(tmpData, rawData);
	spi_lld_start(driver, AEK_MOT_SM81M1_ARRAY_CONFIGURATION[dev]);
	spi_lld_exchange(driver, 3, rawData, rxbuf);
	spi_lld_stop(driver);

	tmpData_A = MCREF_CURR;
	tmpData = updateParityBitMotor(
			(uint32) (((0x08 & ST_SPI_ADDR_MASK) | ST_SPI_WR) << 16)
					| (tmpData_A & ST_SPI_DATA_MASK));
	intToArrayMotor(tmpData, rawData);
	spi_lld_start(driver, AEK_MOT_SM81M1_ARRAY_CONFIGURATION[dev]);
	spi_lld_exchange(driver, 3, rawData, rxbuf);
	spi_lld_stop(driver);

}

/**
 * @brief   Write the register through the SPI peripheral.
 *
 * @param[in] driver         SPI driver pointer
 *
 * @param[in] dev            Device component number
 *
 * @param[in] regAddress     Address of the register
 *
 * @param[in] DataReceived   Data received pointer
 *
 * @param[in] mode           Function should be write in the register
 *
 * @api
 */
void AEK_MOT_SM81M1drv_WriteSPIRegister(SPIDriver* driver, AEK_MOT_SM81M1_DEVICE dev, unsigned char regAddress,int mode) {
	Spi_DataType rawData[3];
	uint32 tmpData_A;
	uint32 tmpData;
	uint8_t rxbuf[3];

	if (mode == 1)
		tmpData_A = MCR1_START;
	if (mode == 2)
		tmpData_A = 0x00;
	if (mode == 3)
		tmpData_A = MCR2_FREG;
	if (mode == 4)
		tmpData_A = MCREF_CURR;
	if (mode == 5)
		tmpData_A = MCR1_POS3;
	if (mode == 6)
		tmpData_A = MCR1_POS4;

	tmpData = updateParityBitMotor(
			(uint32) (((regAddress & ST_SPI_ADDR_MASK) | ST_SPI_WR) << 16)
					| (tmpData_A & ST_SPI_DATA_MASK));
	intToArrayMotor(tmpData, rawData);
	spi_lld_start(driver, AEK_MOT_SM81M1_ARRAY_CONFIGURATION[dev]);
	spi_lld_exchange(driver, 3, rawData, rxbuf);
	spi_lld_stop(driver);

}

/**
 * @brief   Write the register to set the step resolution.
 *
 * @param[in] driver         SPI driver pointer
 *
 * @param[in] dev            Device component number
 *
 * @param[in] regAddress     Address of the register
 *
 * @param[in] DataReceived   Data received pointer
 *
 * @param[in] mode           Step resolution
 *
 * @param[in] pos            Step positioning
 *
 * @param[in] dir            Step direction
 *
 * @api
 */
void AEK_MOT_SM81M1drv_WriteSPIRegisterMOV(SPIDriver* driver, AEK_MOT_SM81M1_DEVICE dev,
		unsigned char regAddress, STEP_MODE mode,
		long int pos, int dir) {
	Spi_DataType rawData[3];
	uint24 tmpData_A = 0x00;
	uint32 tmpData;
	uint8_t rxbuf[3];

	//write register
	tmpData_A |= MCR1_ME;
	switch (mode) {
	case HALF:
		tmpData_A |= MCR1_HALF;
		break;
	case FULL:
		tmpData_A |= MCR1_FULL;
		break;
	case MINI:
		tmpData_A |= MCR1_MINI;
		break;
	case MICRO2:
		tmpData_A |= MCR1_MICRO2;
		break;
	case MICRO3:
		tmpData_A |= MCR1_MICRO3;
		break;
	case MICRO4:
		tmpData_A |= MCR1_MICRO4;
		break;
	case MICRO8:
		tmpData_A |= MCR1_MICRO8;
		break;
	}
	if (dir == 1)
		tmpData_A |= MCR1_DIR;
	//Position
	tmpData_A |= pos << 1;

	tmpData = updateParityBitMotor(
			(uint32) (((regAddress & ST_SPI_ADDR_MASK) | ST_SPI_WR) << 16)
					| (tmpData_A & ST_SPI_DATA_MASK));
	intToArrayMotor(tmpData, rawData);
	spi_lld_start(driver, AEK_MOT_SM81M1_ARRAY_CONFIGURATION[dev]);
	spi_lld_exchange(driver, 3, rawData, rxbuf);
	spi_lld_stop(driver);

}

/**
 * @brief   Read device info stored in the ROM.
 *
 * @param[in] driver         SPI driver pointer
 *
 * @param[in] dev            Device component number
 *
 * @param[in] regAddress     Address of the register
 *
 * @param[in] DataReceived   Data received pointer
 *
 * @api
 */
void AEK_MOT_SM81M1drv_Read_Device_Info(SPIDriver* driver, AEK_MOT_SM81M1_DEVICE dev, unsigned char regAddress) {
	Spi_DataType rawData[3];
	uint32 tmpData;
	uint8_t rxbuf[3];

	tmpData = updateParityBitMotor((uint32) (regAddress | ST_SPI_ROM) << 16);
	intToArrayMotor(tmpData, rawData);

	spi_lld_start(driver, AEK_MOT_SM81M1_ARRAY_CONFIGURATION[dev]);
	spi_lld_exchange(driver, 3, rawData, rxbuf);
	spi_lld_stop(driver);

	GlobalStatusRegister2 = rawData[0];
	if (GlobalStatusRegister2 & 0x20)
		SPI_Errors++;
}

/**
 * @brief   Clear the valued changed in the registers.
 *
 * @param[in] driver         SPI driver pointer
 *
 * @param[in] dev            Device component number
 *
 * @param[in] regAddress     Address of the register
 *
 * @param[in] DataReceived   Data received pointer
 *
 * @api
 */
void AEK_MOT_SM81M1drv_ClearStausRegister(SPIDriver* driver, AEK_MOT_SM81M1_DEVICE dev, unsigned char regAddress) /*u8 *DataReceived)*/
{
	Spi_DataType rawData[3];
	uint32 tmpData;
	uint8_t rxbuf[3];

	tmpData = updateParityBitMotor((uint32) (regAddress | ST_SPI_CLR) << 16);
	intToArrayMotor(tmpData, rawData);

	spi_lld_start(driver, AEK_MOT_SM81M1_ARRAY_CONFIGURATION[dev]);
	spi_lld_exchange(driver, 3, rawData, rxbuf);
	spi_lld_stop(driver);

	GlobalStatusRegister2 = rawData[0];
	if (GlobalStatusRegister2 & 0x20)
		SPI_Errors++;
}

/**
 * @brief   Global status of the device.
 *
 * @return  The 8 bit represented the errors  
 *
 * @api
 */
uint8 AEK_MOT_SM81M1drv_GetGlobalStatus(void) {
	return GlobalStatusRegister2;
}

/**
 * @brief   Read the message stored in the MotorStatus MSR(0x02).
 *
 * @param[in] driver         SPI driver pointer
 *
 * @param[in] dev            Device component number
 *
 * @param[in] regAddress     Address of the register
 *
 * @param[in] DataReceived   Data received pointer
 *
 * @api
 */
void AEK_MOT_SM81M1drv_MotorStatus(SPIDriver* driver, AEK_MOT_SM81M1_DEVICE dev, unsigned char regAddress) {

	AEK_MOT_SM81M1drv_ReadMessage(driver,  dev, regAddress);
}

/**
 * @brief   Read the message stored in the generic register.
 *
 * @param[in] driver         SPI driver pointer
 *
 * @param[in] dev            Device component number
 *
 * @param[in] regAddress     Address of the register
 *
 * @param[in] DataReceived   Data received pointer
 *
 * @api
 */
void AEK_MOT_SM81M1drv_ReadMessage(SPIDriver* driver, AEK_MOT_SM81M1_DEVICE dev, unsigned char regAddress) {
	Spi_DataType rawData[3];
	uint8_t rxbuf[3];
	uint24 tmpData;

	tmpData = updateParityBitMotor((uint24) (regAddress | ST_SPI_RD) << 16);
	intToArrayMotor(tmpData, rawData);

	spi_lld_start(driver, AEK_MOT_SM81M1_ARRAY_CONFIGURATION[dev]);
	spi_lld_exchange(driver, 3, rawData, rxbuf);
	spi_lld_stop(driver);
}

/**
 * @brief   Write the message stored in the generic register.
 *
 * @param[in] driver         SPI driver pointer
 *
 * @param[in] dev            Device component number
 *
 * @param[in] regAddress     Address of the register
 *
 * @param[in] tmpData_A      Temp Stream of 24bits
 *
 * @param[in] DataReceived   Data received pointer
 *
 * @api
 */
void AEK_MOT_SM81M1drv_writeMessage(SPIDriver* driver, AEK_MOT_SM81M1_DEVICE dev, unsigned char regAddress,
		uint24 tmpData_A, uint24* DataReceived) {

	Spi_DataType rawData[3];
	uint24 tmpData;

	tmpData = updateParityBitMotor(
			(uint32) (((regAddress & ST_SPI_ADDR_MASK) | ST_SPI_WR) << 16)
					| (tmpData_A & ST_SPI_DATA_MASK));
	intToArrayMotor(tmpData, rawData);
	spi_lld_start(driver, AEK_MOT_SM81M1_ARRAY_CONFIGURATION[dev]);
	spi_lld_exchange(driver, 3, rawData, DataReceived);
	spi_lld_stop(driver);
}

/**
 * @brief   Define the error describe in the Global Status Register GCR1(0x03).
 *
 * @param[in] driver         SPI driver pointer
 *
 * @param[in] dev            Device component number
 *
 * @param[in] regAddress     Address of the register
 *
 * @param[in] DataReceived   Data received pointer
 *
 * @api
 */
void AEK_MOT_SM81M1drv_GlobalStatusRegister(SPIDriver* driver, AEK_MOT_SM81M1_DEVICE dev,
		unsigned char regAddress) {
	Spi_DataType rawData[3];
	uint32 tmpData;
	uint8_t rxbuf[3];
	//CODE_ERROR errorType;

	tmpData = updateParityBitMotor((uint32) (regAddress | ST_SPI_CLR) << 16);
	intToArrayMotor(tmpData, rawData);

	spi_lld_start(driver, AEK_MOT_SM81M1_ARRAY_CONFIGURATION[dev]);
	spi_lld_exchange(driver, 3, rawData, rxbuf);
	spi_lld_stop(driver);
	//ArrayToIntMotor(DataReceived, rxbuf);
	/*if (DataReceived[0] != 0)
		errorType = VSOV;
	if (DataReceived[1] != 0)
		errorType = VSUV;
	if (DataReceived[2] != 0)
		errorType = VREGOV;
	if (DataReceived[3] != 0)
		errorType = VREGUV;
	if (DataReceived[5] != 0)
		errorType = CPFAIL;
	if (DataReceived[6] != 0)
		errorType = V5UVW;
	if (DataReceived[7] != 0)
		errorType = V5VOV;
	if (DataReceived[8] != 0)
		errorType = V5VUV;
	if (DataReceived[9] != 0)
		errorType = TW;
	if (DataReceived[10] != 0)
		errorType = TSD;
	if (DataReceived[11] != 0)
		errorType = OL;
	if (DataReceived[12] != 0)
		errorType = OC;
	if (DataReceived[13] != 0)
		errorType = SDF;*/
}

/**
 * @brief   Read function in the Register Global Status Register GCR1(0x03).
 *
 * @param[in] driver         SPI driver pointer
 *
 * @param[in] dev            Device component number
 *
 * @param[in] regAddress     Address of the register
 *
 * @param[in] DataReceived   Data received pointer
 *
 * @param[in] operation      Operation define in GCR1 register
 *
 * @param[in] dinmode        Value to set the relative function define in GCR1 register
 *
 * @api
 */
void AEK_MOT_SM81M1drv_GCR1(SPIDriver* driver, AEK_MOT_SM81M1_DEVICE dev, uint8 regAddress, uint24* DataReceived,
		AEK_MOT_SM81M1_GCR1 operation, OPTION_VALUE_GCR1 dinmode) {

	uint24 tmpData_A = 0x00;
	AEK_MOT_SM81M1drv_ReadMessage(driver, dev, regAddress);

	switch (operation) {
	case CPWBE:
		tmpData_A &= ME_CPWBE_MASK_ALL;
		if (dinmode == ON)
			tmpData_A |= ME_CPWBE_ON;
		break;
	case MWBE:
		tmpData_A &= HOLD_MWBE_MASK_ALL;
		if (dinmode == ON)
			tmpData_A |= HOLDM_MWBE_ON;
		break;
	case AOUT:
		tmpData_A &= AOUT_MASK_ALL;
		if (dinmode == DISABLED)
			tmpData_A |= AOUT_DISA;
		if (dinmode == VOLTAGE_PROTECTION)
			tmpData_A |= AOUT_TEMP;
		if (dinmode == BAND_GAP)
			tmpData_A |= AOUT_BAND;
		break;
	case V5VE:
		tmpData_A &= V5VE_MASK_ALL;
		if (dinmode == ON)
			tmpData_A |= V5VE_ON;
		break;
	case MX1:
		tmpData_A &= MX1_MASK_ALL;
		if (dinmode == ON)
			tmpData_A |= MX1_ON;
		break;
	case MX2:
		tmpData_A &= MX2_MASK_ALL;
		if (dinmode == ON)
			tmpData_A |= MX2_ON;
		break;
	case MX3:
		tmpData_A &= MX3_MASK_ALL;
		if (dinmode == OFF)
			tmpData_A |= MX3_OFF;
		if (dinmode == SMODE)
			tmpData_A |= MX3_SMODE;
		if (dinmode == HOLD)
			tmpData_A |= MX3_HOLD;
		break;
	}
	AEK_MOT_SM81M1drv_writeMessage(driver, dev, regAddress, tmpData_A, DataReceived);
}

/**
 * @brief   Write/Read function in the Register Global Status Register GCR2(0x04).
 *
 * @param[in] driver         SPI driver pointer
 *
 * @param[in] dev            Device component number
 *
 * @param[in] regAddress     Address of the register
 *
 * @param[in] DataReceived   Data received pointer
 *
 * @param[in] operation      Operation define in GCR2 register
 *
 * @param[in] dinmode        Value to set the relative function define in GCR2 register
 *
 * @api
 */
void AEK_MOT_SM81M1drv_GCR2(SPIDriver* driver, AEK_MOT_SM81M1_DEVICE dev, uint8 regAddress, uint24* DataReceived,
		AEK_MOT_SM81M1_GCR2 operation, OPTION_VALUE_GCR2 dinmode) {

	uint24 tmpData_A = 0x00;
	AEK_MOT_SM81M1drv_ReadMessage(driver, dev, regAddress);

	switch (operation) {
	case DOUT1:
		tmpData_A &= DOUT1_MASK_ALL;
		if (dinmode == CVRDY)
			tmpData_A |= DOUT1_CVRDY;
		if (dinmode == CVLL)
			tmpData_A |= DOUT1_CVLL;
		if (dinmode == CVRUN)
			tmpData_A |= DOUT1_CVRUN;
		break;
	case DOUT2:
		tmpData_A &= DOUT2_MASK_ALL;
		if (dinmode == PWM)
			tmpData_A |= DOUT2_PWM;
		if (dinmode == ERR)
			tmpData_A |= DOUT2_ERR;
		if (dinmode == EC)
			tmpData_A |= DOUT2_EC;
		break;
	}
	AEK_MOT_SM81M1drv_writeMessage(driver, dev, regAddress, tmpData_A, DataReceived);
}

/**
 * @brief   Convert grade angle in binary value compatible for the PH bits
 *
 * @param[in] driver         SPI driver pointer
 *
 * @param[in] dev            Device component number
 *
 * @param[in] regAddress     Address of the register
 *
 * @param[in] DataReceived   Data received pointer
 *
 * @param[in] grade          Grade value
 *
 * @api
 */
void AEK_MOT_SM81M1drv_Rotation(SPIDriver* driver, AEK_MOT_SM81M1_DEVICE dev, uint8 regAddress,
		uint24* DataReceived, int grade) {
	uint24 tmpData_A = 0x00;
	grade = grade / 11.61;
	grade = grade << 1;

	AEK_MOT_SM81M1drv_ReadMessage(driver, dev, regAddress);
	tmpData_A &= PH_MASK_ALL;
	tmpData_A |= grade;
	AEK_MOT_SM81M1drv_writeMessage(driver, dev, regAddress, tmpData_A, DataReceived);
}

/**
 * @brief   Write/Read function in the Motor Control Register MCR1(0x05).
 *
 * @param[in] driver         SPI driver pointer
 *
 * @param[in] dev            Device component number
 *
 * @param[in] regAddress     Address of the register
 *
 * @param[in] DataReceived   Data received pointer
 *
 * @param[in] operation      Operation define in MCR1 register
 *
 * @param[in] dinmode        Value to set the relative function define in MCR1 register
 *
 * @api
 */
void AEK_MOT_SM81M1drv_MCR1(SPIDriver* driver, AEK_MOT_SM81M1_DEVICE dev, uint8 regAddress, uint24* DataReceived,
		AEK_MOT_SM81M1_MCR1 operation, OPTION_VALUE_MCR1 dinmode) {

	uint24 tmpData_A = 0x00;
	AEK_MOT_SM81M1drv_ReadMessage(driver, dev, regAddress);

	switch (operation) {
	case ME:
		tmpData_A &= ME_CPWBE_MASK_ALL;
		if (dinmode == ONMCR1)
			tmpData_A |= ME_CPWBE_ON;
		break;
	case HOLDM:
		tmpData_A &= HOLD_MWBE_MASK_ALL;
		if (dinmode == ONMCR1)
			tmpData_A |= HOLDM_MWBE_ON;
		break;
	case ASM:
		tmpData_A &= ASM_MASK_ALL;
		if (dinmode == MICROSTEP1_16)
			tmpData_A |= ASM_MICROSTEP1_16;
		if (dinmode == MICROSTEP1_8)
			tmpData_A |= ASM_MICROSTEP1_8;
		if (dinmode == MINISTEP_M)
			tmpData_A |= ASM_MINISTEP;
		if (dinmode == HALFSTEP_M)
			tmpData_A |= ASM_HALFSTEP;
		if (dinmode == FULLSTEP_M)
			tmpData_A |= ASM_FULLSTEP;
		break;
	case SM:
		tmpData_A &= SM_MASK_ALL;
		if (dinmode == MICROSTEP1_16)
			tmpData_A |= SM_MICROSTEP1_16;
		if (dinmode == MICROSTEP1_8)
			tmpData_A |= SM_MICROSTEP1_8;
		if (dinmode == MINISTEP_M)
			tmpData_A |= SM_MINISTEP;
		if (dinmode == HALFSTEP_M)
			tmpData_A |= SM_HALFSTEP;
		if (dinmode == FULLSTEP_M)
			tmpData_A |= SM_FULLSTEP;
		break;
	case DIR:
		tmpData_A &= DIR_MASK_ALL;
		if (dinmode == DECREMENTAL)
			tmpData_A |= DIRECTION_DECRE;
		break;
	}
	AEK_MOT_SM81M1drv_writeMessage(driver, dev, regAddress, tmpData_A, DataReceived);
}

/**
 * @brief   Write/Read function in the Motor Control Register MCR2(0x06).
 *
 * @param[in] driver         SPI driver pointer
 *
 * @param[in] dev            Device component number
 *
 * @param[in] regAddress     Address of the register
 *
 * @param[in] DataReceived   Data received pointer
 *
 * @param[in] operation      Operation define in MCR2 register
 *
 * @param[in] dinmode        Value to set the relative function define in MCR2 register
 *
 * @api
 */
void AEK_MOT_SM81M1drv_MCR2(SPIDriver* driver, AEK_MOT_SM81M1_DEVICE dev, uint8 regAddress, uint24* DataReceived,
		AEK_MOT_SM81M1_MCR2 operation, OPTION_VALUE_MCR2 dinmode) {

	uint24 tmpData_A = 0x00;
	AEK_MOT_SM81M1drv_ReadMessage(driver, dev, regAddress);

	switch (operation) {
	case FREQ:
		tmpData_A &= FREQ_MASK_ALL;
		if (dinmode == MIDFREQ)
			tmpData_A |= FREQ_MIDFREQ;
		if (dinmode == HIGHFREQ)
			tmpData_A |= FREQ_HIGHFREQ;
		break;
	case FTOCE:
		tmpData_A &= FTOCE_MASK_ALL;
		if (dinmode == ONMCR2)
			tmpData_A |= FTOCE_ON;
		break;
	case TBE:
		tmpData_A &= TBE_MASK_ALL;
		if (dinmode == ONMCR2)
			tmpData_A |= TBE_ON;
		break;
	case FT:
		tmpData_A &= FT_MASK_ALL;
		if (dinmode == REGULATIONTWO)
			tmpData_A |= FT_REGULATIONTWO;
		if (dinmode == REGULATIONTHREE)
			tmpData_A |= FT_REGULATIONTHRE;
		if (dinmode == REGULATIONFOUR)
			tmpData_A |= FT_REGULATIONFOUR;
		break;
	case SR:
		tmpData_A &= SR_MASK_ALL;
		if (dinmode == REGULATIONTWO)
			tmpData_A |= SR_REGULATIONTWO;
		if (dinmode == REGULATIONTHREE)
			tmpData_A |= SR_REGULATIONTHRE;
		if (dinmode == REGULATIONFOUR)
			tmpData_A |= SR_REGULATIONFOUR;
		break;
	case DMR:
		tmpData_A &= DMR_MASK_ALL;
		if (dinmode == AUTO_DECAY_MODE2)
			tmpData_A |= DMR_AUTO_DECAY_2;
		if (dinmode == SLOW_DECAY)
			tmpData_A |= DMR_SLOW_DECAY;
		if (dinmode == MIX_DECAY)
			tmpData_A |= DMR_MIXED_DECAY;
		break;
	case SDAFW:
		tmpData_A &= SDA_MASK_ALL;
		if (dinmode == ONMCR2)
			tmpData_A |= SDA_ON;
		break;
	case SDBFW:
		tmpData_A &= SDB_MASK_ALL;
		if (dinmode == ONMCR2)
			tmpData_A |= SDB_ON;
		break;
	case ONDLY:
		tmpData_A &= OLDLY_MASK_ALL;
		if (dinmode == ONMCR2)
			tmpData_A |= OLDLY_ON;
		break;
	case DMH:
		tmpData_A &= DMH_MASK_ALL;
		if (dinmode == ONMCR2)
			tmpData_A |= DMH_ON;
		break;
	}
	AEK_MOT_SM81M1drv_writeMessage(driver, dev, regAddress, tmpData_A, DataReceived);
}

/**
 * @brief   Write/Read function in the Motor Control Register MCR3(0x07).
 *
 * @param[in] driver         SPI driver pointer
 *
 * @param[in] dev            Device component number
 *
 * @param[in] regAddress     Address of the register
 *
 * @param[in] DataReceived   Data received pointer
 *
 * @param[in] operation      Operation define in MCR3 register
 *
 * @param[in] dinmode        Value to set the relative function define in MCR3 register
 *
 * @api
 */
void AEK_MOT_SM81M1drv_MCR3(SPIDriver* driver, AEK_MOT_SM81M1_DEVICE dev, uint8 regAddress, uint24* DataReceived,
		AEK_MOT_SM81M1_MCR3 operation, OPTION_VALUE_MCR1 dinmode) {

	uint24 tmpData_A = 0x00;
	AEK_MOT_SM81M1drv_ReadMessage(driver, dev, regAddress);

	switch (operation) {
	case CVE:
		tmpData_A &= ME_CPWBE_MASK_ALL;
		if (dinmode == ONMCR1)
			tmpData_A |= ME_CPWBE_ON;
		break;
	case AHMSD:
		tmpData_A &= AHMSD_MASK_ALL;
		if (dinmode == ONMCR1)
			tmpData_A |= OLDLY_ON;
		break;
	}
	AEK_MOT_SM81M1drv_writeMessage(driver, dev, regAddress, tmpData_A, DataReceived);
}

/**
 * @brief   Write/Read function in the Motor Control Register MCV.
 *
 * @param[in] driver         SPI driver pointer
 *
 * @param[in] dev            Device component number
 *
 * @param[in] regAddress     Address of the register
 *
 * @param[in] DataReceived   Data received pointer
 *
 * @param[in] voltage        Coil voltage
 *
 * @api
 */
void AEK_MOT_SM81M1drv_MCV(SPIDriver* driver, AEK_MOT_SM81M1_DEVICE dev, uint8 regAddress,
		uint24* DataReceived, int voltage) {
	uint24 tmpData_A = 0x00;

	AEK_MOT_SM81M1drv_ReadMessage(driver, dev, regAddress);
	tmpData_A &= MCV_MASK_ALL;

	voltage = voltage << 1;

	tmpData_A |= voltage;
	AEK_MOT_SM81M1drv_writeMessage(driver, dev, regAddress, tmpData_A, DataReceived);
}

/**
 * @brief   Set the value for the Coil Voltage Timing.
 *
 * @param[in] driver         SPI driver pointer
 *
 * @param[in] dev            Device component number
 *
 * @param[in] regAddress     Address of the register
 *
 * @param[in] DataReceived   Data received pointer
 *
 * @param[in] period         Represented the time expressed in PWM
 *
 * @api
 */
void AEK_MOT_SM81M1drv_VOLTAGE_TIMING(SPIDriver* driver, AEK_MOT_SM81M1_DEVICE dev, uint8 regAddress,
		uint24* DataReceived, int period) {
	uint24 tmpData_A = 0x00;

	AEK_MOT_SM81M1drv_ReadMessage(driver, dev, regAddress);
	tmpData_A &= D_MODE_MASK_ALL;

	period = period << 9;

	tmpData_A |= period;
	AEK_MOT_SM81M1drv_writeMessage(driver, dev, regAddress, tmpData_A, DataReceived);
}

/**
 * @brief   Set the value for the Stall the Detection
 *
 * @param[in] driver         SPI driver pointer
 *
 * @param[in] dev            Device component number
 *
 * @param[in] regAddress     Address of the register
 *
 * @param[in] DataReceived   Data received pointer
 *
 * @param[in] bits           Represent the number of consecutive times the sampled coils voltage value has to be out of the range
 *
 * @api
 */
void AEK_MOT_SM81M1drv_STALL_DETECTION(SPIDriver* driver, AEK_MOT_SM81M1_DEVICE dev, uint8 regAddress,
		uint24* DataReceived, int bits) {
	uint24 tmpData_A = 0x00;

	AEK_MOT_SM81M1drv_ReadMessage(driver, dev, regAddress);
	tmpData_A &= SD_MODE_MASK_ALL;

	bits = bits << 6;

	tmpData_A |= bits;
	AEK_MOT_SM81M1drv_writeMessage(driver, dev, regAddress, tmpData_A, DataReceived);
}

/**
 * @brief   Full scale motor current in HOLD mode
 *
 * @param[in] driver         SPI driver pointer
 *
 * @param[in] dev            Device component number
 *
 * @param[in] regAddress     Address of the register
 *
 * @param[in] DataReceived   Data received pointer
 *
 * @param[in] current        Motor current references
 *
 * @api
 */
void AEK_MOT_SM81M1drv_HOLD_MODE(SPIDriver* driver, AEK_MOT_SM81M1_DEVICE dev, uint8 regAddress,
		uint24* DataReceived, int current) {
	uint24 tmpData_A = 0x00;

	AEK_MOT_SM81M1drv_ReadMessage(driver, dev, regAddress);
	tmpData_A &= HC_MASK_ALL;

	if ((current >= 264 && current <= 362)
			|| (current >= 73 && current <84)) {
		current = current / 20.4;
	} else if ((current >= 168 && current <264)
			|| (current >= 84 && current <101)) {
		current = (current + 28) / 20.4;
	} else if (current >= 101 && current < 168) {
		current = (current + 48) / 20.4;
	} else if (current >= 62 && current < 73) {
		current = 2;
	} else if (current >= 50 && current < 62) {
		current = 1;
	} else if (current >= 0 && current < 50) {
		current = 0;
	}
	current = current << 12;

	tmpData_A |= current;
	AEK_MOT_SM81M1drv_writeMessage(driver, dev, regAddress, tmpData_A, DataReceived);
}

/**
 * @brief   Full scale motor current in RUN mode
 *
 * @param[in] driver         SPI driver pointer
 *
 * @param[in] dev            Device component number
 *
 * @param[in] regAddress     Address of the register
 *
 * @param[in] DataReceived   Data received pointer
 *
 * @param[in] current        Motor current references
 *
 * @api
 */
void AEK_MOT_SM81M1drv_FULL_SCALE(SPIDriver* driver, AEK_MOT_SM81M1_DEVICE dev, uint8 regAddress,
		uint24* DataReceived, int current) {
	uint24 tmpData_A = 0x00;

	if (current >= 1204 && current <= 1502) {
		current = 15;
	} else if ((current >= 920 && current <= 1204) || (current >= 302 && current < 329)){
		current = current/ 78.4;
	} else if (current >= 679 && current < 920) {
		current = (current + 100) / 78.4;
	} else if (current >= 571 && current < 679) {
		current = 9;
	} else if (current >= 396 && current < 571) {
		current = (current + 176) / 78.4;
	} else if (current >= 329 && current < 396) {
		current = (current + 80) / 78.4;
	}else if (current >= 198 && current < 329)  {
		current = (current - 80) / 78.4;
	}else if (current >= 0 && current < 198)  {
		current = 0;
	}

	current = current << 1;

	AEK_MOT_SM81M1drv_ReadMessage(driver, dev, regAddress);
	tmpData_A &= CA_MASK_ALL;
	tmpData_A |= current;
	AEK_MOT_SM81M1drv_writeMessage(driver, dev, regAddress, tmpData_A, DataReceived);
}

/**
 * @brief   Initialization Register AEK_MOT_SM81M1
 *
 * @param[in] dev           Device selected
 *
 * @api
 */
void init_AEK_MOT_SM81M1(AEK_MOT_SM81M1_DEVICE dev) {
	pal_lld_setpad(AEK_MOT_SM81M1_ARRAY_GPIO_PORT[dev], AEK_MOT_SM81M1_ARRAY_GPIO_PORT_NUMBER[dev]);
	osalThreadDelayMilliseconds(2);

	AEK_MOT_SM81M1drv_WriteSPIRegister(AEK_MOT_SM81M1_ARRAY_DRIVER[dev], dev, MCR1, 1);
	AEK_MOT_SM81M1drv_WriteSPIRegister(AEK_MOT_SM81M1_ARRAY_DRIVER[dev], dev, GCR1, 2);
	AEK_MOT_SM81M1drv_WriteSPIRegister(AEK_MOT_SM81M1_ARRAY_DRIVER[dev], dev, MCR2, 3);
	AEK_MOT_SM81M1drv_WriteSPIRegister(AEK_MOT_SM81M1_ARRAY_DRIVER[dev], dev, MCREF, 4);
}

/**
 * @brief   Turn Left Motor Control AEK_MOT_SM81M1
 *
 * @param[in] dev            Device selected 
 *
 * @param[in] step           Number of steps
 *
 * @param[in] time           Delay between commands  
 *
 * @api
 */
void TurnLeft(AEK_MOT_SM81M1_DEVICE dev, int step, int time) {
	int i, j = 0;
	for (i = 0; i < step; i++) {
    	for (j = 0; j <= 63; j++) {
        	AEK_MOT_SM81M1drv_WriteSPIRegisterMOV(AEK_MOT_SM81M1_ARRAY_DRIVER[dev], dev, MCR1, MICRO2,j, 0);
			osalThreadDelayMilliseconds(time);
        }
	}
}

/**
 * @brief   Turn Right Motor Control AEK_MOT_SM81M1
 *
 * @param[in] dev            Device selected
 *
 * @param[in] step           Number of steps
 *
 * @param[in] time           Delay between commands   
 *
 * @api
 */
void TurnRight(AEK_MOT_SM81M1_DEVICE dev, int step, int time) {
	int i, j = 0;
	for (i = 0; i < step; i++) {
		for (j = 63; j >= 0; j--) {
			AEK_MOT_SM81M1drv_WriteSPIRegisterMOV(AEK_MOT_SM81M1_ARRAY_DRIVER[dev], dev, MCR1, MICRO2,j, 1);
			osalThreadDelayMilliseconds(time);
			}
	}
}

/**
* @brief   Turn the Stepper motor in direction and angle defined
*
* @param[in] dev          Device selected
*
* @param[in] grade        Angle of rotation
*
* @param[in] direction    Direction of the rotation
*
* @param[in] time         Delay between commands  
*
* @api
*/
void RotationGrade(AEK_MOT_SM81M1_DEVICE dev, int grade, TURN_DIRECTION direction, int time) {
	int step;
	step = grade / 7;
	if (direction == RIGHT) {
		TurnRight(dev, step, time);
	}
	if (direction == LEFT) {
		TurnLeft(dev, step, time);
	}
}

/**
* @brief   Turn CountrClockwise the Stepper motor in different MODE (Full Step, Half Step or Micro Step)
*
* @param[in] dev          Device selected
*
* @param[in] step         Step of rotation
*
* @param[in] mode         Define mode the rotation
*
* @api
*/
void turnCounterclockwise(AEK_MOT_SM81M1_DEVICE dev, int step, STEP_MODE mode)
{
	int i,x,k;

    if(previousMode != mode || rotation != 0)
    {
    	if(mode != FULL)
    	{
    		position = 0;
    	}
    	else
    	{
    		position = 8;
    	}
    	previousMode = mode;
    	rotation = 0;
    }

	if (mode == MICRO2)  //HALF, FULL, MINI, MICRO8, MICRO2,
    {
    	k = 1;
    }
    if(mode == MICRO8)
    {
    	k = 2;
    }
    if(mode == MINI)
    {
    	k = 4;
    }
    if(mode == HALF)
	{
		k = 8;
	}
    if(mode == FULL)
    {
    	k = 16;
    }

	for (i = 0; i < step; i++)
	{
		for( x = 0 ; x<16 ;x=x+k)
		{
			AEK_MOT_SM81M1drv_WriteSPIRegisterMOV(AEK_MOT_SM81M1_ARRAY_DRIVER[dev], AEK_MOT_SM81M1_DEV0, MCR1,mode,position, 0);
			osalThreadDelayMilliseconds(2);
			position=position+k;
		}

		if(position == 64)
		{
			position=0;
		}
		else if(position == 72)
		{
			position=8;
		}
	}
}

/**
* @brief   Turn Clockwise the Stepper motor in different MODE (Full Step, Half Step or Micro Step)
*
* @param[in] dev          Device selected
*
* @param[in] step         Step of rotation
*
* @param[in] mode         Define mode the rotation
*
* @api
*/
void turnClockwise(AEK_MOT_SM81M1_DEVICE dev, int step, STEP_MODE mode)
{
	int i,x,k=0;

	if(previousMode != mode || rotation != 1)
	{
		if(mode != FULL)
		{
			position = 0;
		}
		else
		{
			position = -8;
		}
		previousMode = mode;
		rotation = 1;
	}

	if (mode == MICRO2)  //HALF, FULL, MINI, MICRO8, MICRO2,
	{
		k = 1;
	}
	else if(mode == MICRO8)
	{
		k = 2;
	}
	else if(mode == MINI)
	{
		k = 4;
	}
	else if(mode == HALF)
	{
		k = 8;
	}
	else if(mode == FULL)
	{
		k = 16;
	}

	for (i = 0; i < step; i++)
	{
		for( x = 16; x>0; x=x-k)
		{
			AEK_MOT_SM81M1drv_WriteSPIRegisterMOV(AEK_MOT_SM81M1_ARRAY_DRIVER[dev], AEK_MOT_SM81M1_DEV0, MCR1,mode,position, 1);
			osalThreadDelayMilliseconds(2);
			position=position-k;

		}

		if(position == -64)
		{
			position=0;
		}
		else if(position == -72)
		{
			position=-8;
		}
	}
}

#endif
/** @} */

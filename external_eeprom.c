/******************************************************************************
 *
 * Module: EXTERNAL_EEPROM
 *
 * File Name: external_eeprom.c
 *
 * Description: Source file for the AVR EXTERNAL_EEPROM driver
 *
 * Author: Mohamed Khaled
 *
 *******************************************************************************/

#include "external_eeprom.h"
#include "twi.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Send Byte to External EEPROM using TWI (I2C)
 */
uint8 EEPROM_writeByte(uint16 address, uint8 data){

	/* Send Start Condition */
	TWI_start();

	/* Check that start condition is done correctly */
	if(TWI_getStatus() != TWI_START){
		return EEPROM_ERROR;
	}

	/* Send Address to select specific EEPROM and Set it to Write mode */
	TWI_writeByte((uint8)((EEPROM_FIXED_BIT_ID<<4) | ((address & 0x700)>>7)));

	/* Check That Address is received */
	if(TWI_getStatus() != TWI_MT_SLA_W_ACK){
		return EEPROM_ERROR;
	}

	/* Send Memory Address to select specific memory location */
	TWI_writeByte((uint8)address);

	/* Check That Memory Address is received */
	if(TWI_getStatus() != TWI_MT_DATA_ACK){
		return EEPROM_ERROR;
	}

	/* Send Data to write on select specific Memory location */
	TWI_writeByte(data);

	/* Check That Data is received */
	if(TWI_getStatus() != TWI_MT_DATA_ACK){
		return EEPROM_ERROR;
	}

	/* Stop TWI */
	TWI_stop();

	/* Succeeded in sending Data */
	return EEPROM_SUCCESS;
}

/*
 * Description :
 * Receive Byte to External EEPROM using TWI (I2C)
 */
uint8 EEPROM_readByte(uint16 address, uint8 * data){

	/* Send Start Condition */
	TWI_start();

	/* Check that start condition is done correctly */
	if(TWI_getStatus() != TWI_START){
		return EEPROM_ERROR;
	}

	/* Send Address to select specific EEPROM and Set it to Write mode */
	TWI_writeByte((uint8)((EEPROM_FIXED_BIT_ID<<4) | ((address & 0x700)>>7)));

	/* Check That Address is received */
	if(TWI_getStatus() != TWI_MT_SLA_W_ACK){
		return EEPROM_ERROR;
	}

	/* Send Memory Address to select specific memory location */
	TWI_writeByte((uint8)address);

	/* Check That Memory Address is received */
	if(TWI_getStatus() != TWI_MT_DATA_ACK){
		return EEPROM_ERROR;
	}

	/* Send Repeated Start Condition */
	TWI_start();

	/* Check that Repeated start condition is done correctly */
	if(TWI_getStatus() != TWI_REP_START){
		return EEPROM_ERROR;
	}

	/* Send Address to select specific EEPROM and Set it to Read mode */
	TWI_writeByte((uint8)((EEPROM_FIXED_BIT_ID<<4) | ((address & 0x700)>>7) | (1)));

	/* Check That Address is received */
	if(TWI_getStatus() != TWI_MT_SLA_R_ACK){
		return EEPROM_ERROR;
	}

	/* Receive Data from a specific Memory location */
	*data = TWI_readByteWithNACK();

	/* Check That Data received */
	if(TWI_getStatus() != TWI_MR_DATA_NACK){
		return EEPROM_ERROR;
	}

	/* Stop TWI */
	TWI_stop();

	/* Succeeded in receiving Data */
	return EEPROM_SUCCESS;
}

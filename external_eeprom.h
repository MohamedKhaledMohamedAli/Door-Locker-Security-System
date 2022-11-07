 /******************************************************************************
 *
 * Module: EXTERNAL_EEPROM
 *
 * File Name: external_eeprom.h
 *
 * Description: Header file for the AVR EXTERNAL_EEPROM driver
 *
 * Author: Mohamed Khaled
 *
 *******************************************************************************/

#ifndef EXTERNAL_EEPROM_H_
#define EXTERNAL_EEPROM_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Fixed Bits for Address */
#define EEPROM_FIXED_BIT_ID 0b1010

#define EEPROM_ERROR 	0xFF
#define EEPROM_SUCCESS	0x00
/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * Send Byte to External EEPROM using TWI (I2C)
 */
uint8 EEPROM_writeByte(uint16 address, uint8 data);

/*
 * Description :
 * Receive Byte to External EEPROM using TWI (I2C)
 */
uint8 EEPROM_readByte(uint16 address, uint8 * data);

#endif /* EXTERNAL_EEPROM_H_ */

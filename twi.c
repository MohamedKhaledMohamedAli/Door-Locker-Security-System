/******************************************************************************
 *
 * Module: TWI
 *
 * File Name: twi.c
 *
 * Description: Source file for the AVR TWI driver
 *
 * Author: Mohamed Khaled
 *
 *******************************************************************************/

#include "twi.h"
#include "common_macros.h"
#include <avr/io.h>

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Initiate TWI (I2C):
 * 1. Enable TWI
 * 2. Disable Interrupts
 * 3. Set Frequency (Rate)
 */
void TWI_init(const TWI_ConfigType * Config_Ptr){

	/* variables to Configure Rate of Two Wire Serial Interface */
	sint16 value = 0;
	uint8 numberPower4 = 0;

	/* Temporary Variable */
	uint8 temp = 1;

	/* Loop to Configure I2C Rate */
	for(numberPower4 = 0;numberPower4 < 4; numberPower4++){

		/* get value of TWBR */
		value = (((F_CPU/(Config_Ptr->bit_rate))-16)/(2 * temp));

		/* Value must not be negative */
		if(value >=0){
			break;
		}

		/* update temp */
		temp = temp * 4;
	}

	/* Configure Rate of I2C */
	TWBR = value;
	TWSR = numberPower4;

	/* Two Wire Bus address my address if any master device want to call me: 0x1
	 * (used in case this MC is a slave device)
	 * General Call Recognition: Off */
	TWAR = Config_Ptr->address;

	/*
	 * Configure TWI Control Register:
	 * 1. TWEN = 1 -> Enable TWI
	 * 2. TWIE = 0 -> Disable Interrupt
	 */
	TWCR = (1<<TWEN);
}

/*
 * Description :
 * Start TWI (I2C)
 */
void TWI_start(void){

	/*
	 * Configure TWI Control Register:
	 * 1. TWSTA = 1 -> Start condition bit
	 * 2. TWEN  = 1 -> Enable TWI
	 * 3. TWINT = 1 -> Clear Flag
	 */
	TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWSTA);

	/* Wait Till Task is Completed */
	while(BIT_IS_CLEAR(TWCR,TWINT));
}

/*
 * Description :
 * Stop TWI (I2C)
 */
void TWI_stop(void){

	/*
	 * Configure TWI Control Register:
	 * 1. TWSTO = 1 -> Stop condition bit
	 * 2. TWEN  = 1 -> Enable TWI
	 * 3. TWINT = 1 -> Clear Flag
	 */
	TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWSTO);
}

/*
 * Description :
 * Send Byte using TWI (I2C)
 */
void TWI_writeByte(uint8 data){

	/* Send data using TWI Data Register */
	TWDR = data;

	/*
	 * Configure TWI Control Register:
	 * 1. TWEN  = 1 -> Enable TWI
	 * 2. TWINT = 1 -> Clear Flag
	 */
	TWCR = (1<<TWEN)| (1<<TWINT);

	/* Wait Till Task is Completed */
	while(BIT_IS_CLEAR(TWCR,TWINT));
}

/*
 * Description :
 * Read Byte using TWI (I2C) then send ACK
 */
uint8 TWI_readByteWithACK(void){

	/*
	 * Configure TWI Control Register:
	 * 1. TWEA  = 1 -> Enable ACK
	 * 2. TWEN  = 1 -> Enable TWI
	 * 3. TWINT = 1 -> Clear Flag
	 */
	TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);

	/* Wait Till Task is Completed */
	while(BIT_IS_CLEAR(TWCR,TWINT));

	/* Receive data using TWI Data Register */
	return TWDR;
}

/*
 * Description :
 * Read Byte using TWI (I2C) then send NACK
 */
uint8 TWI_readByteWithNACK(void){

	/*
	 * Configure TWI Control Register:
	 * 1. TWEN  = 1 -> Enable TWI
	 * 2. TWINT = 1 -> Clear Flag
	 */
	TWCR = (1<<TWEN) | (1<<TWINT);

	/* Wait Till Task is Completed */
	while(BIT_IS_CLEAR(TWCR,TWINT));

	/* Receive data using TWI Data Register */
	return TWDR;
}

/*
 * Description :
 * Get TWI (I2C) status
 */
uint8 TWI_getStatus(void){

	/* TWI Status Register */
	return (TWSR & 0xF8);
}

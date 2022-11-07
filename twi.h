 /******************************************************************************
 *
 * Module: TWI
 *
 * File Name: twi.h
 *
 * Description: Header file for the AVR TWI driver
 *
 * Author: Mohamed Khaled
 *
 *******************************************************************************/

#ifndef TWI_H_
#define TWI_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */

/* Define some equivalent data types */
typedef uint8 TWI_Address;
typedef uint32 TWI_BaudRate;

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

/* Structure to configure I2C */
typedef struct{
 TWI_Address address;
 TWI_BaudRate bit_rate;
}TWI_ConfigType;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * Initiate TWI (I2C):
 * 1. Enable TWI
 * 2. Disable Interrupts
 * 3. Set Frequency (Rate)
 */
void TWI_init(const TWI_ConfigType * Config_Ptr);

/*
 * Description :
 * Start TWI (I2C)
 */
void TWI_start(void);

/*
 * Description :
 * Stop TWI (I2C)
 */
void TWI_stop(void);

/*
 * Description :
 * Send Byte using TWI (I2C)
 */
void TWI_writeByte(uint8 data);

/*
 * Description :
 * Read Byte using TWI (I2C) then send ACK
 */
uint8 TWI_readByteWithACK(void);

/*
 * Description :
 * Read Byte using TWI (I2C) then send NACK
 */
uint8 TWI_readByteWithNACK(void);

/*
 * Description :
 * Get TWI (I2C) status
 */
uint8 TWI_getStatus(void);

#endif /* TWI_H_ */

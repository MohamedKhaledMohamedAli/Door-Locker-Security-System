 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the AVR UART driver
 *
 * Author: Mohamed Khaled
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

/* Enum for mode select of UART */
typedef enum{
	UART_Asynchronous, UART_Synchronous
}UART_ModeSelect;

/* Enum for parity bits modes */
typedef enum{
	UART_DISABLED, UART_EVEN_PARITY = 2, UART_ODD_PARITY
}UART_ParityMode;

/* Enum for stop bit select */
typedef enum{
	UART_ONE_BIT, UART_TWO_BIT
}UART_StopBitSelect;

/* Enum for  Character Size */
typedef enum{
	UART_5_BIT, UART_6_BIT, UART_7_BIT, UART_8_BIT, UART_9_BIT = 7
}UART_CharacterSize;

/* Enum for Clock Polarity */
typedef enum{
	UART_TX_RISING, UART_TX_FALLING, UART_RX_FALLING = 0, UART_RX_RAISING
}UART_ClockPolarity;

/* Structure to configure UART */
typedef struct{
	UART_ModeSelect mode;
	UART_ParityMode parity;
	UART_StopBitSelect stopSelect;
	UART_CharacterSize charcterSize;
	UART_ClockPolarity clock;
	uint32 baudRate;
}UART_ConfigType;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * Configure UART:
 * 1. Disable all interrupts
 * 2. Enable Transmitter and Receiver
 * 3. Configure mode of UART
 * 4. Configure Parity Bit
 * 5. Configure Stop Bit Select
 * 6. Character Size
 * 7. Configure Character Size
 * 8. Configure Baud Rate
 */
void UART_init(const UART_ConfigType * config_ptr);
//void UART_init(void);

/*
 * Description :
 * Send Data using UART
 */
void UART_sendByte(uint8 data);

/*
 * Description :
 * Receive Data using UART
 */
uint8 UART_receiveByte(void);

/*
 * Description :
 * Send String using UART
 */
void UART_sendString(const uint8 * str);

/*
 * Description :
 * Receive String using UART
 */
void UART_receiveString(uint8 * st);

#endif /* UART_H_ */

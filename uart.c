 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Source file for the AVR UART driver
 *
 * Author: Mohamed Khaled
 *
 *******************************************************************************/

#include "uart.h"
#include <avr/io.h>
#include "common_macros.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
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
void UART_init(const UART_ConfigType * config_ptr){
//void UART_init(void){

	uint16 baudRate;

	/* Double the USART Transmission Speed */
	UCSRA = (1<<U2X);

	/*
	 * Configure USART Control and Status Register B:
	 * RXCIE = 0 -> Disable RX Complete Interrupt
	 * TXCIE = 0 -> Disable TX Complete Interrupt
	 * UDRIE = 0 -> Disable USART Data Register Empty Interrupt
	 * RXEN = 1 -> Enable Receiver
	 * TXEN = 1 -> Enable Transmitter
	 * RXB8 = 0 -> Will not use 9 bit mode
	 * TXB8 = 0 -> Will not use 9 bit mode
	 */
	UCSRB = (1<<RXEN) | (1<<TXEN);

	/* Configure  Character Size */
	UCSRB = (UCSRB & 0xFB) | ((GET_BIT(config_ptr->charcterSize,2))<<UCSZ2);

	/*
	 * Configure USART Control and Status Register C:
	 * URSEL = 1 -> To select current register
	 */
	UCSRC = (1<<URSEL);

	/* Configure mode of UART */
	UCSRC = (UCSRC & 0xBF) | ((config_ptr->mode)<<UMSEL);

	/* Configure Parity Bit */
	UCSRC = (UCSRC & 0xCF) | ((config_ptr->parity)<<UPM0);

	/* Configure Stop Bit Select */
	UCSRC = (UCSRC & 0xF7) | ((config_ptr->stopSelect)<<USBS);

	/* Configure Character Size */
	UCSRC = (UCSRC & 0xF9) | (((config_ptr->charcterSize) & 0x03)<<UCSZ0);

	/* Configure Clock Polarity */
	UCSRC = (UCSRC & 0xFE) | (((config_ptr->clock))<<UCPOL);

	/* Calculate the UBRR register value */
	baudRate = (uint16)((F_CPU)/(8 * (config_ptr->baudRate)) - 1);

	/* Configure USART Baud Rate Register to set Baud Rate to specific value */
	UBRRL = baudRate;
	UBRRH = baudRate>>8;
}

/*
 * Description :
 * Send Data using UART
 */
void UART_sendByte(uint8 data){

	/* Wait until register is empty */
	while(BIT_IS_CLEAR(UCSRA,UDRE));

	/* Send data */
	UDR = data;
}

/*
 * Description :
 * Receive Data using UART
 */
uint8 UART_receiveByte(void){

	/* Use polling to wait until there is data to be received */
	while(BIT_IS_CLEAR(UCSRA,RXC));

	/* Return data */
	return UDR;
}

/*
 * Description :
 * Send String using UART
 */
void UART_sendString(const uint8 * str){

	/* Loop through whole string */
	while((*str) != '\0'){

		UART_sendByte(*str);

		str++;
	}
}

/*
 * Description :
 * Receive String using UART
 */
void UART_receiveString(uint8 * st){

//	uint8 i = 0;
//
//	do{
//
//		st[i] = UART_receiveByte();
//
//		i++;
//
//	/* Special Character to determine the end of the string */
//	}while(st[i] != '#');
//
//	/* put null since it is a string */
//	st[i] = '\0';

	uint8 i = 0;

	/* Receive the first byte */
	st[i] = UART_receiveByte();

	/* Receive the whole string until the '#' */
	while(st[i] != '#')
	{
		i++;
		st[i] = UART_receiveByte();
	}

	/* After receiving the whole string plus the '#', replace the '#' with '\0' */
	st[i] = '\0';
}

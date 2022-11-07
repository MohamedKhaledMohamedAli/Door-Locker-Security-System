 /******************************************************************************
 *
 * Module: LCD
 *
 * File Name: lcd.c
 *
 * Description: Source file for the AVR LCD driver
 *
 * Author: Mohamed Khaled
 *
 *******************************************************************************/

#include "lcd.h"
#include "gpio.h"
#include "common_macros.h"
#include <util/delay.h>

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Initialize the LCD :
 * 1. Setting LCD Pins as Output by GPIO Driver
 * 2. Setting the LCD to 4-bit or 8-bit mode
 */
void LCD_init(void){

	/* Configure RS and E and R/W pins direction  */
	GPIO_setupPinDirection(LCD_RS_PORT_ID, LCD_RS_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_E_PORT_ID, LCD_E_PIN_ID, PIN_OUTPUT);

	_delay_ms(20); /* Since the LCD needs 20 mS to get powered up */

#if(LCD_BIT_MODE == 4)

	/* Configure LCD Data pins as Output */
	GPIO_setupPinDirection(LCD_DATA_PORT_ID, LCD_DATA_BIT4_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT_ID, LCD_DATA_BIT5_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT_ID, LCD_DATA_BIT6_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT_ID, LCD_DATA_BIT7_PIN_ID, PIN_OUTPUT);

	/* Configure LCD as 2 lines 4-Bit mode */
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE);

	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT1);
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT2);

#elif(LCD_BIT_MODE == 8)

	/* Configure LCD Data Port as output */
	GPIO_setupPortDirection(LCD_DATA_PORT_ID, PORT_OUTPUT);

	/* Configure LCD as 2 lines 8-Bit mode */
	LCD_sendCommand(LCD_TWO_LINES_EIGHT_BITS_MODE);

#endif

	/* Turn ON DISPLAY and Turn OFF CURSOR */
	LCD_sendCommand(LCD_DISPLAY_ON_CURSOR_OFF);

	/* Clear DISPLAY */
	LCD_sendCommand(LCD_CLEAR_DISPLAY);

}

/*
 * Description :
 * Send Command to LCD
 */
void LCD_sendCommand(uint8 command){

	/* Command Register is selected */
	GPIO_writePin(LCD_RS_PORT_ID, LCD_RS_PIN_ID, LOGIC_LOW);

	_delay_ms(1); /* Time for tas(Address Set-Up Time) = 50nS */

	/* Set E(Enable) High */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_HIGH);

	_delay_ms(1); /* Time for tpw-tdsw = 190nS */

#if(LCD_BIT_MODE == 4)

	/* Send data's (4,5,6,7) bits */
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_BIT4_PIN_ID, GET_BIT(command,4));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_BIT5_PIN_ID, GET_BIT(command,5));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_BIT6_PIN_ID, GET_BIT(command,6));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_BIT7_PIN_ID, GET_BIT(command,7));

	_delay_ms(1); /* Time for tdsw = 100nS */

	/* Set E(Enable) Low */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW);

	_delay_ms(1); /* Time for tah = 13nS */

	/* Set E(Enable) High */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_HIGH);

	_delay_ms(1); /* Time for tpw-tdsw = 190nS */

	/* Send data's (0,1,2,3) bits */
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_BIT4_PIN_ID, GET_BIT(command,0));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_BIT5_PIN_ID, GET_BIT(command,1));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_BIT6_PIN_ID, GET_BIT(command,2));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_BIT7_PIN_ID, GET_BIT(command,3));

	_delay_ms(1); /* Time for tdsw = 100nS */

	/* Set E(Enable) Low */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW);

	_delay_ms(1); /* Time for tah = 13nS */

#elif(LCD_BIT_MODE == 8)

	/* Send Data */
	GPIO_writePort(LCD_DATA_PORT_ID, command);

	_delay_ms(1); /* Time for tdsw = 100nS */

	/* Set E(Enable) Low */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW);

	_delay_ms(1); /* Time for tah = 13nS */

#endif
}

/*
 * Description :
 * Display character on LCD
 */
void LCD_displayCharacter(uint8 character){

	/* Data Register is selected */
	GPIO_writePin(LCD_RS_PORT_ID, LCD_RS_PIN_ID, LOGIC_HIGH);

	_delay_ms(1); /* Time for tas(Address Set-Up Time) = 50nS */

	/* Set E(Enable) High */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_HIGH);

	_delay_ms(1); /* Time for tpw-tdsw = 190nS */

#if(LCD_BIT_MODE == 4)

	/* Send data's (4,5,6,7) bits */
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_BIT4_PIN_ID, GET_BIT(character,4));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_BIT5_PIN_ID, GET_BIT(character,5));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_BIT6_PIN_ID, GET_BIT(character,6));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_BIT7_PIN_ID, GET_BIT(character,7));

	_delay_ms(1); /* Time for tdsw = 100nS */

	/* Set E(Enable) Low */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW);

	_delay_ms(1); /* Time for tah = 13nS */

	/* Set E(Enable) High */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_HIGH);

	_delay_ms(1); /* Time for tpw-tdsw = 190nS */

	/* Send data's (0,1,2,3) bits */
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_BIT4_PIN_ID, GET_BIT(character,0));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_BIT5_PIN_ID, GET_BIT(character,1));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_BIT6_PIN_ID, GET_BIT(character,2));
	GPIO_writePin(LCD_DATA_PORT_ID, LCD_DATA_BIT7_PIN_ID, GET_BIT(character,3));

	_delay_ms(1); /* Time for tdsw = 100nS */

	/* Set E(Enable) Low */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW);

	_delay_ms(1); /* Time for tah = 13nS */

#elif(LCD_BIT_MODE == 8)

	/* Send Data */
	GPIO_writePort(LCD_DATA_PORT_ID, character);

	_delay_ms(1); /* Time for tdsw = 100nS */

	/* Set E(Enable) Low */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW);

	_delay_ms(1); /* Time for tah = 13nS */

#endif
}

/*
 * Description :
 * Display string(array of characters) on LCD
 */
void LCD_displayString(const uint8 * character){
	while((*character) != '\0'){
		LCD_displayCharacter(*character);
		character++;
	}
}

/*
 * Description :
 * Move Cursor on LCD
 */
void LCD_moveCursor(uint8 row, uint8 col){
	uint8 address;

	switch(row){
	case 0:
		address = col;
		break;
	case 1:
		address = col + 0x40;
		break;
	case 2:
		address = col + 0x10;
		break;
	case 3:
		address = col + 0x50;
		break;
	}

	/* Move Cursor to a specific position */
	LCD_sendCommand(address | LCD_FORCE_CURSOR_TO_BEGINNING_1ST_LINE);
}

/*
 * Description :
 * Move Cursor on LCD to a specific position then write string at that position
 */
void LCD_displayStringRowColumn(uint8 row, uint8 col, const uint8 * character){
	LCD_moveCursor(row, col);     /* Move Cursor to a specific position */
	LCD_displayString(character); /* Display string(array of characters) on LCD */
}

/*
 * Description :
 * Clear LCD display
 */
void LCD_clearScreen(void){
	LCD_sendCommand(LCD_CLEAR_DISPLAY); /* Clear LCD display */
}

/*
 * Description :
 * Display numbers on LCD
 */
void LCD_integerToString(int num){
	char buff[16];    /* 16 since the LCD has 16 columns */

	/* change number(by base 10) to array of character and store them in buff array*/
	itoa(num,buff,10);

	/* display the number */
	LCD_displayString(buff);
}

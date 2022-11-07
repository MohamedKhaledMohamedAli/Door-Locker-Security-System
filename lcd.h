 /******************************************************************************
 *
 * Module: LCD
 *
 * File Name: lcd.h
 *
 * Description: Header file for the AVR LCD driver
 *
 * Author: Mohamed Khaled
 *
 *******************************************************************************/

#ifndef LCD_H_
#define LCD_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/* Set LCD's Bit Mode */
#define  LCD_BIT_MODE			8

#if((LCD_BIT_MODE != 4) && (LCD_BIT_MODE != 8))

#error "LCD work either 4 or 8 bits mode only"

#endif
/* LCD's RS Configuration */
#define  LCD_RS_PORT_ID 		PORTB_ID
#define  LCD_RS_PIN_ID 			PIN0_ID

/* LCD's Enable Configuration */
#define  LCD_E_PORT_ID	 		PORTB_ID
#define  LCD_E_PIN_ID 			PIN1_ID

/* LCD's Data Path Configuration */
#define  LCD_DATA_PORT_ID	 	PORTA_ID

#if(LCD_BIT_MODE == 4)

/* LCD's Data Path Pins Configuration for 4-bit mode */
#define  LCD_DATA_BIT4_PIN_ID		PIN3_ID
#define  LCD_DATA_BIT5_PIN_ID		PIN4_ID
#define  LCD_DATA_BIT6_PIN_ID		PIN5_ID
#define  LCD_DATA_BIT7_PIN_ID		PIN6_ID

#endif

/* LCD's Commands */
#define  LCD_CLEAR_DISPLAY		 					0x01
#define  LCD_RETURN_HOME		 					0x02
#define  LCD_SHIFT_CURSOR_LEFT	 					0x04
#define  LCD_SHIFT_DISPLAY_RIGTH 					0x05
#define  LCD_SHIFT_CURSOR_RIGTH	 					0x06
#define  LCD_SHIFT_DISPLAY_LEFT	 					0x07
#define  LCD_DISPLAY_OFF_CURSOR_OFF					0x08
#define  LCD_DISPLAY_OFF_CURSOR_ON					0x0A
#define  LCD_DISPLAY_ON_CURSOR_OFF					0x0C
#define  LCD_DISPLAY_ON_CURSOR_ON					0x0E
#define  LCD_DISPLAY_ON_CURSOR_BLINKING				0x0F
#define  LCD_SHIFT_CURSOR_POSITION_LEFT				0x10
#define  LCD_SHIFT_CURSOR_POSITION_RIGHT			0x14
#define  LCD_SHIFT_ENTIRE_DISPLAY_LEFT 				0x18
#define  LCD_SHIFT_ENTIRE_DISPLAY_RIGTH				0x1C
#define  LCD_FORCE_CURSOR_TO_BEGINNING_1ST_LINE		0x80
#define  LCD_FORCE_CURSOR_TO_BEGINNING_2ND_LINE		0xC0
#define  LCD_TWO_LINES_EIGHT_BITS_MODE				0x38
#define  LCD_TWO_LINES_FOUR_BITS_MODE				0x28
#define  LCD_TWO_LINES_FOUR_BITS_MODE_INIT1   		0x33
#define  LCD_TWO_LINES_FOUR_BITS_MODE_INIT2   		0x32


/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * Initialize the LCD :
 * 1. Setting LCD Pins as Output by GPIO Driver
 * 2. Setting the LCD to 4-bit or 8-bit mode
 */
void LCD_init(void);

/*
 * Description :
 * Send Command to LCD
 */
void LCD_sendCommand(uint8 command);

/*
 * Description :
 * Display character on LCD
 */
void LCD_displayCharacter(uint8 character);

/*
 * Description :
 * Display string(array of characters) on LCD
 */
void LCD_displayString(const uint8 * character);

/*
 * Description :
 * Move Cursor on LCD to a specific position
 */
void LCD_moveCursor(uint8 row, uint8 col);

/*
 * Description :
 * Move Cursor on LCD to a specific position then write string at that position
 */
void LCD_displayStringRowColumn(uint8 row, uint8 col, const uint8 * character);

/*
 * Description :
 * Clear LCD display
 */
void LCD_clearScreen(void);

/*
 * Description :
 * Display numbers on LCD
 */
void LCD_integerToString(int num);

#endif /* LCD_H_ */

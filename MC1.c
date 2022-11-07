/*
 ================================================================================================
 Name        : MC1.c
 Date        : 10/24/2022
 Author      : Mohamed Khaled
 Description : HMI_ECU (Human Machine Interface):
 	 	 	   Responsible interaction with the user just take inputs through keypad
 	 	 	   and display messages on the LCD
 ================================================================================================
 */

#include "lcd.h"
#include "keypad.h"
#include "timer1.h"
#include "uart.h"
#include <avr/io.h>
#include <util/delay.h>

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Password consists of 5 numbers */
#define PASSWORD_SIZE	5

/* Symbol to be displayed after each push in keypad during writing the password */
#define PASSWORD_SYMBOL	'*'

/* Symbol for Enter */
#define ENTER			'='

/* Confirm Password */
#define PASSWORD_CONFIRMED		1
#define PASSWORD_NOT_CONFIRMED	0

/* Determine if password is correct or not */
#define PASSWORD_IS_CORRECT		1
#define PASSWORD_IS_FALSE		0

/* Define Motor's State */
#define MOTOR_CW	1
#define MOTOR_A_CW	2

/* To Communicate between two MC's by defining which function to be called in MC2 */
#define CONFIRM_PASSWORD_AND_STORE_IT	1
#define CHECK_PASSWORD					2
#define MOVE_MOTOR						3
#define ACTIVATE_BUZZER					4

/*******************************************************************************
 *                     Private Global Variable                                 *
 *******************************************************************************/

/* Variable to store temporary Password as array of characters */
static volatile char g_characterPassword[PASSWORD_SIZE + 2];    /* 7 since the password contains 5 digits + hash + Null */

/* Variable to indicate callback has occurred */
static volatile uint8 g_callbackVariable = 0;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * Function to Create Password or Change it
 */
void createSystemPassword(void);

/*
 * Description :
 * Function to get password from user
 */
void getPassword(void);

/*
 * Description :
 * Function to interface with sure and display main options on LCD
 */
void mainOptions(void);

/*
 * Description :
 * Function determine if password is correct or not
 */
uint8 passwordIsCorrect(void);

/*
 * Description :
 * Function to Open Door
 */
void openDoorPlus(void);

/*
 * Description :
 * Callback Function
 */
void callbackFunction(void);

/*
 * Description :
 * Function to Change Password
 */
void changePasswordNegative(void);

/*
 * Description :
 * Function to Lock the system and display error message
 */
void lockSystemAndErrorMessage(void);

/*******************************************************************************
 *                                 Main Functions                              *
 *******************************************************************************/

int main(void){

	/* Configure UART */
	UART_ConfigType UART_config = {UART_Asynchronous, UART_DISABLED, UART_ONE_BIT, UART_8_BIT, 0, 9600};

	/* Initializations */
	LCD_init();					/* Initiate LCD */
	UART_init(&UART_config);	/* Initiate UART */

	/* Enable External Interrupts (I-bit) */
	SREG |= (1<<7);

	/* Infinite Loop */
	for(;;){

		/* Function called to create password */
		createSystemPassword();

		/* Function Called to go to Main menu */
		mainOptions();
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Function to Create Password or Change it
 */
void createSystemPassword(void){

	/* To stay in step 1 until the user enter password and correct confirm passord */
	do{

		/* Set function to be called in MC2 */
		UART_sendByte(CONFIRM_PASSWORD_AND_STORE_IT);

		/* Clear LCD */
		LCD_clearScreen();

		/* Display On LCD "plz enter pass:"	*/
		LCD_displayStringRowColumn(0, 0, "plz enter pass:");

		/* Move Cursor of LCD to next line */
		LCD_moveCursor(1, 0);

		/* get password from user */
		getPassword();

		/* Send Password to Control_ECU (MC2) */
		UART_sendString(g_characterPassword);

		/* Clear Display */
		LCD_clearScreen();

		/* Display On LCD "plz re-enter the" */
		LCD_displayStringRowColumn(0, 0, "plz re-enter the");

		/* Display On LCD "same Pass: " */
		LCD_displayStringRowColumn(1, 0, "same Pass: ");

		/* get password from user */
		getPassword();

		/* Send Password to Control_ECU (MC2) */
		UART_sendString(g_characterPassword);

	/* wait till the Control_ECU (MC2) Tell you there is a match */
	}while(UART_receiveByte() != PASSWORD_CONFIRMED);
}

/*
 * Description :
 * Function to get password from user
 */
void getPassword(void){

	/* Counter */
	uint8 count;

	/* Value entered by keypad */
	uint8 key = 0;

	for(count = 0;count<PASSWORD_SIZE;count++){

		/* Get value from Keypad */
		key = KEYPAD_getPressedKey();

		/* display '*' after each input */
		LCD_displayCharacter(PASSWORD_SYMBOL);

		/* Save Password entered by user */
		g_characterPassword[PASSWORD_SIZE - count - 1] = key;

		/* delay */
		_delay_ms(500);
	}

	/* wait for enter to be pressed */
	while(key != ENTER){

		/* Get value from Keypad */
		key = KEYPAD_getPressedKey();

		/* delay */
		_delay_ms(500);
	}

	/* Add hash and Null */
	g_characterPassword[5] = '#';
	g_characterPassword[6] = '\0';
}

/*
 * Description :
 * Function to interface with sure and display main options on LCD
 */
void mainOptions(void){

	/* Loop */
	for(;;){

		/* Value entered by keypad */
		uint8 key = 0;

		/* Clear LCD */
		LCD_clearScreen();

		/* Display Main Options */
		LCD_displayStringRowColumn(0, 0, "+ : Open Door");
		LCD_displayStringRowColumn(1, 0, "- : Change Pass");

		/* Get value from Keypad */
		key = KEYPAD_getPressedKey();

		if(key == '+'){

			/* delay */
			_delay_ms(500);

			/* Call open door function */
			openDoorPlus();
		}
		else if(key == '-'){

			/* delay */
			_delay_ms(500);

			/* Call change password function */
			changePasswordNegative();
		}
	}
}

/*
 * Description :
 * Function determine if password is correct or not
 */
uint8 passwordIsCorrect(void){

	/* variable to store the value to be returned */
	uint8 ret = PASSWORD_IS_FALSE;

	/* counter */
	uint8 count = 0;

	/* Loop to check password */
	for(count = 0;count < 3;count++){

		/* Clear LCD */
		LCD_clearScreen();

		/* Display On LCD "plz enter pass:"	*/
		LCD_displayStringRowColumn(0, 0, "plz enter pass:");
		LCD_moveCursor(1, 0);

		/* Set MC2 to check on password */
		UART_sendByte(CHECK_PASSWORD);

		/* get password from user */
		getPassword();

		/* Send password to MC2 */
		UART_sendString(g_characterPassword);

		/* wait till MC2 check the password and see if it is correct password of not */
		if(UART_receiveByte() == PASSWORD_IS_CORRECT){


			/* update return variable */
			ret = PASSWORD_IS_CORRECT;

			/* Leave the Loop */
			break;
		}
	}

	/* return variable */
	return ret;
}

/*
 * Description :
 * Function to Open Door
 */
void openDoorPlus(void){

	if(passwordIsCorrect() == PASSWORD_IS_CORRECT){

		/* Send to MC2 To start the motor and rotate CW */
		UART_sendByte(MOVE_MOTOR);

		/* Reset Global Variable */
		g_callbackVariable = 0;

		/* Set Timer1 configurations to wait 15 seconds */
		Timer1_ConfigType config = {0, 31250, TIMER1_CPU_256, TIMER1_CTC};

		/* Set Timer1 */
		Timer1_setCallBack(callbackFunction);
		Timer1_init(&config);

		/* Clear Display */
		LCD_clearScreen();

		/* Display on LCD "Door is Unlocking" */
		LCD_displayStringRowColumn(0, 0, "Door is");
		LCD_displayStringRowColumn(1, 0, "Unlocking");

		/* wait till Door Opens */
		while(g_callbackVariable < 15);

		/* Reset Global Variable */
		g_callbackVariable = 0;

		/* Clear Display */
		LCD_clearScreen();

		/* Display on LCD "Door is Unlocked" */
		LCD_displayStringRowColumn(0, 0, "Door is");
		LCD_displayStringRowColumn(1, 0, "Unlocked");

		/* Hold Door Open for 3 seconds */
		while(g_callbackVariable < 3);

		/* Reset Global Variable */
		g_callbackVariable = 0;

		/* Clear Display */
		LCD_clearScreen();

		/* Display on LCD "Door is Locking" */
		LCD_displayStringRowColumn(0, 0, "Door is");
		LCD_displayStringRowColumn(1, 0, "Locking");

		/* wait for Door to Close for 15 seconds */
		while(g_callbackVariable < 15);

		/* Stop Timer1 */
		Timer1_deInit();

		/* Reset Global Variable */
		g_callbackVariable = 0;
	}
	else{

		/* Display Error Message and lock the system */
		lockSystemAndErrorMessage();
	}
}

/*
 * Description :
 * Callback Function
 */
void callbackFunction(void){

	/* Update global variable */
	g_callbackVariable++;
}

/*
 * Description :
 * Function to Change Password
 */
void changePasswordNegative(void){

	if(passwordIsCorrect() == PASSWORD_IS_CORRECT){

		/* Change Password */
		createSystemPassword();
	}
	else{

		/* Display Error Message and lock the system */
		lockSystemAndErrorMessage();
	}
}

/*
 * Description :
 * Function to Lock the system and display error message
 */
void lockSystemAndErrorMessage(void){

	/* Tell MC2 To Activate Buzzer */
	UART_sendByte(ACTIVATE_BUZZER);

	/* Reset Global Variable */
	g_callbackVariable = 0;

	/* Set Timer1 configurations to wait 1 minute */
	Timer1_ConfigType config = {0, 31250, TIMER1_CPU_256, TIMER1_CTC};

	/* Set Timer1 */
	Timer1_setCallBack(callbackFunction);
	Timer1_init(&config);

	/* Clear LCD */
	LCD_clearScreen();

	/* Display On LCD "Error: Wrong Password" */
	LCD_displayStringRowColumn(0, 0, "Error:");
	LCD_displayStringRowColumn(1, 0, "Wrong Password");

	/* wait for 1 minute */
	while(g_callbackVariable < 60);

	/* Stop Timer1 */
	Timer1_deInit();
}

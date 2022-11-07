/*
 ================================================================================================
 Name        : MC2.c
 Date        : 10/24/2022
 Author      : Mohamed Khaled
 Description : Control_ECU :
 	 	 	   Responsible for all the processing and decisions in the system like password
			   checking, open the door and activate the system alarm
 ================================================================================================
 */

#include "uart.h"
#include "buzzer.h"
#include "dc_motor.h"
#include "external_eeprom.h"
#include "timer1.h"
#include "twi.h"
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* For the configurations of I2C */
#define TWI_RATE	400000  /* rate is 400 kbps */
#define TWI_ADDRESS	0x01	/* Address of MC is 0x01 */

/* For the configurations of Motor Speed */
#define DcMotor_SPEED			50

/* Define Motor's State */
#define MOTOR_CW	1
#define MOTOR_A_CW	2

/* Confirm Password */
#define PASSWORD_CONFIRMED		1
#define PASSWORD_NOT_CONFIRMED	0

/* Determine if password is correct or not */
#define PASSWORD_IS_CORRECT		1
#define PASSWORD_IS_FALSE		0

/* To Communicate between two MC's by defining which function to be called in MC2 */
#define CONFIRM_PASSWORD_AND_STORE_IT	1
#define CHECK_PASSWORD					2
#define MOVE_MOTOR						3
#define ACTIVATE_BUZZER					4

/*******************************************************************************
 *                     Private Global Variable                                 *
 *******************************************************************************/

/* Variable to indicate callback has occurred */
static volatile uint8 g_callbackVariable = 0;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * Function to Confirm the Password and store it on External EEPROM
 */
void confirmPasswordThenStore(void);

/*
 * Description :
 * Function to Check the Password
 */
void checkPassword(void);

/*
 * Description :
 * Function to operate the dc motor
 */
void moveMotor(void);

/*
 * Description :
 * Function to stop the DC Motor
 */
void stopMotor(void);

/*
 * Description :
 * Function to handle callback for buzzer
 */
void buzzerCallback(void);

/*******************************************************************************
 *                                 Main Functions                              *
 *******************************************************************************/

int main(void){

	/* Variable to determine which function to be called */
	uint8 callFunc = 0;

	/* Configure I2C */
	TWI_ConfigType TWI_config = {TWI_ADDRESS, TWI_RATE};

	/* Configure UART */
	UART_ConfigType UART_config = {UART_Asynchronous, UART_DISABLED, UART_ONE_BIT, UART_8_BIT, 0, 9600};

	/* Initializations */
	Buzzer_init();				/* Initiate Buzzer */
	TWI_init(&TWI_config);		/* Initiate I2C */
	UART_init(&UART_config);	/* Initiate UART */
	DcMotor_Init();				/* Initiate DC Motor */

	/* Enable External Interrupts (I-bit) */
	SREG |= (1<<7);

	/* Infinite Loop */
	for(;;){

		/* Receive what function should be called */
		callFunc = UART_receiveByte();

		/* To choose Function */
		switch(callFunc){

		/* to confirm password and store it */
		case CONFIRM_PASSWORD_AND_STORE_IT:

			/* Confirm the password and store it's value in External EEPROM */
			confirmPasswordThenStore();

			break;

		/* To check password */
		case CHECK_PASSWORD:

			/* Call function to check the password entered by user */
			checkPassword();

			break;

		/* To operate the DC Motor */
		case MOVE_MOTOR:

			/* Call function to operate the DC Motor */
			moveMotor();

			break;

		/* Activate Buzzer */
		case ACTIVATE_BUZZER:

			/* Reset callFunc */
			callFunc = 0;

			/* Set Timer1 configurations to wait 1 minute */
			Timer1_ConfigType config = {0, 31250, TIMER1_CPU_256, TIMER1_CTC};

			/* Set Timer1 */
			Timer1_setCallBack(buzzerCallback);
			Timer1_init(&config);

			/* Activate Buzzer for 1 minute */
			Buzzer_on();
		}
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Function to Confirm the Password and store it on External EEPROM
 */
void confirmPasswordThenStore(void){

	/* Variable to Store the Password */
	char password[7] = {0};
	char confPass[7] = {0};

	/* Counter */
	uint8 count = 0;

	/* Receive Password from MC1 using UART */
	UART_receiveString(password);
	UART_receiveString(confPass);

	/* The two Passwords Matches */
	if(strcmp(password, confPass) == 0){

		/* Loop to Store Password inside External EEPROM */
		for(count = 0; count < 6; count++){

			/* Store Password inside External EEPROM */
			EEPROM_writeByte(0x0300 + count, password[count]);

			/* Delay */
			_delay_ms(10);
		}

		/* Send Confirmation to MC1 */
		UART_sendByte(PASSWORD_CONFIRMED);
	}
	else{

		/* Password was entered wrong */
		UART_sendByte(PASSWORD_NOT_CONFIRMED);
	}
}

/*
 * Description :
 * Function to Check the Password
 */
void checkPassword(void){

	/* Variable to get the correct password inside the external EEPROM */
	char correctPassword[7] = {0};

	/* Variable to store password entered by user */
	char password[7] = {0};

	/* Counter */
	uint8 count = 0;

	/* Receive password from MC1 */
	UART_receiveString(password);

	/* Loop to Get correct password value from External EEPROM */
	for(count = 0; count < 6; count++){

		/* Get correct password value */
		EEPROM_readByte(0x0300 + count, &correctPassword[count]);

		/* Delay */
		_delay_ms(10);
	}

	if(strcmp(correctPassword, password) == 0){

		/* if password was entered correct */
		UART_sendByte(PASSWORD_IS_CORRECT);
	}
	else{

		/* if wrong password was entered */
		UART_sendByte(PASSWORD_IS_FALSE);
	}
}

/*
 * Description :
 * Function to operate the DC Motor
 */
void moveMotor(void){

	/* Reset Global Variable */
	g_callbackVariable = 0;

	/* Set Timer1 configurations */
	Timer1_ConfigType config = {0, 31250, TIMER1_CPU_256, TIMER1_CTC};

	/* Set Timer1 */
	Timer1_setCallBack(stopMotor);
	Timer1_init(&config);

	/* Start the motor */
	DcMotor_Rotate(CW, DcMotor_SPEED);

	/* wait 15 second */
	while(g_callbackVariable < 15);

	/* Reset Global Variable */
	g_callbackVariable = 0;

	/* Stop the motor */
	DcMotor_Rotate(STOP, DcMotor_SPEED);

	/* wait 3 second */
	while(g_callbackVariable < 3);

	/* Reset Global Variable */
	g_callbackVariable = 0;

	/* Start the motor */
	DcMotor_Rotate(A_CW, DcMotor_SPEED);

	/* wait 15 second */
	while(g_callbackVariable < 15);

	/* Reset Global Variable */
	g_callbackVariable = 0;

	/* Stop the motor */
	DcMotor_Rotate(STOP, DcMotor_SPEED);
}

/*
 * Description :
 * Function to stop the DC Motor
 */
void stopMotor(void){

	/* Update Global Variable */
	g_callbackVariable++;
}

/*
 * Description :
 * Function to handle callback for buzzer
 */
void buzzerCallback(void){

	/* Variable to count 15 second */
	static uint8 tick = 0;

	if(tick < 60){

		/* Update tick counter */
		tick++;
	}
	else{

		/* Turn off Buzzer */
		Buzzer_off();

		/* Stop Timer1 */
		Timer1_deInit();

		/* Reset Tick */
		tick = 0;
	}
}

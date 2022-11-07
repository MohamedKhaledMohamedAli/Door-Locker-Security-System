 /******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: buzzer.c
 *
 * Description: Source file for the AVR BUZZER driver
 *
 * Author: Mohamed Khaled
 *
 *******************************************************************************/

#include "buzzer.h"
#include "gpio.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Initialize Buzzer:
 * 1. Setup the direction for the buzzer pin as output pin through the GPIO driver
 * 2. Turn off the buzzer through the GPIO
 */
void Buzzer_init(void){

	/* Set Buzzer Pin as Output Pin */
	GPIO_setupPinDirection(BUZZER_PORT_ID, BUZZER_PIN_ID, PIN_OUTPUT);

	/* Turn OFF the Buzzer */
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, BUZZER_TURN_OFF);
}

/*
 * Description :
 * Function to enable the Buzzer through the GPIO
 */
void Buzzer_on(void){

	/* Turn ON the Buzzer */
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, BUZZER_TURN_ON);
}

/*
 * Description :
 * Function to disable the Buzzer through the GPIO
 */
void Buzzer_off(void){

	/* Turn OFF the Buzzer */
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, BUZZER_TURN_OFF);
}

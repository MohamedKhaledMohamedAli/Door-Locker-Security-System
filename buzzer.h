 /******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: buzzer.h
 *
 * Description: Header file for the AVR BUZZER driver
 *
 * Author: Mohamed Khaled
 *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Configure Buzzer Pin */
#define BUZZER_PORT_ID	 PORTD_ID
#define BUZZER_PIN_ID	 PIN6_ID

/* Configure Buzzer Status */
#define BUZZER_TURN_ON	 LOGIC_HIGH
#define BUZZER_TURN_OFF  LOGIC_LOW

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * Initialize Buzzer:
 * 1. Setup the direction for the buzzer pin as output pin through the GPIO driver
 * 2. Turn off the buzzer through the GPIO
 */
void Buzzer_init(void);

/*
 * Description :
 * Function to enable the Buzzer through the GPIO
 */
void Buzzer_on(void);

/*
 * Description :
 * Function to disable the Buzzer through the GPIO
 */
void Buzzer_off(void);

#endif /* BUZZER_H_ */

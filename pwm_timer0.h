 /******************************************************************************
 *
 * Module: PWM_TIMER0
 *
 * File Name: pwm_timer0.h
 *
 * Description: Header file for the AVR PWM_TIMER0 driver
 *
 * Author: Mohamed Khaled
 *
 *******************************************************************************/

#ifndef PWM_TIMER0_H_
#define PWM_TIMER0_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define PWM_TIMER0_PORT_ID PORTB_ID
#define PWM_TIMER0_PIN_ID  PIN3_ID

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

/* enum for Compare Match Output Modes */
typedef enum{
	DISCONNECT_OC0 = 0, NON_INVERTING = 2, INVERTING = 3
}PWM_TIMER0_CompareMatchOutputMode;

/* enum for Clock Select */
typedef enum{
	NO_CLOCK, F_PWM_CPU_CLOCK, F_PWM_CPU_8, F_PWM_CPU_64, F_PWM_CPU_256, F_PWM_CPU_1024
}PWM_TIMER0_Clock;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * The function is responsible for:
 * 1. Trigger the Timer0 with the PWM Mode
 * 2. Setup the PWM mode with Non-Inverting
 * 3. Setup the prescaler with F_CPU/8
 * 4. Setup the compare value based on the required input duty cycle
 * 5. Setup the direction for OC0 as output pin through the GPIO driver
 * 6. The generated PWM signal frequency will be 500Hz to control the DC Motor speed
 */
void PWM_TIMER0_Start(uint8 duty_cycle);

#endif /* PWM_TIMER0_H_ */

 /******************************************************************************
 *
 * Module: PWM_TIMER0
 *
 * File Name: pwm_timer0.c
 *
 * Description: Source file for the AVR PWM_TIMER0 driver
 *
 * Author: Mohamed Khaled
 *
 *******************************************************************************/

#include "pwm_timer0.h"
#include <avr/io.h>
#include "gpio.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
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
void PWM_TIMER0_Start(uint8 duty_cycle){

	/* Set PWM Timer0 pin as output */
	GPIO_setupPinDirection(PWM_TIMER0_PORT_ID, PWM_TIMER0_PIN_ID, PIN_OUTPUT);

	/* Set Duty Cycle */
	OCR0 = ((float)duty_cycle/100) * 0xFF;

	/* Start Timer0 */
	TCNT0 = 0;

	/*
	 * Configure Timer/Counter Control Register:
	 * 1. Clear Bit 7(FOC0) since I will Configure Timer0 to operate in PWM mode
	 * 2. Set Bit 6, 3(WGM01:0) to operate in PWM mode
	 */
	TCCR0 = (1<<WGM00) | (1<<WGM01);

	/* Configure Fast PWM Compare Match Output Mode */
	TCCR0 = (TCCR0 & 0xCF) | ((NON_INVERTING)<<COM00);

	/* Configure clock source to be used by the Timer/Counter0 */
	TCCR0 = (TCCR0 & 0xF8) | ((F_PWM_CPU_64)<<CS00);
}

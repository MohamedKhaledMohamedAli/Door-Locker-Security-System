 /******************************************************************************
 *
 * Module: TIMER1
 *
 * File Name: timer1.c
 *
 * Description: Source file for the AVR TIMER1 driver
 *
 * Author: Mohamed Khaled
 *
 *******************************************************************************/

#include "timer1.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/*******************************************************************************
 *                     Private Global Variable                                 *
 *******************************************************************************/

static volatile void(*g_callbackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

#ifdef TIMER1_COMPARE_MODE

/* ISR Function For Compare Mode */
ISR(TIMER1_COMPA_vect){

	/* Check if Pointer value is updated */
	if(g_callbackPtr != NULL_PTR){

		/* Call the Callback function */
		(*g_callbackPtr)();
	}
}

#else

/* ISR Function For Compare Mode */
ISR(TIMER1_OVF_vect){

	/* Check if Pointer value is updated */
	if(g_callbackPtr != NULL_PTR){

		/* Call the Callback function */
		(*g_callbackPtr)();
	}
}
#endif

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Function to initialize the Timer driver
 */
void Timer1_init(const Timer1_ConfigType * Config_Ptr){

	/*
	 * Configure Timer/Counter1 Control Register A:
	 * 1. COM1A1:0 = 00 => Normal port operation, OC1A disconnected
	 * 2. COM1B1:0 = 00 => Normal port operation, OC1B disconnected
	 * 3. FOC1A/FOC1B = 1 => non-PWM mode
	 *
	 */
	TCCR1A = (1<<FOC1A) | (1<<FOC1B);

	/*
	 * Configure Timer/Counter1 Control Register B:
	 * 1. ICNC1 = 0 => Disable Input Capture Noise Canceler
	 * 2. ICES1 = 0 => Disable Input Capture Edge Select
	 */
	TCCR1B = 0;

	/* Configure Timer1 mode */
	TCCR1A = (TCCR1A & 0xFC) | (((Config_Ptr->mode) & 0x03)<<WGM10);
	TCCR1B = (TCCR1B & 0xE7) | (((Config_Ptr->mode) & 0x0C)<<1);

	/* Configure Timer1 clock pre-scaler */
	TCCR1B = (TCCR1B & 0xF8) | (((Config_Ptr->prescaler) & 0x07)<<CS10);

	/* Set Initial value for Timer1 */
	TCNT1 = Config_Ptr->initial_value;

#ifdef TIMER1_COMPARE_MODE

	/* Set Compare Value */
	OCR1A = Config_Ptr->compare_value;  /* it will be used in compare mode only */
#endif

	/* Configure Timer/Counter Interrupt Mask Register */
#ifdef TIMER1_COMPARE_MODE

	/* Enable Timer/Counter1, Output Compare A Match Interrupt */
	TIMSK |= (1<<OCIE1A);
#else

	/* Enable Timer/Counter1, Overflow Interrupt */
	TIMSK |= (1<<TOIE1);
#endif
}

/*
 * Description :
 * Function to disable the Timer1
 */
void Timer1_deInit(void){

	/* Reset Timer1 counter */
	TCNT1 = 0;

	/*
	 * Turn OFF Timer1:
	 * 1. Clear Mode
	 * 2. Disable Clock Source
	 * 3. Disable Interrupt
	 */
	TCCR1A = 0;
	TCCR1B = 0;
#ifdef TIMER1_COMPARE_MODE

	/* Disable Timer/Counter1, Output Compare A Match Interrupt */
	TIMSK &= ~(1<<OCIE1A);
#else

	/* Enable Timer/Counter1, Overflow Interrupt */
	TIMSK &= ~(1<<TOIE1);
#endif
}

/*
 * Description :
 * Function to set the Call Back function address
 */
void Timer1_setCallBack(void(*a_ptr)(void)){

	/* Set Callback function */
	g_callbackPtr = a_ptr;
}

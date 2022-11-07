 /******************************************************************************
 *
 * Module: TIMER1
 *
 * File Name: timer1.h
 *
 * Description: Header file for the AVR TIMER1 driver
 *
 * Author: Mohamed Khaled
 *
 *******************************************************************************/

#ifndef TIMER1_H_
#define TIMER1_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Configure whether we are Compare Mode or else Normal Mode */
#define TIMER1_COMPARE_MODE

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

/* Enum to Configure Timer1 clock pre-scaler */
typedef enum{

	TIMER1_NO_CLK, TIMER1_CPU_CLK, TIMER1_CPU_8, TIMER1_CPU_64, TIMER1_CPU_256, TIMER1_CPU_1024
}Timer1_Prescaler;

/* Enum to Configure Timer1 mode */
typedef enum{

	TIMER1_NORMAL, TIMER1_PWM_PHASE_8bit, TIMER1_PWM_PHASE_9bit, TIMER1_PWM_PHASE_10bit, TIMER1_CTC
}Timer1_Mode;

/* Structure to Configure Timer1 */
typedef struct {

 uint16 initial_value;

#ifdef TIMER1_COMPARE_MODE

 uint16 compare_value; /* it will be used in compare mode only */
#endif

 Timer1_Prescaler prescaler;

 Timer1_Mode mode;
} Timer1_ConfigType;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * Function to initialize the Timer driver
 */
void Timer1_init(const Timer1_ConfigType * Config_Ptr);

/*
 * Description :
 * Function to disable the Timer1
 */
void Timer1_deInit(void);

/*
 * Description :
 * Function to set the Call Back function address
 */
void Timer1_setCallBack(void(*a_ptr)(void));

#endif /* TIMER1_H_ */

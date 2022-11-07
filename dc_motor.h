 /******************************************************************************
 *
 * Module: DC_MOTOR
 *
 * File Name: dc_motor.h
 *
 * Description: Header file for the AVR DC_MOTOR driver
 *
 * Author: Mohamed Khaled
 *
 *******************************************************************************/

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define DcMotor_IN1_PORT_ID PORTB_ID
#define DcMotor_IN1_PIN_ID  PIN0_ID

#define DcMotor_IN2_PORT_ID PORTB_ID
#define DcMotor_IN2_PIN_ID  PIN1_ID

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

/* enum that represent DC Motor State */
typedef enum{
	CW, A_CW, STOP
}DcMotor_State;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * Initiate DC motor:
 * 1. Setup the direction for the two motor pins through the GPIO driver.
 * 2. Stop at the DC-Motor at the beginning through the GPIO driver.
 */
void DcMotor_Init(void);

/*
 * Description :
 * The function is responsible for:
 * 1. Rotate the DC Motor CW/ or A-CW or stop the motor based on the state input state value
 * 2. Send the required duty cycle to the PWM driver based on the required speed value
 */
void DcMotor_Rotate(DcMotor_State state,uint8 speed);

#endif /* DC_MOTOR_H_ */

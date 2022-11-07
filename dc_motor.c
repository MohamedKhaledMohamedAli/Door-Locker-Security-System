 /******************************************************************************
 *
 * Module: DC_MOTOR
 *
 * File Name: dc_motor.c
 *
 * Description: Source file for the AVR DC_MOTOR driver
 *
 * Author: Mohamed Khaled
 *
 *******************************************************************************/

#include "dc_motor.h"
#include "gpio.h"
#include "pwm_timer0.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Initiate DC motor:
 * 1. Setup the direction for the two motor pins through the GPIO driver.
 * 2. Stop at the DC-Motor at the beginning through the GPIO driver.
 */
void DcMotor_Init(void){

	/* Setup the direction for the two motor pins through the GPIO driver */
	GPIO_setupPinDirection(DcMotor_IN1_PORT_ID, DcMotor_IN1_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(DcMotor_IN2_PORT_ID, DcMotor_IN2_PIN_ID, PIN_OUTPUT);

	/* Stop at the DC-Motor at the beginning through the GPIO driver */
	GPIO_writePin(DcMotor_IN1_PORT_ID, DcMotor_IN1_PIN_ID, LOGIC_LOW);
	GPIO_writePin(DcMotor_IN2_PORT_ID, DcMotor_IN2_PIN_ID, LOGIC_LOW);
}

/*
 * Description :
 * The function is responsible for:
 * 1. Rotate the DC Motor CW/ or A-CW or stop the motor based on the state input state value
 * 2. Send the required duty cycle to the PWM driver based on the required speed value
 */
void DcMotor_Rotate(DcMotor_State state,uint8 speed){
	switch(state){
	case CW:

		/* Rotate the DC Motor CW */
		GPIO_writePin(DcMotor_IN1_PORT_ID, DcMotor_IN1_PIN_ID, LOGIC_HIGH);
		GPIO_writePin(DcMotor_IN2_PORT_ID, DcMotor_IN2_PIN_ID, LOGIC_LOW);

		break;
	case A_CW:

		/* Rotate the DC Motor A-CW */
		GPIO_writePin(DcMotor_IN1_PORT_ID, DcMotor_IN1_PIN_ID, LOGIC_LOW);
		GPIO_writePin(DcMotor_IN2_PORT_ID, DcMotor_IN2_PIN_ID, LOGIC_HIGH);

		break;
	case STOP:

		/* Stop the DC Motor */
		GPIO_writePin(DcMotor_IN1_PORT_ID, DcMotor_IN1_PIN_ID, LOGIC_LOW);
		GPIO_writePin(DcMotor_IN2_PORT_ID, DcMotor_IN2_PIN_ID, LOGIC_LOW);

		break;
	}

	/* Send the required duty cycle to the PWM driver */
	PWM_TIMER0_Start(speed);
}

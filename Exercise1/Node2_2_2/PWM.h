/*
 * PWM.h
 *
 * Created: 29.10.2014 09:23:57
 *  Author: brynjare
 */ 


#ifndef PWM_H_
#define PWM_H_

#include <SPI/SPI.h>
#include <interrupt.h>
#include <Servo.h>
#include "mcp2515_arduino.h"
#include "MCP2515.h"
#include "SPI_arduino.h"
#include "CAN_arduino.h"

void pwm_set_servo(Servo* myservo, int pos);
void pwm_set_servo2(Servo* myservo, int pos);

#endif /* PWM_H_ */
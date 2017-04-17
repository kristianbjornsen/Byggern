/*
 * motor.h
 *
 * Created: 04.11.2014 13:38:20
 *  Author: mikaln
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

#include <Arduino.h>

#define DAC 0b0101000

void init_motor();
void motor_write(int y);
uint16_t motor_read();
void set_direction(int y);
void motor_write_positon_P(int y);
void set_speed(int speed);
void motor_write_positon_PID(int y);


void init_motor_2();
uint16_t motor_read_2();
void motor_write_positon_P_2(int y);
void set_speed_2(int speed);
void set_direction_2(int y);


#endif /* MOTOR_H_ */
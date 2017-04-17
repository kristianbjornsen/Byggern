/*
 * MOTOR.h
 *
 * Created: 22.10.2014 11:29:15
 *  Author: ugurac
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

void MOTOR_init();
void MOTOR_set_speed(int speed);
void MOTOR_enable(int);
void MOTOR_joystick();
void MOTOR_change_direction(int dir);
void MOTOR_byte_select(int byte);
void MOTOR_BLUETOOTH();
void MOTOR_CONTROL();
void MOTOR_encoder_reset(int reset);
int16_t MOTOR_read_encoder();
void MOTOR_move_to(int ref);
void MOTOR_calib();
void MOTOR_joystick2();
#endif /* MOTOR_H_ */
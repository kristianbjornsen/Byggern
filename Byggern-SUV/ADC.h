/*
 * ADC.h
 *
 * Created: 14.09.2014 10:05:39
 *  Author: viliusc
 */ 


#ifndef ADC_H_
#define ADC_H_
typedef enum Direction {RIGHT, LEFT, UP, DOWN, NEUTRAL} Direction;
#include "CAN.h"

int16_t JOY_read(char value);
void JOY_init(void);
void JOY_calib(void);
void JOY_print_direction(void);
Direction JOY_direction(void);
int JOY_button();
int JOY_update(can_message_t msg);
int JOY_get(char value);
int16_t JOY_read_J2(char value);


#endif /* ADC_H_ */
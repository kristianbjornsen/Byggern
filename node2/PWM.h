/*
 * PWM.h
 *
 * Created: 15.10.2014 11:18:39
 *  Author: ugurac
 */ 


#ifndef PWM_H_
#define PWM_H_


void PWM_generate(int x,int x_negative );
void PWM_init();
void PWM_send(unsigned int signal_width);
void PWM_generate_bluetooth(int value);
void PWM_generate_J2(int x);


#endif /* PWM_H_ */
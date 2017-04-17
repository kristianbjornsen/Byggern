/*
 * motor.cpp
 *
 * Created: 04.11.2014 13:38:10
 *  Author: mikaln
 */ 

#include "motor.h"
#include <Wire.h>

int max_pos = 8450;
int min_pos = 390;
int Kp = 1;
int Kd = 0.5;
int Ki = 0.7;
int dt = 20;
int AntiWindUpLimit = 20;

void init_motor() {
	Wire.begin();
	
	DDRF    |=  (1<<DDF2)   // Output encoder enable
            |   (1<<DDF3)   // Counter reset
            |   (1<<DDF4)   // Encoder LSB/MSB select
            |   (1<<DDF5)   // Motor enable
            |   (1<<DDF6);  // Motor direction

	DDRK = 0;		
	
	PORTF &= ~(1 << PF2); // Output encoder enable
	PORTF |= (1 << PF3);  // Counter reset
	PORTF &= ~(1 << PF4); // Encoder LSB/MSB select
	PORTF |= (1 << PF5);  // Motor enable
	
	set_direction(1);
	set_speed(100);
	delay(3000);
	set_speed(0);
	PORTF &= ~(1 << PF3);  // Counter reset
	delayMicroseconds(200);
	PORTF |= (1 << PF3);  // Counter reset

}

void motor_write_positon_P(int y) {
// MÅ SJÅAST ETTER BRETT
	
	static int pos_1, pos_2;
	int input;
	int current_speed;
	static int speed = 0;

	int error;

	pos_1 =  motor_read();

	delay(dt);
	
	pos_2 = motor_read();
	
	current_speed = ((double)pos_2-(double)pos_1); 
	current_speed /= ((double)dt/8);

	error = y - current_speed;
	
	speed = 1*error;
	
	set_direction(speed);
	
	if (abs(speed)> 5 && abs(speed) < 100){
		if(speed < 50)
			set_speed(50);
		else
			set_speed(abs(speed));
	}
	else if (abs(speed) > 100)
		set_speed(100);
	else
		set_speed(speed);


}

void set_direction(int y){
	if( y > 0)
		PORTF |= (1 << PF6);  // Direction right
	else{
		PORTF &= ~(1 << PF6);// Direction left
	}
}
	
uint16_t motor_read(){
	
	uint8_t lsb;
	uint16_t msb;
	uint16_t pos;
	
	PORTF &= ~(1 << PF2); // Output encoder enable
	PORTF &= ~(1 << PF4); // Encoder LSB/MSB select
	delayMicroseconds(20);
	
	//msb = (PINK) << 8;
	msb = PINK;
	msb = ((msb & 0x55) << 1) | ((msb & 0xaa) >> 1);
	msb = ((msb & 0x33) << 2) | ((msb & 0xcc) >> 2);
	msb = ((msb & 0x0f) << 4) | ((msb & 0xf0) >> 4);
	
	PORTF |= (1 << PF4); // Encoder LSB/MSB select
	delayMicroseconds(20);
	
	lsb = PINK;
	lsb = ((lsb & 0x55) << 1) | ((lsb & 0xaa) >> 1);
	lsb = ((lsb & 0x33) << 2) | ((lsb & 0xcc) >> 2);
	lsb = ((lsb & 0x0f) << 4) | ((lsb & 0xf0) >> 4);
	
	PORTF &= ~(1 << PF3);  // Counter reset
	PORTF |= (1 << PF3);  // Counter reset
	PORTF |= (1 << PF2);
	
	pos = lsb + (msb << 8);
	
	return pos;
}

void set_speed(int speed){
		
		Wire.beginTransmission(DAC);
		Wire.write(0);
		Wire.write(abs(speed));
		Wire.endTransmission();
}

void motor_write_positon_PID(int y) {

	
	static int pos_1, pos_2;
	int input;
	int current_speed;
	static int speed = 0;

	static int error;
	int previousError, integralPart;
	
	pos_1 =  motor_read();

	delay(dt);
	
	pos_2 = motor_read();
	
	current_speed = ((double)pos_2-(double)pos_1);
	current_speed /= ((double)dt/8);
	

	previousError = error;
	error = y - current_speed;
	
	integralPart += error;
	if(integralPart > AntiWindUpLimit)
		integralPart = AntiWindUpLimit;
	
	if(integralPart < -AntiWindUpLimit)
		integralPart = -AntiWindUpLimit;
	
	if(current_speed > 5)
		integralPart = 0;
		
	speed = Kp*error + Kd*(error-previousError) + Ki*integralPart;

	set_direction(speed);


	if(abs(speed) > 150)
		set_speed(120);
	else
		set_speed(speed);

	
}

void init_motor_2() {
	Wire.begin();
	
	DDRA    |=  (1<<DDA2)   // Output encoder enable
			|   (1<<DDA3)   // Counter reset
			|   (1<<DDA4)   // Encoder LSB/MSB select
			|   (1<<DDA5)   // Motor enable
			|   (1<<DDA6);  // Motor direction

	DDRC = 0;
	
	PORTA &= ~(1 << PA2); // Output encoder enable
	PORTA |= (1 << PA3);  // Counter reset
	PORTA &= ~(1 << PA4); // Encoder LSB/MSB select
	PORTA |= (1 << PA5);  // Motor enable
	
	set_direction_2(1);
	set_speed_2(100);
	delay(3000);
	set_speed_2(0);
	PORTA &= ~(1 << PA3);  // Counter reset
	delayMicroseconds(200);
	PORTA |= (1 << PA3);  // Counter reset

}

uint16_t motor_read_2(){
	
	uint8_t lsb;
	uint16_t msb;
	uint16_t pos;
	
	PORTA &= ~(1 << PA2); // Output encoder enable
	PORTA &= ~(1 << PA4); // Encoder LSB/MSB select
	delayMicroseconds(20);
	
	//msb = (PINK) << 8;
	msb = PINC;
	msb = ((msb & 0x55) << 1) | ((msb & 0xaa) >> 1);
	msb = ((msb & 0x33) << 2) | ((msb & 0xcc) >> 2);
	msb = ((msb & 0x0f) << 4) | ((msb & 0xf0) >> 4);
	
	PORTA |= (1 << PA4); // Encoder LSB/MSB select
	delayMicroseconds(20);
	
	lsb = PINC;
	lsb = ((lsb & 0x55) << 1) | ((lsb & 0xaa) >> 1);
	lsb = ((lsb & 0x33) << 2) | ((lsb & 0xcc) >> 2);
	lsb = ((lsb & 0x0f) << 4) | ((lsb & 0xf0) >> 4);
	
	PORTA &= ~(1 << PA3);  // Counter reset
	PORTA |= (1 << PA3);  // Counter reset
	PORTA |= (1 << PA2);
	
	pos = lsb + (msb << 8);
	
	return pos;
}

void motor_write_positon_P_2(int y) {

	
	static int pos_1, pos_2;
	int input;
	int current_speed;
	static int speed = 0;

	int error;

	pos_1 =  motor_read_2();

	delay(dt);
	
	pos_2 = motor_read_2();
	
	current_speed = ((double)pos_2-(double)pos_1);
	current_speed /= ((double)dt/8);
	

	error = y - current_speed;
	
	speed = 0.9*error;
	
	set_direction_2(speed);

	if (abs(speed)> 5 && abs(speed) < 100){
		if(speed < 50)
			set_speed_2(50);
		else
			set_speed_2(abs(speed));
	}
	else if (abs(speed) > 100)
		set_speed_2(100);
	else
		set_speed_2(speed);
}
		


void set_speed_2(int speed){

	Wire.beginTransmission(DAC);
	Wire.write(1);
	Wire.write(abs(speed));
	Wire.endTransmission();
}

void set_direction_2(int y){
	if( y > 0)
		PORTA |= (1 << PA6);  // Direction right
	else
		PORTA &= ~(1 << PA6);// Direction left
	
}
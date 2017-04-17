/*
 * PWM.cpp
 *
 * Created: 29.10.2014 09:23:42
 *  Author: brynjare
 */ 

#include "PWM.h"


void pwm_set_servo(Servo* myservo, int pos){
		
		static int old_pos;

		if(abs(pos-old_pos) > 5){
			old_pos = pos;
			pos = map(pos,100,-100,45,135);
			myservo->write(pos);            // sets the servo position according to the scaled value
			delay(50);
		}
}

void pwm_set_servo2(Servo* myservo, int pos){
	
	static int old_pos2;

	if(abs(pos-old_pos2) > 5){
		old_pos2 = pos;
		pos = map(pos,100,-100,45,135);
		myservo->write(pos);            // sets the servo position according to the scaled value
		delay(50);
	}
}
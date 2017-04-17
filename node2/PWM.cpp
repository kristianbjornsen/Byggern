/*
 * PWM.cpp
 *
 * Created: 15.10.2014 11:18:28
 *  Author: ugurac
 */ 

#include "MCP2515.h"
#include "Arduino.h"
#include "Servo.h"
#include "PWM.h"
#include "JOYSTICK.h"
can_message_t position;
Servo servo;
unsigned int width;
void PWM_init(){
	
	servo.attach(3);
	
}

void PWM_generate(int x,int x_negative ){
		//Serial.print("Position x= ");Serial.println(x);
		//Serial.print("Position x_negative= ");Serial.println(x_negative);
		if(x_negative<10&&x<10){
			width=1.5*100;
		}else if(x==0){
			width=(((2.1-1.5)/100)*x_negative+1.5)*100;
		
		}else if(x_negative==0){
			width=((1.5-((2.1-1.5)/100)*x))*100;
		
		}
		PWM_send(width*10);
	
}
void PWM_generate_J2(int x){
	//x goes from 0 to 100
	width=(0.9+x*(2.1-0.9)/100)*1000;
	PWM_send(width);
	
}
void PWM_generate_bluetooth(int B_val){
	width=((2.1-0.9)/16*(B_val-70)+0.9)*1000;
	//Serial.print("width= ");Serial.println(width);
	PWM_send(width);
	
	
}

void PWM_send(unsigned int signal_width){

	//Serial.println(signal_width);
	if(signal_width<=2100 && signal_width>=900){
		servo.write(signal_width);
	}else{
		//Serial.println("PULSE_WIDTH over the limit");
	}
}
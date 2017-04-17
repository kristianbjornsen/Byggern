/*
 * JOYSTICK.cpp
 *
 * Created: 15.10.2014 11:12:40
 *  Author: ugurac
 */ 
#include "MCP2515.h"
#include "Arduino.h"
int pinT=7;
int Joystick;
can_message_t Button;
int Joy_x,Joy_y,SHOOT;
void JOYSTICK_init(){
	Joy_x=50;
	Joy_y=50;
	SHOOT=0;
	Joystick=2; //joystick 2 as default can change to joystick 1
	pinMode(pinT,OUTPUT);
	digitalWrite(pinT,HIGH);
}

void JOYSTICK_read(can_message_t *joy){
	CAN_data_receive(joy);
}
void JOYSTICK_button(){
	if(SHOOT){
		digitalWrite(pinT,LOW);
		//Serial.println("SHooting");
	}else{
		digitalWrite(pinT,HIGH);
	}
	
}
void JOYSTICK_button_BT(){
	digitalWrite(pinT,LOW);
	delay(100);
	digitalWrite(pinT,HIGH);
}

int JOYSTICK_choose(){
	return Joystick;
	
}
void JOYSTICK_change(can_message_t msg){
	if(msg.data[2]==1){
		Joystick=2;
	}else if(msg.data[2]==0){
		Joystick=1;
	}
}
void JOYSTICK_update(can_message_t msg){
	Joy_x=msg.data[1];
	Joy_y=msg.data[0];
	if(JOYSTICK_choose()==1){
		SHOOT=msg.data[4];
	}
	
}
int JOYSTICK_get(char value){
	switch(value){
		case 'x':
			return Joy_x;
		case 'y':
			return Joy_y;
		case 's':
			return SHOOT;
	}
	
}
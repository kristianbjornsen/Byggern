/*
 * MOTOR.cpp
 *
 * Created: 22.10.2014 11:29:01
 *  Author: ugurac
 */
#include <Arduino.h>

#include <Wire/Wire.h>
#include "MOTOR.h"
#include "MCP2515.h"
#include "JOYSTICK.h"
#include "Bluetooth.h"
#include "PWM.h"
#include "GAME.h"

can_message_t joy;

#define MOTOR_OE PF2 //!OE Active low output enable of encoder
#define MOTOR_RST PF3 //Active low reset of encoder
#define MOTOR_DIR PF6 //Sets the direction
#define MOTOR_EN PF5 //Enables motor
#define MOTOR_SEL PF4 //Select Hi/Lo byte of encoder

int BLUETOOTH_STATUS,PWM;
int MOTOR_ENCODER_CALIB_RIGHT,MOTOR_ENCODER_CALIB_LEFT,MOTOR_CALIB_MID;
int Kp_value;
void MOTOR_init(){
	Wire.begin();
	PWM=1500;
	Kp_value = 2;
	BLUETOOTH_STATUS='d';
	//DDRK=0x00;
	DDRF=(1<<MOTOR_EN)|(1<<MOTOR_DIR)|(1<<MOTOR_OE)|(1<<MOTOR_RST)|(1<<MOTOR_SEL);	//Output; 5=EN, 6=DIR , 2=!OE,3=!RST
	MOTOR_encoder_reset(0);
	MOTOR_calib();
	MOTOR_enable(1);
		//reset encoder
	 //
}
void MOTOR_calib(){
	MOTOR_enable(1);
	MOTOR_change_direction(1);
	MOTOR_set_speed(500);
	delay(2000);
	MOTOR_enable(0);
	delay(500);
	MOTOR_encoder_reset(1);
	delay(100);
	MOTOR_encoder_reset(0);
	MOTOR_ENCODER_CALIB_RIGHT=MOTOR_read_encoder();
	MOTOR_enable(1);
	MOTOR_change_direction(0);
	MOTOR_set_speed(500);
	delay(2000);
	MOTOR_enable(0);
	delay(100);
	MOTOR_ENCODER_CALIB_LEFT=MOTOR_read_encoder();
	Serial.println(MOTOR_ENCODER_CALIB_RIGHT);
	Serial.println(MOTOR_ENCODER_CALIB_LEFT);
	MOTOR_CALIB_MID=(MOTOR_ENCODER_CALIB_LEFT-MOTOR_ENCODER_CALIB_RIGHT)/2;
	Serial.println(MOTOR_CALIB_MID);
	
}
void MOTOR_set_speed(int speed){
	byte send;
	send=speed;
	if(speed>150){
		speed=150;
	}else if(speed<0){
		Serial.println("Wrong speed given");
		return;
	}
	Wire.beginTransmission(0b0101000);
	Wire.write(0);
	Wire.write(speed);
	Wire.endTransmission();
}

void MOTOR_enable(int enable){
		if(enable ){
		PORTF|=(1<<MOTOR_EN);
		}
		else
			{PORTF&=~(1<<MOTOR_EN);}
		  //!OE
}
void MOTOR_change_direction(int dir){
//	dir=1 left, dir=0 right
	if(dir){
		PORTF|=(1<<MOTOR_DIR);
		
	}else if(dir==0){
		PORTF&=~(1<<MOTOR_DIR);
		
	}else
		Serial.println("WRONG input");
	
}
void MOTOR_joystick(){
	
	JOYSTICK_read(&joy);
	if(joy.id==1){
		PWM_generate(joy.data[0],joy.data[1]);
		int y,y_negative;
		y=joy.data[3];
		y_negative=joy.data[2];
		//Serial.print("Y verdi: ");Serial.println(y_negative);
		MOTOR_move_to(100-y_negative+y);
	// 	if(y>0 && y_negative==0){
	// 		//MOTOR_move_to(y);
	// 		MOTOR_enable(1);
	// 		MOTOR_change_direction(0);
	// 		MOTOR_set_speed(y);
	// 	}else if(y_negative>0 && y==0){
	// 		//MOTOR_move_to(y_negative*-1);
	// 		MOTOR_enable(1);
	// 		MOTOR_change_direction(1);
	// 		MOTOR_set_speed(y_negative);
	// 	}else{
	// 		//MOTOR_enable(0);
	// 	}
	}
	
	
	
	
}
void MOTOR_joystick2(){
		PWM_generate_J2(100-JOYSTICK_get('x'));;
		int y;
		y=JOYSTICK_get('y');
		//Serial.print("Y verdi: ");Serial.println(y);
		MOTOR_move_to(y*2);
		JOYSTICK_button();
	
}
void MOTOR_BLUETOOTH(){
	int B_val,Xverdi;
	B_val=BLUETOOTH_read();
	if(B_val<=86 && B_val>=70){
		PWM=B_val;
	}
	else if(B_val==115){
		JOYSTICK_button_BT();
		//BLUETOOTH_get_name();
	}else{
	MOTOR_move_to(200-B_val*10);
	}
	PWM_generate_bluetooth(PWM);
	
	
}
void MOTOR_CONTROL(){
	if(GAME_INFO_GET()==1){	
		if(BT_MODE()){
			MOTOR_BLUETOOTH();
		}else if(JOYSTICK_choose()==2){
			MOTOR_joystick2();
		}else{
			MOTOR_joystick2();
		}
	}
	
}
void MOTOR_move_to(int ref){
 //MAX -12290 min = -7876
	double position=MOTOR_read_encoder();
	
	position=(position)*100/MOTOR_CALIB_MID;
	//Serial.print("position reg: ");Serial.println(position);
	//Serial.println();
	//Serial.print("ref: ");Serial.println(ref);
	//int Kp=3; //Change this if motor dont move
	int e=(position-ref);
	if(e<5 && e>-5){
		MOTOR_set_speed(0);
	}
	else if(e<0){
		MOTOR_enable(1);
		MOTOR_change_direction(0);
		MOTOR_set_speed(Kp_value*e*-1);
	}else{
		MOTOR_enable(1);
		MOTOR_change_direction(1);
		MOTOR_set_speed(Kp_value*e);
	}
	
	 
	//Serial.print("e: ");Serial.println(e);
}

void MOTOR_Kp(int Kp) {
	Kp_value = Kp;
	Serial.print("Kp value changed to%d "); Serial.println(Kp_value);
}

void MOTOR_byte_select(int byte){
	
	//byte=0 high byte, byte=1 LOW byte
	if(byte==0){
		PORTF&=~(1<<MOTOR_SEL);	
	}else{
		PORTF|=(1<<MOTOR_SEL);
	}
		
	
}

unsigned char reverse_byte(unsigned char x)
{
	static const unsigned char table[] = {
		0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,
		0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
		0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8,
		0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
		0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4,
		0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
		0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec,
		0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
		0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2,
		0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
		0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea,
		0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
		0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6,
		0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
		0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee,
		0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
		0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1,
		0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
		0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9,
		0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
		0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5,
		0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
		0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed,
		0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
		0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3,
		0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
		0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb,
		0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
		0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7,
		0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
		0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef,
		0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff,
	};
	return table[x];
}


int16_t MOTOR_read_encoder(){
	uint8_t data_high;
	uint8_t data_low;
	int16_t data,OH,OL;
	uint16_t bit;
	
	PORTF&=~(1<<MOTOR_OE);
	MOTOR_byte_select(0);
	delayMicroseconds(20);
	data_high=PINK;
	data_high = reverse_byte(data_high);
	MOTOR_byte_select(1);
	delayMicroseconds(20);
	data_low=PINK;
	data_low = reverse_byte(data_low);
	MOTOR_encoder_reset(1);
	MOTOR_encoder_reset(0);
	PORTF|=(1<<MOTOR_OE);
	data=data_high<<8|data_low;
	return data;
	
}

void MOTOR_encoder_reset(int reset){
	//MOTOR RESET OUTPUT 0 if reset 1, 1 if reset 0
	if(reset){
		PORTF&=~(1<<MOTOR_RST);
		
	}else
		PORTF|=(1<<MOTOR_RST);
	
}
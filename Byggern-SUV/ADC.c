/*
 * ADC.c
 *
 * Created: 14.09.2014 10:05:54
 *  Author: viliusc
 */
#define F_CPU 4915200
#include "CAN.h"
#include "ADC.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>


int16_t calib_const_x = 0;
int16_t calib_const_y = 0;
uint8_t j_right=255;
uint8_t j_left=0;
uint8_t j_up=255;
uint8_t j_down=0;
//typedef enum Direction {RIGHT, LEFT, UP, DOWN, NEUTRAL} Direction;
int joyx,joyy;
void JOY_init(void){
	joyx=50;
	joyy=50;
}
int16_t JOY_read(char value){
	volatile uint8_t *adc_adress= (uint8_t *) 0x1400;
	int16_t data;
	switch (value){
		case 'x':
			adc_adress[0x04]= 4;
			_delay_ms(1);
			data = adc_adress[4] - calib_const_x;
			if (calib_const_x > 0) {data=data-127;}
			break;
		case 'y':
			adc_adress[0x05]= 5;
			_delay_ms(1);
			data = adc_adress[5] - calib_const_y;
			if (calib_const_y > 0) {data=data-127;}
			break;
		default:
			printf("Error");
			                                                 
	}
	if(calib_const_y>0&&calib_const_x>0)
		if(data>0)
			data=data*100/123.0; //Returns in percent value
		else
			data=data*100/131.0; //Returns in percent value
	//printf("joydata: %d\n",data);
	return data;
	
}

//Read for Joytick on adc
int16_t JOY_read_J2(char value){
	volatile uint8_t *adc_adress= (uint8_t *) 0x1400;
	int16_t data;
	switch (value){
		case 'x':
			adc_adress[0x04]= 4;
			_delay_ms(1);
			data = adc_adress[4];
			data=100-(int)((data)*100/255);
			//printf("JOYX: %d\n",data);
		break;
		case 'y':
			adc_adress[0x05]= 5;
			_delay_ms(1);
			data = adc_adress[5];
			data=100-(int)((data+calib_const_y)*100/255);
			//printf("JOYY: %d\n\n",data);
		break;
		default:
			printf("Error");
		
	}
	return data;
	//This give 0 - 100 not -100 to 100
	
}

//Calibrating joystick
void JOY_calib(void){
	calib_const_x = JOY_read('x')- 127;
	calib_const_y = JOY_read('y') -127;	
	printf("Joystick calibrated with calib_x=%d and calib_y=%d\n\r",calib_const_x,calib_const_y);
}

//Returns joystick direction
Direction JOY_direction(void) {
	int8_t x_pos = JOY_get('x');
	int8_t y_pos = JOY_get('y');
	if (x_pos <10)
		return LEFT;
	if (x_pos >90)
		return RIGHT;
	if (y_pos <10)	
		return DOWN;
	if (y_pos>90)
		return UP;
	return NEUTRAL;	
}

//Prints direction
void JOY_print_direction(){
	
	if(JOY_direction()==RIGHT)
		printf("Right\n");
	
	else if(JOY_direction()==LEFT)
		printf("Left\n");
	
	else if(JOY_direction()==UP)
		printf("Up\n");
	
	else if (JOY_direction() == DOWN)
		printf("Down\n");
	
	else
		printf("Neutral\n");
		
}

//checks if button is pressed
int JOY_button(){
	if(!(PINB&(1<<PINB3))){
		return 1;
	}
	return 0;
}

//Updates joystick button values
int JOY_update(can_message_t msg){
	joyx=msg.data[0];
	joyy=msg.data[1];
	//printf("joyx: %d, joyy: %d",joyx,joyy);
}

//Returns the global values for the joystick
int JOY_get(char value){
	if(value=='x'){
		return joyx;
	}else if(value=='y'){
		return joyy;
	}
	
}
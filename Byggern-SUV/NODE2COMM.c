/*
 * NODE2COMM.c
 *
 * Created: 15.10.2014 10:31:51
 *  Author: ugurac
 */ 
#include <avr/io.h>
#include "CAN.h"
#include "ADC.h"
can_message_t position;
void NODE2COMM_init(){
position.id=1;
position.length=5;
position.data[0]=0; //x position
position.data[1]=0; //x_negative position //old version	
position.data[2]=0;	//y position
position.data[3]=0; //y_negative position //old version
position.data[4]=0; //0 false, 1 True for joystick button
}
void NODE2COMM_read_joystick_position(){
	
	position.data[0]=JOY_get('x');
	position.data[1]=JOY_get('y');
	CAN_message_send(position);
}
void NODE2COMM_read_joystick_position2(){
	
	int data_x,data_y;
	data_x=JOY_read_J2('x');
	data_y=JOY_read_J2('y');
	position.data[0]=data_x;
	position.data[1]=data_y;
	position.data[4]=JOY_button();
	CAN_message_send(position);
}
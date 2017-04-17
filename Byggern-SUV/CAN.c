/*
 * CAN.c
 *
 * Created: 28.09.2014 11:14:49
 *  Author: viliusc
 */ 

#include <AVR/io.h>
#include <AVR/interrupt.h>
#include "MCP2515.h"
#include "CAN.h"
#include "GAME.h"
int send_message;
int joystick_id,IR_id;
void CAN_init(){
	send_message=1;
	//SPI_init();
	joystick_id=15;
	MCP2515_init();
	CAN_interrupt_init();
	
}

int CAN_message_send(can_message_t msg){
	
	if(send_message){ //Waiting until interrupt occurs
		MCP2515_write(msg.id,0x31);
		MCP2515_write(msg.length,0x35);
		//MCP2515_request_to_send(0x80+msg.id);
		uint8_t i;
		//printf("%d\n",msg.data[0]);
		for(i=0; i<msg.length; i++){
			MCP2515_write(msg.data[i],0x36+i);
		}
		MCP2515_request_to_send(1);
		send_message=0;
		
		return 1;
		
	}
	
	return 0;
}
void CAN_data_receive(can_message_t *receive){
	receive->id=MCP2515_read(0x61);
	receive->length=MCP2515_read(0x65);
	uint8_t i;
	for(i=0; i<receive->length; i++){
		receive->data[i]=MCP2515_read(0x66+i);
	}
	MCP2515_bit_modify(MCP_CANINTF,MCP_TRANCIEVEBUFFER_0_INTERRUPT, 0x00);
	MCP2515_bit_modify(MCP_CANINTF,MCP_RECIEVEBUFFER_0, 0x00); //empty the rxb0 buffer

	
}
void CAN_interrupt_init(){
	
	EMCUCR=(1<<ISC2);
	GICR=(1<<INT2);
	sei();
}
void CAN_allowed_to_send(){
	
	send_message=1;
	
}
void CAN_recieve(){
	can_message_t read;
	CAN_data_receive(&read);
	switch(read.id) {
		case 123:
			break;
		case 100:
			GAME_highscore_name(read); // Send new highscore name
			break;
		case 20: //Button info
			GAME_button_handler(read);
			GAME_JOYSTICK(read);
			break;
		case 15: //Joystick from USB card info
			JOY_update(read);
			break;
		case 5: //IR data
			GAME_IR_update(read);
			break;
	}
}
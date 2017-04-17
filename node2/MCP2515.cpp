/*
 * MCP2515.cpp
 *
 * Created: 14.10.2014 11:24:58
 *  Author: ugurac
 */ 
/*
 * MCP2515.c
 *
 * Created: 12.10.2014 16:40:45
 *  Author: shivjr
 */
#include "SPI.h"
#include "MCP2515.h" 
#include "Bluetooth.h"
#include "JOYSTICK.h"
#include "GAME.h"
/*
CS=53
MOSI=51
MISO=50
SCK=?


*/
#define SLAVESELECT 53
#define CANCTRL 0x0F
#define NORMALMODE 0x00
//MCP2515 Command Bytes
#define RESET 0xC0
#define READ 0x03
#define READ_RX_BUFFER 0x90
#define WRITE 0x02
#define LOAD_TX_BUFFER 0x40
#define RTS 0x80
#define READ_STATUS 0xA0
#define RX_STATUS 0xB0
#define BIT_MODIFY 0x05


void MCP2515init(){
	unsigned int data;
	SPI.begin();
	digitalWrite(SLAVESELECT,LOW);
	
	MCP2515bit_modify(CANCTRL,MCP_OPERATIONMODEMASK, CONFIGMODE);
	SPI.transfer(RESET);
	MCP2515bit_modify(MCP_RXB0CTRL, 0b01101000, 0xff);
	MCP2515bit_modify(CANCTRL,MCP_OPERATIONMODEMASK, NORMALMODE);
	MCP2515bit_modify(MCP_CANINTE, MCP_RECIEVEBUFFER_0_INTERRUPT, 0xff);
// 	MCP2515write(0x0F,0b10000000);
// 	MCP2515write(0x60, 0b01100000);
	//MCP2515write(0x0F,0b01000000);
	digitalWrite(SLAVESELECT,HIGH);
	
	
}

unsigned int MCP2515read(unsigned int address){
	unsigned int data;
	digitalWrite(SLAVESELECT,LOW);
	SPI.transfer(READ);
	SPI.transfer(address);
	data=SPI.transfer(0);
	digitalWrite(SLAVESELECT,HIGH);
	return data;
}
void MCP2515write(unsigned int data,unsigned int address){
	
	digitalWrite(SLAVESELECT,LOW);
	SPI.transfer(WRITE);
	SPI.transfer(address);
	SPI.transfer(data);
	digitalWrite(SLAVESELECT,HIGH);	
}
void MCP2515request_to_send(unsigned int buffer){
	digitalWrite(SLAVESELECT,LOW);
	SPI.transfer(0b10000001);//SPI.transfer(RTS|buffer);
	digitalWrite(SLAVESELECT,HIGH);
}

void MCP2515bit_modify(unsigned int address, unsigned int mask,unsigned data){
	digitalWrite(SLAVESELECT,LOW);
	SPI.transfer(BIT_MODIFY);
	SPI.transfer(address);
	SPI.transfer(mask);
	SPI.transfer(data);
	digitalWrite(SLAVESELECT,HIGH);
}

void CAN_SEND(can_message_t msg){
	MCP2515write(msg.id,0x31);
	MCP2515write(msg.length,0x35);
	unsigned int i;
	for(i=0; i<msg.length; i++){
		MCP2515write(msg.data[i],0x36+i);
	}
	
	MCP2515request_to_send(1);
	/*digitalWrite(SLAVESELECT,LOW);
	SPI.transfer(0xA0);
	Serial.println(SPI.transfer(0));
	digitalWrite(SLAVESELECT,HIGH);*/
	//MCP2515bit_modify(0x30, (1 << 3), 0xFF);
}

void CAN_data_receive(can_message_t *receive){
	uint8_t buff = (MCP2515read(0x2C) & ( 3 << 0));
	
	//if(buff == 1){
		receive->id=MCP2515read(0x61);
		receive->length=MCP2515read(0x65);
		unsigned int i;
		//Serial.println("Message Length is "); Serial.println(receive->length);
		for(i=0; i<receive->length; i++){
			receive->data[i]=MCP2515read(0x66+i);
		}
		MCP2515bit_modify(MCP_CANINTF,MCP_RECIEVEBUFFER_0_INTERRUPT, 0x00);
// 	}
// 	else if(buff == 2){
// 		receive->id=MCP2515read(0x71);
// 		receive->length=MCP2515read(0x75);
// 		unsigned int i;
// 		for(i=0; i<receive->length; i++){
// 			receive->data[i]=MCP2515read(0x76+i);
// 		}
// 	}
// 	else{
// 		Serial.print("No message, CANINTF == "); Serial.println(MCP2515read(0x2C));
	//}
}

void CAN_read(){
	can_message_t read;
	CAN_data_receive(&read);
	switch(read.id){
		case 20:
			BT_change_status(read);
			JOYSTICK_change(read);
			break;
		case 1:
			if(JOYSTICK_choose()==1)
				JOYSTICK_update(read); //From at162
			break;
		case 123:
			Serial.println("changing kp");
			MOTOR_Kp(read.data[0]);
			break;
		case 15:
			if(JOYSTICK_choose()==2) 
				JOYSTICK_update(read); //FRom USB card
			break;
		case 2:
			GAME_MODUS_UPDATE(read);
			break;
	}
	
}
/*
 *
 * Created: 01.10.2014 12:54:25
 *  Author: viliusc
 */ 

#include <avr/io.h>
#include "SPI.h"
#include "MCP2515.h"


void MCP2515_init(){
	
	SPI_init();
	MCP2515_reset();
	MCP2515_bit_modify(MCP_RXB0CTRL, MCP_RXB0_MASKFILTEROFF, 0xff); //No filter or mask
	//MCP2515_bit_modify(0x60, 0b00000100, 0x00);
	MCP2515_bit_modify(CANCTRL,MCP_OPERATIONMODEMASK, NORMALMODE); //Selecting mode
	MCP2515_bit_modify(MCP_CANINTE, MCP_TRANCIEVEBUFFER_0_INTERRUPT, 0xff); //RECEVIE BUFFER 0 full  - interrupt
	
}
uint8_t MCP2515_read(uint8_t address)
{
	uint8_t result;
	SPI_select(1); // Select CAN-controller
	SPI_send(READ); // Send read command
	SPI_send(address); // Send address
	result = SPI_read(); // Read result
	SPI_select(0); // Deselect CAN-controller
	return result; 
}

void MCP2515_write(uint8_t data,uint8_t address){ //ADDRESS: 36h - 3Dh, 46h - 4Dh, 56h - 5Dh)
	SPI_select(1); // Select CAN-controller
	SPI_send(WRITE); // Send write command
	SPI_send(address); // Send address
	SPI_send(data); //Send data
	SPI_select(0); // Deselect CAN-controller
}
void MCP2515_reset(){
	SPI_select(1); // Select CAN-controller
	SPI_send(RESET);
	SPI_select(0); // Deselect CAN-controller
}
void MCP2515_request_to_send(uint8_t buffer){
	
	SPI_select(1); // Select CAN-controller
	SPI_send(MCP_REQUEST_TO_SEND+buffer);
	SPI_select(0); // Deselect CAN-controlle
	
}

void  MCP2515_bit_modify(uint8_t address, uint8_t mask,uint8_t data){
	SPI_select(1); // Select CAN-controller
	
	SPI_send(BIT_MODIFY); // Send write command
	SPI_send(address); // Send address
	SPI_send(mask); //Send data
	SPI_send(data);
	
	SPI_select(0); // Deselect CAN-controller
}
uint8_t MCP2515_read_status(){
	 //Hva skal vi få ?
	SPI_select(1); // Select CAN-controller
	SPI_send(READ_STATUS);
	uint8_t data=SPI_read();
	SPI_select(0); // Deselect CAN-controller
	return data;
}

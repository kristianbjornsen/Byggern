/*
 * CAN_arduino.cpp
 *
 * Created: 15.10.2014 13:52:08
 *  Author: mikaln
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <Arduino.h>

#include "MCP2515.h"
#include "mcp2515_arduino.h"
#include "SPI_arduino.h"
#include "CAN_arduino.h"

void can_Init_Normal(){
    Serial.print("in can init normal\n");
    mcp2515_init();
    
    //Normal MODE ENABLED
    mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_NORMAL);
    
}

void can_Init_Loopback(){
    
    mcp2515_init();
    
    //LOOPBACK MODE ENABLED
    mcp2515_bit_modify(MCP_CANCTRL,MODE_MASK, MODE_LOOPBACK);
}

void can_message_send(can_message_t* message) {
    
    
    unsigned char buffer = 0b00110110;
    
    mcp2515_bit_modify(0b00110101, 0b00001111, message->length); // TXB0DLC, setter lengde
    
    mcp2515_write(0b00110010, ((message->id & 0x7) << 5));
    mcp2515_write(0b00110001, (message->id >> 3) & 0xFF);
    
    for (uint8_t i = 0; i < message->length; i++) {
        mcp2515_write(buffer+i, message->data[i]);
    }
    
    mcp2515_bit_modify(MCP_TXB0CTRL,0b11111111,0b00001000); // Setter Message transmit request bit
    
    digitalWrite(53, LOW);
    SPI_Send(MCP_RTS_TX0);
    digitalWrite(53, HIGH);
}

int can_error() {
    unsigned char errorByte = 0;
    errorByte = mcp2515_read(0b00110000);
    if(1 == (5<<errorByte) || (1 == (4<<errorByte)))
        return 1;
    
    return 0;
    
}

int can_transmit_complete() {
    unsigned char completeByte = 0;
    return  mcp2515_read(0b00110000)&0b00001110;
    if(1 == (3<<completeByte))
        return 0;
    
    return 1;
}


void can_data_receive(can_message_t * message) {
    
    message->length = 0;
    
    unsigned char readByte = 0;
    unsigned char rxb0dlc = 0;
    unsigned char rxb1dlc = 0;
    readByte = mcp2515_read(MCP_CANINTF);
    rxb0dlc = mcp2515_read(0b01100101);
    rxb1dlc = mcp2515_read(0b01110101);
    
    unsigned char buffer = 0;
    
    if ((readByte & 0b00000001)) { //reading from RXB0
        uint16_t least = ((mcp2515_read(MCP_RXB0SIDL)  & 0b11100000) >> 5);
        uint16_t most  = (mcp2515_read(MCP_RXB0SIDH) << 3);
        uint16_t sum = least + most;
        message->id = sum;
        
        sum = message->id;
        
        message->length =rxb0dlc & 0x0F; // LESER LENGDEN
        
        for (uint8_t i = 0; i < message->length; i++) {
            message->data[i] = mcp2515_read(0b01100110+i); // LESER DATA
        }
        
        mcp2515_bit_modify(MCP_CANINTF, 0b00000001, 0b00000000); //setter tilbake interrupt flag
        
    }
    
    if ((readByte & 0b00000010)) { //reading from RXB1
        uint16_t least = ((mcp2515_read(MCP_RXB1SIDL)  & 0b11100000) >> 5);
        uint16_t most  = (mcp2515_read(MCP_RXB1SIDH) << 3);
        uint16_t sum = least + most;
        message->id = sum;
        
        sum = message->id;
        
        message->length =rxb1dlc&0x0F;
        
        for (uint8_t i = 0; i < message->length; i++) {
            message->data[i] = mcp2515_read(0b01100110+i);// LESER DATA
        }
        
        mcp2515_bit_modify(MCP_CANINTF, 0b00000010, 0b00000000);//setter  interrupt flag = 0
        
    }
}

void can_joystick_receive(can_message_t * message) {
    
    can_data_receive(message);
    
}
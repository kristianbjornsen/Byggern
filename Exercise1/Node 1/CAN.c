/*
 * CAN.c
 *
 * Created: 01.10.2014 11:46:35
 *  Author: brynjare
 */

#include "MCP2515.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "SPI.h"
#include "CAN.h"

void can_Init_Normal(){
    
    mcp2515_init();
    
    //NORMAL MODE ENABLED
    mcp2515_bit_modify(MCP_CANCTRL, 0b11100000, 0b00000000);
    
    
}

void can_Init_Loopback(){
    
    mcp2515_init();
    
    //LOOPBACK MODE ENABLED
    mcp2515_bit_modify(MCP_CANCTRL, 0b11100000, 0b01000000);
    
    
}

void can_message_send(can_message_t *message) {
    
    unsigned char buffer = 0b00110110;
    
    mcp2515_bit_modify(0b00110101, 0b00001111, message->length); // TXB0DLC, setter lengde
    
    
    mcp2515_write(0b00110010, ((message->id & 0x7) << 5)); // Setter id
    mcp2515_write(0b00110001, (message->id >> 3) & 0xFF);
    
    for (uint8_t i = 0; i < message->length; i++) {
        mcp2515_write(buffer+i, message->data[i]);      // Lagrer data
    }
    
    
    mcp2515_bit_modify(0b00110000,0b11111111,0b00001000); // Setter tilbake interrupt flag
}


///KAN KANSKJE SLETTES
int can_error() {
    unsigned char errorByte = 0;
    errorByte = mcp2515_read(0b00110000);
    
    if(1 == (5<<errorByte) || (1 == (4<<errorByte)))
        return 1;
    
    return 0;
    
}

//KAN KANSKJE SLETTES
int can_transmit_complete() {
    
    unsigned char completeByte = 0;
    return  mcp2515_read(0b00110000)&0b00001110;
    if(1 == (3<<completeByte))
        return 0;
    
    return 1;
}


void can_data_receive(can_message_t * message) {
    
    message->length = 0;
    
    unsigned char readByte = 0; // canintf
    unsigned char rxb0dlc = 0;
    unsigned char rxb1dlc = 0;
    readByte = mcp2515_read(MCP_CANINTF);
    rxb0dlc = mcp2515_read(0b01100101);
    rxb1dlc = mcp2515_read(0b01110101);
    
    unsigned char buffer = 0;
    
    
    if( (readByte&0b00000001) ){ // Reading from RXB0
        unsigned int least = ((mcp2515_read(0b01100010) & 0b11100000) >> 5);
        unsigned int most  = (mcp2515_read(0b01100001) << 3);
        volatile unsigned int sum = least + most;
        message->id = sum;
        
        sum = message->id;
        
        
        message->length =rxb0dlc&0x0F;
        for (uint8_t i = 0; i < message->length; i++) {
            message->data[i] = mcp2515_read(0b01100110+i);
        }
        
        mcp2515_bit_modify(0b00101100, 0b00000001, 0b00000000);
    }
    
    else if( (readByte&0b00000010) ) { // Reading from RXB1
        unsigned int least = ((mcp2515_read(0b01110010) & 0b11100000) >> 5);
        unsigned int most = (mcp2515_read(0b01110001) << 3);
        volatile unsigned int sum = least + most;
        message->id = sum;
        
        message->length =rxb1dlc&0x0F;
        
        for (uint8_t i = 0; i < message->length; i++) {
            message->data[i] = mcp2515_read(0b01100110+i);
        }
        
        mcp2515_bit_modify(0b00101100, 0b00000010, 0b00000000); // Setter tilbake interrupt flag
    }
    
}

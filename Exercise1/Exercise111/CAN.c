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
    
    //Normal MODE ENABLED
    mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_NORMAL);
    
    
}

void can_Init_Loopback(){
    
    mcp2515_init();
    
    //LOOPBACK MODE ENABLED
    mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_LOOPBACK);
    
}

void can_message_send(can_message_t *message) {
    
    unsigned char buffer = 0b00110110;
    
    mcp2515_bit_modify(0b00110101, 0b00001111, message->length); // TXB0DLC, setter lengde
    
    mcp2515_write(0b00110010, ((message->id & 0x7) << 5));
    mcp2515_write(0b00110001, (message->id >> 3) & 0xFF);
    
    for (uint8_t i = 0; i < message->length; i++) {
        mcp2515_write(buffer+i, message->data[i]);
    }
    mcp2515_bit_modify(MCP_TXB0CTRL,0b11111111,0b00001000); // Setter Message transmit request bit
    
    PORTB &= ~(1 << 4);
    
    SPI_Send(MCP_RTS_TX0);
    
    PORTB |= (1 << 4);
}

/*
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
*/

void can_data_receive(can_message_t * message) {
    
    message->length = 0;
    
    unsigned char readByte = 0;
    unsigned char rxb0dlc = 0;
    unsigned char rxb1dlc = 0;
    readByte = mcp2515_read(MCP_CANINTF);
    rxb0dlc = mcp2515_read(0b01100101);
    rxb1dlc = mcp2515_read(0b01110101);
    
    unsigned char buffer = 0;
    if(1 == (readByte&0b00000001)) { //reading from RXB0
        unsigned int least = ((mcp2515_read(MCP_RXB0SIDL)  & 0b11100000) >> 5);
        unsigned int most  = (mcp2515_read(MCP_RXB0SIDH) << 3);
        volatile unsigned int sum = least + most;
        
        message->id = sum;
        sum = message->id;
        
        message->length =rxb0dlc&0x0F; // LESER LENGDEN
        
        for (uint8_t i = 0; i < message->length; i++) {
            message->data[i] = mcp2515_read(0b01100110+i);// LESER DATA
        }
        
        mcp2515_bit_modify(MCP_CANINTF, 0b00000001, 0b00000000); //setter  interrupt flag = 0
    }
    
    else if(2 == (readByte&0b00000010)) { //reading from RXB1
        unsigned int least = ((mcp2515_read(MCP_RXB1SIDL)  & 0b1110000) >> 5);
        unsigned int most  = (mcp2515_read(MCP_RXB1SIDH) << 3);
        volatile unsigned int sum = least + most;
        message->id = sum;
        sum = message->id;
        
        message->length =rxb1dlc&0x0F;
        
        for (uint8_t i = 0; i < message->length; i++) {
            message->data[i] = mcp2515_read(0b01100110+i);// LESER DATA
        }
        
        mcp2515_bit_modify(MCP_CANINTF, 0b00000010, 0b00000000);//setter  interrupt flag = 0
    }
    
}


void receive_score(can_message_t * msg,int * playingGame){
    
    char scoreString [33];
    
    stopGame(playingGame);
    
    oled_clear_screen();
    
    oled_set_line(2);
    oled_print_string("Score: ");
	oled_set_line(4);
	
	itoa(msg->data[0], scoreString,10); // KONVERTERER FRA INT TIL STRING
	
	oled_print_number(scoreString);
	
	while(!joy_button_left());
        oled_meny_print();
    
}

/*
 * MCP2515.c
 *
 * Created: 01.10.2014 10:50:17
 *  Author: brynjare
 */
#include "MCP2515.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <math.h>
#include "SPI.h"

char mcp2515_init(){
    
    SPI_Init_Master();
    mcp2515_reset();
    
    if((mcp2515_read(MCP_CANSTAT) & MODE_MASK) != MODE_CONFIG) {
        printf("MCP2515 is NOT in configuration mode after reset!\n");
        
        return 1;
    }
    
    return 0;
}

char mcp2515_read(uint8_t adress){
    PORTB &= ~(1 << 4);
    
    SPI_Send(MCP_READ);
    SPI_Send(adress);
    char temp = SPI_read();
    PORTB |= (1 << 4);
    
    return temp;
}

void mcp2515_write(uint8_t adress, uint8_t val){
    
    PORTB &= ~(1 << 4);
    
    SPI_Send(MCP_WRITE);
    SPI_Send(adress);
    SPI_Send(val);
    
    PORTB |= (1 << 4);
}

void mcp2515_request_to_send(uint8_t buffer){
    
    PORTB &= ~(1 << 4);
    
    SPI_Send(MCP_RTS_ALL | (1<<buffer));
    
    PORTB |= (1 << 4);
}

void mcp2515_bit_modify(uint8_t adress, uint8_t mask, uint8_t data){
    
    PORTB &= ~(1 << 4);
    
    SPI_Send(MCP_BITMOD);
    SPI_Send(adress);
    SPI_Send(mask);
    SPI_Send(data);
    
    PORTB |= (1 << 4);
}

void mcp2515_reset(){
    
    PORTB &= ~(1 << 4);
    
    SPI_Send(MCP_RESET);
    
    PORTB |= (1 << 4);
}

char mcp2515_read_status(){
    
    return mcp2515_read(MCP_CANSTAT);
}

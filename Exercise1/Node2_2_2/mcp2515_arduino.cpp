/*
 * mcp2515_arduino.cpp
 *
 * Created: 15.10.2014 13:27:54
 *  Author: mikaln
 */


#include <SPI/SPI.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "mcp2515_arduino.h"
#include "MCP2515.h"
#include "SPI_arduino.h"

char mcp2515_init(){
    SPI.begin();
    
    mcp2515_reset();
    _delay_ms(100);
    
    
    if((mcp2515_read(MCP_CANSTAT) & MODE_MASK) != MODE_CONFIG) {
        Serial.println("MCP2515 is NOT in configuration modeafter reset!\n");
        
        return 1;
    }
    return 0;
}

char mcp2515_read(uint8_t adress){
    digitalWrite(53, LOW);
    SPI_Send(MCP_READ);
    SPI_Send(adress);
    char temp = SPI_read();
    digitalWrite(53, HIGH);
    
    return temp;
}

void mcp2515_write(uint8_t adress, uint8_t val){
    
    digitalWrite(53, LOW);
    
    SPI_Send(MCP_WRITE);
    SPI_Send(adress);
    SPI_Send(val);
    
    digitalWrite(53, HIGH);
}

void mcp2515_request_to_send(uint8_t buffer){
    
    digitalWrite(53, LOW);
    
    SPI_Send(MCP_RTS_ALL | (1<<buffer));
    
    digitalWrite(53, HIGH);
}

void mcp2515_bit_modify(uint8_t adress, uint8_t mask, uint8_t data){
    
    digitalWrite(53, LOW);
    
    SPI_Send(MCP_BITMOD);
    SPI_Send(adress);
    SPI_Send(mask);
    SPI_Send(data);
    
    digitalWrite(53, HIGH);
}

void mcp2515_reset(){
    
    digitalWrite(SS, LOW);
    
    SPI_Send(MCP_RESET);
    
    digitalWrite(SS, HIGH);
}

unsigned char mcp2515_read_status(){
    
    
    return (mcp2515_read(MCP_CANSTAT) & MODE_MASK);
}
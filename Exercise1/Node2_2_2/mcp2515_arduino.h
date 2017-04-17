/*
 * mcp2515_arduino.h
 *
 * Created: 15.10.2014 13:28:36
 *  Author: mikaln
 */ 


#ifndef MCP2515_ARDUINO_H_
#define MCP2515_ARDUINO_H_
#include <avr/io.h>

char mcp2515_init();
char mcp2515_read(uint8_t adress);
void mcp2515_write(uint8_t adress, uint8_t val);
void mcp2515_request_to_send(uint8_t buffer);
void mcp2515_bit_modify(uint8_t adress, uint8_t data, uint8_t mask);
void mcp2515_reset();
unsigned char mcp2515_read_status();


#endif /* MCP2515_ARDUINO_H_ */
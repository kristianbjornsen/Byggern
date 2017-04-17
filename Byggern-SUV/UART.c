/*
 * UART.c
 *
 * Created: 31.08.2014 08:23:49
 *  Author: ugurac
 */ 
#include "UART.h"
#include <avr/io.h>



void UART_init(unsigned int ubrr){
//set baud rate


UBRR0H=(unsigned char)(ubrr>>8);
UBRR0L=(unsigned char)ubrr;

UCSR0B=(1<<RXEN0)|(1<<TXEN0);

fdevopen(USART_TRANSMIT, USART_RECEIEVE);	


//enable reciver and transmitter
//UCSR0B=(1<<RXEN0)|(1<TXEN0);

//set frame format: 8data, 2 stop bit
UCSR0C=(1<<UMSEL0);
}

	
void USART_TRANSMIT(unsigned char data){
	
	while (!(UCSR0A & (1<<UDRE0))){
		
	}
	UDR0 = data;
	
}
unsigned char USART_RECEIEVE(void){
	
	
	while(!(UCSR0A&(1<<RXC0))){}
		
	return UDR0;
	
}
/*
 * usart.c
 *
 * Created: 03.10.2014 12:27:09
 *  Author: brynjare
 */ 

#define F_CPU 16000000 // clock frequency in Hz
#define FOSC 4915200  //Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "usart.h"

void USART_Init(unsigned int ubrr){
	
	DDRD |= (1<<PD1);
	//Setter Baud Rate:
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	
	//Slaar paa reciever og transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	
	//Setter frame format; 8data
	UCSR0C = (1<<URSEL0)|(3<<UCSZ00);
	fdevopen(USART_Transmit, USART_Receive);
	
}

void USART_Transmit( unsigned char data )
{
	// Vent paa tom transmit buffer
	while ( !( UCSR0A & (1<<UDRE0)) );
	
	// Putter data i bufferm, sender
	UDR0 = data;
}

unsigned char USART_Receive( void )
{
	// Venter paa at data skal mottas
	while ( !(UCSR0A & (1<<RXC0)) );
	
	// Returnerer data
	return UDR0;
}

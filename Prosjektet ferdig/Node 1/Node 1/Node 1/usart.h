/*
 * usart.h
 *
 * Created: 03.10.2014 12:27:23
 *  Author: brynjare
 */ 


#ifndef USART_H_
#define USART_H_

void USART_Init(unsigned int ubrr);
void USART_Transmit( unsigned char data );
unsigned char USART_Receive( void );



#endif /* USART_H_ */
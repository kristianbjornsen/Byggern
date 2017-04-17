/*
 * UART.h
 *
 * Created: 31.08.2014 08:23:35
 *  Author: ugurac
 */ 


#ifndef UART_H_
#define UART_H_

void UART_init();
void USART_TRANSMIT(unsigned char data);
unsigned char USART_RECEIEVE(void);
#endif /* UART_H_ */
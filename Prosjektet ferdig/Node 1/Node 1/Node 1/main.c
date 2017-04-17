/*
 * Node_1.c
 *
 * Created: 18.11.2014 18:32:48
 *  Author: krbjorn
 */ 

#define F_CPU 16000000 // clock frequency in Hz
#define FOSC 4915200  //Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "adc.h"
#include "Joystick.h"
#include "OLED.h"
#include "SPI.H"
#include "CAN.h"
#include "usart.h"
#include "MCP2515.h"
#include "sram.h"

// Globale variabler
int playingGame = 0;
uint16_t id = 0;
can_message_t message;
can_message_t msg;
can_message_t receive_message;

int arr[3] = {};

int main(void)
{
	//Initialiserer:
	USART_Init ( MYUBRR );
	sram_Init();
	adc_init();
	oled_init();
	joystick_init();
	oled_meny_init();
	can_Init_Normal();
	
	printf("CANSTAT is 0x%x\n\r", mcp2515_read(MCP_CANSTAT));
	
	
	while(1)
	{
		// Sjekker om hoyreknapp er trykket paa
		if (joy_button_right() == 2)
		PORTB &= ~(1 << PB3);
		else if (joy_button2_right() > 0)
		PORTE &= ~(1 << PE2);
		
		oled_meny(&playingGame, &id);
		
		if (playingGame) {
			message.id = id;
			joystick_send_to_arduino(&message);
		}
		
		
		PORTB |= (1 << PB3);
		PORTE |= (1 << PE2);
		
		if (mcp2515_read(MCP_CANINTF)&0x03 != 0x00) {
			
			can_data_receive(&receive_message);
			
			if(receive_message.id == scoreId){
				receive_score(&receive_message, &playingGame);
			}
			
			else if (receive_message.id == SCORE_SEND_ID) {
				recieveHighScore(arr,&receive_message);
				printHighScore(arr);
			}
			
			else if(receive_message.id == END_TWO_PLAYER_GAME){
				stopGame(&playingGame);
				endTwoPlayerGame();
			}
			else if(receive_message.id == musicStop){
				oled_meny_print();
			}
		}
	}
}
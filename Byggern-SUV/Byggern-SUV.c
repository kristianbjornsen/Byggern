/*
 * Byggern_SUV.c
 *
 * Created: 31.08.2014 08:13:53
 *  Author: ugurac
 */ 

#define F_CPU 4915200
//#define FOSC 4915200 //System oscillator clock freq
#define BAUD 9600 //transfer rate in bits per second
#define MYUBRR (uint8_t)((4915200/16/9600)-1) 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "ADC.h"
#include "UART.h"
#include "TOUCH.h"
#include "OLED.h"
#include "MCP2515.h"
#include "CAN.h"
#include "SPI.h"
#include "NODE2COMM.h"

//void SLIDER_test();

ISR(INT2_vect){
	CAN_allowed_to_send();
}

int main(void){	
	
	// Control pins
	DDRB=(1<<PINB0);
	PORTB=(1<<PINB0);
	
	// Initialization
	UART_init(31);
	SRAM_init();
	GAME_init();
	JOY_init();
	OLED_init();
	SRAM_init();
	CAN_init();
	NODE2COMM_init(); // OLED ready to use, black background
	// loop back modus
	printf("Initialisation done\n\r");
	
	// Calibration
	JOY_calib();
	
	//Functions for while loop
	
	while(1)
	{	
		//Recieve CAN info
		CAN_recieve();
		
		//Start game
   		if (GAME_main(GAME_play))
  			OLED_highscores();
			  
  		//Printing the menu on OLED screen
  		OLED_menu();
	}
}
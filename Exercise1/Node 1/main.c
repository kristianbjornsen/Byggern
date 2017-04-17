/*
 * GccApplication1.c
 *
 * Created: 27.08.2014 11:09:36
 *  Author: mikaln
 */ 

#define F_CPU 16000000 // clock frequency in Hz
#include <avr/io.h>
#include <stdio.h>
#define FOSC 4915200  //Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#include <util/delay.h>


#include "adc.h"
#include "Joystick.h"
#include "OLED.h"
#include "SPI.H"
#include "CAN.h"
#include "usart.h"

int iandre(int i, int j){
	int sum = i;
	if(j==0)
		return 1;
	for(int a = 1;a < (j); a++){
		sum *= i;
	}
	return sum;
}

void SRAM_test(void)
{
	// Start address for the SRAM
	volatile char *ext_ram = (char *) 0x1800;

	uint16_t i, werrors, rerrors;
	werrors = 0;
	rerrors = 0;
	char testvalue;

	printf("Starting SRAM test...\r\n");

	for (i = 0; i < 0x800; i++) {
		testvalue = ~(i % 256);
		ext_ram[i] = testvalue;
		if (ext_ram[i] != testvalue) {
			//printf("SRAM error (write phase): ext_ram[%d] = %02X (should be %02X)\r\n", i, ext_ram[i], testvalue);
			werrors++;
		}
	}

	for (i = 0; i < 0x800; i++) {
		testvalue = ~(i % 256);
		if (ext_ram[i] != testvalue) {
			//printf("SRAM error (read phase): ext_ram[%d] = %02X (should be %02X)\r\n", i, ext_ram[i], testvalue);
			rerrors++;
		}
	}

	//printf("SRAM test completed with %d errors in write phase and %d errors in read phase\r\n", werrors, rerrors);
}

void sram_Init(){
	MCUCR = (1<<SRE);
	SFIOR = (1<<XMM2);
}

int main(void)
{
	//Initialiserer:
	USART_Init ( MYUBRR );
	sram_Init();
	adc_init();
	oled_init();
	//oled_meny_print();
	//joystick_init();
	//oled_meny_init();
	SPI_Init_Master();
	can_Init_Loopback();
	//mcp2515_init();
	
	can_message_t message;
	
	message.id = 1002;
	message.length = 8;
	for (int i = 0; i < 8; i++) {
		message.data[i] = i;
	}
	

	while(1)
    {
	
	//printf("status: %d\n", mcp2515_read_status());
			can_message_send(&message); 			
			
 			while(!can_transmit_complete());
 			_delay_ms(100);
// 			 for (int i = 0; i < message.length; i++)
// 			 {
// 				 printf("Data %d: %d\n",i,can_data_receive()->data[i]);
// 				 if (i == 7) printf("\n");
// 			 }
			can_data_receive(&message);
 		  	printf("bogo: %d, id: %d\n", 42, message.data[3]);			
						   
 		//printf("%d /n", recivedmessage->data);
 		//printf("%d\n\n", (a>>1));
 		
 		

	
		//oled_meny_print();
		//oled_clear_screen();
		//oled_meny();
		//can_message_send(&message);
		//SPI_Send('A');
		//oled_meny_clearScreenEvertTime();
 		//SPI_Send('A');
		//_delay_ms(100);
		//printf("%c",SPI_read());
		

		//*ext_oled_d = 0b00001111;
		//oled_init();

		
		//oled_print(NULL);
		//
		//_delay_ms(50);
		//printf("%d\n",SPI_read());
		
		
		
    }
	
	
}
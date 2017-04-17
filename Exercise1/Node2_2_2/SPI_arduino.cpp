/*
 * SPI_arduino.cpp
 *
 * Created: 15.10.2014 13:48:34
 *  Author: mikaln
 */ 

#include "SPI_arduino.h"

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <math.h>


void SPI_Init_Slave(void)
{
	DDRB = (1<<DDB6);

	SPCR = (1<<SPE);
}

void SPI_Init_Master(void)
{
	/* Set MOSI and SCK and SS as output, all others input */
	
	DDRB = (1<<DDB0)|(1<<DDB1)|(1<<DDB2);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}


void SPI_Send(char cData)
{
	/* Start transmission */
	SPDR = cData;
	
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)))
	;
}


char SPI_read(void)
{
	SPDR = 0x00;
	
	while(!(SPSR & (1<<SPIF)))
	;
	
	return SPDR;
}
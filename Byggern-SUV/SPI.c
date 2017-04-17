/*
 * SPI.c
 *
 * Created: 01.10.2014 08:24:32
 *  Author: viliusc
 */ 

#include <avr/io.h>

void SPI_init(void)
{
	/* !SS set to output */
	DDRB|=(1<<PB4);
	/* Set MOSI and SCK output*/
	DDRB |= (1<<DDB5)|(1<<DDB7);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}
void SPI_send(uint8_t cData)
{
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)))
	;
}

uint8_t SPI_read(){
	
	/* Start transmission */
	SPDR = 2;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)))
	;
	
	return SPDR;
	
}
void SPI_select(int select){
	if(select==1) PORTB &= ~(1<<PB4);
	else if(select==0) PORTB |= (1<<PB4);
	
}
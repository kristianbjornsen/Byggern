#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "adc.h"

volatile unsigned char *ext_adc = (char *) 0x1400;

void adc_init(){
//Setter PB1 OG PB2 som INPUT
	DDRB |= (0<<PB1) | (0<<PB2); 
	PORTB = 0xff;
}


unsigned char adc_read(unsigned char channel) {
	*ext_adc = (channel + 3);
	_delay_ms(50);
    
	return *ext_adc;
}

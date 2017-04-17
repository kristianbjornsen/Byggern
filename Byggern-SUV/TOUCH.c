/*
 * TOUCH.c
 *
 * Created: 17.09.2014 11:24:42
 *  Author: viliusc
 */ 
#include <AVR/io.h>
#include <AVR/delay.h>

void TOUCH_init(){
	
	
	//For buttons
	
}

int TOUCH_BUTTON(){
	if(PINE&(1<<PINE0)) {return 0;} //LEFT BUTTON
	if(PINE&(1<<PINE2)) {return 1;} //RIGHT BUTTON
	return -1; //NO BUTTON PRESSED
				// NO CODE
				//NOTHING HAPPENS
}

int16_t SLIDER_read(char value){
	volatile uint8_t *adc_adress= (uint8_t *) 0x1400;
	int16_t data;
	switch (value){
		case 'l':
		adc_adress[0x06]= 6;
		_delay_ms(1);
		data = adc_adress[6]; //returns real position
		break;
		case 'r':
		adc_adress[0x07]= 7;
		_delay_ms(1);
		data = adc_adress[7]; //returns real position
		break;
		default:
		printf("Error");
		
	}
	data=(data/255.0)*100; //returns in percent
	return data;
	
}
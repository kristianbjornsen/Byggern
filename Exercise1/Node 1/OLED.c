#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <util/delay.h>
#include <math.h>
#include "font_8x8.h"
#include "Joystick.h"


#include "OLED.h"
const char MenuItem1[] = "   Menu Item 1";
const char MenuItem2[] = "   Menu Item 2";
const char MenuItem3[] = "   Menu Item 3";
const char MenuItem4[] = "	 Undermeny si";

const char * MenutItemPointers[] = {MenuItem1, MenuItem2, MenuItem3, MenuItem4};
volatile unsigned char *ext_oled_c = (char *) 0x1000;
volatile unsigned char *ext_oled_d = (char *) 0x1200;
int previous_line= 1;
int current_line= 1;

#define OLED_COMMAND *((volatile uint8_t*)0x1000)
#define OLED_DATA *((volatile uint8_t*)0x1200)


void oled_init(){
	write_c(0xae); // display off
	write_c(0xa1); // segment remap
	write_c(0xda); // common pads hardware: alternative
	write_c(0x12);
	write_c(0xc8); // common output scan direction:com63~com0
	write_c(0xa8); // multiplex ration mode:63
	write_c(0x3f);
	write_c(0xd5); // display divide ratio/osc. freq. mode
	write_c(0x80);
	write_c(0x81); // contrast control
	write_c(0x50);
	write_c(0xd9); // set pre-charge period
	write_c(0x21);
	write_c(0x20); // Set Memory Addressing Mode
	write_c(0x02);
	write_c(0xdb); // VCOM deselect level mode
	write_c(0x30);
	write_c(0xad); // master configuration
	write_c(0x00);
	write_c(0xa4); // out follows RAM content
	write_c(0xa6); // set normal display
	write_c(0xaf); // display on
	
	//oled_set_line(0);
	//write_c(0x40); // set start line to 0
	//write_c(0xa5);
	//oled_clear_screen();
}

void write_c(unsigned char a){
	*ext_oled_c = a;

}

void write_d(unsigned char a) {
	*ext_oled_d = a;
}

void write_dString_P(unsigned char a) {
	for (int i = 0; i < 8; i++) {
		write_d(pgm_read_word(&font[a-32][i]));
		_delay_ms(10);
	}
}

void oled_set_pos_left() {
	write_c(0x00);
	write_c(0x10);
}

void oled_set_line(uint8_t line) {
	write_c(0xB0 + line);
	oled_set_pos_left();
}

void oled_clear_screen(void) {
	for(uint8_t row = 0; row < 8; row ++) {
		oled_set_line(row);
		for (uint8_t col = 0; col < 128; col++) {
			write_d(0x00);
		}
	}
	oled_set_line(0);
}

void oled_meny_print(void) {
	oled_clear_screen();
	for(int i = 0; i < 3; i++) {
		oled_set_line(2*i);
		
		if (current_line == (i+1))
			oled_print_string(">");
		else
			oled_print_string(" ");
			
		oled_print_string(MenutItemPointers[i]);
		
	}

}

// void oled_meny_init(void) {
// 	oled_meny_print();
// 	
// 	oled_set_pos_left();
// 	oled_set_line(0);
// 	oled_print_string(">");
// 
// }

void oled_meny(void ) {

	if (joystick_dir(joystick_anal()) != NEUTRAL) {
		if(joystick_dir(joystick_anal()) == 3 && current_line != 3){
			previous_line = current_line;
			current_line++;
			while (joystick_dir(joystick_anal()) != NEUTRAL);
// 			oled_set_line(2*(previous_line-1));
// 			oled_print_string("  ");
// 			oled_set_line(2*(current_line-1));
// 			oled_print_string(">");
		
		}
		else if (joystick_dir(joystick_anal()) == 2 && current_line != 1){
			previous_line = current_line;
			current_line--;
			while (joystick_dir(joystick_anal()) != NEUTRAL);
// 			oled_set_line(2*(previous_line-1));
// 			oled_print_string("  ");
// 			oled_set_line(2*(current_line-1));
// 			oled_print_string(">");
		}
		else if (joystick_dir(joystick_anal()) == 2 && current_line == 1){
			previous_line = current_line;
			current_line = 3;
			while (joystick_dir(joystick_anal()) != NEUTRAL);
// 			oled_set_line(0);
// 			oled_print_string("  ");
// 			oled_set_line(4);
// 			oled_print_string(">");
		}
		else if (joystick_dir(joystick_anal()) == 3 && current_line == 3){
			previous_line = current_line;
			current_line = 1;
			while (joystick_dir(joystick_anal()) != NEUTRAL);
// 			oled_set_line(4);
// 			oled_print_string("  ");
// 			oled_set_line(0);
// 			oled_print_string(">");
		}	
	
		oled_set_line(2*(previous_line-1));
		oled_print_string("  ");
		oled_set_line(2*(current_line-1));
		oled_print_string(">");
	}
	if (joystick_dir(joystick_anal()) == RIGHT) {
		oled_clear_screen();
		oled_print_string(MenutItemPointers[3]);
		while(joystick_dir(joystick_anal()) != LEFT);
		oled_meny_print();
		
	}
	
	printf("current = %d\tprev = %d\n\n", current_line,previous_line);
	

	

}

void oled_meny_clearScreenEvertTime(void ) {

	if(joystick_dir(joystick_anal()) == 3 && current_line != 3){
		previous_line = current_line;
		current_line++;
		oled_meny_print();
		while (joystick_dir(joystick_anal()) != NEUTRAL);
	}
	
	else if (joystick_dir(joystick_anal()) == 2 && current_line != 1){
		previous_line = current_line;
		current_line--;
		oled_meny_print();
		while (joystick_dir(joystick_anal()) != NEUTRAL);
		
	}
	else if (joystick_dir(joystick_anal()) == 2 && current_line == 1){
		previous_line = current_line;
		current_line = 3;
		oled_meny_print();
		while (joystick_dir(joystick_anal()) != NEUTRAL);
		
	}
	else if (joystick_dir(joystick_anal()) == 3 && current_line == 3){
		previous_line = current_line;
		current_line = 1;
		oled_meny_print();
		while (joystick_dir(joystick_anal()) != NEUTRAL);
	}
	
	
	
	if (joystick_dir(joystick_anal()) == RIGHT) {
		oled_clear_screen();
		oled_print_string("e");
		while(joystick_dir(joystick_anal()) != LEFT);
		oled_meny_print();
		
	}
	
	printf("current = %d\tprev = %d\n\n", current_line,previous_line);
	

	

}

void oled_print_string(char *data) {

	while (*data != '\0') {
		write_dString_P(*data);
		data++;
		
	}
}

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <util/delay.h>
#include <math.h>
#include "font_5x7.h"
#include "Joystick.h"
#include "OLED.h"
#include "sram.h"
#include "score.h"

const char MenuItem1[] = "   Play Game";
const char MenuItem2[] = "   Sram-Test";
const char MenuItem3[] = "   Play Music";
const char MenuItem4[] = "	 Playing Game";
const char MenuItem5[] = "Running SRAM-test.";

const char * MenutItemPointers[] = {MenuItem1, MenuItem2, MenuItem3, MenuItem4, MenuItem5};

volatile unsigned char *ext_oled_c = (char *) 0x1000;
volatile unsigned char *ext_oled_d = (char *) 0x1200;
int previous_line= 1;
int current_line= 1;
int submenu1 = 0;
int submenu2 = 0;

int8_t scoreArray[3] = {};


void oled_init() {
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
	
	oled_set_line(0);
	write_c(0x40); // set start line to 0
	
	oled_clear_screen();
}

void write_c(unsigned char a){
	*ext_oled_c = a;
}

void write_d(unsigned char a) {
	*ext_oled_d = a;
}

void write_dString_P(unsigned char a) {
	for (int i = 0; i < 5; i++) {
		write_d(pgm_read_word(&myfont[a-32][i]));
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
	
	if (submenu1 == 0 && submenu2 == 0){
		for(int i = 0; i < 3; i++) {
			oled_set_line(2*i);
			
			if (current_line == (i+1))
			oled_print_string(">");
			else
			oled_print_string(" ");
			
			if (!(submenu1 || submenu2)) {
				oled_print_string(MenutItemPointers[i]);
			}
		}
	}
	
	else if (submenu1 && !submenu2) {
		
		oled_set_line(0);
		oled_print_string("    Game Mode");
		oled_set_line(2);
		oled_print_string("  One Player Ping Pong");
		oled_set_line(4);
		oled_print_string("  2 P Penalty Shootout");
		oled_set_line(6);
		oled_print_string("  View High Scores");
		
		for(int i = 1; i < 4; i++) {
			oled_set_line(2*i);
			printf("Currentline: %d\n",current_line);
			
			if (current_line == (i))
			oled_print_string(">");
			else
			oled_print_string(" ");
		}
		
	}
	
	else if (submenu2) {
		
		if (current_line == 1) {
			oled_clear_screen();
			oled_set_line(0);
			oled_print_string("      Playing Game...");
		}
		
		else if (current_line == 2) {
			oled_set_line(0);
			oled_print_string("Playing Game...");
			
		}
		
		else if (current_line == 3) {
			
		}
	}
	
	else
	oled_print_string(MenutItemPointers[current_line+2]);
	
}


void oled_meny_init(void) {
	oled_meny_print();
	
	oled_set_pos_left();
	oled_set_line(0);
	oled_print_string(">");
	
}


void oled_meny(int * playingGame, uint16_t * id) {
	
	if(submenu1 || submenu2) { // SJEKKER OM VI GAAR UT AV SUBMENUEN
		if(joy_button_left()) {
			if (submenu2) {
				submenu2 = 0;
				*playingGame = 0;
			}
			else {
				submenu1 = 0;
			}
			
			current_line = 1;
			previous_line = 1;
			oled_meny_print();
		}
	}
	
	
	if (!(*playingGame)) {
		
		
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
		
		if (joystick_dir(joystick_anal()) == RIGHT && current_line == 1 && !submenu1) {
			submenu1 = 1;
			oled_meny_print();
			_delay_ms(1500); // For aa ikke komme rett inn i spill
		}
		
		if (joystick_dir(joystick_anal()) == RIGHT && current_line == 1 && submenu1) {
			submenu2 = 1;
			oled_meny_print();
			*playingGame = 1;
			*id = onePlayer;
		}
		
		if (joystick_dir(joystick_anal()) == RIGHT && current_line == 2 && submenu1) {
			submenu2 = 1;
			oled_meny_print();
			*playingGame = 1;
			*id = twoPlayer;
		}
		
		if (joystick_dir(joystick_anal()) == RIGHT && current_line == 3 && submenu1) {
			submenu2 = 1;
			requestHighScore();
		}
		
		if (joystick_dir(joystick_anal()) == RIGHT && current_line == 2 && !(submenu1)) {
			print_Sram_test();
		}
		
		if (joystick_dir(joystick_anal()) == RIGHT && current_line == 3 && !(submenu1)) {
			playMusic();
		}
		
	}
	
}

void stopGame(int * playingGame){
	
	*playingGame = 0;
	submenu2 = 0;
	submenu1 = 0;
	current_line = 1;
	previous_line = 1;
	
}

void oled_print_string(char *data) {
	
	while (*data != '\0') {
		write_dString_P(*data);
		data++;
		
	}
}

void write_dnumber(unsigned char a) {
	for (int i = 0; i < 5; i++) {
		write_d(pgm_read_word(&myfont[a-32][i]));
	}
}

void oled_print_number(char *data) {
	
	while (*data != '\0') {
		write_dnumber(*data);
		data++;
		
	}
}

void print_Sram_test(){
	int errors[2];
	char werrors [33];
	char rerrors [33];
	
	oled_clear_screen();
	oled_set_line(0);
	oled_print_string("Running sram test");
	SRAM_test_return_errors(errors);
	_delay_ms(20000);
	
	itoa(errors[0], werrors,10); // KONVERTERER FRA INT TIL STRING
	itoa(errors[1],rerrors,10);
	
	oled_set_line(2);
	oled_print_string("  Write errors: ");
	oled_print_number(werrors);
	oled_set_line(5);
	oled_print_string("  Read errors: ");
	oled_print_number(rerrors);
	SRAM_test();
	while(joy_button_left() == 0);
	oled_meny_print();
}

void printHighScore(int* arr) {
	
	oled_clear_screen();
	
	char scoreString [33];
	
	oled_set_line(0);
	oled_print_string("   HIGH SCORES: ");
	
	oled_set_line(2);
	oled_print_string("1. ");
	itoa(arr[0], scoreString,10);
	oled_print_number(scoreString);
	
	oled_set_line(4);
	oled_print_string("2. ");
	itoa(arr[1], scoreString,10);
	oled_print_string(scoreString); /// tall!!
	
	oled_set_line(6);
	oled_print_string("3. ");
	itoa(arr[2], scoreString,10);
	oled_print_string(scoreString); /// tall!!
	
}

void endTwoPlayerGame(can_message_t * winner)
{
	char scoreString [33];
	
	oled_clear_screen();
	oled_set_line(2);
	oled_print_string("WINNER WINNER!!");
	oled_set_line(4);
	oled_print_string("PLAYER: ");
	itoa(winner->data[0], scoreString, 10);
	oled_print_string(scoreString);
	while(!joy_button_left());
	
	oled_meny_print();
}


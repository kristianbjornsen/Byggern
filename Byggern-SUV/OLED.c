/*
 * OLED.c
 *
 * Created: 17.09.2014 13:45:01
 *  Author: viliusc
 */ 
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/delay.h>
#include <stdlib.h>
#include "font_5x7.h"
#include "Logo.h"
#include "NODE2COMM.h"
#include "ADC.h"
#include "GAME.h"
#include "CAN.h"
#define OLED_C *((volatile uint8_t *) 0x1000)
#define OLED_D *((volatile uint8_t *) 0x1200)
//#define SRAM_D *((volatile uint8_t *) 0x1800)

uint8_t last_position;
uint8_t menu_length,menu_number,amount_menu, last_menu, curr_menu;
Direction last_pos,curr_pos;
char *menu[4][8];
char *names[10];
char *scores[5];
can_message_t GAME;
char *Kp_string;
void OLED_init()
{
	names[0]="Looser";
	names[1]="Winner";
	names[2]="Nerd";
	names[3]="Zombie";
	names[4]="No Name";
	names[5]="Awesome guy";
	
	scores[0] = "0";
	scores[1] = "0";
	scores[2] = "0";
	scores[3] = "0";
	scores[4] = "0";
	menu_number=0;
	amount_menu=4;
	last_position=3;
	menu_length=8;
	last_pos=NEUTRAL;
	last_menu = 0;
	Kp_string = "1";
	write_c(0xae); // display off
	write_c(0xa1); //segment remap
	write_c(0xda); //common pads hardware: alternative
	write_c(0x12);
	write_c(0xc8); //common output scan direction:com63~com0
	write_c(0xa8); //multiplex ration mode:63
	write_c(0x3f);
	write_c(0xd5); //display divide ratio/osc. freq. mode
	write_c(0x80);
	write_c(0x81); //contrast control
	write_c(0x50);
	write_c(0xd9); //set pre-charge period
	write_c(0x21);
	write_c(0x20); //Set Memory Addressing Mode
	write_c(0x00);
	write_c(0xdb); //VCOM deselect level mode
	write_c(0x30);
	write_c(0xad); //master configuration
	write_c(0x00);
	write_c(0xa4); //out follows RAM content
	write_c(0xa6);
	write_c(0xaf); //set normal display*/	OLED_command_adress[]
	OLED_screen_clear();
	//OLED_scroll(1);
	menu[0][0]=" ";
	menu[0][1]="Welcome to SwagPong v1.3";
	menu[0][2]=" ";
	menu[0][3]="Play game";
	menu[0][4]="Highscores";
	menu[0][5]="About";
	menu[0][6]=" ";
	menu[0][7]=" ";
	
	menu[1][0]= "Top 5 scores";
	menu[1][1]= " ";
	menu[1][2]= scores[0];
	menu[1][3]= scores[1];
	menu[1][4]= scores[2];
	menu[1][5]= scores[3];
	menu[1][6]= scores[4];
	menu[1][7]= " ";
	
	
	menu[2][0]="MADE BY: Shiv, Ugur";
	menu[2][1]="and Vilius";
	menu[2][2]=" ";
	menu[2][3]="COPYRIGHT INFO";
	menu[2][4]="If you copy our work";
	menu[2][5]="We will sue you,";
	menu[2][6]="take your pants,";
	menu[2][7]="and your wife";
	
	menu[3][0]="SETTINGS";
	menu[3][1]=" ";
	menu[3][2]="CONTROLLER: Kp=";
	menu[3][3]="DIFFICULTY: EASY";
	menu[3][4]="SOUND: OFF";
	menu[3][5]="SHOW LOGO";
	menu[3][6]=" ";
	menu[3][7]=" ";
	
	//OLED_scroll(1);
	//OLED_menu_print();
}
// write command to OLED control register
void OLED_screen_clear(){
	
	OLED_change_page(0,7); //Makes the screen to start from left top corner
	OLED_change_coloumn(0,127); //Makes the screen to use whole screen
	int j,i;
	for(i = 0; i < 8; i++){
		for(j=0; j<128; j++){
			OLED_D =0b00000000;
		}
	}
	//OLED_D=0b00000001;	 // The screen stopped scrolling. Too many pixels was being put out to the screen
}
void OLED_scroll(int value){
	if(value){
		write_c(0x29); // Vertical and horizontal scroll
		write_c(0x00); //dummy
		write_c(0x00); //page0 as start
		write_c(0x80); //time interval between each scroll step as 6 frames
		write_c(0x07); //end page adress
		write_c(0x44); //vertical scrolling offsett
		write_c(0x2E);
	}else{
		write_c(0x2E);
	}
	
}
void write_c(uint8_t adress){
	OLED_C = adress;
}

// write data to OLED data register
void write_d(uint8_t adress){
	OLED_D = adress;
}

// print a symbol to OLED 
void OLED_print(char letter){
	for(int i=0; i<5; i++){
		OLED_D = (pgm_read_byte(&myfont[letter -' '][i]));		
	}	
}

// print a string to OLED 
int OLED_print_string(char *list){
	int i=0;
	while(list[i]!='\0'){
		OLED_print(list[i]);
		i++;
	}
	return i;
}
//Write charachters with inverted bits
void OLED_print_invert(char letter){
	for(int i=0; i<5; i++){
		OLED_D = ~(pgm_read_byte(&myfont[letter -' '][i]));
	}
}
//Write string with inverted bits
int OLED_print_string_invert(char *list){
	int i=0;
	while(list[i]!='\0'){
		OLED_print_invert(list[i]);
		i++;
	}
	return i;
}
// print a 64x128 figure to OLED 
void OLED_print_logo(int choice){
	int j,i;
	switch (choice){
	case 1:
		for(i = 0; i < 8; i++){
			for(j=127; j>=0; j--){
				OLED_D =pgm_read_byte(&logo[i][j]);
			}
		}
		_delay_ms(1000);
		for(i = 0; i < 8; i++){
			for(j=127; j>=0; j--){
				OLED_D =~pgm_read_byte(&logo[i][j]);
			}
		}
		_delay_ms(1000);
		break;
	case 2:
		_delay_ms(10);
		for(i = 0; i < 8; i++){
			for(j=127; j>=0; j--){
				OLED_D =pgm_read_byte(&ash[i][j]);
			}
		}
		_delay_ms(1000);
		for(i = 0; i < 8; i++){
			for(j=127; j>=0; j--){
				OLED_D =~pgm_read_byte(&ash[i][j]);
			}
		}
		
		break;
	default:
		OLED_print_string("Error");
	}
}

void OLED_clear(void) {
	int i;
	for (i=0; i<8; i++)
		OLED_print_choice(" ", 0);
}

// print menu to OLED
void OLED_loading(){
	int i=OLED_print_string("LOADING")*5;
	printf("%d",i);
	for(i; i<128*8; i){
		_delay_ms(100);
		i+=OLED_print_string(".")*5;
	}
}
void OLED_menu_print(int list){
	// 2 Lines above menu
	int j;
 	
	//Menu List
	for(j=0; j<menu_length; j++){ // print only cases possible to select. Look at the menu above
		if(list==j){
			OLED_print_choice(menu[menu_number][j], 1); // 1 indicates current choice by drawing an arrow
		}
		else {OLED_print_choice(menu[menu_number][j], 0);}
	}
	
}

void OLED_highscores(void){
	menu_number = 1;
	last_pos = 3; // optional
}
void OLED_print_empty_line(int amount){
	int i,j;
	for(i = 0; i < amount; i++){
		for(j=0; j<128; j++){
			OLED_D =0b00000000;
		}
	}
}
void OLED_print_choice(char* string, int arrow){
	int i;
	//Calculate string length
	int j;
	for(j=0; string[j]!= '\0'; ++j) {}
	int start = (128-5*j)/2;
	if (start <=0)
		start = 0;
	for(i=0; i<128; i++){
		if(arrow && i==10 && menu_number == 0) {
			i+=5*OLED_print_string("-->"); //Spørsmål: Hvorfor skal vi ikke ha -1 her
		}
		if(i==start){
			i+=5*OLED_print_string(string)-1;// men skal ha det her
		}
		else OLED_D=0;
	}
}

int OLED_check_for_game() {
	if (last_position == 3 && menu_number == 0 && JOY_direction()== RIGHT) {
		GAME_play = 1;
		return 1;
	}
	return 0;
}

// handle the menu of OLED
void OLED_menu(void){ 
	//_delay_ms(20);
									// Needed for stability, else arrow goes up and down all by itself
	OLED_pointer_position();
	OLED_check_for_game();
	OLED_menu_print(last_position);

// 	OLED_scroll(1);
// 	_delay_ms(1000);
// 	OLED_scroll(0);
}

 // return current pointer position
void OLED_pointer_position(void){
	
	int from,to;	
	curr_pos = JOY_direction();
	printf("Menu number is %d\n", menu_number);
	curr_menu = last_menu;
	if(JOY_direction() == RIGHT)
		printf("RIGHT!\n");
	switch (JOY_direction()){
		case UP:
			if(curr_pos != last_pos){
				last_position--;
				if(last_position<3){
					last_position=5;
				}
				//OLED_menu_print(last_position);
			}
			last_pos=curr_pos;
			break;
		case DOWN:
			if(curr_pos != last_pos){
				last_position++;
				if(last_position>5){
					last_position=3;
				}
				//OLED_menu_print(last_position);
			}
			last_pos=curr_pos;
			break;
		case LEFT:
			if(curr_pos != last_pos) {
				OLED_outro();
				menu_number = 0;
			}
			last_pos = curr_pos;
			break;
		case RIGHT:
			printf("Joy dir -%d\n", JOY_direction());
			if(curr_pos != last_pos && menu_number != amount_menu){
				if(menu_number == 0 && last_position == 4){
					printf("GHGer\n");
					//OLED_outro();
					menu_number = 1;
					
				}
				else if(menu_number == 0 && last_position == 5){
					OLED_outro();
					menu_number = 2;
				}
				else if(menu_number == 3 ){ //&& last_position == 2
					printf("Go to Change KP\n");
					OLED_change_Kp();
				}
				else{
					printf("ElseerP\n");
				}
			}
			last_pos = curr_pos;
			break;
		case NEUTRAL:
			last_pos=NEUTRAL;
	}
}


// print the screen saver
void OLED_screen_saver(void){
	//OLED_print_logo(2);
	if (!(PINB&(1<<PINB3)) && last_position==1)
			OLED_print_logo(2);
		else if(!(PINB&(1<<PINB3)) && last_position==2)
			OLED_print_logo(1);
		else if(!(PINB&(1<<PINB3)) && last_position==3){	
		}
}

void OLED_game(){
	OLED_change_page(4,4);
	OLED_change_coloumn(0,110);
	OLED_print_string("Your Score is: ");
	OLED_change_coloumn(90,110);
	uint8_t point=0;
	int e=0;
	while(1){
		if(e){
			point=GAME.data[0];
			printf("Point: %d",GAME.data[0]);
			OLED_print((char)point);
		}
		NODE2COMM_read_joystick_position();
	}
}
void OLED_change_page(int from, int to){
	write_c(0x22); //Page addressing mode
	write_c(from);
	write_c(to);
	
}
void OLED_change_coloumn(int from,int to){
	write_c(0x21); //set coloumn address
	write_c(from); //start
	write_c(to); // end
	
}
int OLED_if_active(void) {
	curr_pos = JOY_direction();
	_delay_ms(100);
	if(curr_pos != last_pos) {
		last_pos=curr_pos;
		return 1;
	}
	return 0;
}

void OLED_change(int to_menu) {
	volatile uint8_t *SRAM_D= (uint8_t *) 0x1800;
	int cleared_pixels = 0;
	for(int i=0; i<0x512; i++){
		SRAM_D[i]=OLED_D;
		printf("%d,%d\n",SRAM_D[i],OLED_D);
	}
	printf("%d",SRAM_D[0x256]);
	while (cleared_pixels != 1) {
		/* random int between 0 and 128*64*/
		int r = rand() % 128*32;
		for (int i=0; i<r; i++){
			_delay_ms(10);
			OLED_D;// = SRAM_D[i];
		}
		OLED_D = 0b00000001;
		// check for all black
		cleared_pixels = 1;
		for(int i = 0; i < 8; i++){
			for(int j=0; j<128; j++){
				if (OLED_D == 0b00000001)
					cleared_pixels = 0;
			}
		}
	}
}
void OLED_write_pixel(int y, int x){
	int page,coloumn,data,pos;
	write_c(0x22); //Page addressing mode
	if(y < 8)
	page = 0x0;
	else if(y < 16)
	page = 0x1;
	else if(y < 24)
	page = 0x2;
	else if(y < 32)
	page = 0x3;
	else if(y < 40)
	page = 0x4;
	else if(y < 48)
	page = 0x5;
	else if(y < 56)
	page = 0x6;
	else
	page = 0x7;
	write_c(page);
	write_c(page);
	
	write_c(0x21); //set coloumn address
	write_c(x); //start
	write_c(x); // end
	
	pos=(page+1)*8-y;
	data=(1<<pos);
	write_d(data);
}

void OLED_change_highscore(int menu_nr, int place, char *name_string) {
	menu[menu_nr][place] = name_string;
}

char* OLED_get_highscore_menu(int menu_nr) {
	return menu[menu_nr]; 	
}

void OLED_congrats_highscore2(){
	OLED_change_page(1,1);
	OLED_change_coloumn(14, 114);
	OLED_print_string("New Highscore!");
	OLED_change_page(3,3);
	OLED_change_coloumn(14, 114);
	OLED_print_string("Write your name on ");
	OLED_change_page(5,5);
	OLED_change_coloumn(5, 123);
	OLED_print_string("Ugur's mobile device");
}
void OLED_congrats_highscore(){
	OLED_change_page(1,1);
	OLED_change_coloumn(14, 114);
	OLED_print_string("Press Shoot button");
	OLED_change_page(3,3);
	OLED_change_coloumn(14, 114);
	OLED_print_string("To choose a name ");
}

void OLED_outro() {
// 	OLED_change_page(1,1);
// 	
// 	OLED_change_coloumn(i, i+8);
}

void OLED_print_names(int i){
	OLED_print_string(names[i]);
	
}
char * OLED_get_names(int i){
	
	return names[i];
}
void OLED_change_menu_number(int i){
	menu_number=i;
	
}

void OLED_change_Kp() {
	can_message_t change_Kp;
	char * str[100];
	change_Kp.id = 123;
	change_Kp.data[0] = 1;
	change_Kp.length = 1;
	OLED_change_page(2,2);
	OLED_change_coloumn(90,95);
	Kp_string = itoa2(change_Kp.data[0], str, 10);
	OLED_print_string(Kp_string);
	printf("B1 is %d\n", GAME_BUTTON_GET(1));
	while (GAME_BUTTON_GET(1)!=0) {
		CAN_recieve();
		curr_pos = JOY_direction();
		if(curr_pos!=last_pos){
			last_position++;
			if (JOY_direction() == UP){
				if (change_Kp.data[0] < 9)
					change_Kp.data[0] += 1;
			}
			
			else if(curr_pos != last_pos && JOY_direction() == DOWN){
				if(change_Kp.data[0] > 0)
					change_Kp.data[0] -= 1;
			}
		}
		last_pos = curr_pos;
		
		while(CAN_message_send(change_Kp)){}
		Kp_string = itoa2(change_Kp.data[0], str, 10);
		OLED_print_string(Kp_string);
		//printf("Kp value is %d, id: %d\n", change_Kp.data[0],change_Kp.id);
	}
}
// menu[3][0]="SETTINGS";
// menu[3][1]=" ";
// menu[3][2]="CONTROLLER: Kp=9";
// menu[3][3]="DIFFICULTY: EASY";
// menu[3][4]="SOUND: OFF";
// menu[3][5]="SHOW LOGO";
// menu[3][6]=" ";
// menu[3][7]=" ";
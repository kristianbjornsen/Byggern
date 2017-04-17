/*
 * GAME.c
 *
 * Created: 01.11.2014 17:31:04
 *  Author: ugurac
 */ 
#include "CAN.h"
#include "OLED.h"
#include "GAME.h"
#include <avr/delay.h>
#include <avr/io.h>
uint8_t GAME_id;
int GAME_play,IR_STATUS;
can_message_t GAME_INFO, IR_reset,GAME_START;
int B1,B2,B3,B0,JOYSTICK;
void GAME_init(){
	B0=0;
	B1=0;
	B2=0;
	B3=0;
	JOYSTICK=2;
	GAME_id=5;
	GAME_START.id=2;
	GAME_START.length=1;
	GAME_START.data[0]=1; // 1 game started, 0 ended
	IR_reset.id = 69;
	IR_reset.data[0] = 0;
	GAME_play = 0; // 1 to play
	
	IR_STATUS=0; //IF ball detected status becomes 1 and not detected means 0
}

int GAME_read(can_message_t *message){
	CAN_data_receive(&GAME_INFO);
	
	//FIX dette med CAN_recieve funksjoN§§§§§§§
	//printf("GAME_DATA: %d\n",GAME_INFO.data[0]);
	if(GAME_INFO.id==GAME_id){
		message->id=GAME_INFO.id;
		message->data[0]=GAME_INFO.data[0];
		message->length=GAME_INFO.length;
		return 1;
	}
	return 0;
}

int GAME_main(int flag) {
	if(flag){ // flag = 1 starts the game
		//CAN_message_send(IR_reset); // Reset the hp score by running IR_init() on Node 2
		GAME_start();
		GAME_play = 0;
		return 1;
	}
	return 0;
}
void GAME_OLED_corner(){
	OLED_change_page(0,0);
	for(int i=0; i<98; i++){
		OLED_change_coloumn(i,i);
		write_d(0b00000001);
	}
	OLED_change_coloumn(0,0);
	for(int i=0; i<8; i++){
		OLED_change_page(i,i);
		write_d(0xFF);
	}
	
	OLED_change_page(7,7);
	for(int i=1; i<98; i++){
		OLED_change_coloumn(i,i);
		write_d(0b10000000);
	}
	OLED_change_coloumn(0,0);
	write_d(0xFF);
	
	OLED_change_coloumn(97,97);
	for(int i=0; i<8; i++){
		OLED_change_page(i,i);
		write_d(0xFF);
	}
}
void GAME_OLED_blink(int k){
	OLED_change_page(1,1);
	OLED_change_coloumn(1,97);
	for(int i=1; i<98; i++){
		if(k){
		write_d(0xFF);
		}else{
		write_d(0x00);
		}
	}
	
	OLED_change_page(3,3);
	OLED_change_coloumn(1,97);
	for(int i=1; i<98; i++){
		if(k){
			write_d(0xFF);
		}else{
			write_d(0x00);
		}
	}
	
	OLED_change_page(4,4);
	OLED_change_coloumn(1,97);
	for(int i=1; i<98; i++){
		if(k){
			write_d(0xFF);
			}else{
			write_d(0x00);
		}
	}
	
	OLED_change_page(6,6);
	OLED_change_coloumn(1,97);
	for(int i=1; i<98; i++){
		if(k){
			write_d(0xFF);
		}else{
			write_d(0x00);
		}
	}
}
void GAME_start(){
	OLED_print_logo(1);
	GAME_START.data[0]=1;
	CAN_recieve();
	
	GAME_send_game_condition();//SENDING message about game is starting
	OLED_screen_clear();
	GAME_OLED_corner();
	GAME_button_screen_update();
	
	OLED_change_page(2,2);
	OLED_change_coloumn(10,70);
	OLED_print_string("Lifes left: ");
	
	OLED_change_page(5,5);
	OLED_change_coloumn(10,70);
	OLED_print_string("Your score: ");
	
	//OLED_change_coloumn(65,100);
	
	
	int lives = 9;
	int score_int = 0;
	int score_temp = 0;
	int current_IR, last_IR = 0;
	char *string, *lives_string;
	char str[10];
	while(lives > 0 && B0!=1){
		if(B1==1){
			OLED_screen_clear();
			while(B1!=0){ // Go into Settings menu
				GAME_SETTINGS();			
			}
			OLED_screen_clear();
			GAME_OLED_corner();
			GAME_button_screen_update();
			
			OLED_change_page(2,2);
			OLED_change_coloumn(10,70);
			OLED_print_string("Lifes left: ");
			
			OLED_change_page(5,5);
			OLED_change_coloumn(10,70);
			OLED_print_string("Your score: ");
			
			
		}							// Important because of noise handling. Sporadic values of B0 up to 49, 81 etc
		current_IR = IR_STATUS;
		if(current_IR != last_IR && current_IR == 1) {
			lives--;
		}
		last_IR = current_IR;
		
		
		OLED_change_page(2,2);
		OLED_change_coloumn(70, 80);
		lives_string = itoa2(lives, str, 10);
		OLED_print_string(lives_string);
		
		score_temp++;

		score_int = score_temp/100;
		string = itoa2(score_int, str, 10);
		
		if(lives<3 && score_int%10==0){
			GAME_OLED_blink(1);
			GAME_OLED_blink(0);
		}

		OLED_change_page(5,5);
		OLED_change_coloumn(70, 100);
		OLED_print_string(string);

		if(JOYSTICK==2 && B3==0){
			//Do nothing
			//NODE2COMM_read_joystick_position();
		}else if(B3==0){
			NODE2COMM_read_joystick_position2();
		}
		CAN_recieve();
		//CAN_recieve_new(&button);
		GAME_button_screen_update();
	}
	
	can_message_t GAME_over,ack;
	GAME_over.id = 123;
	GAME_over.length=1;
	GAME_over.data[0]=1;
	ack.id=255;
	while(CAN_message_send(GAME_over)){}
	//for(int i = 2; i <7; i++) { 
		//if(score_int > OLED_get_highscore_menu(1)[i]){	// convert strings to integers
// 			can_message_t GAME_over;					// Send request message to node 3 to initiate Name sequence
// 			GAME_over.id = 123;
// 			GAME_over.length=1;
// 			GAME_over.data[0]=1;
// 			CAN_message_send(GAME_over);
			
			GAME_buttons_reset();						// Print congratulations on the screen
			OLED_screen_clear();
			OLED_congrats_highscore();
			OLED_change_page(5,5);
			OLED_change_coloumn(20,60);
			int name,i=0;
			int j=0;
			
			while(!JOY_button()){
			
				i++;
				j=i/100;
				if(j%10==0){
					OLED_change_coloumn(20,60);
					OLED_print_names(name);
					name++;
				}
				if(name==6){
					name=0;
					i=0;

				}
				
				printf("number is: %d\n",name);
			}
			
			char*new_name=OLED_get_names(name);
			
// 			can_message_t new_name;						// Obtain name
// 			while(new_name.id != 69) {
// 				CAN_data_receive(&new_name);
// 				printf("new_name id: %d\n",new_name.id);
// 			}
// 			while(CAN_message_send(ack)){}
// 			char *name_string;
// 			for(int i = 0; i < new_name.length; i++){
// 				name_string = name_string + new_name.data[i];
// 				printf("%c",new_name.data[i]);
// 			} // name of the player
// 			//GAME_move_numbers(1, OLED_get_highscore_menu(1));
			
			OLED_change_highscore(1, 2, new_name); // third argument string+name_string
			//break;
		//}
	//}
	
	//OLED_change_highcore(1,7,name_string);

	OLED_screen_clear();
	GAME_play = 0;
	GAME_buttons_reset();
// 	CAN_recieve();
// 	GAME_START.data[0]=0;
// 	GAME_send_game_condition();	
	CAN_recieve();
	GAME_send_game_condition();
	 //SENDING message about game is starting
}

void GAME_move_numbers(int i, char *my_list){ // KOM HER
	int temp;
	while(i < (7-1)){
		temp = my_list[i+1];
		my_list[i] = my_list[i+1];
	}
}


char* intTostring(int num, char* str)
{	
	int base=10;
	int i = 0;
	int isNegative = 0;

	/* Handle 0 explicitely, otherwise empty string is printed for 0 */
	if (num == 0)
	{
		str[i++] = '0';
		str[i] = '\0';
		return str;
	}

	// In standard itoa(), negative numbers are handled only with
	// base 10. Otherwise numbers are considered unsigned.
	if (num < 0 && base == 10)
	{
		isNegative = 1;
		num = -num;
	}

	// Process individual digits
	while (num != 0)
	{
		int rem = num % base;
		str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
		num = num/base;
	}

	// If number is negative, append '-'
	if (isNegative)
	str[i++] = '-';

	str[i] = '\0'; // Append string terminator

	// Reverse the string
	printf(str);
	return str;
}

int GAME_CAN_read(){
	
	
}
void GAME_JOYSTICK(can_message_t msg){
	if(msg.data[2]){
		JOYSTICK=2;
	}else{
		JOYSTICK=1;
	}
}
void GAME_button_handler(can_message_t msg){
	if(msg.data[0]==0 || msg.data[0]==1)
		B0=msg.data[0];//EXIT
	if(msg.data[1]==0 || msg.data[1]==1)	
		B1=msg.data[1]; // NG
	if(msg.data[2]==0 || msg.data[2]==1)
		B2=msg.data[2]; //SET
	if(msg.data[3]==0 || msg.data[3]==1)
		B3=msg.data[3]; //BT
	//printf("B1 %d\n",B1);
	//BUTTON_screen_update();
	
}
void GAME_button_screen_update(){
	OLED_change_page(0,0);
	OLED_change_coloumn(100,120);
	if(B3){
		OLED_print_string_invert("BT");
	}else{
		OLED_print_string("BT");
	}
	
	
	OLED_change_page(2,2);
	OLED_change_coloumn(100,120);
	if(B2){
		OLED_print_string_invert("JS");
	}else{
		OLED_print_string("JS");
	}
	
	OLED_change_page(4,4);
	OLED_change_coloumn(100,120);
	if(B1){
		OLED_print_string_invert("SET"); // INVERTERES IKKE PGA IKKE FÅR SIGNAL
		}else{
		OLED_print_string("SET");
	}
	OLED_change_page(6,6);
	OLED_change_coloumn(100,120);
	if(B0){
		//OLED_print_string_invert("Exit");
		}else{
		OLED_print_string("Exit");
	}
	OLED_change_coloumn(98,98);
	for(int i=0; i<8; i++){
		OLED_change_page(i,i);
		write_d(0xFF);
	}
}

void GAME_IR_update(can_message_t msg){
	IR_STATUS=msg.data[0];
	
}
int GAME_IR_STATUS_GET(){
	
	return IR_STATUS;
}

char* GAME_highscore_name(can_message_t read) {
	
	char *new_name;
	int string_length = read.data[0];
	for(int i = 0; i < string_length; i++)
		new_name += (char)read.data[i];
	return new_name;
}


void reverse(char *str , int length)
{
	int start = 0;
	int end = length -1;
	char *temp;
	while (start < end)
	{
		temp[start] = str[end];
		str[end]=str[start];
		str[start]=temp[start];
		start++;
		end--;
	}
}

// Implementation of itoa()
char* itoa2(int num, char* str, int base)
{
	int i = 0;
	int isNegative = 0;
	
	/* Handle 0 explicitely, otherwise empty string is printed for 0 */
	if (num == 0)
	{
		str[i++] = '0';
		str[i] = '\0';
		return str;
	}
	
	// In standard itoa(), negative numbers are handled only with
	// base 10. Otherwise numbers are considered unsigned.
	if (num < 0 && base == 10)
	{
		isNegative = 1;
		num = -num;
	}
	
	// Process individual digits
	while (num != 0)
	{
		int rem = num % base;
		str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
		num = num/base;
	}
	
	// If number is negative, append '-'
	if (isNegative)
	str[i++] = '-';
	
	str[i] = '\0'; // Append string terminator
	
	// Reverse the string
	reverse(str, i);
	
	return str;
}

void GAME_buttons_reset() {
	B0 = 0;
	B1 = 0;
	B2 = 0;
	B3 = 0;
}

void GAME_send_game_condition(){
	while(CAN_message_send(GAME_START)){}
}

void GAME_SETTINGS(){
	CAN_recieve();
	OLED_change_menu_number(3);
	OLED_menu();
	
	//OLED_pointer_position();
	//OLED_menu_print();
}

int GAME_BUTTON_GET(int num){
	return B1;
}
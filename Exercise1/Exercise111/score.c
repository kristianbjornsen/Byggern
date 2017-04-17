/*
 * score.c
 *
 * Created: 16.11.2014 14:47:10
 *  Author: mikaln
 */ 

#include <avr/io.h>
#include "CAN.h"
#include "score.h"


void requestHighScore() {
	can_message_t msg;
	
	msg.id = SCORE_REQUEST_ID;
	msg.length = 0;
	
	can_message_send(&msg);
}

void recieveHighScore(int * arr, can_message_t * msg) {
    
    for (int i = 0; i < 3; i++) {
        arr[i] = msg->data[i];
    }
}


void playMusic() {
	
	can_message_t startMusic;
	startMusic.id = PLAY_MUSIC_ID;
	can_message_send(&startMusic);
	oled_clear_screen();
	oled_set_line(2);
	oled_print_string("PLAYING MUSIC!");
	
}
	
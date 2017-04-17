#include <Arduino.h>
#include <SPI/SPI.h>
#include <interrupt.h>
#include <Servo.h>
#include <Wire.h>
#include <EEPROM.h>

#include "mcp2515_arduino.h"
#include "MCP2515.h"
#include "SPI_arduino.h"
#include "CAN_arduino.h"
#include "PWM.h"
#include "motor.h"
#include "Music.h"

#define scoreId 1
#define onePlayer 2
#define twoPlayer 3
#define SCORE_REQUEST_ID 4
#define SCORE_RECEIVE_ID 5
#define SCORE_SEND_ID 6
#define HIGHSCORE 7
#define END_TWO_PLAYER_GAME 8
#define PLAY_MUSIC_ID 9
#define musicStop 10

//SERVO
Servo myservo;
Servo myservo2;
static int pos;
int old_pos;

//ADC
int sensorPin = A0;
int sensorPinTwo = A14;
int sensorValue = 0;
int sensorValue2 = 0;
int sensorValueA = 0;
int sensorValueB = 0;

// MESSAGES
can_message_t msg;
can_message_t message;
can_message_t scoreMessage;
can_message_t endTwoPlayerGame;
can_message_t musicStopped;

// HIGH SCORE 
unsigned long startTime;
unsigned long endTime;
int timerStarted = 0;
int score;
int sentScore;

// 2P
int twoPlayerScoreA = 0;
int twoPlayerScoreB = 0;
int x_joystick1 = 0;
int y_joystick1 = 0;
int x_joystick2 = 0;
int y_joystick2 = 0;


void setup()
{
	Serial.begin(9600);
	SPI_Init_Master();
	can_Init_Normal();
	Serial.print("INIT DONE. STATUS: ");
	Serial.println(mcp2515_read(MCP_CANSTAT)&MODE_MASK, HEX);
	init_motor();
	init_motor_2();
	myservo.attach(3, 900, 2100);
	myservo2.attach(4,900,2100);
	music_init();
}


void loop()
{
	
	if(mcp2515_read(MCP_CANINTF)&0x03 != 0x00)
	{
			can_data_receive(&msg);
			if(msg.id == twoPlayer){
				x_joystick1 = msg.data[0];
				y_joystick1 = msg.data[1];
				x_joystick2 = msg.data[2];
				y_joystick2 = msg.data[3];
				twoPlayer_score_counter();
			}

			else if (msg.id == onePlayer) {
				if (!timerStarted) {
					startTime = millis();
					timerStarted = 1;
					sentScore = 0;
				}
				x_joystick1 = msg.data[0];
				y_joystick1 = msg.data[1];
				score_counter();
			}

			else if (msg.id == SCORE_REQUEST_ID) {
				sendScoreToOled();
			}

		else if (msg.id == PLAY_MUSIC_ID) {
			play_music();
			musicStopped.id = musicStop;
			can_message_send(&musicStopped);			
		}


	}

 	pwm_set_servo(&myservo,x_joystick1);

	delayMicroseconds(20);

	pwm_set_servo2(&myservo2,x_joystick2);
		
 	motor_write_positon_P(y_joystick1);

	set_direction_2(y_joystick2);
	set_speed_2(1.2*y_joystick2);
}

// ******************** FUNKSJONER ******************** //

void score_counter() {
	for(int j =0; j<4; j++)
		sensorValue += analogRead(sensorPin); 

	sensorValue = sensorValue/4;

	if(sensorValue <100) {
		endTime = millis();
		score = (endTime - startTime)/1000;
		scoreMessage.id = scoreId;
		scoreMessage.length = 1;
		scoreMessage.data[0] = score;
		
		if (sentScore == 0) {
			can_message_send(&scoreMessage);
			addScore(&scoreMessage);
			sentScore = 1;
			timerStarted = 0;
			delay(1000);
			mcp2515_bit_modify(MCP_CANINTF, 0b00000001, 0b00000000); //setter tilbake interrupt flag
		}
	}
}

void twoPlayer_score_counter() {

	for(int j = 0; j<4; j++) 
		sensorValueA += analogRead(sensorPinTwo);
		sensorValueB += analogRead(sensorPin);
	sensorValueA /= 4;
	sensorValueB /= 4;


	if (sensorValueA < 5) {
		while(analogRead(sensorPinTwo) < 45);
		twoPlayerScoreA += 1;
	}

	else if (sensorValueB < 200){
		while (analogRead(sensorPin) < 400);
		twoPlayerScoreB += 1;
	}

	if (twoPlayerScoreA == 3) {
		endTwoPlayerGame.id = END_TWO_PLAYER_GAME;
		endTwoPlayerGame.length = 1;
		endTwoPlayerGame.data[0] = 1;
		can_message_send(&endTwoPlayerGame);
		twoPlayerScoreA = 0;
		twoPlayerScoreB = 0;
		delay(3000);
		}

	else if (twoPlayerScoreB == 3){
		endTwoPlayerGame.id = END_TWO_PLAYER_GAME;
		endTwoPlayerGame.length = 1;
		endTwoPlayerGame.data[0] = 2;
		can_message_send(&endTwoPlayerGame);
		twoPlayerScoreA = 0;
		twoPlayerScoreB = 0;
		delay(3000);
	}
		
}




void addScore(can_message_t * scoreMsg) {
	
	
	if ( scoreMsg->id == 1 ) {
		
		if ( scoreMsg->data[0] > EEPROM.read(0) ) {
			
			EEPROM.write(2, EEPROM.read(1));
			EEPROM.write(1, EEPROM.read(0));
			EEPROM.write(0, scoreMsg->data[0]);
			
		} else if ( scoreMsg->data[0] > EEPROM.read(1) ) {
			
			EEPROM.write(2, EEPROM.read(1));
			EEPROM.write(1, scoreMsg->data[0]);
		
		} else if ( scoreMsg->data[0] > EEPROM.read(2) ) {
			
			EEPROM.write(2, scoreMsg->data[0]);
			
		}
	}
}


void sendScoreToOled() {
		//Retrieve
		can_message_t hiscoremsg;
		hiscoremsg.id = SCORE_SEND_ID;
		hiscoremsg.length = 3; 
		
		for (int8_t i = 0; i < 3; i++) {
			hiscoremsg.data[i] = EEPROM.read(i);
		}
		
		//Send
		can_message_send(&hiscoremsg);
}

void clearHighScore() {
	for (int i = 0; i < 3; i++) {
		EEPROM.write(i,0);
	}
}
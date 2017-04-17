#include "Arduino.h"
#include "SPI.h"
#include "MCP2515.h"
#include "IR.h"
#include "JOYSTICK.h"
#include "MOTOR.h"
int IR_value,analogPin,TRANSISTORpin,IR_STATUS_BALL,IR_STATUS_NO_BALL;
can_message_t IR,GAME;
void IR_init(){
	IR_value=0;
	
	IR_STATUS_BALL=0; //VARIABLES that checks if we have sent the IR status over the CAN,
	IR_STATUS_NO_BALL=0;//	 so we dont send it multiple times and make the CAN bus unnecessary busy
	TRANSISTORpin=7;
	analogPin=0;
	IR.id=5;
	IR.length=1;
	IR.data[0]=0;
}

int IR_read_value(){
	IR_value=analogRead(analogPin);
	
}

bool IR_filter(){
	int sum=0;
	for(int i=0; i<10; i++){
		sum+=IR_read_value();
	}
	
	IR_value=sum/10;
	Serial.print("IR value: ");Serial.println(IR_value);
	if(IR_value>20&& IR_value<70){
		return false;
	}else if(IR_value<=20){ // Ball hit
		Serial.print("Ball Hit"); Serial.println(IR_value);
		return true;
	}else{
		Serial.print("ERROR: IR not functioning!");
		return false;
	}
	//CAN_SEND(IR);
}
void IR_send_data(){
	//Serial.print("IR value: ");Serial.println(IR_value);
	bool statement=IR_filter();
	if(statement && IR_STATUS_BALL==0){
		IR_STATUS_NO_BALL=0;
		IR_STATUS_BALL=1;
		IR.data[0]=1;
		//CAN_SEND(IR); // Count only one point at a time
		//Serial.println("Ball hit")
		
	}else if(statement==false && IR_STATUS_NO_BALL==0) {
		IR_STATUS_NO_BALL=1;
		IR_STATUS_BALL=0;
		IR.data[0]=0;
		//CAN_SEND(IR);
		//CAN_SEND(IR); //Sends 1 if IR detects the ball and 0 if not
	}
	CAN_SEND(IR);
	
}

void IR_count_point(){
	CAN_SEND(IR);
	//Serial.print("Your Point is: ");Serial.println(IR.data[0]); // Had local variable COUNT before

}
void GAME_modus(){
	//JOYSTICK_read(&IR);
	//MOTOR_enable(0);
// 	while((!IR.data[4])){
// 		JOYSTICK_read(&IR);
// 	}
	
}

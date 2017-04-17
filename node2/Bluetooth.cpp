/*
 * Bluetooth.cpp
 *
 * Created: 29.10.2014 19:36:52
 *  Author: ugurac
 */ 

#include "Arduino.h"
#include "MCP2515.h"
int BlUETOOTH_value;
int BTPIN=2;
int BTMODE=0;
unsigned int Ndata[8];
void BLUETOOTH_init(){
	pinMode(BTPIN,OUTPUT);
	digitalWrite(BTPIN,LOW);
}

int BLUETOOTH_read(){
	
	if(Serial.available()){
		BlUETOOTH_value=Serial.read();	
		
	}
	//Serial.print("Bluetooth value: ");Serial.println(BlUETOOTH_value);
	return BlUETOOTH_value;
	
}
void BLUETOOTH_enable(int i){
	if(i){
		digitalWrite(BTPIN,HIGH);
	}else{
		digitalWrite(BTPIN,LOW);
	}
	
}
int BT_MODE(){
	return BTMODE;
}

void BT_change_status(can_message_t msg){
	if(msg.data[3]==0 || msg.data[3]==1)
	BTMODE=msg.data[3];
	if(BTMODE==1){
		BLUETOOTH_enable(1);
	}else if(BTMODE==0){
		BLUETOOTH_enable(0);
	}
	
}
int BLUETOOTH_write(char value){
	Serial.write('n');
}
int* BLUETOOTH_get_name1(){
		BLUETOOTH_write('n');
		Serial.println("Venter");
		int name[8];
		int i=0;
		while(i<9){
			if(Serial.available()){
				Ndata[i]=Serial.read();
				i++;
				
			}
			Serial.println("Venter");
		}
		for(int i=0; i<8; i++){
			Serial.print(Ndata[i]);
		}
		can_message_t NAME;
		NAME.id=69;
		NAME.length=8;
		for(int i=0; i<8; i++){
			NAME.data[i]=Ndata[i];
		}
// 		can_message_t ack;
// 		ack.id=0;
		CAN_SEND(NAME);
// 		Serial.println(ack.id);
// 		while(ack.id!=255){
// 			Serial.println(ack.id);
// 			
// 			CAN_data_receive(&ack);
// 			Serial.println("sender");
// 		}
}
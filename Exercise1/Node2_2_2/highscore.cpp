/*
 * highscore.cpp
 *
 * Created: 16.11.2014 14:36:38
 *  Author: mikaln
 */ 


// void addScore(can_message_t * scoreMsg) {
// 	
// // 	can_message_t msg;
// // 	can_data_receive(msg);
// 	
// 	if ( scoreMsg->id == 1 ) {
// 		
// 		if ( scoreMsg->data[0] > EEPROM.read(0) ) {
// 			
// 			EEPROM.write(2, EEPROM.read(1));
// 			EEPROM.write(1, EEPROM.read(0));
// 			EEPROM.write(0, scoreMsg->data[0]);
// 			
// 		} else if ( scoreMsg->data[0] > EEPROM.read(1) ) {
// 			
// 			EEPROM.write(2, EEPROM.read(1));
// 			EEPROM.write(1, scoreMsg->data[0]);
// 		
// 		} else if ( scoreMsg->data[0] > EEPROM.read(2) ) {
// 			
// 			EEPROM.write(2, scoreMsg->data[0]);
// 			
// 		}
// 	}
// }
// 
// 
// void sendScoreToOled() {
// 	
// 	can_message_t messag;
// 	
// 	can_data_receive(&messag);
// 	
// 	if (messag.id == SCORE_REQUEST_ID)
// 	{
// 		//Retrieve
// 		can_message_t msg;
// 		msg.id = SCORE_SEND_ID;
// 		msg.length = 3; 
// 		
// 		for (int8_t i = 0; i < 3; i++) {
// 			msg.data[i] = EEPROM.read(i);
// 		}
// 		
// 		//Send
// 		can_message_send(&msg);
// 	}
// 	
// 	
// }


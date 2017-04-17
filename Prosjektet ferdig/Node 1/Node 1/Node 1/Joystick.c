#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <math.h>
#include "Joystick.h"
#include "CAN.h"

float x_volt_ref = 128.0;
float y_volt_ref = 128.0;
float x_volt_ref_joystick2 = 130.0;
float y_volt_ref_joystick2 = 133.0;

void joystick_init(){
    
    DDRB    |=  (1 << DDB3) ;  // Button output to rele
    PORTB   |=  (1 << PB3);	 // Set button high
    
    DDRE    |=  (1 << DDE2) ;  // Button output to rele
    PORTE   |=  (1 << PE2);	 // Set button high
    
    float x_offset = 0.0;
    float y_offset = 0.0;
    
    float x_offset_joystick2 = 0.0;
    float y_offset_joystick2 = 0.0;
    
    for (int i = 0; i < 10; i++) {
        
        // Kalibrerer joystick 1 og 2
        
        y_offset += adc_read(1);
        x_offset += adc_read(2);
        _delay_ms(20);
        y_offset_joystick2	+= adc_read(3);
        x_offset_joystick2	+= adc_read(4);
    }
    
    x_volt_ref = (x_offset / 10.0);
    y_volt_ref = (y_offset / 10.0);
    x_volt_ref_joystick2 = (x_offset_joystick2 / 10.0);
    y_volt_ref_joystick2 = (y_offset_joystick2 / 10.0);
}


Position joystick_dir(anal_pos pos) {
    
    if (pos.x_percent > 0+margin) {
        
        if (pos.x_percent > abs(pos.y_percent))
            return RIGHT;
        else if (pos.y_percent > 0)
            return UP;
        else return DOWN;
    }
    
    else if (pos.x_percent < 0 - margin) {
        
        if (abs(pos.x_percent) > abs(pos.y_percent))
            return LEFT;
        else if (pos.y_percent > 0)
            return UP;
        else return DOWN;
    }
    
    else if (pos.y_percent > 0 + margin) return UP;
    else if (pos.y_percent < 0 - margin) return DOWN;
    
    else return NEUTRAL;
}

anal_pos joystick_anal() {
    
    anal_pos var;
    
    var.y_percent = 100*(adc_read(1)-y_volt_ref)/y_volt_ref;
    var.x_percent = 100*(adc_read(2)-x_volt_ref)/x_volt_ref;
    
    
    var.y_percent_joystick2 = 100*(adc_read(3)-y_volt_ref_joystick2)/y_volt_ref_joystick2;
    var.x_percent_joystick2 = 100*(adc_read(4)-x_volt_ref_joystick2)/x_volt_ref_joystick2;
    
    _delay_ms(200); //Noedvendig????????
    
    
    return var;
}

int joy_button_right(){
    return PINB & (1<<1) ;
}

int joy_button_left(){
    return PINB & (1<<2);
}

int joy_button2_right(){
    return PINB & (1<<0) ;
}
/*
int left_slider(){
    
    return((100*adc_read(3))/255);
}

int right_slider(){
    return((100*(adc_read(4))/255));
}
*/
int joystick_button() {
    return PINB & (1<<3);
}

int joystick_send_to_arduino(can_message_t * message) {
    
    anal_pos pos = joystick_anal();
    
    if (message->id == onePlayer) {
        message->data[0] = pos.x_percent;
        message->data[1] = pos.y_percent;
        message->length = 2;
    }
    
    else if (message->id == twoPlayer) {
        message->data[0] = pos.x_percent;
        message->data[1] = pos.y_percent;
        message->data[2] = pos.x_percent_joystick2;
        message->data[3] = pos.y_percent_joystick2;
        message->length = 4;
    }
    
    can_message_send(message);
    
}
/*
void hit(){
    
    if (joy_button_right() == 2){
        PORTB &= ~(1 << PB3);
        _delay_ms(200);
        PORTB |= (1 << PB3);
    }
    
}*/
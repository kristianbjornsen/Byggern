#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <math.h>
#include "Joystick.h"

float x_volt_ref = 128.0;
float y_volt_ref = 128.0;

void joystick_init(){
    
    float x_offset = 0.0;
    float y_offset = 0.0;
    
    for (int i = 0; i < 10; i++){
        
        x_offset += adc_read(2);
        y_offset += adc_read(1);
    }
    
    x_volt_ref = (x_offset / 10.0);
    y_volt_ref = (y_offset / 10.0);
    
}


Position joystick_dir(anal_pos pos){
    
    
    
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
    
    _delay_ms(200);
    
    
    return var;
}

int joy_button_right(){
    return PINB & (1<<1) ;
}

int joy_button_left(){
    return PINB & (1<<2);
}

int left_slider(){
    
    return((100*adc_read(3))/255);
}

int right_slider(){
    return((100*(adc_read(4))/255));
}

int joystick_button() {
    return PINB & (1<<3);
}

int joystick_send_arduino() {
    
    Position pos = NEUTRAL;
    pos = joystick_dir(joystick_anal());
    
    
}
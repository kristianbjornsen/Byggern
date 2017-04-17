#ifndef JOYSTICK_H_
#define JOYSTICK_H_


#define x_volt_max 255
#define y_volt_max 255
#define x_volt_min 0
#define y_volt_min 0
#define margin 10




typedef struct {
    int x_percent;
    int y_percent;
} anal_pos;

typedef enum {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    NEUTRAL
} Position;

void joystick_init();
Position joystick_dir(anal_pos);
anal_pos joystick_anal();
int joystick_button();
int joy_button_left();
int joy_button_right();

int left_slider();
int right_slider();

#endif
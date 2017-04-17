/*
 * OLED.h
 *
 * Created: 17.09.2014 14:00:03
 *  Author: viliusc
 */ 


#ifndef OLED_H_
#define OLED_H_


void OLED_init(void);
void write_c(int adress);
void write_d(int adress);
void OLED_print(char bokstav);
void OLED_print_logo(int choice);
int OLED_print_string(char *list);
void OLED_pointer_position(void);
void OLED_menu_print(void);
void OLED_screen_saver(void);
void OLED_menu(void);
void OLED_change(int to_menu); 
void OLED_loading();
void OLED_print_choice(char*string, int arrow);
void OLED_write_pixel(int y, int x);
void OLED_check_for_game(void);
void OLED_highscores(void);
void OLED_clear(void);
int OLED_print_string_invert(char *list);
void OLED_screen_clear();
void OLED_change_highscore(int menu_nr, int place, char* name_string);
void OLED_congrats_highscore(void);
char* OLED_get_highscore_menu(int menu_nr);
char * OLED_get_names(int i);
void OLED_outro(void);
void OLED_change_menu_number(int i);
void OLED_change_Kp(void);
#endif /* OLED_H_ */
#ifndef DRAW_H
#define DRAW_H

#include <font.h>


void set_pixel(unsigned int, unsigned int, char,char,char);
void get_pixel(unsigned int, unsigned int, char*,char*,char*);
void rect(unsigned int,unsigned int,unsigned int,unsigned int, char,char,char);
void update_screen();
void update_screen_rect(unsigned int,unsigned int,unsigned int,unsigned int);

void draw_symbol(Font*, char, unsigned int, unsigned int);
void draw_string(Font*, char*, unsigned int, unsigned int, unsigned int);


#endif // DRAW_H

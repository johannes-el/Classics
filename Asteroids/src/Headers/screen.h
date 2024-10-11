#ifndef SCREEN_H_
#define SCREEN_H_
#include <raylib.h>
#include "consts.h"

float* init_star_x_positions();
float* init_star_y_positions();
bool draw_start_screen();
void draw_game_screen(float *xPositions, float *yPositions);
void init_rocks();
void unload_rocks();
void throw_rocks();


void init_mouse();
void mouse_anim();
void unload_mouse();

void init_flames();
void draw_flames();

#endif // SCREEN_H_

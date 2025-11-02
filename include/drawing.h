#ifndef DRAWING_H
#define DRAWING_H

#include "arena.h"
#include "robot.h"

// calculate max dimensions
int calculate_max_arena_width();
int calculate_max_arena_height();

// functions called from main
void draw_background(Arena*);
void draw_foreground(Robot*, Arena*);

#endif
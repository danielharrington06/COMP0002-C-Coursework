#ifndef DRAWING_H
#define DRAWING_H

#include "arena.h"
#include "robot.h"

typedef struct {
    int x;
    int y;
} Point;

// calculate drawing dimensions
void calculate_window_dimensions(Arena*);
int caculate_max_window_width();
int calculate_max_window_height();

// background drawing functions
void draw_border(Arena*);
void draw_grid(Arena*);
void draw_obstacle(int, int);
void draw_obstcles(Arena*);

// foreground drawing function
void draw_robot(Robot*);
void draw_markers(Arena**);
void draw_marker(int, int);

// used to translate from arena coord to drawapp coord
Point* get_draw_coord_from_arena_coord(int, int);

#endif
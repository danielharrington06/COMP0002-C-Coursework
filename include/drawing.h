#ifndef DRAWING_H
#define DRAWING_H

#include "arena.h"
#include "robot.h"

typedef struct {
    double x;
    double y;
} Point;

// calculate max arena dimensions
int caculate_max_arena_width();
int calculate_max_arena_height();

// background drawing functions
void draw_border(Arena*);
void draw_grid(Arena*);
void draw_obstacle(int, int);
void draw_obstcles(Arena*);

// general drawing functions
Point* equ_triangle_coords(double);
void rotate_point(Point*, int);
void rotate_points(Point*, int, int);
void draw_triangle(Point*, int, int);

// foreground drawing functions
void draw_robot(Robot*);
void draw_marker(int, int);
void draw_markers(Arena*);

// functions called from main
void calculate_window_dimensions(Arena*);
void draw_background(Arena*);
void draw_foreground(Arena*, Robot*);

#endif
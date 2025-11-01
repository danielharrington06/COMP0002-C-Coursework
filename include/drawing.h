#ifndef DRAWING_H
#define DRAWING_H

#include "arena.h"
#include "robot.h"

// calculate max dimensions
static void calculate_window_dimensions(Arena*);
int caculate_max_arena_width();
int calculate_max_arena_height();

// background drawing functions
static void draw_border(Arena*);
static void draw_grid(Arena*);
static void draw_obstacle(int, int);
static void draw_obstcles(Arena*);

// general drawing functions
static void rotate_point(Point*, int);
static void rotate_points(Point*, int, int);
static void draw_triangle(Point*, int, int);

// foreground drawing functions
static void draw_robot(Robot*);
static void draw_marker(int, int);
static void draw_markers(Arena*);

// functions called from main
void draw_background(Arena*);
void draw_foreground(Arena*, Robot*);

#endif
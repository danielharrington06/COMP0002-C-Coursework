// This file contains functions to draw all of the onscreen features

#include "../include/arena.h"
#include "../include/drawing.h"

#include "../lib/graphics.h"

#include <malloc.h>
#include <math.h>

#define PI 3.141592653

const int TILE_SIZE = 40;
const int GRIDLINE_WIDTH = 2;
const int BORDER_THICKNESS = 8;
const int OBJECT_PADDING = 5; // padding between edge of an obstacle or marker to edge of tile
const int MAX_WINDOW_WIDTH = 1600;
const int MAX_WINDOW_HEIGHT = 800;

// these are determined by arena dimensions and TILE_SIZE and BORDER_THICKNESS
int WINDOW_WIDTH = 0;
int WINDOW_HEIGHT = 0;

// this function calculates window dimensions (width and height); pre-requesite: arenaWidth and arenaHeight are less than their maximum values
void calculate_window_dimensions(Arena *arena) 
{
    WINDOW_WIDTH = 2*BORDER_THICKNESS + TILE_SIZE*arena->arenaWidth + (arena->arenaWidth+1)*GRIDLINE_WIDTH;
    WINDOW_HEIGHT = 2*BORDER_THICKNESS + TILE_SIZE*arena->arenaHeight + (arena->arenaHeight+1)*GRIDLINE_WIDTH;
}

// this function calculates the maximum arenaWidth for the screen, taking into account tile size and display width
int calculate_max_arena_width() 
{
    return (MAX_WINDOW_WIDTH - 2*BORDER_THICKNESS) / TILE_SIZE; // integer division on purpose
}

// this function calculates the maximum arenaHeight for the screen, taking into account tile size and max display height
int calculate_max_arena_height() 
{
    return (MAX_WINDOW_HEIGHT - 2*BORDER_THICKNESS) / TILE_SIZE; // integer division on purpose
}

// this function draws the red border around the screen
void draw_border(Arena *arena) 
{
    setColour(red);
    filRect(0, 0, arena->arenaWidth, arena->arenaHeight);
    setColour(white);
    fillRect(BORDER_THICKNESS, BORDER_THICKNESS, arena->arenaWidth-2*BORDER_THICKNESS, arena->arenaHeight-2*BORDER_THICKNESS);
}

// this function draws the black grid across the screen
void draw_grid(Arena *arena)
{
    setColour(black);
    // vertical lines first
    for (int i = 0; i < arena->arenaWidth + 1; i++) {
        fillRect(BORDER_THICKNESS+i*TILE_SIZE, BORDER_THICKNESS, 2, WINDOW_HEIGHT-2*BORDER_THICKNESS);
    }

    // horizontal lines second
    for (int i = 0; i < arena->arenaHeight + 1; i++) {
        fillRect(BORDER_THICKNESS, BORDER_THICKNESS+i*TILE_SIZE, WINDOW_HEIGHT-2*BORDER_THICKNESS, 2);
    }
}

// this function draws a single obstacle at arena position (x, y)
void draw_obstacle(int x, int y) 
{
    // convert arena position (x, y) to coordinates for top left of shape
    int coordX = BORDER_THICKNESS + x*TILE_SIZE + OBJECT_PADDING;
    int coordY = BORDER_THICKNESS + y*TILE_SIZE + OBJECT_PADDING;
    int obstacle_size = TILE_SIZE-2*OBJECT_PADDING;

    // draw
    setColour(black);
    fillRect(coordX, coordY, obstacle_size, obstacle_size);
}

// this function iterates over arenaGrid and calls the function to render obstacles
void draw_obstacles(Arena *arena)
{
    for (int i = 0; i < arena->arenaWidth; i++) {
        for (int j = 0; j < arena->arenaHeight; i++) {
            if (arena->arenaGrid[i][j] == TILE_OBSTACLE) {
                draw_obstacle(i, j);
            }
        }
    }
}

// this function returns a pointer to an array of Points representing the vertices of an equilateral triangle with its base at the bottom
Point* equ_triangle_coords(double triangle_circumrad)
{
    // a triangle's circumradius is the distance from the center to any vertex

    Point *vertices = malloc(3 * sizeof(Point));
    if (vertices == NULL) {
        return NULL;
    }

    vertices[0].x = 0;
    vertices[0].y = triangle_circumrad;
    vertices[1].x = triangle_circumrad*cos(PI/3);
    vertices[1].y = triangle_circumrad*sin(PI/3);
    vertices[2].x = triangle_circumrad*cos(2*PI/3);
    vertices[2].y = triangle_circumrad*sin(2*PI/3);
}

// this function rotates a single point around the origin
void rotate_point(Point *point, int degrees) 
{
    double radians = ((double)degrees) * PI / 180.0;

    /* uses a matrix rotation 
    (x)(cos(a) -sin(a)) _ (xcos(a) + ysin(a))
    (y)(sin(a) cos(a))) ‾ (-xsin(a) + ycos(a))
    */
    
    double newX = point->x*cos(radians) - point->y*sin(radians);
    double newY = point->x*sin(radians) + point->y*cos(radians);

    point->x = newX;
    point->y = newY;
}

// this function rotates points around origin and returns them in place
void rotate_points(Point* vertices, int numV, int degrees) 
{
    for (int i = 0; i < numV; i++) {
        rotate_point(&vertices[i], degrees);
    }
}

// this function draws the robot at its current arenaGrid position and direction
void draw_robot(Robot *robot) 
{
    /* 
    general idea here is to define the coordinates needed if the robot 
    was drawn on mathetmatical coordinate axes with its center at (0, 0)
     
    this can then be flipped vertically and translated to the correct position
    */

    // triangle radius is the distance from center to vertice
    double triangle_circumrad = TILE_SIZE/2 - BORDER_THICKNESS;

    Point* vertices = equ_triangle_coords(triangle_circumrad);
    if (vertices == NULL) {
        printf("Malloc returned null in equ_triangle_coords\n");
        return;
    }

    rotate_points(vertices, 3, robot->direction*90);

    free(vertices);
}

// this function draws a single marker at arena position (x, y)
void draw_marker(int x, int y) 
{
    // convert arena position (x, y) to coordinates for top left of shape
    int coordX = BORDER_THICKNESS + x*TILE_SIZE + OBJECT_PADDING;
    int coordY = BORDER_THICKNESS + y*TILE_SIZE + OBJECT_PADDING;
    int obstacle_size = TILE_SIZE-2*OBJECT_PADDING;

    // draw
    setColour(gray);
    fillRect(coordX, coordY, obstacle_size, obstacle_size);
}

// this function iterates over arenaGrid and calls the function to render markers
void draw_markers(Arena *arena)
{
    for (int i = 0; i < arena->arenaWidth; i++) {
        for (int j = 0; j < arena->arenaHeight; i++) {
            if (arena->arenaGrid[i][j] == TILE_MARKER) {
                draw_marker(i, j);
            }
        }
    }
}
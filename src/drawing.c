// This file contains functions to draw all of the onscreen features

#include "../include/arena.h"
#include "../include/drawing.h"

const int TILE_SIZE = 40;
const int GRIDLINE_WIDTH = 2;
const int BORDER_THICKNESS = 8;
const int OBJECT_PADDING = 5; // padding between edge of 

// these are determined by arena dimensions and TILE_SIZE and BORDER_THICKNESS
int WINDOW_WIDTH = 0;
int WINDOW_HEIGHT = 0;

// this function calculates window dimensions (width and height)
// pre-requesite is that arenaWidth and arenaHeight are less than their maximum values (determined by what can fit on screen)
void calculate_window_dimensions(Arena *arena) 
{
    WINDOW_WIDTH = 2*BORDER_THICKNESS + TILE_SIZE*arena->arenaWidth + (arena->arenaWidth+1)*GRIDLINE_WIDTH;
    WINDOW_HEIGHT = 2*BORDER_THICKNESS + TILE_SIZE*arena->arenaHeight + (arena->arenaHeight+1)*GRIDLINE_WIDTH;
}

// this function calculates the maximum arenaWidth for the screen, taking into account tile size and display width
int calculate_max_window_width(Arena *arena) 
{

}

// this function calculates the maximum arenaHeight for the screen, taking into account tile size and max display height
int calculate_max_window_height(Arena *arena) 
{

}

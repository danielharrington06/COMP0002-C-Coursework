// This function contains utility functions used by numerous programs
#include "../include/utils.h"
#include "../include/robot.h"

#include <stdlib.h>
#include <math.h>

//  this function generates a random Direction from robot.c to use to generate a cluster
Direction random_direction()
{
    return (Direction)(rand() % 4);
}

// this function generates a random coordinate from 0 to width_height which represents either the width or height of the grid
int random_coord(int width_height)
{
    return rand() % width_height;
}

// this function finds the min of two integers
int min(int a, int b) 
{
    if (a < b) return a;
    return b;
}

// this function finds the max of two integers
int max(int a, int b)
{
    if (a > b) return a;
    return b;
}

// this function checks that a coordinate is in bounds of the grid, returning 1 if in bounds and 0 if out of bounds; coord, width, height
int check_coord_in_bounds(Coord *coord, int width, int height)
{
    // check out of bounds
    if (coord->x < 0 || coord->y < 0) return 0;
    if (coord->x >= width || coord->y >= height) return 0;

    return 1;
}

// this function calculates the squared distance between two coordinates; x1, y1, x2, y2
double calc_squared_dist_coords(int x1, int y1, int x2, int y2)
{
    return pow(x1 - x2, 2) + pow(y1 - y2, 2);
}
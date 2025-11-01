// This function contains utility functions used by numerous programs
#include "../include/utils.h"

#include <stdlib.h>

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
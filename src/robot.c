// This file contains code to allow the robot to move and the algorithm for it to follow

#include "../include/arena.h"
#include "../include/drawing.h"
#include "../include/robot.h"

#include "../lib/graphics.h"

#include <stdlib.h>

// this function causes the robot to move forward in current direction; pre-requisite: can_move_forward() is true
static void forward(Robot *robot) 
{
    switch(robot->direction) {
        case (NORTH):
            robot->y--;
            break;
        case(EAST):
            robot->x++;
            break;
        case(SOUTH):
            robot->y++;
            break;
        case(WEST):
            robot->x--;
    }
}

// this function rotates the robot 90 degrees anticlockwise (left 90 degree turn)
static void turn_left(Robot *robot) 
{
    robot->direction = robot->direction + 3 % 4;
}

// this function rotates the robot 90 degrees clockwise (right 90 degree turn)
static void turn_right(Robot *robot) 
{
    robot->direction = robot->direction + 1 % 4;
}

// this function checks if the robot is at the marker
static int is_at_marker(Robot *robot, Arena *arena) 
{
    return arena->arenaGrid[robot->x][robot->y] == TILE_MARKER;
}

// this function checks if the robot can move forward
static int can_move_forward(Robot *robot, Arena *arena) 
{
    // store a copy of x and y
    int x = robot->x;
    int y = robot->y;

    // simulate the forward movement on copied x and y
    switch(robot->direction) {
        case (NORTH):
            y--;
            break;
        case(EAST):
            x++;
            break;
        case(SOUTH):
            y++;
            break;
        case(WEST):
            x--;
    }

    // check if it hits an obstacle
    return !(arena->arenaGrid[x][y] == TILE_OBSTACLE); //negate as comparison is checking if it is obstacle
}

// this function removes a marker from the arena and adds it to the robot's collection; pre-requisite: is_at_marker() is true
static void pickup_marker(Robot *robot, Arena *arena) 
{
    arena->arenaGrid[robot->x][robot->y] = TILE_EMPTY;
    arena->numMarker--;
    robot->markerCount++;
}

// this function drops a marker onto the grid; pre-requesite: is_at_marker() is false
static void drop_marker(Robot *robot, Arena *arena) 
{
    robot->markerCount--;
    arena->numMarker++;
    arena->arenaGrid[robot->x][robot->y] = TILE_MARKER;
}

// this function returns the number of markers the robot is carrying
static int get_marker_carry_count(Robot *robot) 
{
    return robot->markerCount;
}

// this function returns the number of markers left on the grid
static int get_marker_arena_count(Arena *arena) 
{
    return arena->numMarker;
}

//  this function generates a random Direction from robot.c to use to generate a cluster
Direction random_direction()
{
    return (Direction)(rand() % 4);
}


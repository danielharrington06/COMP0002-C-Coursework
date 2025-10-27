// This file contains code to allow the robot to move and the algorithm for it to follow

#include "../include/arena.h"
#include "../include/drawing.h"
#include "../include/robot.h"

#include "../lib/graphics.h"

// this function causes the robot to move forward in current direction
// pre-requisite is that can_move_forward has already been called to check
void forward(Robot *robot) {

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
void anticlockwise_turn(Robot *robot) {

    robot->direction = robot->direction + 3 % 4;
}

// this function rotates the robot 90 degrees clockwise (right 90 degree turn)
void clockwise_turn(Robot *robot) {

    robot->direction = robot->direction + 1 % 4;
}

// this function checks if the robot is at the marker
int is_at_marker(Robot *robot, Arena *arena) {

    return arena->arenaGrid[robot->x][robot->y] == TILE_MARKER;
}

// this function checks if the robot can move forward
int can_move_forward(Robot *robot, Arena *arena) {

    // store x and y for simplicity
    int x = robot->x;
    int y = robot->y;

    // simulate the forward movement
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
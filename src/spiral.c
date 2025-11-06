// This program contains the spiral algorithm the robot uses to visit all available tiles

#include "../include/drawing.h"
#include "../include/robot.h"
#include "../include/spiral.h"
#include "../include/utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <malloc.h>

// functions to deal with the path using stack implementation from utils.h

// this function creates the stack and pushes the current position (start to it)
static Stack* setup_path_stack(Robot *robot)
{
    Stack* path = create_stack(robot->arenaWidth*robot->arenaHeight*2); // capacity assumes travelling over each tile
    push(path, (Coord){robot->x, robot->y});

    return path;
}

// this function pushes the current robot position to the stack
static void push_pos_to_path(Robot *robot, Stack *path)
{
    push(path, (Coord){robot->x, robot->y});
}

// this function returns the Coord at the top of the stack
static Coord backtrack_path_tile(Stack *path)
{
    pop(path);
    return peek(path);
}

// this function gets the cardinal direction of an adjacent Coord tile; pre-requisite: tile is adjacent to robot
static Direction direction_of_adj_tile(Robot *robot, Coord tile)
{
    int dx = tile.x - robot->x;
    int dy = tile.y - robot->y;
    if (dx == 1) {
        return EAST;
    }
    else if (dx == -1) {
        return WEST;
    }
    else if (dy == 1) {
        return SOUTH;
    }
    else if (dy == -1) {
        return NORTH;
    }
    else {
        fprintf(stderr, "Tile cannot have been adjacent to robots position in direction_of_adj_tile\n");
        exit(EXIT_FAILURE);
    }
}

// this function checks if the current tile is a marker and if so picks it up
static void check_for_and_pickup_marker(Robot *robot, Arena *arena)
{
    if (is_at_marker(robot, arena)) {
        pickup_marker(robot, arena);
        draw_foreground(robot, arena);
    }
}

// this function rotates the robot until its direction matches a specified direction
static void rotate_to_direction(Robot *robot, Arena *arena, Direction direction)
{
    int offset = (4 + direction - robot->direction) % 4; // 4 + needed to ensure % works as MOD not remainder
    switch (offset) {
        case (0):
            // already in correct direction
            break;
        case(1):
            // turn once right
            turn_right(robot);
            draw_foreground(robot, arena);
            break;
        case(2):
            // turn 180 degrees
            turn_right(robot);
            draw_foreground(robot, arena);
            turn_right(robot);
            draw_foreground(robot, arena);
            break;
        case(3):
            turn_left(robot);
            draw_foreground(robot, arena);
            break;
        default:
            fprintf(stderr, "offset not 0 to 3 in rotate_to_direction\n");
            exit(EXIT_FAILURE);
            break;
    }
}

// this function returns the coordinates of the nearest unvisited tile; pre-requisite: num unknown tiles > 0
static Coord get_nearest_unknown_tile(Robot *robot)
{
    // set minDistSqr to height^2 + width^2 as this is greater than the great
    double minDistSqr = pow(robot->arenaHeight, 2) + pow(robot->arenaWidth, 2);
    Coord closestCoord = {-1, -1};

    for (int y = 0; y < robot->arenaHeight; y++) {
        for (int x = 0; x < robot->arenaWidth; x++) {
            double dist = calc_squared_dist_coords(robot->x, robot->y, x, y);
            if (dist < minDistSqr) {
                minDistSqr = dist;
                closestCoord.x = x;
                closestCoord.y = y;
            }
        }
    }

    // return NULL for out of bounds coords (not set) - this should never occur
    if (!check_coord_in_bounds(closestCoord, robot->arenaWidth, robot->arenaHeight)) {
        fprintf(stderr, "get_nearest_unknown_tile found no unvisited tile\n");
        exit(EXIT_FAILURE);
    }

    return closestCoord;
}

// this function moves the robot to its starting position to facilitate spiral algorithm
static void reach_spiral_start(Robot *robot, Arena *arena, Stack *path)
{
    // draw starting position
    draw_foreground(robot, arena);

    // move forward until an obstacle or arena wall is faced
    while (can_move_forward(robot, arena) && get_marker_arena_count(arena)) 
    {
        forward(robot);
        push_pos_to_path(robot, path);
        draw_foreground(robot, arena);
        
        check_for_and_pickup_marker(robot, arena);
    }

    turn_right(robot);
    draw_foreground(robot, arena);
    mark_current_tile_visited(robot);
}

// this function spirals round, using a left hand to wall technique, moving in a section as needed
static void spiral_step(Robot *robot, Arena *arena, Stack *path)
{
    if (check_left_tile_unknown(robot)) {
        turn_left(robot);
    }
    else if (can_move_forward(robot, arena) && check_forward_tile_unknown(robot)) { 
        // ahead in bounds and tile is unknown
        forward(robot);
        mark_current_tile_visited(robot);
        push_pos_to_path(robot, path);
    }
    else if (can_move_forward(robot, arena) && !check_forward_tile_unknown(robot)) { 
        // ahead in bounds but already visited
        turn_right(robot);
    }
    else { 
        // out of bounds or an obstacle
        mark_ahead_tile_obstacle(robot);
        turn_right(robot);
    }

    draw_foreground(robot, arena);
    check_for_and_pickup_marker(robot, arena);
}

// this function backtracks, popping nodes from its route as long as it is called
static void backtrack_step(Robot *robot, Arena *arena, Stack *path)
{
    Coord prevTile = backtrack_path_tile(path);
    Direction dirOfPrevTile = direction_of_adj_tile(robot, prevTile);
    rotate_to_direction(robot, arena, dirOfPrevTile);
    draw_foreground(robot, arena);
    forward(robot); // should not push position to path as currently at that position
    draw_foreground(robot, arena);
}

// this function attempts to move back onto an unknown tile
static int move_onto_unknown_tile(Robot *robot, Arena *arena, Stack *path)
{
    Coord unvisitedTile = adjacent_unvisited_tile(robot);
    Direction dir = direction_of_adj_tile(robot, unvisitedTile);
    rotate_to_direction(robot, arena, dir);
    draw_foreground(robot, arena);
    if (can_move_forward(robot, arena)) {
        forward(robot);
        mark_current_tile_visited(robot);
        push_pos_to_path(robot, path);
        draw_foreground(robot, arena);
        fprintf(stderr, "Backtrack succesful\n"); // !! remove later
        return 1;
    }
    return 0;
}

// this function moves forward until it reaches the edge of the arena or an obstacle and spirals inwards to find all markers
void find_markers(Robot *robot, Arena *arena)
{
    Stack* path = setup_path_stack(robot);

    reach_spiral_start(robot, arena, path);

    // then spiral clockwise (by keeping already visited tiles or unvisitable tiles to the left)
    while (get_marker_arena_count(arena) > 0) // !! change this to count num unvisited tiles
    { 
        while (!is_surrounded_by_known(robot) && get_marker_arena_count(arena) > 0)
        {
            spiral_step(robot, arena, path);
        }

        int on_unknown_tile = 0;
        while (!on_unknown_tile && get_marker_arena_count(arena) > 0)
        {
            while (is_surrounded_by_known(robot) && get_marker_arena_count(arena) > 0)
            {
                backtrack_step(robot, arena, path);
            }

            on_unknown_tile = move_onto_unknown_tile(robot, arena, path);
        }
    }
}

/*
current plan: just store where has been and backtrack until reaching a position where the robot is not surrounded
*/

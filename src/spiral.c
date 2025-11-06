// This program contains the spiral algorithm the robot uses to visit all available tiles

#include "../include/robot.h"
#include "../include/spiral.h"

#include "stdlib.h"
#include "stdio.h"

// main algorithm to find markers:

// this function checks if the current tile is a marker and if so picks it up
static void check_for_and_pickup_marker(Robot *robot, Arena *arena)
{
    if (is_at_marker(robot, arena)) {
        pickup_marker(robot, arena);
        draw_foreground(robot, arena);
    }
}

// this function returns the coordinates of the nearest unvisited tile; pre-requisite: num unknown tiles > 0
static Coord* get_nearest_unknown_tile(Robot *robot)
{
    // set minDistSqr to height^2 + width^2 as this is greater than the great
    double minDistSqr = pow(robot->arenaHeight, 2) + pow(robot->arenaWidth, 2);
    Coord *closestCoord = malloc(sizeof(Coord));
    closestCoord->x = -1;
    closestCoord->y = -1;

    for (int y = 0; y < robot->arenaHeight; y++) {
        for (int x = 0; x < robot->arenaWidth; x++) {
            double dist = calc_squared_dist_coords(robot->x, robot->y, x, y);
            if (dist < minDistSqr) {
                minDistSqr = dist;
                closestCoord->x = x;
                closestCoord->y = y;
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

// this function moves the robot from its current position to the target coordinate
//.. the nature of the spiral method is such that the nearest unvisited node will 
//.. always be reachable by travelling over already visited routes
static void move_to_coord(Robot *robot, Arena *arena, Coord target)
{

}

// this function moves forward until it reaches the edge of the arena or an obstacle and spirals inwards to find all markers
void find_markers(Robot *robot, Arena *arena)
{
    // draw starting position
    draw_foreground(robot, arena);

    // move forward until an obstacle or arena wall is faced
    while (can_move_forward(robot, arena)) 
    {
        forward(robot);
        draw_foreground(robot, arena);
        
        check_for_and_pickup_marker(robot, arena);
    }

    turn_right(robot);
    draw_foreground(robot, arena);

    // then spiral clockwise (by keeping already visited tiles or unvisitable tiles to the left)
    while (get_marker_arena_count(arena) > 0) // !! change this to count num unvisited tiles
    { 
        if (check_left_tile_unknown(robot)) {
            turn_left(robot);
        }
        else if (can_move_forward(robot, arena) && check_forward_tile_unknown(robot)) { // ahead in bounds and R_UNKNOWN
            mark_current_tile_visited(robot);
            forward(robot);
        }
        else if (can_move_forward(robot, arena) && !check_forward_tile_unknown(robot)) { // ahead in bounds and R_VISITED
            turn_right(robot);
        }
        else { // out of bounds or R_BLOCKED
            mark_ahead_tile_obstacle(robot);
            turn_right(robot);
        }
        draw_foreground(robot, arena);

        check_for_and_pickup_marker(robot, arena);
    }
}


/* 
big idea: store a list of the route that has been travelled - backtrack along this until adjacent to necessary tile
when single wall: go towards nearest unvisited, backtracking the entire loop by checking if the node is in there and 
popping off the list of nodes as far as needed until an obstacle is hit, at which point follow backtrack until the
direction the program would go can be travelled to
*/

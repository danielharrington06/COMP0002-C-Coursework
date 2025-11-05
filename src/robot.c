// This file contains code to allow the robot to move and the algorithm for it to follow

#include "../include/arena.h"
#include "../include/drawing.h"
#include "../include/robot.h"
#include "../include/utils.h"

#include "../lib/graphics.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <math.h>

// this function returns the coord of the point that is forward (which may be out of bounds); caller has responsibility to free
static Coord* get_forward_coord(Robot *robot)
{
    // create a copy of x and y to return later 
    Coord *coord = malloc(sizeof(coord));
    coord->x = robot->x;
    coord->y = robot->y;

    // simulate the forward movement on coord in correct direction
    switch(robot->direction) {
        case(NORTH):
            coord->y--;
            break;
        case(EAST):
            coord->x++;
            break;
        case(SOUTH):
            coord->y++;
            break;
        case(WEST):
            coord->x--;
    }

    return coord;
}

// this function returns the coord of the point that is left (which may be out of bounds); caller has responsibility to free
static Coord* get_left_coord(Robot *robot)
{
    // create a copy of x and y to return later 
    Coord *coord = malloc(sizeof(coord));
    coord->x = robot->x;
    coord->y = robot->y;

    // simulate the leftward movement on coord in correct direction
    switch(robot->direction) {
        case(NORTH):
            coord->x--;
            break;
        case(EAST):
            coord->y--;
            break;
        case(SOUTH):
            coord->x++;
            break;
        case(WEST):
            coord->y++;
    }

    return coord;
}

// this function causes the robot to move forward in current direction; pre-requisite: can_move_forward() is true
static void forward(Robot *robot) 
{
    Coord *coord = get_forward_coord(robot);
    if (coord == NULL) {
        fprintf(stderr, "get_forward_coord returned NULL pointer\n");
        exit(EXIT_FAILURE);
    }
    robot->x = coord->x;
    robot->y = coord->y;
    free(coord);
}

// this function rotates the robot 90 degrees anticlockwise (left 90 degree turn)
static void turn_left(Robot *robot) 
{
    robot->direction = (robot->direction + 3) % 4;
}

// this function rotates the robot 90 degrees clockwise (right 90 degree turn)
static void turn_right(Robot *robot) 
{
    robot->direction = (robot->direction + 1) % 4;
}

// this function checks if the robot is at the marker
static int is_at_marker(Robot *robot, Arena *arena) 
{
    return arena->arenaGrid[robot->y][robot->x] == T_MARKER;
}

// this function checks if the robot can move forward
static int can_move_forward(Robot *robot, Arena *arena) 
{
    Coord *coord = get_forward_coord(robot);
    if (coord == NULL) {
        fprintf(stderr, "can_move_forward returned NULL pointer\n");
        exit(EXIT_FAILURE);
    }

    // check out of bounds
    if (!check_coord_in_bounds(coord, robot->arenaWidth, robot->arenaHeight)) return 0;

    // check if it hits an obstacle
    int obstacle_ahead = arena->arenaGrid[coord->y][coord->x] == T_OBSTACLE;
    free(coord);

    return !obstacle_ahead; // negate as function returns true if in bounds and not obstacle
}

// this function removes a marker from the arena and adds it to the robot's collection; pre-requisite: is_at_marker() is true
static void pickup_marker(Robot *robot, Arena *arena) 
{
    arena->arenaGrid[robot->y][robot->x] = T_EMPTY;
    arena->numMarker--;
    robot->markerCount++;
}

// this function drops a marker onto the grid; pre-requesite: is_at_marker() is false
static void drop_marker(Robot *robot, Arena *arena) 
{
    robot->markerCount--;
    arena->numMarker++;
    arena->arenaGrid[robot->y][robot->x] = T_MARKER;
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

// this function checks the robot's memory to see if the tile ahead is unknown (and reachable)
static int check_forward_tile_unknown(Robot *robot)
{
    Coord *coord = get_forward_coord(robot);
    if (coord == NULL) {
        fprintf(stderr, "check_forward_tile_unknown returned NULL pointer\n");
        exit(EXIT_FAILURE);
    }
    
    // check for out of bounds
    if (!check_coord_in_bounds(coord, robot->arenaWidth, robot->arenaHeight)) return 0;

    int is_unknown = robot->memory[coord->y][coord->x] == R_UNKNOWN; // other options are visited and blocked, neither of which we want
    free(coord);
    return is_unknown;
}

// this function checks the robot's memory to see if the tile to its left is unknown (and reachable)
static int check_left_tile_unknown(Robot *robot)
{
    Coord *coord = get_left_coord(robot);
    if (coord == NULL) {
        fprintf(stderr, "check_left_tile_unknown returned NULL pointer\n");
        exit(EXIT_FAILURE);
    }
    
    // check for out of bounds
    if (!check_coord_in_bounds(coord, robot->arenaWidth, robot->arenaHeight)) return 0;

    int is_unknown = robot->memory[coord->y][coord->x] == R_UNKNOWN; // other options are visited and blocked, neither of which we want
    free(coord);
    return is_unknown;
}

// this function sets the current tile to visited in robot's memory
static void mark_current_tile_visited(Robot *robot)
{
    robot->memory[robot->y][robot->x] = R_VISITED;
}

// this function marks the tile in front as obstacle if not out of bounds
static void mark_ahead_tile_obstacle(Robot *robot)
{
    Coord *coord = get_forward_coord(robot);
    if (coord == NULL) {
        fprintf(stderr, "get_forward_coord returned NULL pointer\n");
        exit(EXIT_FAILURE);
    }

    // check out of bounds
    if (!check_coord_in_bounds(coord, robot->arenaWidth, robot->arenaHeight)) return;

    robot->memory[coord->y][coord->x] = R_BLOCKED;
    free(coord);
}

// this function checks if the current tile is a marker and if so picks it up
static void check_for_and_pickup_marker(Robot *robot, Arena *arena)
{
    if (is_at_marker(robot, arena)) {
        pickup_marker(robot, arena);
        draw_foreground(robot, arena);
    }
}

// this function counts the number of unknown tiles in the arena
int num_unknown_tiles(Robot *robot)
{
    int count = 0;
    for (int y = 0; y < robot->arenaHeight; y++) {
        for (int x = 0; x < robot->arenaWidth; x++) {
            if (robot->memory[y][x] == R_UNKNOWN) count++;
        }
    }
    return count;
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

// functions to deal with robot struct:

// this function allocates memory for robot's memory
static void allocate_robots_memory(Robot *robot)
{
    // get width and height for ease
    int width = robot->arenaWidth;
    int height = robot->arenaHeight;

    // allocate memory for row pointers
    robot->memory = calloc(height, sizeof(RobotTile *));
    if (robot->memory == NULL) {
        fprintf(stderr, "Failed to allocate memory for robot memory row pointers\n");
        exit(EXIT_FAILURE);
    }

    // allocate each row
    for (int i = 0; i < height; i++) {
        robot->memory[i] = calloc(width, sizeof(RobotTile));
        if (robot->memory[i] == NULL) {
            fprintf(stderr, "Failed to allocate memory for a row in robot memory\n");
            // free already allocated memory
            for (int j = 0; j < i; j++) {
                free(robot->memory[j]);
            }
            free(robot->memory);
            exit(EXIT_FAILURE);
        }
    }
}

// this function creates a robot struct; pre-requisite: arena dimensions already set; caller has responsibility to free
Robot* create_robot(Arena *arena)
{
    // allocate memory
    Robot* robot = malloc(sizeof(Robot));
    if (robot == NULL) return NULL;

    // these will be changed
    robot->x = 0;
    robot->y = 0;
    robot->direction = NORTH;

    // this is correct starting value
    robot->markerCount = 0;
    robot->arenaWidth = arena->arenaWidth;
    robot->arenaHeight = arena->arenaHeight;

    // allocate robot's memory
    allocate_robots_memory(robot);

    return robot;
}

// this function frees robot memory's memory
static void free_robots_memory(Robot *robot)
{
    for (int i = 0; i < robot->arenaHeight; i++) {
        free(robot->memory[i]);
    }
    free(robot->memory);
}

// this function frees overall robot struct memory
void free_robot(Robot *robot)
{
    free_robots_memory(robot);
    free(robot);
}

// this function randomly assigns the robot to a position in the arena
static void place_robot_random(Robot *robot, Arena *arena)
{
    // generate x and y until empty tile    
    int x, y;
    do {
        // add 1 and -2 is used to not place robot at edge
        x = 1 + random_coord(robot->arenaWidth-2);
        y = 1 + random_coord(robot->arenaHeight-2);
    } while (arena->arenaGrid[y][x] != T_EMPTY);

    // assign this as robot start on arena 
    arena->arenaGrid[y][x] = T_R_START;
    
    // assign values to robot
    robot->x = x;
    robot->y = y;
    robot->direction = random_direction();
}

// this function places the robot with a specific 
static void place_robot_specific(Robot *robot, Arena *arena, Coord coord, Direction direction)
{
    // if the entered position is taken, place the robot randomly
    if (arena->arenaGrid[coord.y][coord.x] != T_EMPTY) {
        place_robot_random(robot, arena);
        return;
    }

    // assign x, y as start on arena
    arena->arenaGrid[coord.y][coord.x] = T_R_START;

    // assign values to robot
    robot->x = coord.x;
    robot->y = coord.y;
    robot->direction = direction;
}

// this function parses an entered direction
static Direction parse_direction(const char *input) {
    // make a lowercase copy for case-insensitive comparison
    char dir[16];
    int i;
    for (i = 0; input[i] && i < 15; i++)
        dir[i] = tolower((unsigned char)input[i]);
    dir[i] = '\0';

    if (strcmp(dir, "north") == 0) return NORTH;
    if (strcmp(dir, "east") == 0) return EAST;
    if (strcmp(dir, "south") == 0) return SOUTH;
    if (strcmp(dir, "west") == 0) return WEST;

    // signals invalid direction, will lead to position being randomised
    return -1;
}

// this function deals with command line inputs and either places robot in given position or randomly places it
void place_robot(int argc, char *argv[], Robot *robot, Arena *arena)
{
    // specific position
    if (argc == 6) {
        Coord coord;
        coord.x = atoi(argv[3]);
        coord.y = atoi(argv[4]);
        Direction direction = parse_direction(argv[5]);

        // check out of bounds - if so, give random position and direction
        if (!check_coord_in_bounds(&coord, robot->arenaWidth, robot->arenaHeight)) {
            printf("Error: x and y must be between 0 and %d / %d. Random position and direction generated.\n", robot->arenaWidth - 1, robot->arenaHeight - 1);
            place_robot_random(robot, arena);
            return;
        }

        // check invalid direction - if so, give random direction, but we know x, y is in range
        if (direction == -1) {
            printf("Error: direction must be north, east, south, west. Random direction generated.\n");
            place_robot_specific(robot, arena, coord, random_direction());
            return;
        }

        // valid x, y, direction
        place_robot_specific(robot, arena, coord, direction);
        return;
    }

    place_robot_random(robot, arena);
}

// main algorithm to find markers:

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
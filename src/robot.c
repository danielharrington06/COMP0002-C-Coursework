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

// this function returns the coord of the point that is in the direction specified (this can be adjusted to work for non forward directions by offsetting robot->direction) (which may be out of bounds); caller has responsibility to free
Coord get_coord_in_direction(Robot *robot, Direction direction)
{
    Coord coord = {robot->x, robot->y}; // create a copy of x and y to return later 
    switch(direction) { // simulate the forward movement on coord in correct direction
        case(NORTH):
            coord.y--;
            break;
        case(EAST):
            coord.x++;
            break;
        case(SOUTH):
            coord.y++;
            break;
        case(WEST):
            coord.x--;
    }
    return coord;
}

// this function causes the robot to move forward in current direction; pre-requisite: can_move_forward() is true
void forward(Robot *robot) 
{
    Coord coord = get_coord_in_direction(robot, robot->direction);
    robot->x = coord.x;
    robot->y = coord.y;
}

// this function rotates the robot 90 degrees anticlockwise (left 90 degree turn)
void turn_left(Robot *robot) 
{
    robot->direction = (robot->direction + 3) % 4;
}

// this function rotates the robot 90 degrees clockwise (right 90 degree turn)
void turn_right(Robot *robot) 
{
    robot->direction = (robot->direction + 1) % 4;
}

// this function checks if the robot is at the marker
int is_at_marker(Robot *robot, Arena *arena) 
{
    return arena->arenaGrid[robot->y][robot->x] == T_MARKER;
}

// this function checks if the robot can move forward
int can_move_forward(Robot *robot, Arena *arena) 
{
    Coord coord = get_coord_in_direction(robot, robot->direction);

    // check out of bounds
    if (!check_coord_in_bounds(coord, robot->arenaWidth, robot->arenaHeight)) return 0;

    // check if it hits an obstacle
    int obstacle_ahead = arena->arenaGrid[coord.y][coord.x] == T_OBSTACLE;

    return !obstacle_ahead; // negate as function returns true if in bounds and not obstacle
}

// this function removes a marker from the arena and adds it to the robot's collection; pre-requisite: is_at_marker() is true
void pickup_marker(Robot *robot, Arena *arena) 
{
    arena->arenaGrid[robot->y][robot->x] = T_EMPTY;
    arena->numMarker--;
    robot->markerCount++;
}

// this function drops a marker onto the grid; pre-requesite: is_at_marker() is false
void drop_marker(Robot *robot, Arena *arena) 
{
    robot->markerCount--;
    arena->numMarker++;
    arena->arenaGrid[robot->y][robot->x] = T_MARKER;
}

// this function returns the number of markers the robot is carrying
int get_marker_carry_count(Robot *robot) 
{
    return robot->markerCount;
}

// this function returns the number of markers left on the grid
int get_marker_arena_count(Arena *arena) 
{
    return arena->numMarker;
}

// this function checks the robot's memory to see if the tile ahead is unknown (and reachable)
int check_forward_tile_unknown(Robot *robot)
{
    Coord coord = get_coord_in_direction(robot, robot->direction);
    
    // check for out of bounds
    if (!check_coord_in_bounds(coord, robot->arenaWidth, robot->arenaHeight)) return 0;

    return robot->memory[coord.y][coord.x] == R_UNKNOWN; // other options are visited and blocked, neither of which we want
}

// this function checks the robot's memory to see if the tile to its left is unknown (and reachable)
int check_left_tile_unknown(Robot *robot)
{
    Coord coord = get_coord_in_direction(robot, (robot->direction - 1) % 4);
    
    // check for out of bounds
    if (!check_coord_in_bounds(coord, robot->arenaWidth, robot->arenaHeight)) return 0;

    return robot->memory[coord.y][coord.x] == R_UNKNOWN; // other options are visited and blocked, neither of which we want
}

// this function sets the current tile to visited in robot's memory
void mark_current_tile_visited(Robot *robot)
{
    robot->memory[robot->y][robot->x] = R_VISITED;
}

// this function marks the tile in front as obstacle if not out of bounds
void mark_ahead_tile_obstacle(Robot *robot)
{
    Coord coord = get_coord_in_direction(robot, robot->direction);

    // check out of bounds
    if (!check_coord_in_bounds(coord, robot->arenaWidth, robot->arenaHeight)) return;

    robot->memory[coord.y][coord.x] = R_BLOCKED;
}

// this function returns true if the given coord is a known tile (visited or a known obstacle)
int is_tile_known(Robot *robot, Coord tile) 
{
    return robot->memory[tile.y][tile.x] == R_VISITED || robot->memory[tile.y][tile.x] == R_BLOCKED;
}

// this function checks if the robot is surrounded by visited tiles and is trapped in the spiral algorithm
int is_surrounded_by_known(Robot *robot)
{
    Coord n = get_coord_in_direction(robot, NORTH);
    Coord e = get_coord_in_direction(robot, EAST);    
    Coord s = get_coord_in_direction(robot, SOUTH);
    Coord w = get_coord_in_direction(robot, WEST);

    // default to true as if out of bounds the tile is known
    int nKnown = 1;
    int eKnown = 1;
    int sKnown = 1;
    int wKnown = 1;

    // for each corodinate set Visited to true if known (either visited or blocked)
    if (check_coord_in_bounds(n, robot->arenaWidth, robot->arenaHeight)) nKnown = is_tile_known(robot, n);
    if (check_coord_in_bounds(e, robot->arenaWidth, robot->arenaHeight)) eKnown = is_tile_known(robot, e);
    if (check_coord_in_bounds(s, robot->arenaWidth, robot->arenaHeight)) sKnown = is_tile_known(robot, s);
    if (check_coord_in_bounds(w, robot->arenaWidth, robot->arenaHeight)) wKnown = is_tile_known(robot, w);
    
    return nKnown && eKnown && sKnown && wKnown; // if any are unknown, false is returned

}

// this function gets an adjacent tile that is unvisited; pre-requisite: there is an adjacent unvisited tile
Coord adjacent_unvisited_tile(Robot *robot)
{
    Coord n = get_coord_in_direction(robot, NORTH);
    Coord e = get_coord_in_direction(robot, EAST);   
    Coord s = get_coord_in_direction(robot, SOUTH);
    Coord w = get_coord_in_direction(robot, WEST);

    // return the node if the coordinate is in bounds and unknown
    if (check_coord_in_bounds(n, robot->arenaWidth, robot->arenaHeight) && !is_tile_known(robot, n)) return n;
    if (check_coord_in_bounds(e, robot->arenaWidth, robot->arenaHeight) && !is_tile_known(robot, e)) return e;
    if (check_coord_in_bounds(s, robot->arenaWidth, robot->arenaHeight) && !is_tile_known(robot, s)) return s;
    if (check_coord_in_bounds(w, robot->arenaWidth, robot->arenaHeight) && !is_tile_known(robot, w)) return w;

    fprintf(stderr, "Reached end of adjacent_unvisited_tile without finding an unvisited adjacent tile\n");
    exit(EXIT_FAILURE);
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
        fprintf(stderr, "Calloc returned null for row pointers in allocate_robots_memory\n");
        exit(EXIT_FAILURE);
    }

    // allocate each row
    for (int i = 0; i < height; i++) {
        robot->memory[i] = calloc(width, sizeof(RobotTile));
        if (robot->memory[i] == NULL) {
            fprintf(stderr, "Calloc returned null for a row of robots memory in allocate_robots_memory\n");
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
    if (robot == NULL) {
        fprintf(stderr, "Malloc returned null in create_robot\n");
        exit(EXIT_FAILURE);
    }

    robot->x = 0; // will be changed
    robot->y = 0; // will be changed
    robot->direction = NORTH; // will be changed
    robot->markerCount = 0;
    robot->arenaWidth = arena->arenaWidth;
    robot->arenaHeight = arena->arenaHeight;
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

// functions to deal with the path using stack implementation from utils.h

// this function creates the stack and pushes the current position (start to it)
void setup_path_stack(Robot *robot)
{
    robot->path = create_stack(robot->arenaWidth*robot->arenaHeight+8); // capacity assumes travelling over each tile
    push(robot->path, (Coord){robot->x, robot->y});
}

// this function pushes the current robot position to the stack
void push_pos_to_path(Robot *robot)
{
    push(robot->path, (Coord){robot->x, robot->y});
}

// this function returns the Coord at the top of the stack
Coord backtrack_path_tile(Robot *robot)
{
    pop(robot->path);
    Coord result = peek(robot->path);
    if (result.x == -1 && result.y == -1) { // backtracked to the start and could not find tile
        fprintf(stderr, "A marker is unreachable. Program will stop.\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

// functions for placing the robot at the start of the program

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

    setup_path_stack(robot);
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
        Direction direction = parse_direction(argv[5]); // returns -1 if invalid input, dealt with below

        // check out of bounds - if so, give random position and direction
        if (!check_coord_in_bounds(coord, robot->arenaWidth, robot->arenaHeight)) {
            fprintf(stderr, "Error: x and y must be between 0 and %d / %d. Random position and direction generated.\n", robot->arenaWidth - 1, robot->arenaHeight - 1);
            place_robot_random(robot, arena);
            return;
        }

        // check invalid direction - if so, give random direction, but we know x, y is in range
        if (direction == -1) {
            fprintf(stderr, "Error: direction must be north, east, south, west. Random direction generated.\n");
            place_robot_specific(robot, arena, coord, random_direction());
            return;
        }

        // valid x, y, direction
        place_robot_specific(robot, arena, coord, direction);
        return;
    }

    place_robot_random(robot, arena);
}
// This file contains code to allow the robot to move and the algorithm for it to follow

#include "../include/arena.h"
#include "../include/drawing.h"
#include "../include/robot.h"

#include "../lib/graphics.h"

#include <stdlib.h>
#include <malloc.h>

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

    // check out of bounds
    if (x < 0 || y < 0) return 0;
    if (x >= arena->arenaWidth || y >= arena->arenaHeight) return 0;

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

// functions called from main:

// this function allocates memory for robot's memory
static void allocate_robots_memory(Robot *robot)
{
    // get width and height for ease
    int width = robot->arenaWidth;
    int height = robot->arenaHeight;

    // allocate memory for row pointers
    robot->memory = calloc(height, sizeof(RobotTile *));
    if (robot->memory == NULL) {
        perror("Failed to allocate memory for row pointers");
        exit(EXIT_FAILURE);
    }

    // allocate each row
    for (int i = 0; i < height; i++) {
        robot->memory[i] = calloc(width, sizeof(RobotTile));
        if (robot->memory[i] == NULL) {
            perror("Failed to allocate memory for a row");
            // free already allocated memory
            for (int j = 0; j < i; j++) {
                free(robot->memory[j]);
            }
            free(robot->memory);
            exit(EXIT_FAILURE);
        }
    }
}

// this function creates a robot struct; pre-requisite: arena dimensions already set
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
    allocate_robots_memory(&robot);
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
    free_robot_memory(&robot);
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
    } while (arena->arenaGrid[x][y] != TILE_EMPTY);

    // assign this as robot start on arena 
    arena->arenaGrid[x][y] = TILE_ROBOT_START;
    
    // assign values to robot
    robot->x = x;
    robot->y = y;
    robot->direction = random_direction();
}

// this function places the robot with a specific 
static void place_robot_specific(Robot *robot, Arena *arena, int x, int y, Direction direction)
{
    // if the entered position is taken, place the robot randomly
    if (arena->arenaGrid[x][y] != TILE_EMPTY) {
        place_robot_random(&robot, &arena);
        return;
    }

    // assign x, y as start on arena
    arena->arenaGrid[x][y] = TILE_ROBOT_START;

    // assign values to robot
    robot->x = x;
    robot->y = direction;
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
    if (argc == 4) {
        int x = atoi(argv[1]);
        int y = atoi(argv[2]);
        Direction direction = parse_direction(argv[3]);

        // check out of bounds - if so, give random position and direction
        if (x < 0 || y < 0 || x >= robot->arenaWidth || y >= robot->arenaHeight) {
                printf("Error: x and y must be between 0 and %d / %d. Random position and direction generated.\n", robot->arenaWidth - 1, robot->arenaHeight - 1);
                place_robot_random(&robot, &arena);
                return;
        }

        // check invalid direction - if so, give random direction, but we know x, y is in range
        if (direction == -1) {
            printf("Error: direction must be north, east, south, west. Random direction generated.\n");
            place_robot_specific(robot, arena, x, y, random_direction());
            return;
        }

        // valid x, y, direction
        place_robot_specific(&robot, &arena, x, y, direction);
        return;
    }

    place_robot_random(&robot, &arena);
}
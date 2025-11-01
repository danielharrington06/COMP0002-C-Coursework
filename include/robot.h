#ifndef ROBOT_H
#define ROBOT_H

#include "arena.h"
#include "utils.h"

typedef enum {
    ROBOT_UNKNOWN = 0,
    ROBOT_VISITED = 1,
    ROBOT_BLOCKED = 2
} RobotTile;

typedef struct {
    int x;
    int y;
    Direction direction;
    int markerCount;
    int arenaWidth;
    int arenaHeight;  
    RobotTile **memory;
} Robot;

// robot movement/controls that are pieced together by an algorithm
static void forward(Robot*);
static void turn_left(Robot*);
static void turn_right(Robot*);
static int is_at_marker(Robot*, Arena*);
static int can_move_forward(Robot*, Arena*);
static void pickup_marker(Robot*, Arena*);
static void drop_marker(Robot*, Arena*);
static int get_marker_carry_count(Robot*);
static int get_marker_arena_count(Arena*);

// functions dealing with robot struct
static void allocate_robots_memory(Robot*);
Robot* create_robot(Arena*);
static void free_robots_memory(Robot*);
void free_robot(Robot*);
static void place_robot_random(Robot*, Arena*);
static void place_robot_specific(Robot*, Arena*, int, int, Direction);
static Direction parse_direction(const char*);
void place_robot(int, char**, Robot*, Arena*);

#endif
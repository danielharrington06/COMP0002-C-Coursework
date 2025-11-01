#ifndef ROBOT_H
#define ROBOT_H

#include "arena.h"

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
void forward(Robot*);
void turn_left(Robot*);
void turn_right(Robot*);
int is_at_marker(Robot*, Arena*);
int can_move_forward(Robot*, Arena*);
void pickup_marker(Robot*, Arena*);
void drop_marker(Robot*, Arena*);
int get_marker_carry_count(Robot*);
int get_marker_arena_count(Arena*);

// functions to setup robot
void allocate_robots_memory(Robot*);
Robot* create_robot(Arena*);
void free_robots_memory(Robot*);
void free_robot(Robot*);
void place_robot_random(Robot*, Arena*);
void place_robot_specific(Robot*, Arena*, int, int, Direction);
Direction parse_direction(const char*);
void place_robot(int, char**, Robot*, Arena*);

// called from within and from arena.c
Direction random_direction();

#endif
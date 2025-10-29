#ifndef ROBOT_H
#define ROBOT_H

#include "arena.h"

typedef enum {
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3
} Direction;

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
    int mapWidth;
    int mapHeight;  
    RobotTile **memory;
} Robot;

void forward(Robot*);
void turn_left(Robot*);
void turn_right(Robot*);
int is_at_marker(Robot*, Arena*);
int can_move_forward(Robot*, Arena*);
void pickup_marker(Robot*, Arena*);
void drop_marker(Robot*, Arena*);
int get_marker_carry_count(Robot*);
int get_marker_arena_count(Arena*);

Direction random_direction();

#endif
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
    UNKNOWN = 0,
    VISITED = 1,
    OBSTACLE = 2
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
void left(Robot*);
void right(Robot*);
int is_at_marker(Robot*);
int can_move_forward(Robot*);
void pickup_marker(Robot*);
void drop_marker(Robot*);
int get_marker_carry_count(Robot*);
int get_marker_arena_count(Arena*);

#endif
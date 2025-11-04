#ifndef ROBOT_H
#define ROBOT_H

#include "arena.h"
#include "utils.h"

typedef enum {
    R_UNKNOWN = 0,
    R_VISITED = 1,
    R_BLOCKED = 2
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

// functions dealing with robot struct
Robot* create_robot(Arena*);
void free_robot(Robot*);
void place_robot(int, char**, Robot*, Arena*);

// main algorithm to find markers
void find_markers(Robot*, Arena*);

#endif
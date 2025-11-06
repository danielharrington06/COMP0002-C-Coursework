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

// functions to move the robot, sense its environemtn and deal with its memory of the arena
Coord* get_coord_in_direction(Robot*, Direction);
void forward(Robot*);
void turn_left(Robot*);
void turn_right(Robot*);
int is_at_marker(Robot*, Arena*);
int can_move_forward(Robot*, Arena*);
void drop_marker(Robot*, Arena*);
int get_marker_carry_count(Robot*);
int get_marker_arena_count(Arena*);
int check_forward_tile_unknown(Robot*);
int check_left_tile_unknown(Robot*);
void mark_current_tile_visited(Robot*);
void mark_ahead_tile_obstacle(Robot*);
int is_surrounded_by_visited(Robot*);
int num_unknown_tiles(Robot*);

// functions dealing with robot struct
Robot* create_robot(Arena*);
void free_robot(Robot*);
void place_robot(int, char**, Robot*, Arena*);

// main algorithm to find markers
void find_markers(Robot*, Arena*);

#endif
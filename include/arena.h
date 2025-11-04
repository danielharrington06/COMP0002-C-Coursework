#ifndef ARENA_H
#define ARENA_H

#include "utils.h"

typedef enum {
    T_EMPTY = 0,
    T_OBSTACLE = 1,
    T_MARKER = 2,
    T_R_START = 3
} ArenaTile;

typedef struct {
    int arenaWidth;
    int arenaHeight;
    ArenaTile **arenaGrid;
    int numMarker;
} Arena;

// options for the type of obstacle formation
typedef enum {
    O_NONE = 0,
    O_RANDOM = 1,
    O_CLUSTERED = 2,
    O_SINGLE_WALL = 3,
    O_CENTRAL_CAVERN = 4
} ObstacleFormation;

typedef enum {
    M_EDGE = 0,
    M_ANYWHERE = 1,
    M_RANDOM = 2
} MarkerFormation;

// functions to generate obstacles and markers
void generate_obstacles(Arena*, int, ObstacleFormation);
void generate_markers(Arena*, int, MarkerFormation);

// functions dealing with arena struct
Arena* create_arena(int, int);
void free_arena(Arena*);

// function to determine arena size
int determine_arena_width(int, char**);
int determine_arena_height(int, char**);

#endif
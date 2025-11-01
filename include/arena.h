#ifndef ARENA_H
#define ARENA_H

#include "utils.h"

typedef enum {
    TILE_EMPTY = 0,
    TILE_OBSTACLE = 1,
    TILE_MARKER = 2,
    TILE_ROBOT_START = 3
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
int is_tile_in_direction_free(Arena*, int, int, Direction);
static void generate_obstacles_random(Arena*, int);
static void generate_obstacles_clustered(Arena*, int);
static void generate_obstacles_wall(Arena*, int);
static void generate_obstacles_cavern(Arena*);
void generate_obstacles(Arena*, int, ObstacleFormation);
static void generate_marker_edge(Arena*);
static void generate_marker_anywhere(Arena*);
static void generate_markers_random(Arena*, int);
void generate_markers(Arena*, int, MarkerFormation);

// functions dealing with arena struct
static void allocate_arena_grid(Arena*);
Arena* create_arena(int, int);
static void free_arena_grid(Arena*);
void free_arena(Arena*);

#endif
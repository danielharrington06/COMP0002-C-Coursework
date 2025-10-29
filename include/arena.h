#ifndef ARENA_H
#define ARENA_H

typedef enum {
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3
} Direction;

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
void generate_obstacles_random(Arena*, int);
void generate_obstacles_clustered(Arena*, int);
void generate_obstacles_wall(Arena*, int);
void generate_obstacles_cavern(Arena*);
void generate_obstacles(Arena*, int, ObstacleFormation);
void generate_marker_edge(Arena*);
void generate_marker_anywhere(Arena*);
void generate_markers_random(Arena*, int);
void generate_markers(Arena*, int, MarkerFormation);

#endif
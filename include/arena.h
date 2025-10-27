#ifndef ARENA_H
#define ARENA_H

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
    RANDOM = 0,
    CLUSTERED_RANDOM = 1,
    SINGLE_WALL = 2,
    CENTRAL_CAVERN = 3
} ObstacleFormation;

void generate_random_obstacles(Arena*, int, ObstacleFormation);
void generate_markers(Arena*, int);

#endif
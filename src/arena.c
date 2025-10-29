// This file defines how the arena works

#include "../include/arena.h"
#include "../include/drawing.h"

#include "../lib/graphics.h"

#include <stdlib.h>

// this function generates a random coordinate from 0 to width_height which represents either the width or height of the grid
int random_coord(int width_height)
{
    return rand() % width_height;
}

int is_tile_in_direction_free(Arena *arena, int x, int y, Direction direction)
{
    switch(direction) {
        case(NORTH):
            y--;
            break;
        case(EAST):
            x++;
            break;
        case(SOUTH):
            y++;
            break;
        case(WEST):
            x--;
            break;
    }
    
    // check out of bounds
    if (x < 0 || y < 0) return 0;
    if (x >= arena->arenaWidth || y >= arena->arenaHeight) return 0;

    // check if empty
    return arena->arenaGrid[x][y] == TILE_EMPTY;
}

// this function generates obstacles randomly; pre-requisite: arenaGrid is completely empty
void generate_obstacles_random(Arena *arena, int numObstacles) 
{
    // make sure we aren't trying to place more obstacles than half the grid
    assert(numObstacles < arena->arenaWidth*arena->arenaHeight/2);

    // theoretically could become an infinite loop, but in reality unlikely to if only filling half the grid
    for (int i = 0; i < numObstacles; i++) {
        // generate (x, y) until (x, y) is an empty tile
        int x, y;
        do {
            x = random_coord(arena->arenaWidth);
            y = random_coord(arena->arenaHeight);
        } while (arena->arenaGrid[x][y] != TILE_EMPTY);

        arena->arenaGrid[x][y] = TILE_OBSTACLE;
    }
}

// this function generates obstacles clustered together - it tries to place them in groups of up to3; pre-requisite: arenaGrid is completely empty
void generate_obstacles_clustered(Arena *arena, int numClusters)
{
    // make sure we aren't trying to place more obstacles than half the grid
    assert(numClusters < arena->arenaWidth*arena->arenaHeight/(2*3));

    // theoretically could become an infinite loop, but in reality unlikely to if only filling half the grid
    for (int i = 0; i < numClusters; i++) {
        // generate (x, y) until (x, y) is an empty tile
        int x, y;
        do {
            x = random_coord(arena->arenaWidth);
            y = random_coord(arena->arenaHeight);
        } while (arena->arenaGrid[x][y] != TILE_EMPTY);

        arena->arenaGrid[x][y] = TILE_OBSTACLE;

        int placed = 0; // keep track of how many have been placed
        int dir = (int)random_direction(); // get random direction to start at
        for (int i = 0; i < 4; i++) {
            if (is_tile_in_direction_free(arena, x, y, (Direction)dir)) {
                arena->arenaGrid[x][y] = TILE_OBSTACLE;
                placed++;
            }
            if (placed == 2) break; // clusters of up to 3
        }
    }
}

// this function determines which function to use to generate obstacles and then calls them
void generate_obstacles(Arena *arena, int numObstacles, ObstacleFormation formation)
{
    switch(formation) {
        case RANDOM:
            generate_obstacles_random(arena, numObstacles);
            break;
        case CLUSTERED_RANDOM:
            generate_obstacles_clustered(arena, numObstacles);
            break;
        case SINGLE_WALL:
            generate_obstacles_wall(arena, numObstacles);
            break;
        case CENTRAL_CAVERN:
            generate_obstacles_cavern(arena);
            break;
        default:
            printf("Formation was not a valid selection\n");
            assert(0); // crash the code
            break;
    }
}
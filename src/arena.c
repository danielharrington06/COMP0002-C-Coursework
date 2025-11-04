// This file defines how the arena works

#include "../include/arena.h"
#include "../include/config.h"
#include "../include/drawing.h"
#include "../include/robot.h"
#include "../include/utils.h"

#include "../lib/graphics.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

// this function determines if the tile relative to (x, y) in direction direction is empty
static int is_tile_in_direction_free(Arena *arena, Coord coord, Direction direction)
{
    switch(direction) {
        case(NORTH):
            coord.y--;
            break;
        case(EAST):
            coord.x++;
            break;
        case(SOUTH):
            coord.y++;
            break;
        case(WEST):
            coord.x--;
            break;
    }
    
    // check out of bounds
    if (!check_coord_in_bounds(&coord, arena->arenaWidth, arena->arenaHeight)) return 0;

    // check if empty
    return arena->arenaGrid[coord.y][coord.x] == T_EMPTY;
}

// this function generates obstacles randomly; pre-requisite: arenaGrid is completely empty
static void generate_obstacles_random(Arena *arena, int numObstacles) 
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
        } while (arena->arenaGrid[y][x] != T_EMPTY);

        arena->arenaGrid[y][x] = T_OBSTACLE;
    }
}

// this function generates obstacles clustered together - it tries to place them in groups of up to3; pre-requisite: arenaGrid is completely empty
static void generate_obstacles_clustered(Arena *arena, int numClusters)
{
    // make sure we aren't trying to place more obstacles than half the grid
    assert(numClusters < arena->arenaWidth*arena->arenaHeight/(2*3));

    // theoretically could become an infinite loop, but in reality unlikely to if only filling half the grid
    for (int i = 0; i < numClusters; i++) {
        // generate (x, y) until (x, y) is an empty tile
        Coord coord;
        do {
            coord.x = random_coord(arena->arenaWidth);
            coord.y = random_coord(arena->arenaHeight);
        } while (arena->arenaGrid[coord.y][coord.x] != T_EMPTY);

        arena->arenaGrid[coord.y][coord.x] = T_OBSTACLE;

        // spawn the others in the cluster
        int placed = 0; // keep track of how many have been placed
        int dir = (int)random_direction(); // get random direction to start at
        for (int i = 0; i < 4; i++) {
            if (is_tile_in_direction_free(arena, coord, (Direction)dir)) {
                arena->arenaGrid[coord.y][coord.x] = T_OBSTACLE;
                placed++;
            }
            if (placed == 2) break; // clusters of up to 3
        }
    }
}

// this function generates a vertical wall from the bottom of the screen to near the top with a length specified; pre-requisite: arenaGrid is completely empty
static void generate_obstacles_wall(Arena* arena, int numObstacles)
{
    assert(numObstacles < arena->arenaHeight);

    int x = arena->arenaWidth/3;

    for (int i = 0; i < numObstacles; i++) {
        arena->arenaGrid[x][arena->arenaHeight - 1 - i] = T_OBSTACLE;
    }
}

// this function creates a cavern by finding a central arenaGrid coordinate and determines whether each coordinate is within a set radius; pre-requisite: arenaGrid is completely empty
static void generate_obstacles_cavern(Arena *arena)
{
    // define radius and centre of the cavern
    int radius = min(arena->arenaWidth, arena->arenaHeight)/2 - 1;
    int centreX = (arena->arenaWidth-1)/2;
    int centreY = (arena->arenaHeight-1)/2;

    for (int x = 0; x < arena->arenaWidth; x++) {
        for (int y = 0; y < arena->arenaHeight; y++) {
            int distToCentre = pow(centreX-x, 2) + pow(centreY-y, 2);
            if (distToCentre >= radius) {
                arena->arenaGrid[y][x] = T_OBSTACLE;
            }
        }
    }
}

// this function determines which function to use to generate obstacles and then calls it; pass numObstacles = 0 if not needed
void generate_obstacles(Arena *arena, int numObstacles, ObstacleFormation formation)
{
    switch(formation) {
        case O_NONE:
            break;
        case O_RANDOM:
            generate_obstacles_random(arena, numObstacles);
            break;
        case O_CLUSTERED:
            generate_obstacles_clustered(arena, numObstacles);
            break;
        case O_SINGLE_WALL:
            generate_obstacles_wall(arena, numObstacles);
            break;
        case O_CENTRAL_CAVERN:
            generate_obstacles_cavern(arena);
            break;
        default:
            fprintf(stderr, "Formation was not a valid selection\n");
            exit(EXIT_FAILURE); // crash the code
            break;
    }
}

// this function generates a single marker somewhere along the edge of the grid; pre-requisite: no obstacles placed
static void generate_marker_edge(Arena *arena)
{
    int x, y;
    do { // assign x and y coords until the tile is empty
        // randomly choose a number from 0 to 4 to represent top, right, bottom or left
        // each option includes the first tile (e.g. top includes top left, right includes top right)
        int r = rand() % 4;

        // deal with top/bottom first
        if (r % 2 == 0) {
            int pos = random_coord(arena->arenaWidth-1);
            if (r == 0) { x = pos; y = 0; } // top
            if (r == 2) { x = pos+1; y = arena->arenaHeight-1; } // bottom
        }
        else { // then left/right
            int pos = random_coord(arena->arenaHeight-1);
            if (r == 1) { x = arena->arenaWidth-1; y = pos; } // right
            if (r == 3) { x = 0; y = pos+1; } // left
        }
    } while (arena->arenaGrid[y][x] != T_EMPTY);

    arena->arenaGrid[y][x] = T_MARKER;
}

static void generate_marker_anywhere(Arena *arena)
{
    int x = rand() % arena->arenaWidth;
    int y = rand() % arena->arenaHeight;

    arena->arenaGrid[y][x] = T_MARKER;
}

// this function generates markers randomly; pre-requesite: obstacles have already been spawned
static void generate_markers_random(Arena *arena, int numMarkers)
{
    // theoretically could become an infinite loop, but in reality unlikely to
    for (int i = 0; i < numMarkers; i++) {
        // generate (x, y) until (x, y) is an empty tile
        int x, y;
        do {
            x = random_coord(arena->arenaWidth);
            y = random_coord(arena->arenaHeight);
        } while (arena->arenaGrid[y][x] != T_EMPTY);

        arena->arenaGrid[y][x] = T_MARKER;
    }
}

// this function determines which function to use to generate markers and then calls it; pass numMarkers = 0 if not needed
void generate_markers(Arena *arena, int numMarkers, MarkerFormation formation)
{
    switch(formation) {
        case M_EDGE:
            generate_marker_edge(arena);
            arena->numMarker = 1;
            break;
        case M_ANYWHERE:
            generate_marker_anywhere(arena);
            arena->numMarker = 1;
            break;
        case M_RANDOM:
            generate_markers_random(arena, numMarkers);
            arena->numMarker = numMarkers;
            break;
        default:
            printf("Formation was not a valid selection\n");
            assert(0);
            break;
    }
}

// functions to deal with arena struct:

// functions called from main:

// this function allocates memory for robot's memory
static void allocate_arena_grid(Arena *arena)
{
    // get width and height for ease
    int width = arena->arenaWidth;
    int height = arena->arenaHeight;

    // allocate memory for row pointers
    arena->arenaGrid = calloc(height, sizeof(ArenaTile*));
    if (arena->arenaGrid == NULL) {
        printf("Failed to allocate memory for arenaGrid row pointers\n");
        exit(EXIT_FAILURE);
    }

    // allocate each row
    for (int i = 0; i < height; i++) {
        arena->arenaGrid[i] = calloc(width, sizeof(ArenaTile));
        if (arena->arenaGrid[i] == NULL) {
            fprintf(stderr, "Failed to allocate memory for a row in arenaGrid\n");
            // free already allocated memory
            for (int j = 0; j < i; j++) {
                free(arena->arenaGrid[j]);
            }
            free(arena->arenaGrid);
            exit(EXIT_FAILURE);
        }
    }
}

// this function creates an arena struct; pre-requisite: arena dimensions already set; caller has responsibility to free
Arena* create_arena(int width, int height)
{
    // allocate memory
    Arena* arena = malloc(sizeof(Arena));
    if (arena == NULL) return NULL;

    // this will be changed
    arena->numMarker = 0;

    // this is correct starting value
    arena->arenaWidth = width;
    arena->arenaHeight = height;

    // allocate robot's memory
    allocate_arena_grid(arena);

    return arena;
}

// this function frees arenaGrid memory
static void free_arena_grid(Arena *arena)
{
    for (int i = 0; i < arena->arenaHeight; i++) {
        free(arena->arenaGrid[i]);
    }
    free(arena->arenaGrid);
}

// this function frees overall arena struct memory
void free_arena(Arena *arena)
{
    free_arena_grid(arena);
    free(arena);
}

// functions to determine arena size:

// this function determines the arena width with DEFAULT_SIZE as default
int determine_arena_width(int argc, char *argv[])
{
    int width;
    if (argc == 3 || argc == 6) { // dimensions have been specified correctly
        width = atoi(argv[1]);
        return min(width, calculate_max_arena_width());
    }

    return DEFAULT_ARENA_WIDTH;
}

// this function determines the arena height with DEFAULT_SIZE as default
int determine_arena_height(int argc, char *argv[])
{
    int height;
    if (argc == 3 || argc == 6) { // dimensions have been specified correctly
        height = atoi(argv[2]);
        return min(height, calculate_max_arena_height());
    }

    return DEFAULT_ARENA_HEIGHT;
}
// This file defines how the arena works

#include "../include/arena.h"
#include "../include/config.h"
#include "../include/drawing.h"
#include "../include/robot.h"
#include "../include/utils.h"

#include "../lib/graphics.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// this function ensures that the values for obstacle and markers in config.c are correct
void check_obstacle_marker_values(Arena *arena, ObstacleFormation of, int numObstacles, MarkerFormation mf, int numMarkers)
{
    if (of == O_CAVERN && mf == M_EDGE) {
        fprintf(stderr, "Obstacle formation is cavern and marker formation is edge which is incompatible\n");
        exit(EXIT_FAILURE);
    }
    if (numObstacles > (arena->arenaWidth * arena->arenaHeight)/3) {
        fprintf(stderr, "Number of obstacles: %d exceeds 1/3 number of tiles: %d\n", numObstacles, (arena->arenaWidth * arena->arenaHeight)/3);
        exit(EXIT_FAILURE);
    }
    if (numMarkers > (2*arena->arenaWidth * arena->arenaHeight)/3) {
        fprintf(stderr, "Number of markers: %d exceeds 2/3 number of tiles: %d\n", numMarkers, (2 * arena->arenaWidth * arena->arenaHeight)/3);
        exit(EXIT_FAILURE);
    }
}

// this function generates obstacles randomly; pre-requisites: arenaGrid is completely empty AND numObstacles is less than a third of the grid
static void generate_obstacles_random(Arena *arena, int numObstacles) 
{
    // make sure we aren't trying to place more obstacles than half the grid
    if (numObstacles >= arena->arenaWidth*arena->arenaHeight/3) {
        fprintf(stderr, "Number of obstacles cannot exceed 1/3 the grid.\n");
        exit(EXIT_FAILURE);
    }
    // theoretically could become an infinite loop, but in reality unlikely to if only filling a third of the grid
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

// this function generates a vertical wall from the bottom of the screen to near the top with a length specified; pre-requisite: arenaGrid is completely empty
static void generate_obstacles_wall(Arena* arena, int numObstacles)
{
    if (numObstacles >= arena->arenaHeight) {
        fprintf(stderr, "For a single wall, the number of obstacles must be less than the arena height\n");
        exit(EXIT_FAILURE);
    }

    int x = arena->arenaWidth/3;

    for (int i = 0; i < numObstacles; i++) {
        arena->arenaGrid[arena->arenaHeight - 1 - i][x] = T_OBSTACLE;
    }
}

// this function creates a cavern by finding a central arenaGrid coordinate and determines whether each coordinate is within a set radius; pre-requisite: arenaGrid is completely empty
static void generate_obstacles_cavern(Arena *arena)
{
    double radius = min(arena->arenaWidth, arena->arenaHeight) / 2.0 - 1;
    double centreX = arena->arenaWidth / 2.0;
    double centreY = arena->arenaHeight / 2.0;

    for (int y = 0; y < arena->arenaHeight; y++) {
        for (int x = 0; x < arena->arenaWidth; x++) {
            // offset x and y by +0.5 to measure from cell center - necessary or it doesnt work
            double sqrDistToCentre = calc_squared_dist_coords(centreX, centreY, x + 0.5, y + 0.5);

            if (sqrDistToCentre >= radius * radius) {
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
        case O_WALL:
            generate_obstacles_wall(arena, numObstacles);
            break;
        case O_CAVERN:
            generate_obstacles_cavern(arena);
            break;
    }
}

// this function generates a single marker somewhere along the edge of the grid; pre-requisite: no obstacles placed
static void generate_marker_edge(Arena *arena, int numMarkers)
{
    numMarkers = min(numMarkers, (arena->arenaHeight + arena->arenaWidth - 2)); // cap half the possible spaces
    for (int i = 0; i < numMarkers; i++) {
        int x, y;

        do { // randomly choose a number from 0 to 4 to represent top, right, bottom or left (heuristic as for not very similar width/height, vertical or horizontal tiles have significantly higher chance of being selected)
            // each option includes the first tile for the section (e.g. top includes top left, right includes top right)
            int r = rand() % 4;
            if (r % 2 == 0) { // deal with top/bottom first
                int pos = random_coord(arena->arenaWidth-1); // pos is an x position along top or bottom edge
                if (r == 0) { x = pos; y = 0; } // top
                if (r == 2) { x = pos+1; y = arena->arenaHeight-1; } // bottom
            }
            else { // then left/right
                int pos = random_coord(arena->arenaHeight-1); // pos is a y position along left or right edge
                if (r == 1) { x = arena->arenaWidth-1; y = pos; } // right
                if (r == 3) { x = 0; y = pos+1; } // left
            }
        } while (arena->arenaGrid[y][x] != T_EMPTY); // account for obstacles

        arena->arenaGrid[y][x] = T_MARKER;
    }
}

// this function generates markers randomly; pre-requesite: obstacles have already been spawned
static void generate_markers_random(Arena *arena, int numMarkers)
{
    numMarkers = min(numMarkers, 2*arena->arenaWidth*arena->arenaHeight/3);
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
            generate_marker_edge(arena, numMarkers);
            arena->numMarker = numMarkers;
            break;
        case M_RANDOM:
            generate_markers_random(arena, numMarkers);
            arena->numMarker = numMarkers;
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
        printf("Malloc returned null for row pointers in allocate_arena_grid\n");
        exit(EXIT_FAILURE);
    }

    // allocate each row
    for (int i = 0; i < height; i++) {
        arena->arenaGrid[i] = calloc(width, sizeof(ArenaTile));
        if (arena->arenaGrid[i] == NULL) {
            fprintf(stderr, "Calloc returned null for a row in arenaGrid in allocate_arena_grid\n");
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
    if (arena == NULL) {
        fprintf(stderr, "Malloc returned null in create_arena\n");
        exit(EXIT_FAILURE);
    }
    arena->numMarker = 0; // this will get changed to real value in later function
    arena->arenaWidth = width;
    arena->arenaHeight = height;
    allocate_arena_grid(arena); // allocate memory for arenaGrid

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
        return max(4, min(width, calculate_max_arena_width()));
    }

    return DEFAULT_ARENA_WIDTH;
}

// this function determines the arena height with DEFAULT_SIZE as default
int determine_arena_height(int argc, char *argv[])
{
    int height;
    if (argc == 3 || argc == 6) { // dimensions have been specified correctly
        height = atoi(argv[2]);
        return max(4, min(height, calculate_max_arena_height()));
    }

    return DEFAULT_ARENA_HEIGHT;
}
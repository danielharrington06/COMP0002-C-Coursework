// This file defines how the arena works

#include "../include/arena.h"
#include "../include/drawing.h"

#include "../lib/graphics.h"

// this function determines which function to use to generate obstacles and then calls them
void generate_obstacles(Arena* arena, int numObstacles, ObstacleFormation formation)
{
    switch(formation) {
        case RANDOM:
            break;
        case CLUSTERED_RANDOM:
            break;
        case SINGLE_WALL:
            break;
        case CENTRAL_CAVERN:
            break;
            
    }
}
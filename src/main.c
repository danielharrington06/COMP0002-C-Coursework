// This is the main file from which other functions are called

#include "../include/arena.h"
#include "../include/drawing.h"
#include "../include/spiral.h"
#include "../include/utils.h"

#include "../lib/graphics.h"

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
// setup
    unsigned int seed = time(NULL);
    fprintf(stderr, "%d\n", seed);
    srand(1762602886); // seed random with time

    // determine arena dimensions
    // determine arena width and height from the min of assigned size and max possible size
    const int ARENA_WIDTH = determine_arena_width(argc, argv); 
    const int ARENA_HEIGHT = determine_arena_height(argc, argv);

    // create arena and robot, dealing with memory allocation failures - error messages dealt with in functions
    Arena *arena = create_arena(ARENA_WIDTH, ARENA_HEIGHT);
    if (arena == NULL) return 1;
    Robot *robot = create_robot(arena);
    if (robot == NULL) return 1;

// start

    generate_obstacles(arena, 12, O_RANDOM);

    // use command line arguments to place robot correctly
    place_robot(argc, argv, robot, arena);

    generate_markers(arena, 20, M_RANDOM);
    
    // render background
    draw_background(arena);
    sleep(1000);

// loop
    foreground();
    find_markers(robot, arena); // using spiral method

// end
    free_robot(robot);
    free_arena(arena);
    
    return 0;
}
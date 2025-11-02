// This is the main file from which other functions are called

#include "../include/arena.h"
#include "../include/drawing.h"
#include "../include/robot.h"
#include "../include/utils.h"

#include "../lib/graphics.h"

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int width = 12;
int height = 12;

int main(int argc, char *argv[])
{
// setup
    srand(time(NULL)); // seed random with time

    // determine arena width and height from the min of assigned size and max possible size
    const int ARENA_WIDTH = min(width, calculate_max_arena_width()); 
    const int ARENA_HEIGHT = min(height, calculate_max_arena_height());

    // create arena and robot, dealing with memory allocation failures - error messages dealt with in functions
    Arena *arena = create_arena(ARENA_WIDTH, ARENA_HEIGHT);
    if (arena == NULL) return 1;
    Robot *robot = create_robot(arena);
    if (robot == NULL) return 1;

// start

    // use command line arguments to place robot correctly
    place_robot(argc, argv, robot, arena);

    // generate obstacles and markers
    generate_obstacles(arena, 0, O_NONE);
    generate_markers(arena, 1, M_EDGE);
    
    // render
    draw_background(arena);
    sleep(500);

// loop
    foreground();
    while (1) {
        draw_foreground(arena, robot);
    }

// end
    free_robot(robot);
    free_arena(arena);
    
    return 0;
}
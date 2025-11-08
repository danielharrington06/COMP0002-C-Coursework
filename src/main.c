// This is the main file from which other functions are called

#include "../include/arena.h"
#include "../include/config.h"
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
    // seed random with time
    unsigned int seed = time(NULL);
    //fprintf(stderr, "%d\n", seed); // used for testing so a configuration that gives a bug can be replayed
    srand(seed); // seed random with time otherwise arena is the same every time

    const int ARENA_WIDTH = determine_arena_width(argc, argv); 
    const int ARENA_HEIGHT = determine_arena_height(argc, argv);

    // create arena and robot, malloc failures and error messages dealt with in these functions (program brought to early end)
    Arena *arena = create_arena(ARENA_WIDTH, ARENA_HEIGHT);
    Robot *robot = create_robot(arena);

// start
    check_obstacle_marker_values(arena, obstacleFormation, numObstacles, markerFormation, numMarkers);
    
    generate_obstacles(arena, numObstacles, obstacleFormation); // have to generate obstacles first
    place_robot(argc, argv, robot, arena); // use command line arguments to place robot correctly
    generate_markers(arena, numMarkers, markerFormation); 
    
    // render background
    draw_background(arena);
    sleep(500);

// loop
    foreground();
    find_markers(robot, arena); // using spiral method

// end
    free_robot(robot);
    free_arena(arena);
    
    return 0;
}
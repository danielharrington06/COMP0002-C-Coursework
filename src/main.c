// This is the main file from which other functions are called

#include "../include/arena.h"
#include "../include/drawing.h"
#include "../include/robot.h"

#include "../lib/graphics.h"

#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    // setup
    srand(time(NULL));
    
    Arena *arena = create_arena(width, height);
    if (arena == NULL) return 1;

    Robot *robot = create_robot(&arena);
    if (robot == NULL) return 1;

    // start
    place_robot(argc, argv, &robot, &arena);

    // loop

    // end
    free_robot(&robot);
    
    return 0;
}
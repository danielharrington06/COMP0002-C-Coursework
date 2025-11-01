// This is the main file from which other functions are called

#include "../include/arena.h"
#include "../include/drawing.h"
#include "../include/robot.h"
#include "../include/utils.h"

#include <stdlib.h>
#include <time.h>

int width = 10;
int height = 10;

int main(int argc, char *argv[])
{
    // setup
    srand(time(NULL));
    
    const int ARENA_WIDTH = max(width, calculate_max_arena_width());
    const int ARENA_HEIGHT = max(height, calculate_max_arena_height());
    
    Arena *arena = create_arena(ARENA_WIDTH, ARENA_HEIGHT);
    if (arena == NULL) return 1;

    Robot *robot = create_robot(&arena);
    if (robot == NULL) return 1;

    // start
    place_robot(argc, argv, robot, arena);

    // loop

    // end
    free_robot(robot);
    free_arena(arena);
    
    return 0;
}
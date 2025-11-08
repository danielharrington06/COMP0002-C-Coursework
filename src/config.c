#include "../include/config.h"
#include "../include/arena.h"

const int DEFAULT_ARENA_WIDTH = 16;
const int DEFAULT_ARENA_HEIGHT = 16;

const int MIN_ARENA_WIDTH = 4;
const int MIN_ARENA_HEIGHT = 4;

const int TILE_SIZE = 40;
const int GRIDLINE_WIDTH = 2;
const int BORDER_THICKNESS = 8;
const int OBJECT_PADDING = 4;
const int MAX_WINDOW_WIDTH = 1600;
const int MAX_WINDOW_HEIGHT = 800;
const int TIME_INTERVAL = 50;

const ObstacleFormation obstacleFormation = O_CAVERN; // O_NONE, O_RANDOM, O_WALL, O_CAVERN
const unsigned int numObstacles = 12; // less than 1/3 number of tiles in grid, does not matter when O_CAVERN

const MarkerFormation markerFormation = M_RANDOM; // M_EDGE, M_RANDOM
const unsigned int numMarkers = 4; // less than 2/3 number of tiles in grid
#include "../include/config.h"
#include "../include/arena.h"

const int DEFAULT_ARENA_WIDTH = 16;
const int DEFAULT_ARENA_HEIGHT = 16;

const int MIN_ARENA_WIDTH = 4;
const int MIN_ARENA_HEIGHT = 4;

const int TILE_SIZE = 40;
const int GRIDLINE_WIDTH = 2; // 2 so that objects don't appear uncentered in tiles
const int BORDER_THICKNESS = 8;
const int OBJECT_PADDING = 4;
const int MAX_WINDOW_WIDTH = 1600; // good value for 1080p display
const int MAX_WINDOW_HEIGHT = 800; // good value for 1080p display

// meant to be changed between program compilations
const int TIME_INTERVAL = 60;

const ObstacleFormation obstacleFormation = O_RANDOM; // O_NONE, O_RANDOM, O_WALL, O_CAVERN
const unsigned int numObstacles = 12; // has no impact when O_CAVERN, also must be less than 1/3 number of tiles in grid, does not matter when O_CAVERN

const MarkerFormation markerFormation = M_RANDOM; // M_EDGE, M_RANDOM - M_RANDOM used in real usage, M_EDGE just for stage 1
const unsigned int numMarkers = 8; // must be less than 2/3 number of tiles in grid
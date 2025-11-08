#ifndef CONFIG_H
#define CONFIG_H

#include "arena.h"

// Arena defaults
extern const int DEFAULT_ARENA_WIDTH;
extern const int DEFAULT_ARENA_HEIGHT;

// Drawing constants
extern const int TILE_SIZE;
extern const int GRIDLINE_WIDTH;
extern const int BORDER_THICKNESS;
extern const int OBJECT_PADDING;
extern const int MAX_WINDOW_WIDTH;
extern const int MAX_WINDOW_HEIGHT;
extern const int TIME_INTERVAL;

// obstacle configuration
extern const ObstacleFormation obstacleFormation;
extern const unsigned int numObstacles; // should not exceed 1/3 number of tiles

// marker configuration
extern const MarkerFormation markerFormation; // do not do M_EDGE if obstacles have been generated with O_CAVERN
extern const unsigned int numMarkers;

#endif

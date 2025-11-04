#ifndef UTILS_H
#define UTILS_H

typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    int x;
    int y;
} Coord;

typedef enum {
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3
} Direction;

Direction random_direction();
int random_coord(int);
int min(int, int);
int max(int, int);

#endif
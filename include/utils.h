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
int check_coord_in_bounds(Coord, int, int);
double calc_squared_dist_coords(int, int, int, int);

// stack

typedef struct {
    int top;
    unsigned int capacity;
    Coord *array;
} Stack;

Stack* create_stack(unsigned int capacity);
void push(Stack*, Coord);
Coord pop(Stack*);
Coord peek(Stack*);
void free_stack(Stack*);

#endif
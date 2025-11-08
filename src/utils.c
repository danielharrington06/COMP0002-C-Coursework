// This function contains utility functions used by numerous programs
#include "../include/utils.h"
#include "../include/robot.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

//  this function generates a random Direction out of NORTH, SOUTH, EAST, WEST
Direction random_direction()
{
    return (Direction)(rand() % 4);
}

// this function generates a random coordinate from 0 to width_height - 1 which can be used to represent either the width or height of the grid
int random_coord(int width_height)
{
    return rand() % width_height;
}

// this function finds the min of two integers
int min(int a, int b) 
{
    if (a < b) return a;
    return b;
}

// this function finds the max of two integers
int max(int a, int b)
{
    if (a > b) return a;
    return b;
}

// this function checks that a coordinate is in bounds of the grid, returning 1 if in bounds and 0 if out of bounds
int check_coord_in_bounds(Coord coord, int width, int height)
{
    // return 0 if out of bounds
    if (coord.x < 0 || coord.y < 0) return 0;
    if (coord.x >= width || coord.y >= height) return 0;

    return 1;
}

// this function calculates the squared distance between two coordinates
double calc_squared_dist_coords(double x1, double y1, double x2, double y2)
{
    return pow(x1 - x2, 2) + pow(y1 - y2, 2);
}

// functions for stack (used in spiral.c)

// function to create a stack of given capacity
Stack* create_stack(unsigned int capacity) 
{
    Stack *stack = malloc(sizeof(Stack));
    if (stack == NULL) {
        fprintf(stderr, "Malloc returned null for stack in create_stack\n");
        exit(EXIT_FAILURE);
    }

    stack->capacity = capacity;
    stack->top = -1;
    stack->array = malloc(capacity * sizeof(Coord));
    if (stack == NULL) {
        fprintf(stderr, "Malloc returned null for stack array in create_stack\n");
        exit(EXIT_FAILURE);
    }
    return stack;
}

// this function checks if the stack is full
static int is_full(Stack *stack) 
{
    return stack->top == stack->capacity - 1;
}

// this function checks if the stack is empty
static int is_empty(Stack *stack) 
{
    return stack->top == -1;
}

// this function pushes a Coord onto the stack
void push(Stack *stack, Coord coord) 
{
    if (is_full(stack)) {
        fprintf(stderr, "Stack overflow - cannot push Coord(%d, %d)\n", coord.x, coord.y);
        exit(EXIT_FAILURE);
    }
    stack->array[++stack->top] = coord;
}

// this function pops a Coord from the stack
Coord pop(Stack *stack) 
{
    if (is_empty(stack)) {
        fprintf(stderr, "Stack underflow - cannot pop\n");
        Coord empty = {-1, -1}; // sentinel value
        return empty;
    }
    return stack->array[stack->top--];
}

// this function peeks at the top Coord without removing it
Coord peek(Stack *stack) 
{
    if (is_empty(stack)) {
        fprintf(stderr, "Stack empty - cannot peek\n");
        Coord empty = {-1, -1};
        return empty;
    }
    return stack->array[stack->top];
}

// this function frees the stack memory
void free_stack(Stack *stack) 
{
    free(stack->array);
    free(stack);
}
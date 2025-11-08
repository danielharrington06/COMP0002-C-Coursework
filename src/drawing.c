// This file contains functions to draw all of the onscreen features

#include "../include/arena.h"
#include "../include/config.h"
#include "../include/drawing.h"

#include "../lib/graphics.h"

#include <stdlib.h>
#include <malloc.h>
#include <math.h>

#define PI 3.141592653

// these are determined by arena dimensions and TILE_SIZE and BORDER_THICKNESS
int WINDOW_WIDTH = 0;
int WINDOW_HEIGHT = 0;

// this function calculates window dimensions (width and height); pre-requesite: arenaWidth and arenaHeight are less than their maximum values
static void calculate_window_dimensions(Arena *arena) 
{
    WINDOW_WIDTH = 2*BORDER_THICKNESS + TILE_SIZE*arena->arenaWidth;
    WINDOW_HEIGHT = 2*BORDER_THICKNESS + TILE_SIZE*arena->arenaHeight;
}

// this function calculates the maximum arenaWidth for the screen, taking into account tile size and display width
int calculate_max_arena_width()
{
    return (MAX_WINDOW_WIDTH - 2*BORDER_THICKNESS) / TILE_SIZE; // integer division on purpose
}

// this function calculates the maximum arenaHeight for the screen, taking into account tile size and max display height
int calculate_max_arena_height() 
{
    return (MAX_WINDOW_HEIGHT - 2*BORDER_THICKNESS) / TILE_SIZE; // integer division on purpose
}

// this function draws the red border around the screen
static void draw_border(Arena *arena) 
{
    setColour(red);
    fillRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    setColour(white);
    fillRect(BORDER_THICKNESS, BORDER_THICKNESS, WINDOW_WIDTH-2*BORDER_THICKNESS, WINDOW_HEIGHT-2*BORDER_THICKNESS);
}

// this function draws the black grid across the screen
static void draw_grid(Arena *arena)
{
    setColour(black);
    // vertical lines first
    for (int i = 0; i < arena->arenaWidth + 1; i++) {
        fillRect(BORDER_THICKNESS+i*TILE_SIZE-1, BORDER_THICKNESS, 2, WINDOW_HEIGHT-2*BORDER_THICKNESS);
    }

    // horizontal lines second
    for (int i = 0; i < arena->arenaHeight + 1; i++) {
        fillRect(BORDER_THICKNESS, BORDER_THICKNESS+i*TILE_SIZE-1, WINDOW_WIDTH-2*BORDER_THICKNESS, 2);
    }
}

// this function draws a single obstacle at arena position (x, y)
static void draw_obstacle(int x, int y) 
{
    // convert arena position (x, y) to coordinates for top left of shape
    int coordX = BORDER_THICKNESS + x*TILE_SIZE + OBJECT_PADDING;
    int coordY = BORDER_THICKNESS + y*TILE_SIZE + OBJECT_PADDING;
    int obstacle_size = TILE_SIZE-2*OBJECT_PADDING;

    // draw
    setColour(black);
    fillRect(coordX, coordY, obstacle_size, obstacle_size);
}

// this function iterates over arenaGrid and calls the function to render obstacles
static void draw_obstacles(Arena *arena)
{
    for (int y = 0; y < arena->arenaHeight; y++) {
        for (int x = 0; x < arena->arenaWidth; x++) {
            if (arena->arenaGrid[y][x] == T_OBSTACLE) {
                draw_obstacle(x, y);
            }
        }
    }
}

// this function returns a pointer to an array of Points representing the vertices of an equilateral triangle with its base at the bottom
static Point* equ_triangle_coords(double triangle_circumrad)
{
    // a triangle's circumradius is the distance from the center to any vertex
    Point *vertices = malloc(3 * sizeof(Point));
    if (vertices == NULL) {
        fprintf(stderr, "Malloc returned null in equ_triangle_coords\n");
        exit(EXIT_FAILURE);
    }

    vertices[0].x = triangle_circumrad*cos(PI/2);
    vertices[0].y = triangle_circumrad*sin(PI/2) - OBJECT_PADDING/2; // - OBJECT_PADDING/2 just to visually center it a bit better
    vertices[1].x = triangle_circumrad*cos(-PI/6);
    vertices[1].y = triangle_circumrad*sin(-PI/6) - OBJECT_PADDING/2;
    vertices[2].x = triangle_circumrad*cos(7*PI/6);
    vertices[2].y = triangle_circumrad*sin(7*PI/6) - OBJECT_PADDING/2;

    return vertices;
}

// this function returns a pointer to an array of Points representing the vertices of the rectangle with its base at the bottom
static Point* rect_coords(double triangle_circumrad)
{
    // a triangle's circumradius is the distance from the center to any vertex
    // not a feature of the rectangle, but used for scaling
    Point *vertices = malloc(4 * sizeof(Point));
    if (vertices == NULL) {
        fprintf(stderr, "Malloc returned null in equ_triangle_coords\n");
        exit(EXIT_FAILURE);
    }

    vertices[0].x = -triangle_circumrad;
    vertices[0].y = triangle_circumrad*sin(-PI/6) - OBJECT_PADDING/2; // bottom of triangle
    vertices[1].x = triangle_circumrad;
    vertices[1].y = triangle_circumrad*sin(7*PI/6) - OBJECT_PADDING/2; // bottom of triangle
    vertices[2].x = triangle_circumrad;
    vertices[2].y = triangle_circumrad*sin(-PI/6) - OBJECT_PADDING/2 - triangle_circumrad*0.3;
    vertices[3].x = -triangle_circumrad;
    vertices[3].y = triangle_circumrad*sin(7*PI/6) - OBJECT_PADDING/2 - triangle_circumrad*0.3;

    return vertices;
}

// this function rotates a single point around the origin
static void rotate_point(Point *point, double degrees) 
{
    double radians = ((double)degrees) * PI / 180.0;

    /* uses a matrix rotation 
    (x)(cos(a) -sin(a)) _ (xcos(a) + ysin(a))
    (y)(sin(a) cos(a))) ‾ (-xsin(a) + ycos(a))
    */
    
    double newX = point->x*cos(radians) - point->y*sin(radians);
    double newY = point->x*sin(radians) + point->y*cos(radians);

    point->x = newX;
    point->y = newY;
}

// this function rotates points around origin and returns them in place
static void rotate_points(Point* vertices, int numV, int degrees) 
{
    for (int i = 0; i < numV; i++) {
        rotate_point(&vertices[i], degrees);
    }
}

// this function draws a triangle on a specific arenaGrid tile
static void draw_triangle(Point* vertices, int x, int y)
{
    setColour(blue);
    int numVertices = 3;

    // convert from arenaGrid x,y to coordinate x, y
    int offsetX = BORDER_THICKNESS + x*TILE_SIZE + 0.5*TILE_SIZE;
    int offsetY = BORDER_THICKNESS + y*TILE_SIZE + 0.5*TILE_SIZE;

    int xCoords[3];
    int yCoords[3];

    // iterate over xCoords then yCoords to offset them to the right position
    // - vertices[i] needed to get it to render correctly (conversion from cartesian to drawapp coords)
    for (int i = 0; i < numVertices; i++) xCoords[i] = round(-vertices[i].x + offsetX);
    for (int i = 0; i < numVertices; i++) yCoords[i] = round(-vertices[i].y + offsetY);

    fillPolygon(numVertices, xCoords, yCoords);
}

// this function draws a rectangle on a specific arenaGrid tile at the base of the robot
static void draw_rectangle(Point* vertices, int x, int y)
{
    setColour(red);
    int numVertices = 4;

    // convert from arenaGrid x,y to coordinate x, y
    int offsetX = BORDER_THICKNESS + x*TILE_SIZE + 0.5*TILE_SIZE;
    int offsetY = BORDER_THICKNESS + y*TILE_SIZE + 0.5*TILE_SIZE;

    int xCoords[4];
    int yCoords[4];

    // iterate over xCoords then yCoords to offset them to the right position
    // - vertices[i] needed to get it to render correctly (conversion from cartesian to drawapp coords)
    for (int i = 0; i < numVertices; i++) xCoords[i] = round(-vertices[i].x + offsetX);
    for (int i = 0; i < numVertices; i++) yCoords[i] = round(-vertices[i].y + offsetY);

    fillPolygon(numVertices, xCoords, yCoords);
}

// this function draws the robot at its current arenaGrid position and direction
static void draw_robot(Robot *robot) 
{
    /* 
    general idea here is to first define the coordinates needed if the robot 
    was drawn on mathetmatical coordinate axes with its center at (0, 0)
     
    this can then be flipped vertically and translated to the correct position
    */

    // triangle radius is the distance from center to vertice
    double triangle_circumrad = TILE_SIZE/2 - OBJECT_PADDING;

    // generate cartesian vertices
    Point* triVertices = equ_triangle_coords(triangle_circumrad);
    if (triVertices == NULL) {
        fprintf(stderr, "Malloc returned null for triangle vertices in equ_triangle_coords\n");
        exit(EXIT_FAILURE);
    }
    Point* rectVertices  = rect_coords(triangle_circumrad);
    if (rectVertices == NULL) {
        fprintf(stderr, "Malloc returned null for rectangle vertices in equ_triangle_coords\n");
        exit(EXIT_FAILURE);
    }

    rotate_points(triVertices, 3, robot->direction*90); // rotate cartesian coords of triangle to match robots direction
    draw_triangle(triVertices, robot->x, robot->y); // now translate onto drawapp grid

    rotate_points(rectVertices, 4, robot->direction*90);
    draw_rectangle(rectVertices, robot->x, robot->y);

    free(triVertices);
    free(rectVertices);
}

// this function draws a single marker at arena position (x, y)
static void draw_marker(int x, int y) 
{
    // convert arena position (x, y) to coordinates for top left of shape
    int coordX = BORDER_THICKNESS + x*TILE_SIZE + OBJECT_PADDING;
    int coordY = BORDER_THICKNESS + y*TILE_SIZE + OBJECT_PADDING;
    int obstacle_size = TILE_SIZE-2*OBJECT_PADDING;

    // draw
    setColour(gray);
    fillArc(coordX, coordY, obstacle_size, obstacle_size, 0, 360);
}

// this function iterates over arenaGrid and calls the function to render markers
static void draw_markers(Arena *arena)
{
    for (int y = 0; y < arena->arenaHeight; y++) {
        for (int x = 0; x < arena->arenaWidth; x++) {
            if (arena->arenaGrid[y][x] == T_MARKER) {
                draw_marker(x, y);
            }
        }
    }
}

// functions called from main here:

// this function draws the background - called once at start; pre-requisite: arena dimensions set, window dimensions set, obstacles generated
void draw_background(Arena *arena)
{
    calculate_window_dimensions(arena);
    setWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    background();
    printf("%d, %d\n", WINDOW_WIDTH, WINDOW_HEIGHT);
    draw_border(arena);
    draw_grid(arena);
    draw_obstacles(arena);
}

// this function draws the foreground - called once per robot moveent; pre-requisite: robot created, markers generated
void draw_foreground(Robot *robot, Arena *arena)
{
    clear();
    draw_markers(arena);
    draw_robot(robot); // draw robot second so that its on top of marker
    sleep(TIME_INTERVAL);
}
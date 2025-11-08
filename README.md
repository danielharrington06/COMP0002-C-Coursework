# COMP0002 C Coursework

## Description

This program implements a robot simulation and drawing system using C and the provided drawing library (`graphics.c`, `graphics.h` and `drawapp-4.5.jar`).

The program displays a tile-based arena with markers and obstacles placed at random positions. Unless otherwise specified, the arena's width and height are both 16 tiles.

A robot, represented by a triangle, moves around the arena following a spiral algorithm of my own design.

How it works:
- The robot moves forward until it reaches the edge of the arena or an obstacle
- It stores where it has visited in its memory, its own understanding of the arena grid
- Once reaching the edge of the arena or an obstacle, it spirals around, keeping the tiles it knows (visited tiles and obstacles) on its left
- If it finds itself in a position where all the tiles around it are known, it backtracks until this is no longer the case, using a stack data structure to store where it has been

This algorithm guarantees that the robot visits every tile in the arena (so long as it physically can) and does so in an efficient way such that it can minimise the number of tiles it repeats for abstract and complicated obstacle placements.

## File Structure

The folder should be arranged as follows **before compiling**:
```vbnet
COMP0002-C-Coursework/
│
├── src/
│   ├── arena.c
│   ├── config.c
│   ├── drawing.c
│   ├── main.c
│   ├── robot.c
│   ├── spiral.c
│   └── utils.c
│
├── include/
│   ├── arena.h
│   ├── config.h
│   ├── drawing.h
│   ├── robot.h
│   ├── spiral.h
│   └── utils.h
│
├── lib/
│   ├── graphics.c
│   └── graphics.h
│
└── drawapp-4.5.jar
```

## Compilation

From the main coursework directory, run:
```bash
gcc -Wall -Werror src/*.c lib/graphics.c -Iinclude -o robot-prog.out -lm
```

## Running the Program

To run the program with the drawing application:
```bash
./robot-prog.out | java -jar drawapp-4.5.jar
```
This will randomise the robot's position and direction in an arena of default size 16x16.

### Command Line Arguments

```
To specify grid dimensions:
```bash
./robot-prog.out <width> <height> | java -jar drawapp-4.5.jar
```
To specify grid dimenions and a starting position + direction:
```bash
./robot-prog.out <width> <height> <row> <col> <direction> | java -jar drawapp-4.5.jar
```
Example:
```bash
./robot-prog.out 10 10 4 4 east | java -jar drawapp-4.5.jar
```

The grid width and height is at minimum, 4, if lower values are entered.
Their maximum value is determined based upon the values for `TILE_SIZE`, `DEFAULT_WINDOW_WIDTH` etc. in `config.c`.

If the specified position is taken up by an obstacle or is out of bounds, a random position will be generated.

If the direction is invalid, a random direction will be chosen, but the position kept.

## Program Structure Overview

!! include explanation of what is in each `.c` file

talk about `arenaGrid[y][x]`

like the drawapp, the coordinate system has its origin in the top left corner and the x-axis extends to the right, and the y-axis extends downwards
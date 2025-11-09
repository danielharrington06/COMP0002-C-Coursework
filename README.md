# COMP0002 C Coursework

## Description

This program implements a robot simulation and drawing system using C and the provided drawing library (`graphics.c`, `graphics.h` and `drawapp-4.5.jar`).

The program displays a tile-based arena with markers and obstacles placed at random positions. Unless otherwise specified, the arena's width and height are both 16 tiles.

A robot, represented by a triangle, moves around the arena following a **spiral algorithm** of my own design.

**Spiral Algorithm** - How it Works:
- The robot moves forward until it reaches the edge of the arena or an obstacle
- It stores where it has visited in its memory, its own understanding of the arena grid
- Once reaching the edge of the arena or an obstacle, it spirals around, keeping the tiles it knows (visited tiles and obstacles) on its left
- If it finds itself in a position where all the tiles around it are known, it backtracks until this is no longer the case, popping from a stack data structure to determine where it has been

This algorithm guarantees that the robot visits every tile in the arena (so long as it physically can) and does so in an efficient way such that it can minimise the number of tiles it repeats for abstract and complicated obstacle placements. It also looks pretty cool and organised!

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

The grid width and height must be at least `4` and if a lower value is entered, the width/height is forced to be `4`.

Their maximum values are determined based upon the values for `TILE_SIZE`, `DEFAULT_WINDOW_WIDTH` etc. in `config.c` by the functions `calculate_max_arena_width` and calculate_max_arena_height` in `drawing.c`.

If the specified position is taken up by an obstacle or is out of bounds, a random position and direction is generated.

If the direction is invalid, a random direction is chosen, but the position kept.

## Suggestion on How to Test

First, compile the program and run it with default settings:
```bash
gcc -Wall -Werror src/*.c lib/graphics.c -Iinclude -o robot-prog.out -lm
./robot-prog.out | java -jar drawapp-4.5.jar
```
Then, experiment with different arena sizes and starting positions (no need to recompile):
```bash
./robot-prog.out <width> <height> | java -jar drawapp-4.5.jar
./robot-prog.out <width> <height> <row> <col> <direction> | java -jar drawapp-4.5.jar
```
Then, in `config.c`, change the number of markers and number of obstacles in lines `19` and `22`:
```c
const unsigned int numObstacles = <change this>;
```
```c
const unsigned int numMarkers = <change this>;
```
Recompile and run.
```bash
gcc -Wall -Werror src/*.c lib/graphics.c -Iinclude -o robot-prog.out -lm
./robot-prog.out | java -jar drawapp-4.5.jar
```
Then, try a segmented arena with a wall between two parts. In `config.c`, change the obstacle formation and number of obstacles in lines `18` and `19`:
```c
const ObstacleFormation obstacleFormation = O_WALL;
const unsigned int numObstacles = 14; // 14 as default height is 16
```
Recompile and run, as before.

Then, try an arena with a *central cavern*/a circular area in the middle which is more awkward for the robot to navigate. In `config.c`, change the obstacle formation and set the number of obstacles in lines `18` and `19`:
```c
const ObstacleFormation obstacleFormation = O_WALL;
const unsigned int numObstacles = 0; // unnecessary change but good practice to set to 0
```
Recompile and run, as before.

Then run with different size arenas, such as:
```bash
./robot-prog.out 19 19 | java -jar drawapp-4.5.jar # odd arena width and height
./robot-prog.out 20 20 | java -jar drawapp-4.5.jar # even arena width and height
./robot-prog.out 21 16 | java -jar drawapp-4.5.jar # different arena width and height
./robot-prog.out 100 200 10 10 south | java -jar drawapp-4.5.jar # defaults to max size
```

Then, feel free to try other things that I've not mentioned by altering values in `config.c`.

## Program Structure Overview

Purpose of each `.c` file:
- `main.c` - calls all other functions to setup the robot and arena, to find the markers using the spiral algorithm and to free memory
- `arena.c` - generates and stores information about the arena, obstacles and markers
- `robot.c` - functions used by the robot to move around the arena, sense things in the arena and remember where it has been and what tiles are blocked by obstacles
- `drawing.c`- to render the arena, obstacles, markers and robot
- `spiral.c` - the main **spiral algorithm** used to traverse every available tile and find the markers
- `utils.c` - utility functions such as `max`, `check_coord_in_bounds` as well as an implementation of a stack
- `config.c` - stores configuration variables which are not set at the command line

## General Notes

In the source code, to access a specific coordinate `(x, y)`, `arenaGrid[y][x]` is used as the implementation stores rows together.

Like the drawapp, the arena's origin is in the top left corner, the x-axis extends to the right, and the y-axis extends downwards.

Where an error occurs, whether it can be resolved or not, I have used `fprintf` to `stderr` so that the message is displayed in the command line while drawapp runs. Where the error stops the program from running, I then had the program call `exit(EXIT_FAILURE)`.

The code enforces limits on the number of obstacles and markers that can be placed. The number of obstacles must be less than `1/3` of the number of tiles (to reduce the chance that obstacles block a marker from being reached) and the number of markers must be less than `2/3` of the number of tiles (so markers can always be placed). In reality, it is best to keep below these thresholds otherwise markers may get blocked from being reached by the robot.
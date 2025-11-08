# COMP0002 C Coursework

## 📘 Description

This program implements a robot simulation and drawing system using C and the provided drawing library (`graphics.c`, `graphics.h` and `drawapp-4.5.jar`).

The program displays a tile-based arena with markers and obstacles placed at random positions.

A robot, represented by a triangle, moves around the arena following a control algorithm. It can:
- Move forward, rotate left/right, and detect walls or obstacles
- Detect and pick up markers
- Drop markers at specific locations (e.g. a corner)
- Remember where it has been and mark locations as visited and blocked (by an obstacle).

The arena and obstacles are drawn on the background layer once, while the robot is animated on the foreground layer, giving the effect of movement without redrawing the background.

## ⚙️ File Structure

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

## 🧱 Compilation

From the main coursework directory, run:
```bash
gcc -Wall -Werror src/*.c lib/graphics.c -Iinclude -o robot-prog.out -lm
```

## ▶️ Running the Program

To run the program with the drawing application:
```bash
./robot-prog.out | java -jar drawapp-4.5.jar
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
./robot-prog.out 10 10 3 4 east | java -jar drawapp-4.5.jar
```

The grid width and height is at minimum, 4, if lower values are entered.
Their maximum value is determined based upon the values for `TILE_SIZE`, `DEFAULT_WINDOW_WIDTH` etc. in `config.c`.
They both default to 12 if no other values are given.

If the specified position is taken up by an obstacle or is out of bounds, a random position will be generated.

If the direction is invalid, a random direction will be chosen, but the position kept.

## 📁 Program Structure Overview

!! include explanation of what is in each `.c` file

talk about `arenaGrid[y][x]`

like the drawapp, the coordinate system has its origin in the top left corner and the x-axis extends to the right, and the y-axis extends downwards
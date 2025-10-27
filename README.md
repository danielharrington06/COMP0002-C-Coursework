# COMP0002 C Coursework

## 📘 Description

This program implements a robot simulation and drawing system using C and the provided drawing library (`graphics.c`, `graphics.h`, and `drawapp-4.5.jar`).

The program displays a tile-based arena surrounded by walls, with markers and obstacles placed at random positions.
A robot, represented by a triangle, moves around the arena following a control algorithm. It can:

- Move forward, turn left/right, and detect walls or obstacles.

- Detect and pick up markers.

- Drop markers at specific locations (e.g. a corner).

The arena and obstacles are drawn on the background layer once, while the robot is animated on the foreground layer, giving the effect of movement without redrawing the background.

## ⚙️ File Structure

The folder should be arranged as follows **before compiling**:
```vbnet
COMP0002-C-Coursework/
│
├── src/
│   ├── main.c
│   ├── robot.c
│   ├── arena.c
│   └── drawing.c
│
├── include/
│   ├── robot.h
│   ├── arena.h
│   └── drawing.h
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
gcc -Wall -Werror src/*.c lib/graphics.c -Iinclude -o robot-prog.out
```

## ▶️ Running the Program

To run the program with the drawing application:
```bash
./robot-prog.out | java -jar drawapp-4.5.jar
```

To specify a starting position and direction:
```bash
./robot-prog.out <row> <col> <direction> | java -jar drawapp-4.5.jar
```
Example:
```bash
./robot-prog.out 3 4 east | java -jar drawapp-4.5.jar
```

## 📁 Program Structure Overview

!! include explanation of what is in each `.c` file

## 🗺️ Map Coordinate System Explanation

The map (arena) uses a zero-indexed coordinate system, where the top-left corner of the arena is (0, 0) and the bottom-right corner is (arenaWidth - 1, arenaHeight - 1).

The x-coordinate increases from left to right.

The y-coordinate increases from top to bottom.

Each tile in the arena grid can therefore be uniquely identified by its (x, y) coordinates within these bounds.

For example, in an arena of size 10 × 8,

The top-left tile is (0, 0)

The bottom-right tile is (9, 7)

This coordinate system is consistent throughout the program for both the arena and the robot’s internal memory map, ensuring that positional logic and drawing functions align correctly.
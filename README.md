# COMP0002 C Coursework

## 📘 Description

This program implements a robot simulation and drawing system using C and the provided drawing library (`graphics.c`, `graphics.h` and `drawapp-4.5.jar`).

The program displays a tile-based arena with markers and obstacles placed at random positions.

A robot, represented by a triangle, moves around the arena following a control algorithm. It can:
- Move forward, rotate left/right, and detect walls or obstacles
- Detect and pick up markers
- Drop markers at specific locations (e.g. a corner)

The arena and obstacles are drawn on the background layer once, while the robot is animated on the foreground layer, giving the effect of movement without redrawing the background.

## ⚙️ File Structure

The folder should be arranged as follows **before compiling**:
```vbnet
COMP0002-C-Coursework/
│
├── src/
│   ├── main.c
│   ├── arena.c
│   ├── drawing.c
│   ├── robot.c
│   └── utils.c
│
├── include/
│   ├── arena.h
│   ├── drawing.h
│   ├── robot.h
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

To specify a starting position and direction:
```bash
./robot-prog.out <row> <col> <direction> | java -jar drawapp-4.5.jar
```
Example:
```bash
./robot-prog.out 3 4 east | java -jar drawapp-4.5.jar
```

If the specified position is taken up by an obstacle or is out of bounds, a random position will be generated.

If the directino is invalid, a random direction will be chosen, but the position kept.

## 📁 Program Structure Overview

!! include explanation of what is in each `.c` file

## 🗺️ Map Coordinate System Explanation

The program uses a **zero-indexed coordinate system** for both the arena and the robot’s internal map.
- The top-left corner of the arena is at `(0, 0)`
- The bottom-right corner is at `(arenaWidth - 1, arenaHeight - 1)`
- The x-coordinate increases from left to right
- The y-coordinate increases from top to bottom

Each tile in the arena grid is uniquely identified by its `(x, y)` coordinates within these bounds.

Example:
- For an arena of size 10 × 8:
- Top-left tile → `(0, 0)`
- Bottom-right tile → `(9, 7)`
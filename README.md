# Maze Solver: State Space Search Algorithms

This repository contains an implementation of various systematic state-space search algorithms used to find a path through a 2D maze. 

## Features

The core of this project is the implementation of five distinct search algorithms:

1. **Random Search:**
2. **Depth-First Search (DFS):** 
3. **Breadth-First Search (BFS):** 
4. **Greedy Search:** 
5. **A* Search (A-Star):** 

**Additional Features:**
* Loads maze configurations from external text files.
* Terminal-based step-by-step visualization of the algorithm's operation (node expansion).
* Calculates and displays the total number of expanded nodes and the final path length.

## Input Format

The program takes text files as input. The maze format uses specific characters to define the environment:

* `X` : Impenetrable walls
* ` ` (Space) : Free passage/navigable space
* `start X, Y` : Starting coordinates
* `end X, Y` : Target/Goal coordinates

*Note: The coordinate system uses Cartesian coordinates where the origin (0,0) is in the upper left corner, and the positive y-axis is oriented downwards.*

**Example `maze.txt`:**
```text
XXXXXXXXXXXXXX
X            X
X  XXXXXXX   X
X     X      X
X X X X XXX  X
X X          X
XX   X XXXXX X
X   X        X
X X X XXX  XXX
X            X
X XXX X    X X
X            X
XXXXXXXXXXXXXX
start 1, 7
end 5, 3

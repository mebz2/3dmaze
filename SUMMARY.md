# Project Summary

## Task 1: Project Scaffolding
**Branch:** `feature/project-scaffolding`  
**Date:** 2026-09-26

### Description
Set up the initial project structure for the 3D Maze application. Created all foundational files needed to compile and run a minimal FreeGLUT window.

### Changes
- Created `CMakeLists.txt` — CMake 3.10+, C++17 standard, finds and links OpenGL and GLUT (FreeGLUT)
- Created `src/main.cpp` — Minimal FreeGLUT program that opens an 800×600 black window titled "3D Maze" with depth testing enabled, perspective projection (45° FOV), and double buffering
- Created `.gitignore` — Ignores build directories (`build/`, `cmake-build-*/`, `out/`), CMake artifacts, compiled objects (`.o`, `.obj`, `.exe`), IDE files (`.vs/`, `.vscode/`, `.idea/`), and OS files
- Created `README.md` — Build instructions for Linux and Windows, prerequisites, controls table, Windows troubleshooting for FreeGLUT linking
- Created `SUMMARY.md` — This file, for tracking project progress
- Created `include/.gitkeep` — Empty include directory placeholder for future header files

### Tools Used
- CMake (build system)
- FreeGLUT (windowing and input via GLUT API)
- OpenGL + GLU (rendering and utilities)
- g++ with C++17

### Verification
- Project compiles with `mkdir build && cd build && cmake .. && make`
- Running `./3DMaze` opens an 800×600 black window titled "3D Maze"

---

## Task 2: Maze Data Structure & Wall Rendering
**Branch:** `feature/maze-structure`  
**Date:** 2026-09-26

### Description
Added a hardcoded 15×15 maze grid and rendered it as 3D colored walls with a green floor, viewed from a static bird's-eye camera.

### Changes
- Created `include/maze.h` — Maze class with grid storage, `isWall()`, `getWidth()`, `getHeight()`, `draw()`, private `drawCube()` and `drawFloor()` helpers
- Created `src/maze.cpp` — Full implementation:
  - Hardcoded 15×15 maze layout (1=wall, 0=path), surrounded by walls on all borders
  - `drawCube()` renders a 6-face GL_QUADS cube (height 2.0) with correct outward-facing normals per face and CCW winding order
  - Alternating brown wall colors based on `(x+z) % 2` for visual variety
  - Green floor quad spanning the maze area with correct CCW winding
  - `isWall()` with bounds checking (out of bounds = wall)
- Modified `src/main.cpp` — Added `#include "maze.h"`, global Maze instance, static `gluLookAt` camera positioned above/behind the maze, calls `maze.draw()` in display callback
- Modified `CMakeLists.txt` — Added `src/maze.cpp` to sources

### Review Notes
- Reviewer found floor quad had CW winding order conflicting with its +Y normal — fixed to CCW order

### Tools Used
- OpenGL fixed-function pipeline (GL_QUADS, glNormal3f, glColor3f)
- gluLookAt for static camera

### Verification
- Compiles cleanly with no warnings
- Running the program shows a bird's-eye view of the maze with brown walls on a green floor

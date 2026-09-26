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

---

## Task 3: First-Person Camera & Movement
**Branch:** `feature/first-person-camera`  
**Date:** 2026-09-26

### Description
Implemented a first-person camera system with WASD movement, mouse look, and wall collision detection, allowing the player to navigate through the maze interactively.

### Changes
- Created `include/camera.h` — Camera class with position (x,y,z), yaw/pitch angles, movement methods taking const Maze& for collision, mouse look, getters for minimap use
- Created `src/camera.cpp` — Full implementation:
  - `apply()` computes look-at target from yaw/pitch using `gluLookAt`
  - `canMove()` checks 4 corners of a 0.2-radius bounding box against `maze.isWall()`
  - Movement with wall-sliding: tries full diagonal move first, falls back to X-only or Z-only
  - `look()` applies mouse sensitivity (0.15), clamps pitch to ±89° to prevent gimbal flip
  - Strafing computed via yaw ± 90°
- Rewrote `src/main.cpp`:
  - Flag-based WASD input (glutKeyboardFunc + glutKeyboardUpFunc) for smooth movement
  - Mouse look via glutPassiveMotionFunc with warp-to-center and firstMouse guard
  - Timer-based game loop at ~60fps (16ms interval) for consistent movement
  - ESC key exits the program
  - Cursor hidden with glutSetCursor(GLUT_CURSOR_NONE)
  - Player starts at (1.5, 1.0, 1.5) — cell (1,1), eye at half wall height
  - FOV changed to 60° for natural first-person feel
  - Background changed to dark gray (0.1, 0.1, 0.1)
- Modified `CMakeLists.txt` — Added `src/camera.cpp` to sources

### Review Notes
- Reviewer passed with no issues — all collision, camera, and input mechanics approved

### Tools Used
- OpenGL fixed-function pipeline (gluLookAt, gluPerspective)
- FreeGLUT input callbacks (keyboard, passive motion, timer)
- C++ math (cosf, sinf for direction vectors)

### Verification
- Compiles cleanly
- Player walks through maze in first person with WASD
- Mouse controls view direction (horizontal + vertical)
- Cannot walk through walls (collision + wall sliding works)
- ESC quits the application

---

## Task 4: Lighting
**Branch:** `feature/lighting`  
**Date:** 2026-09-26

### Description
Added OpenGL fixed-function lighting to give the maze depth and atmosphere. Walls now have visible shading based on their orientation relative to the light source.

### Changes
- Modified `src/main.cpp`:
  - Enabled `GL_LIGHTING`, `GL_LIGHT0`, `GL_COLOR_MATERIAL` in `main()` initialization
  - Set `glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE)` so existing `glColor3f` calls in maze.cpp serve as material colors automatically
  - Configured light properties: ambient (0.3), diffuse (0.8), specular (0.2) — enough ambient to see in shadow, strong diffuse for visible shading
  - Point light positioned at (7.5, 10.0, 7.5) — centered above the 15×15 maze
  - Light position set in `display()` after `camera.apply()` so it remains fixed in world space

### No Other Files Modified
- `src/maze.cpp` already had correct `glNormal3f` calls on all 6 cube faces and the floor from Task 2
- Camera and input systems untouched

### Review Notes
- Reviewer passed with no issues — lighting setup, positioning, and material configuration all approved

### Tools Used
- OpenGL fixed-function lighting pipeline (GL_LIGHT0, glLightfv)
- GL_COLOR_MATERIAL for automatic material-from-color

### Verification
- Compiles cleanly
- Walls have visible shading — faces toward the light are brighter, away are darker
- Floor is lit
- Moving through the maze feels more 3D due to lighting cues
- No regressions to movement, collision, or input

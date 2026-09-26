# 3D Maze — OpenGL + FreeGLUT (C++) Build Plan

## Overview

A first-person 3D maze game rendered with OpenGL (fixed-function or modern pipeline) using FreeGLUT for windowing, input, and the game loop. The player navigates a procedurally generated maze from a first-person perspective and tries to reach the exit.

---

## 1. Project Setup

- **Toolchain**: g++ (or clang++) with C++17.
- **Dependencies**: `libgl`, `libglu`, `freeglut3-dev` (Debian/Ubuntu) or equivalent.
- **Build system**: A `Makefile` (or CMakeLists.txt) linking `-lGL -lGLU -lglut -lm`.
- **Directory layout**:
  ```
  3dmaze/
  ├── plan.md
  ├── Makefile
  ├── src/
  │   ├── main.cpp          # entry point, GLUT callbacks
  │   ├── maze.h / maze.cpp # maze data & generation
  │   ├── player.h / player.cpp   # camera / player state
  │   ├── renderer.h / renderer.cpp # OpenGL drawing
  │   └── utils.h           # math helpers, constants
  └── textures/             # optional wall/floor textures
  ```

---

## 2. Window & GLUT Initialization (`main.cpp`)

| Task | Details |
|------|---------|
| Initialize GLUT | `glutInit`, `glutInitDisplayMode(GLUT_DOUBLE \| GLUT_RGB \| GLUT_DEPTH)` |
| Create window | `glutInitWindowSize(1024, 768)`, `glutCreateWindow("3D Maze")` |
| Register callbacks | `glutDisplayFunc`, `glutReshapeFunc`, `glutKeyboardFunc`, `glutSpecialFunc`, `glutPassiveMotionFunc`, `glutTimerFunc` / `glutIdleFunc` |
| OpenGL defaults | Enable `GL_DEPTH_TEST`, set clear color, configure `glMatrixMode(GL_PROJECTION)` with `gluPerspective` |
| Enter main loop | `glutMainLoop()` |

---

## 3. Maze Data Structure & Generation (`maze.h / maze.cpp`)

### 3.1 Data Representation

- 2D grid of cells: `std::vector<std::vector<Cell>>`.
- Each `Cell` stores 4 boolean walls: `top`, `bottom`, `left`, `right`.
- Grid dimensions configurable (e.g., `MAZE_WIDTH = 16`, `MAZE_HEIGHT = 16`).
- Each cell maps to a world-space tile of size `CELL_SIZE` (e.g., 2.0 units).

### 3.2 Maze Generation Algorithm

Choose one (recursive backtracker is simplest):

1. **Recursive Backtracker (DFS)**
   - Start at cell (0, 0), mark visited.
   - Pick a random unvisited neighbour, remove the wall between them, recurse.
   - Backtrack when stuck.
   - Produces long, winding corridors — great for a maze game.

2. *Alternatives (optional later)*: Kruskal's, Prim's, Eller's.

### 3.3 Start & Exit

- **Start**: cell (0, 0) — player spawns here.
- **Exit**: cell (WIDTH-1, HEIGHT-1) — mark with a distinct colour or a floating marker.

---

## 4. Player / Camera (`player.h / player.cpp`)

### 4.1 State

```cpp
struct Player {
    float x, z;        // position on the XZ plane
    float yaw;          // horizontal look angle (degrees)
    float pitch;        // vertical look angle (degrees), clamped ±89°
    float eyeHeight;    // Y position of camera (e.g., 0.5)
    float speed;        // movement speed (units/sec)
    float sensitivity;  // mouse look sensitivity
};
```

### 4.2 Camera Matrix

Each frame, compute the look-at target from yaw/pitch and call:

```cpp
gluLookAt(x, eyeHeight, z,
          x + dx, eyeHeight + dy, z + dz,
          0, 1, 0);
```

### 4.3 Input Handling

| Input | Action |
|-------|--------|
| `W` / `↑` | Move forward |
| `S` / `↓` | Move backward |
| `A` / `←` | Strafe left |
| `D` / `→` | Strafe right |
| Mouse move | Rotate yaw/pitch |
| `ESC` | Quit |

- Use `glutPassiveMotionFunc` for mouse look; warp pointer to centre each frame with `glutWarpPointer` to enable infinite rotation.
- Use a key-state array (`bool keys[256]`) set in `KeyboardDown` / `KeyboardUp` callbacks so multiple keys work simultaneously.

---

## 5. Collision Detection

- Treat the player as a small circle (radius ~0.15) on the XZ plane.
- Before applying movement, check the new position against nearby cell walls.
- For each wall segment (a line from point A to point B), test circle-vs-line-segment intersection.
- If collision, slide the player along the wall (project velocity onto the wall's tangent) for smooth feel.
- Prevent the player from leaving the maze boundary.

---

## 6. Rendering (`renderer.h / renderer.cpp`)

### 6.1 Drawing Walls

For every cell in the grid, check each wall flag and draw a quad:

```
Wall quad = 4 vertices, normal facing inward.
Height: WALL_HEIGHT (e.g., 1.0)
Width:  CELL_SIZE
```

Use `glBegin(GL_QUADS)` … `glEnd()` (or VBOs for performance).

### 6.2 Floor & Ceiling

- Draw a large quad at Y = 0 (floor) spanning the entire maze.
- Optionally draw a ceiling quad at Y = WALL_HEIGHT.
- Use different colours or textures for each.

### 6.3 Exit Marker

- Draw a coloured pillar, spinning cube, or glowing quad at the exit cell to guide the player.

### 6.4 Lighting (optional but recommended)

- Enable `GL_LIGHTING` and `GL_LIGHT0`.
- Place a point light at the player's position (acts as a flashlight / torch) so distant corridors are dark.
- Set material properties (`glMaterialfv`) or use `glColorMaterial`.

### 6.5 Textures (optional)

- Load `.bmp` or `.png` images for walls, floor, ceiling.
- Use `glGenTextures`, `glBindTexture`, `glTexImage2D`.
- Assign texture coordinates to each quad.

---

## 7. Game Loop & Timing

- Use `glutTimerFunc(16, timerCallback, 0)` for ~60 FPS.
- In the timer callback:
  1. Compute `deltaTime`.
  2. Process held keys → update player position (with collision).
  3. Call `glutPostRedisplay()`.
- In the display callback:
  1. `glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)`.
  2. Set camera (`glLoadIdentity` + `gluLookAt`).
  3. Draw maze geometry.
  4. Draw exit marker.
  5. Draw HUD / minimap (optional).
  6. `glutSwapBuffers()`.

---

## 8. Win Condition

- Each frame, check if the player's cell == exit cell.
- On win:
  - Display a congratulations message (render text with `glutBitmapCharacter`).
  - Optionally regenerate the maze and restart.

---

## 9. Optional Enhancements (Post-MVP)

| Feature | Notes |
|---------|-------|
| **Minimap** | Render a 2D orthographic overlay showing visited cells. |
| **Fog** | `glFog` to limit visibility and increase atmosphere. |
| **Skybox** | Textured cube around the scene (if no ceiling). |
| **Sound** | Footstep sounds via a lightweight library (e.g., OpenAL, SDL_mixer). |
| **Multiple levels** | Increase grid size or difficulty each level. |
| **Timer / score** | Track completion time, display on HUD. |
| **Animated exit** | Rotating or pulsing marker using `glRotatef` over time. |
| **Modern OpenGL** | Port to shaders (vertex + fragment) with VAO/VBO for better performance. |

---

## 10. Build & Run

```bash
# Install dependencies (Debian/Ubuntu)
sudo apt install freeglut3-dev libgl-dev libglu-dev

# Build
make          # or: g++ -std=c++17 src/*.cpp -o maze -lGL -lGLU -lglut -lm

# Run
./maze
```

---

## Implementation Order (Suggested)

1. **Skeleton** — GLUT window, coloured background, quit on ESC.
2. **Camera** — Free-look camera with WASD + mouse; no maze yet, just an empty plane.
3. **Maze generation** — Generate and print to console to verify.
4. **Render walls** — Draw the generated maze as 3D quads.
5. **Collision** — Prevent walking through walls.
6. **Exit & win** — Mark the exit, detect arrival, show message.
7. **Polish** — Lighting, textures, fog, minimap, sound.

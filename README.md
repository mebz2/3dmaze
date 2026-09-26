# 3D Maze - OpenGL/FreeGLUT

A first-person interactive 3D maze built with OpenGL and FreeGLUT in C++. Navigate through the maze using keyboard and mouse controls.

## Prerequisites

- **CMake** 3.10 or higher
- **C++17** compatible compiler (g++, clang++, or MSVC)
- **OpenGL** (usually pre-installed on most systems)
- **FreeGLUT** (open-source alternative to GLUT)

## Building on Linux

1. Install FreeGLUT (if not already installed):
   ```bash
   sudo apt-get install freeglut3-dev
   ```

2. Build the project:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

3. Run:
   ```bash
   ./3DMaze
   ```

## Building on Windows

> **Important:** You MUST download FreeGLUT and link it properly for the project to build.

1. Download FreeGLUT from [freeglut.sourceforge.net](https://freeglut.sourceforge.net/) or use the pre-built MSVC package from [transmissionzero.co.uk/software/freeglut-devel/](https://www.transmissionzero.co.uk/software/freeglut-devel/).

2. Extract the archive and note the installation path (e.g., `C:\freeglut`).

3. Build with CMake, passing the FreeGLUT path:
   ```cmd
   mkdir build && cd build
   cmake .. -DCMAKE_PREFIX_PATH="C:/path/to/freeglut"
   cmake --build . --config Release
   ```

   Alternatively, use CMake GUI and set `CMAKE_PREFIX_PATH` to your FreeGLUT directory.

4. **Copy `freeglut.dll`** from the FreeGLUT `bin` directory to the same directory as `3DMaze.exe`.

5. Run:
   ```cmd
   3DMaze.exe
   ```

### Windows Troubleshooting

- If CMake cannot find GLUT, ensure `CMAKE_PREFIX_PATH` points to the directory containing the `lib`, `include`, and `bin` folders of FreeGLUT.
- If you get a missing DLL error at runtime, make sure `freeglut.dll` is in the same directory as the executable or in your system PATH.

## Controls

| Key       | Action          |
|-----------|-----------------|
| W         | Move forward    |
| S         | Move backward   |
| A         | Strafe left     |
| D         | Strafe right    |
| Mouse     | Look around     |
| ESC       | Quit            |

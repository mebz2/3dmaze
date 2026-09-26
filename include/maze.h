#ifndef MAZE_H
#define MAZE_H

class Maze {
public:
    // Maze dimensions
    static const int WIDTH = 15;
    static const int HEIGHT = 15;

    Maze();

    // Check if a cell is a wall
    bool isWall(int x, int z) const;

    // Get dimensions
    int getWidth() const;
    int getHeight() const;

    // Draw the maze
    void draw() const;

private:
    // 1 = wall, 0 = path
    int grid[HEIGHT][WIDTH];

    // Draw a single unit cube at position (x, 0, z) with height 2.0
    void drawCube(float x, float z) const;

    // Draw the floor plane
    void drawFloor() const;
};

#endif

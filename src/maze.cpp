#include "maze.h"
#include <GL/glut.h>
#include <GL/gl.h>

Maze::Maze() {
    int layout[HEIGHT][WIDTH] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,1,0,0,0,0,0,1,0,0,0,1},
        {1,0,1,0,1,0,1,1,1,0,1,0,1,0,1},
        {1,0,1,0,0,0,0,0,1,0,0,0,1,0,1},
        {1,0,1,1,1,1,1,0,1,1,1,0,1,0,1},
        {1,0,0,0,0,0,1,0,0,0,0,0,1,0,1},
        {1,1,1,1,1,0,1,1,1,1,1,1,1,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,1,1,1,1,1,0,1,1,1,1,1,1,1},
        {1,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
        {1,0,1,0,1,1,1,1,1,0,1,1,1,0,1},
        {1,0,0,0,1,0,0,0,0,0,1,0,0,0,1},
        {1,1,1,0,1,0,1,1,1,1,1,0,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };

    for (int z = 0; z < HEIGHT; z++) {
        for (int x = 0; x < WIDTH; x++) {
            grid[z][x] = layout[z][x];
        }
    }
}

bool Maze::isWall(int x, int z) const {
    if (x < 0 || x >= WIDTH || z < 0 || z >= HEIGHT) {
        return true;
    }
    return grid[z][x] == 1;
}

int Maze::getWidth() const {
    return WIDTH;
}

int Maze::getHeight() const {
    return HEIGHT;
}

void Maze::drawCube(float x, float z) const {
    // Alternate wall colors for visual variety
    if (((int)x + (int)z) % 2 == 0) {
        glColor3f(0.6f, 0.4f, 0.2f);
    } else {
        glColor3f(0.55f, 0.35f, 0.18f);
    }

    float x0 = x;
    float x1 = x + 1.0f;
    float y0 = 0.0f;
    float y1 = 2.0f;
    float z0 = z;
    float z1 = z + 1.0f;

    glBegin(GL_QUADS);

    // Front face (z1 side) — normal (0, 0, 1)
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(x0, y0, z1);
    glVertex3f(x1, y0, z1);
    glVertex3f(x1, y1, z1);
    glVertex3f(x0, y1, z1);

    // Back face (z0 side) — normal (0, 0, -1)
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(x1, y0, z0);
    glVertex3f(x0, y0, z0);
    glVertex3f(x0, y1, z0);
    glVertex3f(x1, y1, z0);

    // Left face (x0 side) — normal (-1, 0, 0)
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(x0, y0, z0);
    glVertex3f(x0, y0, z1);
    glVertex3f(x0, y1, z1);
    glVertex3f(x0, y1, z0);

    // Right face (x1 side) — normal (1, 0, 0)
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(x1, y0, z1);
    glVertex3f(x1, y0, z0);
    glVertex3f(x1, y1, z0);
    glVertex3f(x1, y1, z1);

    // Top face (y1 side) — normal (0, 1, 0)
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(x0, y1, z1);
    glVertex3f(x1, y1, z1);
    glVertex3f(x1, y1, z0);
    glVertex3f(x0, y1, z0);

    // Bottom face (y0 side) — normal (0, -1, 0)
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(x0, y0, z0);
    glVertex3f(x1, y0, z0);
    glVertex3f(x1, y0, z1);
    glVertex3f(x0, y0, z1);

    glEnd();
}

void Maze::drawFloor() const {
    glColor3f(0.2f, 0.5f, 0.2f);
    glNormal3f(0.0f, 1.0f, 0.0f);

    glBegin(GL_QUADS);
    glVertex3f(0.0f, 0.0f, (float)HEIGHT);
    glVertex3f((float)WIDTH, 0.0f, (float)HEIGHT);
    glVertex3f((float)WIDTH, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glEnd();
}

void Maze::draw() const {
    drawFloor();

    for (int z = 0; z < HEIGHT; z++) {
        for (int x = 0; x < WIDTH; x++) {
            if (grid[z][x] == 1) {
                drawCube((float)x, (float)z);
            }
        }
    }
}

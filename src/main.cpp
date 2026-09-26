#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdlib>
#include "maze.h"
#include "camera.h"

// Globals
Maze maze;
Camera camera(1.5f, 1.0f, 1.5f);  // Start at cell (1,1), eye height 1.0

// Movement flags
bool keyW = false, keyA = false, keyS = false, keyD = false;

// Window dimensions for mouse centering
int windowWidth = 800;
int windowHeight = 600;
bool firstMouse = true;

const float MOVE_SPEED = 0.08f;
const int TIMER_MS = 16;  // ~60 FPS

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    camera.apply();

    // Position light above the center of the maze
    GLfloat lightPos[] = {7.5f, 10.0f, 7.5f, 1.0f};  // w=1.0 = point light
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    maze.draw();

    glutSwapBuffers();
}

void reshape(int width, int height) {
    if (height == 0) height = 1;
    windowWidth = width;
    windowHeight = height;
    float aspect = static_cast<float>(width) / static_cast<float>(height);

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, aspect, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void timer(int value) {
    // Process movement
    if (keyW) camera.moveForward(MOVE_SPEED, maze);
    if (keyS) camera.moveBackward(MOVE_SPEED, maze);
    if (keyA) camera.strafeLeft(MOVE_SPEED, maze);
    if (keyD) camera.strafeRight(MOVE_SPEED, maze);

    glutPostRedisplay();
    glutTimerFunc(TIMER_MS, timer, 0);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': case 'W': keyW = true; break;
        case 's': case 'S': keyS = true; break;
        case 'a': case 'A': keyA = true; break;
        case 'd': case 'D': keyD = true; break;
        case 27: exit(0); break;  // ESC
    }
}

void keyboardUp(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': case 'W': keyW = false; break;
        case 's': case 'S': keyS = false; break;
        case 'a': case 'A': keyA = false; break;
        case 'd': case 'D': keyD = false; break;
    }
}

void mouseMotion(int x, int y) {
    if (firstMouse) {
        glutWarpPointer(windowWidth / 2, windowHeight / 2);
        firstMouse = false;
        return;
    }

    int centerX = windowWidth / 2;
    int centerY = windowHeight / 2;
    float dx = static_cast<float>(x - centerX);
    float dy = static_cast<float>(y - centerY);

    if (dx != 0.0f || dy != 0.0f) {
        camera.look(dx, dy);
        glutWarpPointer(centerX, centerY);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("3D Maze");

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Light properties
    GLfloat ambientLight[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat diffuseLight[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat specularLight[] = {0.2f, 0.2f, 0.2f, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutPassiveMotionFunc(mouseMotion);
    glutTimerFunc(TIMER_MS, timer, 0);

    glutSetCursor(GLUT_CURSOR_NONE);

    glutMainLoop();
    return 0;
}

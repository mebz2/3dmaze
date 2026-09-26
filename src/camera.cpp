#include "camera.h"
#include <GL/glu.h>
#include <cmath>

static float toRadians(float degrees) {
    return degrees * 3.14159265f / 180.0f;
}

Camera::Camera(float startX, float startY, float startZ)
    : posX(startX), posY(startY), posZ(startZ), yaw(-90.0f), pitch(0.0f) {
    // yaw = -90 means looking along -Z initially (into the maze)
}

void Camera::apply() const {
    float radYaw = toRadians(yaw);
    float radPitch = toRadians(pitch);

    float lookX = posX + cosf(radPitch) * cosf(radYaw);
    float lookY = posY + sinf(radPitch);
    float lookZ = posZ + cosf(radPitch) * sinf(radYaw);

    gluLookAt(posX, posY, posZ,
              lookX, lookY, lookZ,
              0.0f, 1.0f, 0.0f);
}

bool Camera::canMove(float nx, float nz, const Maze& maze) const {
    const float radius = 0.2f;

    // Check 4 corners of bounding box around new position
    if (maze.isWall((int)(nx - radius), (int)(nz - radius))) return false;
    if (maze.isWall((int)(nx + radius), (int)(nz - radius))) return false;
    if (maze.isWall((int)(nx - radius), (int)(nz + radius))) return false;
    if (maze.isWall((int)(nx + radius), (int)(nz + radius))) return false;

    return true;
}

void Camera::moveForward(float speed, const Maze& maze) {
    float radYaw = toRadians(yaw);
    float nx = posX + cosf(radYaw) * speed;
    float nz = posZ + sinf(radYaw) * speed;

    // Try full movement first
    if (canMove(nx, nz, maze)) {
        posX = nx;
        posZ = nz;
    }
    // Try sliding along X axis
    else if (canMove(nx, posZ, maze)) {
        posX = nx;
    }
    // Try sliding along Z axis
    else if (canMove(posX, nz, maze)) {
        posZ = nz;
    }
}

void Camera::moveBackward(float speed, const Maze& maze) {
    moveForward(-speed, maze);
}

void Camera::strafeLeft(float speed, const Maze& maze) {
    float radYaw = toRadians(yaw - 90.0f);
    float nx = posX + cosf(radYaw) * speed;
    float nz = posZ + sinf(radYaw) * speed;

    if (canMove(nx, nz, maze)) {
        posX = nx;
        posZ = nz;
    } else if (canMove(nx, posZ, maze)) {
        posX = nx;
    } else if (canMove(posX, nz, maze)) {
        posZ = nz;
    }
}

void Camera::strafeRight(float speed, const Maze& maze) {
    strafeLeft(-speed, maze);
}

void Camera::look(float dx, float dy) {
    const float sensitivity = 0.15f;
    yaw += dx * sensitivity;
    pitch -= dy * sensitivity;  // Inverted: moving mouse up = look up

    // Clamp pitch to prevent flipping
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
}

float Camera::getX() const { return posX; }
float Camera::getZ() const { return posZ; }
float Camera::getYaw() const { return yaw; }

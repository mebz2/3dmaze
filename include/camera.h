#ifndef CAMERA_H
#define CAMERA_H

#include "maze.h"

class Camera {
public:
    Camera(float startX, float startY, float startZ);

    // Apply the camera transform (call gluLookAt)
    void apply() const;

    // Movement
    void moveForward(float speed, const Maze& maze);
    void moveBackward(float speed, const Maze& maze);
    void strafeLeft(float speed, const Maze& maze);
    void strafeRight(float speed, const Maze& maze);

    // Mouse look
    void look(float dx, float dy);

    // Getters for minimap (Task 5)
    float getX() const;
    float getZ() const;
    float getYaw() const;

private:
    float posX, posY, posZ;    // Position
    float yaw;                  // Horizontal angle in degrees
    float pitch;                // Vertical angle in degrees

    // Collision: check if position (nx, nz) is walkable
    bool canMove(float nx, float nz, const Maze& maze) const;
};

#endif

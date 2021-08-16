#ifndef CAMERA_H
#define CAMERA_H

#include <glm/vec3.hpp>

#include "Model3D.hpp"

#include <iostream>
#include <cmath>

#define ORIGIN glm::vec3(0.0f, 0.0f, 0.0f)

class Camera
{
public:
    glm::vec3 u;
    glm::vec3 v;
    glm::vec3 n;

    glm::vec3 position;
    glm::vec3 lookAtPoint;

    float nearPlane;
    float farPlane;
    float fieldOfView;

    Camera();
    void ResetPosition();
    void FrameObject(Model3D object);
    void Rotate(float pitch, float roll, float yaw);
    void RotateRoll(float angle);
    void RotatePitch(float angle);
    void RotateYaw(float angle);
    void PrintDefinition();
};

#endif

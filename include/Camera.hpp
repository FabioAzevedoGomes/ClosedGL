#ifndef CAMERA_H
#define CAMERA_H

#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "Model3D.hpp"
#include "Properties.hpp"

#include <iostream>
#include <cmath>

#define ORIGIN glm::vec3(0.0f, 0.0f, 0.0f)

class Camera
{
public:
    glm::vec3 u;
    glm::vec3 v;
    glm::vec3 n;

    float pitch;
    float roll;
    float yaw;

    glm::vec3 position;
    glm::vec3 lookAtPoint;

    float nearPlane;
    float farPlane;
    float fieldOfView;

    Camera();
    void Reset();

    void FrameObject(Model3D object);
    void MoveTo(MovementOptions direction, float movementSpeed);
    void LookAt();
    glm::vec3 GetAnglesToViewVector(glm::vec3 viewVector);
    void Rotate(float pitch, float roll, float yaw);
    void PrintDefinition();

private:
    void UpdateAngles(float pitch, float roll, float yaw);
    void RotateRoll(float angle);
    void RotatePitch(float angle);
    void RotateYaw(float angle);
};

#endif

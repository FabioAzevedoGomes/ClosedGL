#ifndef CAMERA_H
#define CAMERA_H

#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/string_cast.hpp>

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

    glm::vec3 position;
    glm::vec3 lookAtPoint;

    float nearPlane;
    float farPlane;
    float horizontalFieldOfView;
    float verticalFieldOfView;

    float viewWidth, viewHeight;

    Camera();
    void Reset();
    void FrameObject(Model3D object);

    void MoveTo(MovementOptions direction, float movementSpeed);
    void Rotate(float pitch, float roll, float yaw);
    void LookAtFramedObject();

    glm::mat4 GetProjectionMatrix();
    glm::mat4 GetViewMatrix();

    void PrintDefinition();

private:
    void RotateRoll(float angle);
    void RotatePitch(float angle);
    void RotateYaw(float angle);
};

#endif

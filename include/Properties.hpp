#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <glm/vec3.hpp>

#include <cmath>
#include <iostream>

enum RenderModes
{
    Standard = 0,
    Wireframe = 1
};

enum NormalOrientation
{
    Clockwise = 0,
    CounterClockwise = 1
};

enum LightingModes
{
    Gouraud,
    Phong
};

enum MovementOptions
{
    None = 0,
    Up = 1,
    Down = 2,
    Left = 3,
    Right = 4,
    Forwards = 5,
    Backwards = 6
};

typedef struct
{
    int renderMode = Wireframe;
    int normalOrientation = Clockwise;

    //LightingModes lightingMode = Gouraud;

    glm::vec3 modelColor;
    glm::vec3 backgroundColor = glm::vec3(0.0f);

    bool resetCamera = false;

    float rotationPitch = 0.0f;
    float rotationYaw = 0.0f;
    float rotationRoll = 0.0f;

    bool keepLookingAtModel = false;
    bool shouldMove = false;
    float speed = 0.1f;
    int movementDirection = None;

    float fieldOfView = M_PI / 2.0f;
    float nearPlane = 0.1f;
    float farPlane = 10000.0f;

    bool reloadFile = false;
    std::string modelFilePath;

} Properties;

#endif

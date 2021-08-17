#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <glm/vec3.hpp>

#include <cmath>
#include <iostream>

enum RenderModes
{
    Standard = 0,
    Wireframe = 1,
    Points = 2
};

enum PolygonOrientation
{
    Clockwise = 0,
    CounterClockwise = 1
};

enum CullingModes
{
    NoCulling,
    BackfaceCulling,
    FrontFaceCulling
};

enum LightingModes
{
    Gouraud,
    Phong
};

enum MovementOptions
{
    NoMovement = 0,
    Up = 1,
    Down = 2,
    Left = 3,
    Right = 4,
    Forward = 5,
    Backwards = 6
};

typedef struct
{
    int renderMode = Standard;
    int orientation = Clockwise;
    int cullingMode = BackfaceCulling;

    //LightingModes lightingMode = Gouraud;

    glm::vec3 modelColor;
    glm::vec3 backgroundColor = glm::vec3(0.0f);

    bool resetCamera = false;
    bool keepLookingAtModel = false;

    float rotationPitch = 0.0f;
    float rotationYaw = 0.0f;
    float rotationRoll = 0.0f;

    bool shouldMove = false;
    float speed = 0.1f;
    MovementOptions movementDirection = NoMovement;

    float fieldOfView = M_PI / 2.0f;
    float nearPlane = 0.1f;
    float farPlane = 10000.0f;

    bool reloadFile = false;
    std::string modelFilePath;

} Properties;

#endif

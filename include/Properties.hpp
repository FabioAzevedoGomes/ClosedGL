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

enum LightingModes
{
    Gouraud,
    Phong
};

typedef struct
{
    int renderMode = Wireframe;

    //LightingModes lightingMode = Gouraud;

    glm::vec3 modelColor;

    float rotationPitch = 0.0f;
    float rotationYaw = 0.0f;
    float rotationRoll = 0.0f;

    float fieldOfView = M_PI / 2.0f;
    float nearPlane = 0.1f;
    float farPlane = 10000.0f;

    bool reloadFile = false;
    std::string modelFilePath;

} Properties;

#endif

#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <glm/vec3.hpp>

#include <cmath>

enum RenderModes
{
    Standard,
    Wireframe
};

enum LightingModes
{
    Gouraud,
    Phong
};

typedef struct
{
    bool changed = true;
    RenderModes renderMode = Wireframe;
    LightingModes lightingMode = Gouraud;

    glm::vec3 modelColor;
    float rotationPitch = 0.0f;
    float rotationYaw = 0.0f;
    float rotationRoll = 0.0f;

} Properties;

#endif

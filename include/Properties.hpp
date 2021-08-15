#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <glm/vec3.hpp>

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

} Properties;

#endif

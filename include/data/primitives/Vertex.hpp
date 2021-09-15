#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

typedef struct {
    glm::vec4 position;
    glm::vec3 normal;
    glm::vec3 color;
    int colorIndex;
    float wp; // W coordinate before prespective division
} Vertex;

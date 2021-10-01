#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

typedef struct {
    glm::vec4 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 texture_coords;
    int colorIndex;
    float wp; // W coordinate before prespective division
} Vertex;

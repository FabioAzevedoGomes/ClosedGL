#pragma once

#include <glm/vec3.hpp>

#include "Vertex.hpp"
#include "Edge.hpp"

typedef struct {
    Vertex vertices[3];
    glm::vec3 faceNormal;
    bool clipped;
} Triangle;

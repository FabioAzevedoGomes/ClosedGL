#ifndef VERTEX_H
#define VERTEX_H

#include <glm/vec3.hpp>

typedef struct {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    int colorIndex;
} Vertex;

#endif

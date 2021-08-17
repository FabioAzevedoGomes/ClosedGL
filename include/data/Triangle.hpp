#include <glm/vec3.hpp>

#include "Vertex.hpp"

typedef struct {
    Vertex vertices[3];
    glm::vec3 faceNormal;
} Triangle;

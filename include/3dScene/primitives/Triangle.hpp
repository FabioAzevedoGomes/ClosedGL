#pragma once

#include <glm/vec3.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include "Edge.hpp"

typedef struct
{
    Vertex vertices[3];
    glm::vec3 faceNormal;
    bool clipped;

    void print()
    {
        std::cout << "\t v0: " << glm::to_string(vertices[0].position) << std::endl;
        std::cout << "\t v1: " << glm::to_string(vertices[1].position) << std::endl;
        std::cout << "\t v2: " << glm::to_string(vertices[2].position) << std::endl;
        std::cout << "\t Face normal: " << glm::to_string(faceNormal) << std::endl;
        std::cout << "\t Clipped: " << clipped << std::endl;
    }
} Triangle;

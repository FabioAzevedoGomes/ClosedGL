#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtx/string_cast.hpp>

typedef struct {
    glm::vec4 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 texture_coords;
    int colorIndex;
    float wp; // W coordinate before prespective division

    void printInfo() {

        std::cout << "Vertex:" << std::endl;
        std::cout << "- Position: " << glm::to_string(position) << std::endl
                  << "- Normal: " << glm::to_string(normal) << std::endl
                  << "- Color: " << glm::to_string(color) << std::endl
                  << "- Tex: " << glm::to_string(texture_coords) << std::endl
                  << "- wp: " << wp << std::endl;
    }
} Vertex;

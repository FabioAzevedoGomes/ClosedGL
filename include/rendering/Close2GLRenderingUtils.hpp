#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Triangle.hpp"

bool isInsideNDCFrustum(glm::vec3 vertex);
std::vector<glm::vec4> projectTriangleToNDC(Triangle triangle, glm::mat4 modelViewProj);
std::vector<glm::vec4> projectVerticesToViewport(std::vector<glm::vec4> triangleVertices, glm::mat4 viewport);

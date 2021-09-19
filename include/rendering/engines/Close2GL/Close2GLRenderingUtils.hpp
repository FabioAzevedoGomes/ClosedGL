#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Triangle.hpp"

bool isInsideNDCFrustum(Triangle);
void perspectiveDivideVertex(Vertex &);
void projectTriangleToNDC(Triangle &, glm::mat4);
void projectTriangleToViewport(Triangle &, glm::mat4);

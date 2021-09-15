#pragma once

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Triangle.hpp"

class RasterizationStrategy
{
public:
    virtual void DrawTriangleToBuffer(Triangle triangle, float ***colorBuffer, float **depthBuffer){};
};
#ifndef BBOX_H
#define BBOX_H

#include <glm/vec3.hpp>
#include <limits>

typedef struct
{
    glm::vec3 top = glm::vec3(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
    glm::vec3 bottom = glm::vec3(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

    void update(glm::vec3 vertexPosition)
    {
        if (vertexPosition.x > top.x)
            top.x = vertexPosition.x;
        if (vertexPosition.x < bottom.x)
            bottom.x = vertexPosition.x;

        if (vertexPosition.y > top.y)
            top.y = vertexPosition.y;
        if (vertexPosition.y < bottom.y)
            bottom.y = vertexPosition.y;

        if (vertexPosition.z > top.z)
            top.z = vertexPosition.z;
        if (vertexPosition.z < bottom.z)
            bottom.z = vertexPosition.z;
    }

    glm::vec3 getMiddlePoint()
    {
        return glm::vec3((top.x + bottom.x) / 2.0f, (top.y + bottom.y) / 2.0f, (top.z + bottom.z) / 2.0f);
    }
} BoundingBox;

#endif

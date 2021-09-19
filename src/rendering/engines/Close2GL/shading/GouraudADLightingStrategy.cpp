#include "GouraudADLightingStrategy.hpp"

void GouraudADLightingStrategy::ShadeTriangleRelativeToCamera(Triangle &triangle, Camera &camera)
{
    glm::vec4 lightPosition = glm::inverse(camera.GetViewMatrix()) * LIGHT_SOURCE_POSITION;

    for (int vertex = 0; vertex < 3; vertex++)
    {
        glm::vec4 lightVector = glm::normalize(lightPosition - triangle.vertices[vertex].position);
        glm::vec4 normal = glm::normalize(glm::vec4(triangle.vertices[vertex].normal, 0.0f));

        glm::vec3 diffuseTerm = uniformMaterial.diffuseColor * uniformMaterial.diffuseIntensity * std::max(SHADOW_MIN_INTENSITY, glm::dot(normal, lightVector));

        glm::vec3 ambientTerm = uniformMaterial.ambientColor * uniformMaterial.ambientIntensity;

        triangle.vertices[vertex].color = diffuseTerm + ambientTerm;
        triangle.vertices[vertex].color.x = std::min(triangle.vertices[vertex].color.x, 1.0f);
        triangle.vertices[vertex].color.y = std::min(triangle.vertices[vertex].color.y, 1.0f);
        triangle.vertices[vertex].color.z = std::min(triangle.vertices[vertex].color.z, 1.0f);
    }
}

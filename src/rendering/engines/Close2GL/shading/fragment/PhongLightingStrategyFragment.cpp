#include "PhongLightingStrategyFragment.hpp"

void PhongLightingStrategyFragment::ShadeFragmentRelativeToCamera(Vertex &fragment, Camera &camera)
{   
    fragment.normal /= fragment.wp;
    fragment.color /= fragment.wp;

    glm::vec4 lightPosition = glm::inverse(camera.GetViewMatrix()) * LIGHT_SOURCE_POSITION;

    glm::vec4 lightVector = glm::normalize(lightPosition - fragment.position); // TODO: Convert fragmnet position back to 3d space
    glm::vec4 normal = glm::normalize(glm::vec4(fragment.normal, 0.0f));
    glm::vec4 cameraToVertexVector = glm::normalize(glm::vec4(camera.position, 1.0f) - fragment.position);

    glm::vec3 diffuseTerm = uniformMaterial.diffuseColor * uniformMaterial.diffuseIntensity * std::max(0.0f, glm::dot(normal, lightVector));
    glm::vec3 ambientTerm = uniformMaterial.ambientColor * uniformMaterial.ambientIntensity;
    glm::vec4 h = glm::normalize(lightVector + cameraToVertexVector);
    glm::vec4 specularTerm = glm::vec4(uniformMaterial.specularColor, 1.0f) * uniformMaterial.specularIntensity * std::pow(std::max(0.0f, glm::dot(normal, h)), uniformMaterial.shineCoefficient);

    fragment.color = diffuseTerm + ambientTerm + glm::vec3(specularTerm);
    fragment.color.x = std::min(fragment.color.x, 1.0f);
    fragment.color.y = std::min(fragment.color.y, 1.0f);
    fragment.color.z = std::min(fragment.color.z, 1.0f);
}

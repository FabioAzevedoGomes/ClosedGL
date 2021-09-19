#include <glm/vec3.hpp>

typedef struct {
    glm::vec3 ambientColor;
    float ambientIntensity;
    glm::vec3 diffuseColor;
    float diffuseIntensity;
    glm::vec3 specularColor;
    float specularIntensity;
    float shineCoefficient;
} Material;
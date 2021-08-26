#include <glm/vec3.hpp>

typedef struct {
    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    float shineCoefficient;
} Material;
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.hpp"

#define ORIGIN glm::vec3(0.0f, 0.0f, 0.0f)

class ModelViewProjectionUtils
{
public:
    static glm::mat4x4 model;
    static glm::mat4x4 view;
    static glm::mat4x4 projection;

    static glm::mat4x4 GetIdentityMatrix();
    static glm::mat4x4 GetViewMatrix(Camera camera);
    static glm::mat4x4 GetPerspectiveProjectionMatrix(Camera camera, float aspectRatio);
    static glm::mat4x4 GetOrtographicProjectionMatrix(Camera camera, float aspectRatio);

    static glm::mat4x4 CalculateModelViewProjectionMatrix(Camera camera, float aspectRatio);
};
#include "ModelViewProjectionUtils.hpp"

glm::mat4x4 ModelViewProjectionUtils::model;
glm::mat4x4 ModelViewProjectionUtils::projection;
glm::mat4x4 ModelViewProjectionUtils::view;

float dotProductBetween(glm::vec3 u, glm::vec3 v)
{
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

glm::mat4x4 ModelViewProjectionUtils::CalculateModelViewProjectionMatrix(Camera camera, float aspectRatio)
{
    ModelViewProjectionUtils::view = glm::lookAt(camera.position, camera.lookAtPoint, camera.u);
    ModelViewProjectionUtils::projection = glm::perspective(camera.fieldOfView, aspectRatio, camera.nearPlane, camera.farPlane);
    ModelViewProjectionUtils::model = ModelViewProjectionUtils::GetIdentityMatrix();

    return projection * view * model;
}

glm::mat4x4 ModelViewProjectionUtils::GetIdentityMatrix()
{
    return glm::mat4(1.0f);
}

glm::mat4x4 ModelViewProjectionUtils::GetPerspectiveProjectionMatrix(Camera camera, float aspectRatio)
{
    return glm::perspective(camera.fieldOfView, aspectRatio, camera.nearPlane, camera.farPlane);

    glm::mat4x4 P(
        camera.nearPlane, 0.0f, 0.0f, 0.0f,
        0.0f, camera.nearPlane, 0.0f, 0.0f,
        0.0f, 0.0f, camera.nearPlane + camera.farPlane, -camera.farPlane * camera.nearPlane,
        0.0f, 0.0f, 1.0f, 0.0f);

    glm::mat4x4 ortographic = ModelViewProjectionUtils::GetOrtographicProjectionMatrix(camera, aspectRatio);

    return -ortographic * P;
}

glm::mat4x4 ModelViewProjectionUtils::GetOrtographicProjectionMatrix(Camera camera, float aspectRatio)
{

    float top = fabs(camera.nearPlane) * tanf(camera.fieldOfView / 2.0f);
    float bottom = -top;
    float right = top * aspectRatio;
    float left = -right;

    return glm::mat4x4(
        2.0f / (right - left), 0.0f, 0.0f, -(right + left) / (right - left),
        0.0f, 2.0f / (top - bottom), 0.0f, -(top + bottom) / (top - bottom),
        0.0f, 0.0f, 2.0f / (camera.farPlane - camera.nearPlane), -(camera.farPlane + camera.nearPlane) / (camera.farPlane - camera.nearPlane),
        0.0f, 0.0f, 0.0f, 1.0f);
}

glm::mat4x4 ModelViewProjectionUtils::GetViewMatrix(Camera camera)
{
    glm::vec4 lookAtPoint = glm::vec4(camera.position, 1.0f) + glm::vec4(camera.n, 0.0f);

    return glm::lookAt(camera.position, glm::vec3(lookAtPoint), camera.v);
}

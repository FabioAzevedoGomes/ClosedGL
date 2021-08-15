#include "Camera.hpp"

Camera::Camera()
{
    this->ResetPosition();
}

void Camera::ResetPosition()
{
    this->u = glm::vec3(1.0f, 0.0f, 0.0f);
    this->v = glm::vec3(0.0f, 1.0f, 0.0f);
    this->n = glm::vec3(0.0f, 0.0f, -1.0f);

    this->position = ORIGIN;
    this->fieldOfView = M_PI / 3.0f;

    this->nearPlane = 0.1f;
    this->farPlane = 10000.0f;
}

void Camera::FrameObject(Model3D object)
{
    lookAtPoint = object.boundingBox.getMiddlePoint();

    float boundingBoxSideLength = fabs(object.boundingBox.top.x - object.boundingBox.bottom.x);

    float px, py, pz;
    px = lookAtPoint.x;
    py = lookAtPoint.y;
    pz = sqrt(pow(boundingBoxSideLength, 2) - pow(object.boundingBox.bottom.x, 2));

    this->position = glm::vec3(px, py, pz);
}

void Camera::RotateRoll(float alpha)
{
    glm::vec3 newU = u * (float)cos(alpha) + v * (float)sin(alpha);
    glm::vec3 newV = -u * (float)sin(alpha) + v * (float)cos(alpha);

    this->u = newU;
    this->v = newV;
}

void Camera::RotatePitch(float alpha)
{
    // Right hand coordinate system rotation
    glm::vec3 newV = v * (float)cos(alpha) + n * (float)sin(alpha);
    glm::vec3 newN = -v * (float)sin(alpha) + n * (float)cos(alpha);

    this->v = newV;
    this->n = newN;
}

void Camera::RotateYaw(float alpha)
{
    // Right hand coordinate system rotation
    glm::vec3 newU = u * (float)cos(alpha) + n * (float)sin(alpha);
    glm::vec3 newN = -u * (float)sin(alpha) + n * (float)cos(alpha);

    this->u = newU;
    this->n = newN;
}

void Camera::PrintDefinition()
{
    std::cout << "Current Camera definition: " << std::endl
              << " - U vector: (" << u.x << ", " << u.y << ", " << u.z << ")" << std::endl
              << " - V vector: (" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl
              << " - W vector: (" << n.x << ", " << n.y << ", " << n.z << ")" << std::endl
              << " - Position: (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl
              << " - LookAt: (" << lookAtPoint.x << ", " << lookAtPoint.y << ", " << lookAtPoint.z << ")" << std::endl
              << std::endl;
}

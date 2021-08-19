#include "Camera.hpp"

#define U glm::vec3(1.0f, 0.0f, 0.0f)
#define V glm::vec3(0.0f, 1.0f, 0.0f)
#define N glm::vec3(0.0f, 0.0f, -1.0f)

Camera::Camera()
{
    this->Reset();
}

void Camera::Reset()
{
    this->u = U;
    this->v = V;
    this->n = N;

    this->pitch = 0.0f;
    this->roll = 0.0f;
    this->yaw = 0.0f;

    this->position = ORIGIN;
    this->fieldOfView = M_PI / 2.0f;

    this->nearPlane = 0.1f;
    this->farPlane = 10000.0f;
}

void Camera::FrameObject(Model3D object)
{
    lookAtPoint = object.boundingBox.getMiddlePoint();

    float bboxFrontLength = fabs(object.boundingBox.top.x - object.boundingBox.bottom.x);
    float bboxFrontHeight = fabs(object.boundingBox.top.y - object.boundingBox.bottom.y);
    float bboxSideLength = fabs(object.boundingBox.top.z - object.boundingBox.bottom.z);
    float theta = fieldOfView / 2.0f;
    float distanceToFront = (std::max(bboxFrontLength, bboxFrontHeight) / 2.0f) / tan(theta);

    float px, py, pz;
    px = lookAtPoint.x;
    py = lookAtPoint.y;
    pz = lookAtPoint.z + distanceToFront + fabs(bboxSideLength / 2.0f);

    this->position = glm::vec3(px, py, pz);
}

void Camera::MoveTo(MovementOptions direction, float movementSpeed)
{
    glm::vec3 movement;
    switch (direction)
    {
    case Forward:
        movement = movementSpeed * n;
        break;
    case Backwards:
        movement = -movementSpeed * n;
        break;
    case Up:
        movement = movementSpeed * v;
        break;
    case Down:
        movement = -movementSpeed * v;
        break;
    case Right:
        movement = movementSpeed * u;
        break;
    case Left:
        movement = -movementSpeed * u;
        break;
    case NoMovement:
    default:
        movement = glm::vec3(0.0f);
        break;
    }

    this->position = this->position + movement;
}

void Camera::LookAt()
{
    glm::vec3 newN = glm::normalize(lookAtPoint - this->position);
    glm::vec3 newU = glm::normalize(glm::cross(newN, v));
    glm::vec3 newV = glm::normalize(glm::cross(newU, newN));

    this->n = newN;
    this->u = newU;
    this->v = newV;
}

void Camera::Rotate(float pitch, float roll, float yaw)
{
    this->u = U;
    this->v = V;
    this->n = N;
    UpdateAngles(pitch, roll, yaw);

    RotatePitch(pitch);
    RotateRoll(roll);
    RotateYaw(yaw);
}

void Camera::UpdateAngles(float pitch, float roll, float yaw)
{
    this->pitch = pitch;
    this->roll = roll;
    this->yaw = yaw;
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
              << " - LookAt: (" << lookAtPoint.x << ", " << lookAtPoint.y << ", " << lookAtPoint.z << ")" << std::endl;
}

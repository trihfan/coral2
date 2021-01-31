#include "CameraFreeMovement.h"
#include <algorithm>

using namespace coral;

CameraFreeMovement::CameraFreeMovement(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : 
    front(glm::vec3(0, 0, -1)), movement_speed(SPEED), mouse_sensitivity(SENSITIVITY), zoom(ZOOM)
{
    this->position = position;
    this->world_up = up;
    this->yaw = yaw;
    this->pitch = pitch;
    updateCameraVectors();
}

CameraFreeMovement::CameraFreeMovement(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch) :
    CameraFreeMovement(glm::vec3(pos_x, pos_y, pos_z), glm::vec3(up_x, up_y, up_z), yaw, pitch)
{
}

const glm::vec3& CameraFreeMovement::getPosition() const
{
    return position;
}

const glm::mat4& CameraFreeMovement::getViewProjectionMatrix()
{
    return viewProjectionMatrix;
}

void CameraFreeMovement::processKeyboard(CameraMovement direction, float deltaTime)
{
    float velocity = movement_speed * deltaTime;
    if (direction == CameraMovement::forward)
    {
        position += front * velocity;
    }
    if (direction == CameraMovement::backward)
    {
        position -= front * velocity;
    }
    if (direction == CameraMovement::left)
    
    {
        position -= right * velocity;
    }
    if (direction == CameraMovement::right)
    {
        position += right * velocity;
    }

    updateCameraVectors();
}

void CameraFreeMovement::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= mouse_sensitivity;
    yoffset *= mouse_sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        pitch = std::clamp(pitch, -89.f, 89.f);
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void CameraFreeMovement::processMouseScroll(float yoffset)
{
    zoom = std::clamp(zoom - yoffset, 1.f, 45.f);
    updateCameraVectors();
}

void CameraFreeMovement::updateCameraVectors()
{
    // calculate the new Front vector
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    // also re-calculate the Right and Up vector
    right = glm::normalize(glm::cross(front, world_up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up = glm::normalize(glm::cross(right, front));

    // compute matrix
    viewProjectionMatrix = glm::perspective(glm::radians(zoom), viewport.x / viewport.y,nearFar.x, nearFar.y) * glm::lookAt(position, position + glm::vec3(0, 0, -1), up);
}
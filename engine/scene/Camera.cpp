#include "Camera.h"
#include <algorithm>

using namespace coral;

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : 
    front(glm::vec3(0, 0, -1)), movement_speed(SPEED), mouse_sensitivity(SENSITIVITY), zoom(ZOOM)
{
    position = position;
    world_up = up;
    yaw = yaw;
    pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch) :
    Camera(glm::vec3(pos_x, pos_y, pos_z), glm::vec3(up_x, up_y, up_z), yaw, pitch)
{
}

const glm::vec3& Camera::getPosition() const
{
    return position;
}

glm::mat4 Camera::getViewProjectionMatrix()
{
    return glm::lookAt(position, position + front, up) * glm::perspective(glm::radians(zoom), 800.f / 600.f, 0.1f, 100.f);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime)
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
}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
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

void Camera::processMouseScroll(float yoffset)
{
    zoom = std::clamp(zoom - yoffset, 1.f, 45.f);
}

void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    // also re-calculate the Right and Up vector
    right = glm::normalize(glm::cross(front, world_up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up = glm::normalize(glm::cross(right, front));
}
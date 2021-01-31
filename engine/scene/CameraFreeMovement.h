#ifndef CAMERAFREEMOVEMENT_H
#define CAMERAFREEMOVEMENT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Camera.h"

namespace coral
{
    // Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
    enum class CameraMovement
    {
        forward,
        backward,
        left,
        right
    };

    // Default camera values
    constexpr float YAW = -90;
    constexpr float PITCH = 0;
    constexpr float SPEED = 2.5f;
    constexpr float SENSITIVITY = 0.1f;
    constexpr float ZOOM = 45;

    // An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
    class CameraFreeMovement : public Camera
    {
    public:
        // constructor with vectors
        CameraFreeMovement(glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 up = glm::vec3(0, 1, 0), float yaw = YAW, float pitch = PITCH);

        // constructor with scalar values
        CameraFreeMovement(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch);

        const glm::vec3& getPosition() const;
        
        // returns the view matrix calculated using Euler Angles and the LookAt Matrix
        const glm::mat4& getViewProjectionMatrix();

        // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
        void processKeyboard(CameraMovement direction, float delta_time);

        // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
        void processMouseMovement(float x_offset, float y_offset, GLboolean constrain_pitch = true);

        // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
        void processMouseScroll(float y_offset);

    private:
        // camera Attributes
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 world_up;
        glm::vec2 viewport = glm::vec2(800, 600);
        glm::vec2 nearFar = glm::vec2(0.1f, 100);
        
        // euler Angles
        float yaw;
        float pitch;

        // camera options
        float movement_speed;
        float mouse_sensitivity;
        float zoom;

        // matrix
        glm::mat4 viewProjectionMatrix;

        // calculates the front vector from the Camera's (updated) Euler Angles
        void updateCameraVectors();
    };
}
#endif
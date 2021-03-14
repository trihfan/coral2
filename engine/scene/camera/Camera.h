#pragma once

#include "scene/Node.h"
#include <glm/glm.hpp>
#include <vector>

namespace coral
{
    class Camera : public Node
    {
    public:
        // Construction
        Camera();
        virtual ~Camera() override = default;

        // Set the projection
        void setPerspective(float fovy, const glm::vec4& viewport, const glm::vec2& zNearFar);
        void setOrthogonal(const glm::vec4& viewport, const glm::vec2& zNearFar);

        // Set the view
        void setView(const glm::vec3& center, const glm::vec3& up);

        // Get the viewport
        const glm::vec4& getViewport() const;

        // Get view center
        const glm::vec3& getViewCenter() const;

        // Get up
        const glm::vec3& getUp() const;

        // Get the projection matrix
        const glm::mat4& getProjectionMatrix() const;

        // Get the view matrix
        const glm::mat4& getViewMatrix() const;

        // Background color
        const glm::vec4& getBackgroundColor();
        void setBackgroundColor(const glm::vec4& color);

    private:
        // View projection matrix
        glm::vec4 viewport;
        glm::vec3 center;
        glm::vec3 up;
        glm::mat4 projection;
        glm::mat4 view;

        // Background color
        glm::vec4 backgroundColor;

        // Update the view projection matrix
        void updateMatrix();
    };
}

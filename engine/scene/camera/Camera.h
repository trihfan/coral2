#ifndef CAMERA_H
#define CAMERA_H

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
        virtual ~Camera() = default;

        // Signals
        Signal<const glm::mat4&> viewProjectionChanged;

        // Set the projection
        void setPerspective(float fovy, const glm::vec4& viewport, const glm::vec2& zNearFar);
        void setOrthogonal(const glm::vec4& viewport, const glm::vec2& zNearFar);

        // Set the view
        void setView(const glm::vec3& center, const glm::vec3& up);

        // Get the viewport
        const glm::vec4& getViewport() const;

        // Get view center
        const glm::vec3& getViewCenter() const;

        // Get the projection view matrix
        const glm::mat4& getViewProjectionMatrix() const;

    private:
        // View projection matrix
        glm::vec4 viewport;
        glm::vec3 center;
        glm::vec3 up;
        glm::mat4 projection;
        glm::mat4 viewProjection;

        // Update the view projection matrix
        void updateMatrix();
    };
}
#endif
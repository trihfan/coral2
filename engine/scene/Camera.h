#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <vector>
#include "Node.h"

namespace coral
{
    class Camera : public Node
    {
    public:
        // Construction
        Camera() = default;
        virtual ~Camera() = default;

        // Get the projection view matrix
        const glm::mat4& getViewProjectionMatrix() const;

    protected:
        // Front vector
        void setFront(const glm::vec3& front);
        const glm::vec3& getFront() const;

        // Up vector
        void setUp(const glm::vec3& up);
        const glm::vec3& getUp() const;

        // Projection matrix
        void setProjectionMatrix(const glm::mat4& projection);

    private:
        // View projection matrix
        glm::mat4 projection;
        glm::mat4 viewProjection;

        // View
        glm::vec3 front;
        glm::vec3 up;
    };
}
#endif
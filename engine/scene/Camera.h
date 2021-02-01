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
        Camera();
        virtual ~Camera() = default;

        // Properties
        Property<glm::mat4> projection;
        Property<glm::mat4> view;
        
        // Signals
        Signal<const glm::mat4&> viewProjectionChanged;

        // Get the projection view matrix
        const glm::mat4& getViewProjectionMatrix() const;

    private:
        // View projection matrix
        glm::mat4 viewProjection;

        // Update the view projection matrix
        void updateMatrix();
    };
}
#endif
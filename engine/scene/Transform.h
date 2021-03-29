#pragma once

#include "Property.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace coral
{
    /**
     * Transform contains tranformation information for a node
     */
    class Transform
    {
    public:
        // construction
        Transform();
        virtual ~Transform() = default;

        // Directly set the matrix
        // Modifying the translation / rotation or scale will override this call
        void setMatrix(const glm::mat4& matrix);

        // Rotation
        void setRotationFromEulerAngles(const glm::vec3& eulerAngles);

        // World transform values
        const glm::vec3& getPosition() const;
        const glm::mat4& getMatrix() const;

        // Update the tranform
        void setDirty();
        void update(const Transform* parent);

        // Properties
        Property<glm::vec3> translation;
        Property<glm::quat> rotation;
        Property<glm::vec3> scale;

        // Signals
        Signal<const glm::mat4&> matrixChanged;

    private:
        // Computed transform
        glm::vec3 worldPosition;
        glm::mat4 matrix;
        glm::mat4 fixedMatrix;
        bool useFixedMatrix;
        bool dirty;

        void setDirtyAndNotFixedMatrix();
    };
}

#pragma once
#include <assimp/material.h>
#include <glm/glm.hpp>

namespace coral
{
    class AssimpHelpers
    {
    public:
        static glm::mat4 convertMatrixToGLMFormat(const aiMatrix4x4& from);
        static glm::vec3 getGLMVec(const aiVector3D& vec);
        static glm::quat getGLMQuat(const aiQuaternion& pOrientation);
    };

}

#pragma once

#include "scene/Node.h"

namespace coral
{
    class Bone : public Node
    {
    public:
        Bone(int id, const glm::mat4& offset = glm::mat4(1));
        int getId();
        const glm::mat4& getOffset() const;

    private:
        int id;
        glm::mat4 offset;
    };
}

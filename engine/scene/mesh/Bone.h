#pragma once
#include "scene/Node.h"

namespace coral
{
    class Bone : public Node
    {
    public:
        Bone(int id, const glm::mat4& offset = glm::mat4(1));
        int getId();
        void setAnimated(bool animated);
        glm::mat4 getBoneMatrix() const;

    private:
        int id;
        bool animated;
        glm::mat4 offset;
    };
}

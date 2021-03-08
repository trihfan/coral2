#pragma once

#include "scene/Node.h"
#include <glm/glm.hpp>

namespace coral
{
    class Light : public Node
    {
    public:
        glm::vec3 color;

    private:
    };
}

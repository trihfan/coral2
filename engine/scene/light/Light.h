#ifndef LIGHT_H
#define LIGHT_H

#include "Property.h"
#include "scene/Node.h"
#include <glm/glm.hpp>

namespace coral
{
    class Light : public Node
    {
    public:
        Property<glm::vec3> color;

    private:
    };
}
#endif

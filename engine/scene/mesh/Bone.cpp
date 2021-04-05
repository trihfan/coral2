#include "Bone.h"
#include "utils/AssimpHelpers.h"
#include <assimp/anim.h>

using namespace coral;

static const glm::mat4 identity(1);

Bone::Bone(int id, const glm::mat4& offset)
    : id(id)
    , animated(false)
    , offset(offset)
{
}

void Bone::setAnimated(bool animated)
{
    this->animated = animated;
}

int Bone::getId()
{
    return id;
}

glm::mat4 Bone::getBoneMatrix() const
{
    if (animated)
    {
        return transform().getMatrix() * offset;
    }
    return transform().getMatrix();
}

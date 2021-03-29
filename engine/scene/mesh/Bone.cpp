#include "Bone.h"
#include "utils/AssimpHelpers.h"
#include <assimp/anim.h>

using namespace coral;

static const glm::mat4 identity(1);

Bone::Bone(int id, const glm::mat4& offset)
    : id(id)
    , offset(offset)
{
}

int Bone::getId()
{
    return id;
}

glm::mat4 Bone::getBoneMatrix() const
{
    return transform().getMatrix() * offset;
}

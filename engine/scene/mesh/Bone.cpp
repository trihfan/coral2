#include "Bone.h"
#include "utils/AssimpHelpers.h"
#include <assimp/anim.h>

using namespace coral;

Bone::Bone(int id, const glm::mat4& offset)
    : id(id)
    , offset(offset)
{
}

int Bone::getId()
{
    return id;
}

const glm::mat4& Bone::getOffset() const
{
    return offset;
}

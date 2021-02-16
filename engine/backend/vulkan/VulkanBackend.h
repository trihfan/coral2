#ifndef VULKANBACKEND_H
#define VULKANBACKEND_H

#include "backend/interfaces/Backend.h"

namespace coral
{
    class VulkanBackend : public Backend
    {
    public:
        bool init() override;
        bool destroy() override;
    };
}

#endif
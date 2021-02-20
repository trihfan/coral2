#pragma once

#include "Object.h"
#include "utils/Singleton.h"

namespace coral
{
    class Pipeline : public Object
    {
    public:
        void setRenderPass(const std::string& renderpass);
        void setShader();
    };

    class PipelineManager
    {
        MAKE_SINGLETON(PipelineManager)
    public:
    };
}

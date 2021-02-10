#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Object.h"
#include "utils/Singleton.h"
#include "glad/glad.h"

namespace coral
{
    enum ResourceType
    {
        texture
    };
    struct Resource
    {
        ResourceType type;
        GLenum internalType;
    };

    //
    class Framebuffer : public Object
    {
    public:
        void bind();

    private:
        //GLuint framebufferId;
    };

    // The framebuffer manager
    class FramebufferManager
    {
        MAKE_ENGINE_SINGLETON(FramebufferManager)
    public:
        static std::shared_ptr<Framebuffer> getFramebuffer(const std::string& name);
        static std::shared_ptr<Framebuffer> getFramebufferFor(const std::vector<std::string>& colorOutputs);
    };
}
#endif

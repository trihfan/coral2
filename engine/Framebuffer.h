#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Object.h"
#include "utils/Singleton.h"

namespace coral
{
	//
	class Framebuffer : public Object
	{
	public:
		void bind();

    private:
        GLuint framebufferId;
	};

	// The framebuffer manager
    class FramebufferManager
    {
        MAKE_ENGINE_SINGLETON(FramebufferManager)
    public:
        std::shared_ptr<Framebuffer> getFramebufferFor(const std::vector<std::string>& colorOutputs);
    };
}
#endif

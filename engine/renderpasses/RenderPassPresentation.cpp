#include "RenderPassPresentation.h"
#include "Engine.h"

using namespace coral;

void RenderPassPresentation::internalRender(RenderQueue& queue)
{
   glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
   glBindFramebuffer(GL_READ_FRAMEBUFFER, 1);
   glBlitFramebuffer(0, 0, Engine::current_parameters.width, Engine::current_parameters.height, 
                     0, 0, Engine::current_parameters.width, Engine::current_parameters.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}
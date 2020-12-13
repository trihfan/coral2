#include "Engine.h"
#include "Shader.h"

using namespace coral;

int main()
{
    ShaderManager::addShaderPath("resources/shaders");
    Engine::create();

    Engine::destroy();
}

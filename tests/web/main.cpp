#include "Engine.h"
#include "NvEncoder/NvEncoderGL.h"
#include "Object.h"
#include "ObjectFactory.h"
#include "OpenglBackend.h"
#include "materials/BasicMaterial.h"
#include "resources/Shader.h"
#include "scene/Scene.h"
#include "scene/SceneManager.h"
#include "scene/camera/Camera.h"
#include "scene/mesh/Mesh.h"
#include <chrono>
#include <fstream>
#include <GL/glew.h>
#include <numeric>
#include <thread>
#include <vector>

#include <GLFW/glfw3.h>

using namespace coral;

// screen
int width = 800;
int height = 600;
GLFWwindow* window;

// encoder
NvEncoderGL* encoder;

// functions
bool createWindow();
bool setupEngine();
bool initializeEncoder();

int main(int argc, char** argv)
{
    // Create window
    if (!createWindow())
    {
        return 1;
    }

    // engine
    if (!setupEngine())
    {
        return 1;
    }

    // encoder
    if (!initializeEncoder())
    {
        return 1;
    }

    GLuint framebuffer;
    glCreateFramebuffers(1, &framebuffer);

    // Main loop
    std::ofstream output("output.h264");
    while (!glfwWindowShouldClose(window))
    {
        // Start
        auto start = std::chrono::steady_clock::now();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }

        // Render frame
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        Engine::frame();

        // Get next buffer
        const NvEncInputFrame* encoderInputFrame = encoder->GetNextInputFrame();
        NV_ENC_INPUT_RESOURCE_OPENGL_TEX* resource = static_cast<NV_ENC_INPUT_RESOURCE_OPENGL_TEX*>(encoderInputFrame->inputPtr);

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glBindTexture(resource->target, resource->texture);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, resource->target, resource->texture, 0);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        
        // encode frame
        std::vector<std::vector<uint8_t>> packet;
        encoder->EncodeFrame(packet);

        // send frame
        for (std::vector<uint8_t>& packet : packet)
        {
            output.write(reinterpret_cast<char*>(packet.data()), packet.size());
        }

        // Swap
        glfwSwapBuffers(window);
        glfwPollEvents();

        // limit to 60 fps
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count();
        auto remaining = 1000 * 16 - elapsed;
        std::this_thread::sleep_for(std::chrono::microseconds(remaining > 0 ? remaining : 0));
    }

    delete encoder;
    glfwTerminate();
    Engine::destroy();
    return 0;
}

bool createWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "viewer", NULL, NULL);
    if (window == NULL)
    {
        Logs(error) << "Failed to create GLFW window";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    return true;
}

bool initializeEncoder()
{
    encoder = new NvEncoderGL(width, height, NV_ENC_BUFFER_FORMAT_ARGB);

    NV_ENC_INITIALIZE_PARAMS initializeParams = { NV_ENC_INITIALIZE_PARAMS_VER };
    NV_ENC_CONFIG encodeConfig = { NV_ENC_CONFIG_VER };
    initializeParams.encodeConfig = &encodeConfig;

    encoder->CreateDefaultEncoderParams(&initializeParams, NV_ENC_CODEC_H264_GUID, NV_ENC_PRESET_P7_GUID, NV_ENC_TUNING_INFO_ULTRA_LOW_LATENCY);
    encoder->CreateEncoder(&initializeParams);

    return true;
}

bool setupEngine()
{
    ShaderManager::addShaderPath("resources/shaders");
    Engine::create(std::make_unique<OpenglBackend>());
    Engine::resize(width, height);

    // scene
    static auto scene = ObjectFactory::createWithName<Scene>("scene");
    SceneManager::setCurrentScene(scene);

    // camera
    static auto camera = ObjectFactory::createWithName<Camera>("camera");
    camera->setPerspective(45.f, glm::vec4(0, 0, width, height), glm::vec2(0.1f, 100.f));
    camera->setView(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    camera->position = glm::vec3(0, 0, 3);
    scene->add(camera);

    // material
    static auto material = ObjectFactory::createWithName<BasicMaterial>("material");
    material->color = glm::vec3(0.1, 0.8, 0.3);
    material->shininess = 128;

    // vertices
    std::vector<Vertex> vertices {
        Vertex { glm::vec3(-0.5f, -0.5f, 0.f), glm::vec3(0.0f, 0.0f, 1.0f) },
        Vertex { glm::vec3(0.5f, -0.5f, 0.f), glm::vec3(0.0f, 0.0f, 1.0f) },
        Vertex { glm::vec3(-0.5f, 0.5f, 0.f), glm::vec3(0.0f, 0.0f, 1.0f) },
        Vertex { glm::vec3(-0.5f, 0.5f, 0.f), glm::vec3(0.0f, 0.0f, 1.0f) },
        Vertex { glm::vec3(0.5f, -0.5f, 0.f), glm::vec3(0.0f, 0.0f, 1.0f) },
        Vertex { glm::vec3(0.5f, 0.5f, 0.f), glm::vec3(0.0f, 0.0f, 1.0f) },
    };

    std::vector<unsigned int> indices(vertices.size());
    std::iota(indices.begin(), indices.end(), 0);

    // mesh
    static auto mesh = ObjectFactory::createWithName<Mesh>("mesh", vertices, indices);
    mesh->setMaterial(material);
    scene->add(mesh);

    // Lights
    static auto light1 = ObjectFactory::create<PointLight>();
    light1->position = glm::vec3(0, -0.5, 3);
    light1->color = glm::vec3(1, 1, 1);
    light1->constant = 1;
    light1->linear = 0.09f;
    light1->quadratic = 0.032f;
    scene->add(light1);

    static auto light2 = ObjectFactory::create<PointLight>();
    light2->position = glm::vec3(0, 0.5, 3);
    light2->color = glm::vec3(1, 1, 1);
    light2->constant = 1;
    light2->linear = 0.09f;
    light2->quadratic = 0.032f;
    scene->add(light2);
    return true;
}

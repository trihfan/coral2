#include "AssetManager.h"
#include "Engine.h"
#include "FileUtils.h"
#include "base/Object.h"
#include "base/ObjectFactory.h"
#include "materials/MeshMaterial.h"
#include "resources/ShaderComposer.h"
#include "scene/Scene.h"
#include "scene/SceneManager.h"
#include "scene/animation/Animator.h"
#include "scene/camera/OrbitCamera.h"
#include "scene/light/PointLight.h"
#include "scene/mesh/Mesh.h"
#include "scene/mesh/Model.h"
#include "scene/text/Text.h"
#include <numeric>

// Opengl specifics
#ifdef ENABLE_OPENGL
#include "OpenglBackend.h"
#endif

// VUlkan specifics
#ifdef ENABLE_VULKAN
#include "VulkanBackend.h"
#endif

// Emscripten specifics
#ifdef __EMSCRIPTEN__
#define GLFW_INCLUDE_GLEXT
#include <emscripten.h>
#endif

#include <GLFW/glfw3.h>

using namespace coral;

// variables
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;
static double lastX = SCR_WIDTH / 2.;
static double lastY = SCR_HEIGHT / 2.;
static bool firstMouse = true;
static bool mousePressed = false;
static GLFWwindow* window;

// Scene
static const std::string assetsDirectory = FileUtils::getAppDirectory() + "/assets";
static ptr<OrbitCamera> camera;
static ptr<Text> text;
void setupScene();

// Animation
static std::vector<ptr<Animator>> animators;

// callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button(GLFWwindow* window, int button, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

enum BackendType
{
    opengl,
    vulkan
};

void mainloop()
{
    // input
    processInput(window);

    // render
    try
    {
        Engine::frame();
    }
    catch (...)
    {
        Logs(error) << "Unknown exception";
    }

    // Finish
    glfwSwapBuffers(window);
    glfwPollEvents();
}

int main(int argc, char* argv[])
{
    BackendType type = opengl;
    //if (argc > 1 && std::string(argv[1]) == "-vulkan")
    {
        type = vulkan;
    }

    // glfw: initialize and configure
    // ------------------------------0
    glfwInit();
    if (type == opengl)
    {
#ifdef EMSCRIPTEN
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#else
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    }
    else if (type == vulkan)
    {
        if (!glfwVulkanSupported())
        {
            std::cout << "Vulkan not supported" << std::endl;
            return -1;
        }
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "viewer", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    std::unique_ptr<backend::Backend> backend;
#ifdef ENABLE_OPENGL
    if (type == opengl)
    {
        backend = std::make_unique<backend::opengl::OpenglBackend>();
    }
#endif
#ifdef ENABLE_VULKAN
    if (type == vulkan)
    {
        backend = std::make_unique<backend::vulkan::VulkanBackend>(window);
    }
#endif

    // setup engine
    Engine::create(std::move(backend), SCR_WIDTH, SCR_HEIGHT);
    setupScene();

    // main loop
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainloop, 0, true);
#else
    while (!glfwWindowShouldClose(window))
    {
        mainloop();
    }
#endif

    glfwTerminate();
    Engine::destroy();
    return 0;
}

void setupScene()
{
    AssetManager::addDirectory(assetsDirectory);

    // scene
    auto scene = ObjectFactory::createWithName<Scene>("scene");
    SceneManager::setCurrentScene(scene);

    // camera
    camera = ObjectFactory::createWithName<OrbitCamera>("camera");
    camera->setBackgroundColor(glm::vec4(1, 1, 1, 1));
    camera->setPerspective(45, glm::vec4(0, 0, SCR_WIDTH, SCR_HEIGHT), glm::vec2(0.1f, 100));
    camera->setView(glm::vec3(0, 0.5, 0), glm::vec3(0, 1, 0));
    camera->setTranslation(glm::vec3(0, 0.5, 4));
    camera->setDistanceMinMax(0.3f, 100);
    scene->add(camera);

    // Models
    /*auto zombie = ObjectFactory::create<Model>(AssetManager::get(assetsDirectory, "models/Zombie.fbx").url);
    zombie->setScale(glm::vec3(0.01, 0.01, 0.01));
    zombie->setTranslation(glm::vec3(-0.5, 0, 0));
    scene->add(zombie);

    auto animator = ObjectFactory::create<Animator>();
    animator->addAnimation({ zombie->getAnimation("Armature|ThrillerPart1"), 0. });
    animator->setLoopAnimation(true);
    scene->add(animator);

    auto zombie2 = ObjectFactory::create<Model>(AssetManager::get(assetsDirectory, "models/Zombie2.fbx").url);
    zombie2->setScale(glm::vec3(0.01, 0.01, 0.01));
    zombie2->setTranslation(glm::vec3(0.5, 0, 0));
    scene->add(zombie2);

    auto animator2 = ObjectFactory::create<Animator>();
    animator2->addAnimation({ zombie2->getAnimation("mixamo.com"), 0. });
    animator2->setLoopAnimation(true);
    scene->add(animator2);

    // Lights
    auto createLight = [scene](const glm::vec3& position) {
        auto light = ObjectFactory::create<PointLight>();
        light->setTranslation(position);
        light->color = glm::vec3(1, 1, 1);
        light->constant = 1;
        light->linear = 0.09f;
        light->quadratic = 0.032f;
        scene->add(light);
    };
    createLight(glm::vec3(5, 1, 5));
    createLight(glm::vec3(5, 1, -5));
    createLight(glm::vec3(-5, 1, -5));
    createLight(glm::vec3(-5, 1, 5));

    // Text
    TextFormat format;
    format.font = "assets/fonts/SukiYaki.otf";
    format.size = 50;
    text = ObjectFactory::create<Text>(format);
    text->setColor(glm::vec3(0.1, 0.1, 0.1));
    text->setTranslation(glm::vec3(15, 15, -1));
    text->setText("coral 0.1");
    scene->add(text);*/
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    camera->setPerspective(45, glm::vec4(0, 0, width, height), glm::vec2(0.1f, 100));
    Engine::resize(width, height);
}

void mouse_button(GLFWwindow*, int button, int action, int)
{
    if (button == 0)
    {
        mousePressed = action == GLFW_PRESS ? true : false;
        firstMouse = true;
    }
}

void mouse_callback(GLFWwindow*, double xpos, double ypos)
{
    if (!mousePressed)
    {
        return;
    }

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    // update camera.
    camera->move(static_cast<float>(xoffset) * 0.1f, static_cast<float>(yoffset) * 0.1f);
}

void scroll_callback(GLFWwindow*, double, double yoffset)
{
    camera->zoom(static_cast<float>(yoffset) * 0.3f);
}

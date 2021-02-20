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
#include <iostream>

#include <numeric>
#include <thread>
#include <vector>

#include <GLFW/glfw3.h>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>

using namespace coral;

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

// screen
int width = 100;
int height = 100;
GLFWwindow* window;

// encoder
NvEncoderGL* encoder;

// functions
void do_session(tcp::socket socket);
bool createWindow();
bool setupEngine();
bool initializeEncoder();

int main(int argc, char** argv)
{
    try
    {
        auto const address = net::ip::address_v4::from_string("127.0.0.1");
        auto const port = static_cast<unsigned short>(9090);

        // The io_context is required for all I/O
        net::io_context ioc{1};

        // The acceptor receives incoming connections
        tcp::acceptor acceptor{ioc, {address, port}};
        for(;;)
        {
            // This will receive the new connection
            tcp::socket socket{ioc};

            // Block until we get a connection
            acceptor.accept(socket);

            // Launch the session, transferring ownership of the socket
            std::thread(&do_session, std::move(socket)).detach();
        }
    }
    catch (const std::exception& e)
    {
        Logs(error) << "Error: " << e.what();
        return 1;
    }

    return 0;
}

void do_session(tcp::socket socket)
{
    Logs(success) << "connection";

    try
    {
        // Construct the stream by moving in the socket
        websocket::stream<tcp::socket> ws{std::move(socket)};

        // Set a decorator to change the Server of the handshake
        ws.set_option(websocket::stream_base::decorator([](websocket::response_type& res){
                res.set(http::field::server, std::string(BOOST_BEAST_VERSION_STRING) + " websocket-server-sync");
        }));

        // Accept the websocket handshake
        ws.accept();

        // Create window
        if (!createWindow())
        {
            return;
        }

        // engine
        if (!setupEngine())
        {
            return;
        }

        // encoder
        if (!initializeEncoder())
        {
            return;
        }

        GLuint framebuffer;
        glCreateFramebuffers(1, &framebuffer);

        // Main loop=
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
            std::vector<std::vector<uint8_t>> packet1;
            std::vector<std::vector<uint8_t>> packet2;
            encoder->EncodeFrame(packet1);
            encoder->EndEncode(packet2);

            // send frame
            size_t size1 = 0;
            for (const auto& p : packet1)
            {
                size1 += p.size();
            }
            size_t size2 = 0;
            for (const auto& p : packet2)
            {
                size2 += p.size();
            }

            std::string data;
            data.resize(size1 + size2 + 4);
            uint8_t duration = 10;
            uint8_t audioLength = 0;

            std::memcpy(data.data(), &duration, 2);
            std::memcpy(data.data(), &audioLength, 2);

            std::memcpy(data.data() + 4, packet1.data(), size1);
            std::memcpy(data.data() + 4 + size1, packet2.data(), size2);

            Logs(info) << ws.write(net::buffer(data));

            // Swap
            //glfwSwapBuffers(window);
            //glfwPollEvents();

            // limit to 60 fps
            auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count();
            auto remaining = 1000 * 16 - elapsed;
            std::this_thread::sleep_for(std::chrono::microseconds(remaining > 0 ? remaining : 0));
        }

        delete encoder;
        glfwTerminate();
        Engine::destroy();
    }
    catch(beast::system_error const& se)
    {
        // This indicates that the session was closed
        if(se.code() != websocket::error::closed)
        {
            Logs(error) << "Error: " << se.code().message() << std::endl;
        }
    }
    catch(std::exception const& e)
    {
        Logs(error) << "Error: " << e.what() << std::endl;
    }
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

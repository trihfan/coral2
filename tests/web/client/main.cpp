#include "NvEncoder/NvEncoderGL.h"
#include "Logs.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <thread>
#include <fstream>

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
bool initializeEncoder();

int main(int argc, char** argv)
{
    try
    {
        // The io_context is required for all I/O
        net::io_context ioc{1};

        tcp::resolver resolver{ioc};
        websocket::stream<tcp::socket> ws{ioc};

        // Look up the domain name
        auto const results = resolver.resolve("127.0.0.1", "9090");

        // Make the connection on the IP address we get from a lookup
        auto ep = net::connect(ws.next_layer(), results);

        // Update the host_ string. This will provide the value of the
        // Host HTTP header during the WebSocket handshake.
        // See https://tools.ietf.org/html/rfc7230#section-5.4
        std::string host = std::string("127.0.0.1:") + std::to_string(ep.port());

        // Set a decorator to change the User-Agent of the handshake
        ws.set_option(websocket::stream_base::decorator(
            [](websocket::request_type& req)
            {
                req.set(http::field::user_agent,
                    std::string(BOOST_BEAST_VERSION_STRING) +
                        " websocket-client-coro");
            }));

        // Perform the websocket handshake
        ws.handshake(host, "/");
        ws.binary(true);

        std::ofstream output("output.h264", std::ios_base::binary);
        if (!output.is_open())
        {
            exit(1);
        }

        while(true)
        {
            // This buffer will hold the incoming message
            beast::flat_buffer buffer;

            // Read a message into our buffer
            ws.read(buffer);

            output.write((char*)buffer.data().data(), buffer.size());
            Logs(info) << buffer.size();
        }

        ws.close(websocket::close_code::normal);
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

        // decode
        /*if (!initializeDecoder())
        {
            return;
        }*/

        GLuint framebuffer;
        glCreateFramebuffers(1, &framebuffer);

        // Main loop
        while (!glfwWindowShouldClose(window))
        {
            // Start
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(window, true);
            }

            // Swap
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        delete encoder;
        glfwTerminate();
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
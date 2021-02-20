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
int width = 600;
int height = 600;
GLFWwindow* window;

// functions
bool createWindow();

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
        std::string host = std::string("127.0.0.1:8080");

        // Set a decorator to change the User-Agent of the handshake
        ws.set_option(websocket::stream_base::decorator([](websocket::request_type& req) {
            req.set(http::field::user_agent,
                std::string(BOOST_BEAST_VERSION_STRING) + " websocket-client-coro");
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

project(web)

# directories
set(server_path ${web_path}/server)
set(client_path ${web_path}/client)

# server side render on linux
if(NOT APPLE AND UNIX)
    add_subdirectory(server)
endif()

# emscripten client
#if(EMSCRIPTEN)
    add_subdirectory(client)
#else()

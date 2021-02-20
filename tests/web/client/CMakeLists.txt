project (client)

# opengl
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(glfw3 CONFIG REQUIRED)

# Boost
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# make lib
link_directories(client ${lib_output_directory})
file(GLOB_RECURSE sources *.h *.cpp *.c)
add_executable(client ${sources})

# dependencies
target_link_libraries(client PRIVATE engine glfw opengl_backend GLEW::GLEW)

# set output directories
set_target_properties(client PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(client PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
set_target_properties(client PROPERTIES FOLDER "coral/web")

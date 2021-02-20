project (server)

# opengl
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(glfw3 CONFIG REQUIRED)

# Boost
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# nvidia encode
find_library(NVENCODEAPI_LIB nvidia-encode)

# make lib
link_directories(server ${lib_output_directory})
file(GLOB_RECURSE sources *.h *.cpp *.c)
add_executable(server ${sources})

# dependencies
target_link_libraries(server PRIVATE engine glfw opengl_backend ${NVENCODEAPI_LIB} GLEW::GLEW)

# set output directories
set_target_properties(server PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(server PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
set_target_properties(server PROPERTIES FOLDER "coral/web")

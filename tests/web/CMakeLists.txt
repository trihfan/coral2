project (web)

# opengl
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(glfw3 CONFIG REQUIRED)

# Websocketpp
find_package(websocketpp CONFIG REQUIRED)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# nvidia encode
find_library(NVENCODEAPI_LIB nvidia-encode)

# make lib
link_directories(web ${lib_output_directory})
file(GLOB_RECURSE sources *.h *.cpp *.c)
set_folder_structure("${sources}" ${web_path})
add_executable(web ${sources})

# dependencies
target_link_libraries(web PRIVATE engine glfw opengl_backend ${NVENCODEAPI_LIB} websocketpp::websocketpp GLEW::GLEW)

# set output directories
set_target_properties(web PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(web PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(web PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
set_target_properties(web PROPERTIES FOLDER "coral/web")

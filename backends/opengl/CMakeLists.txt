project(backend_opengl)
set_property(GLOBAL PROPERTY backend_opengl_path ${CMAKE_CURRENT_SOURCE_DIR})

# Dependencies
get_property(utils_path GLOBAL PROPERTY utils_path)
get_property(backend_interfaces_path GLOBAL PROPERTY backend_interfaces_path)
find_package(OpenGL REQUIRED)

# Opengl version (gl, gles)
set(backend_opengl_version "gl")
if(EMSCRIPTEN)
    set(backend_opengl_version "gles")
endif()

if (${backend_opengl_version} STREQUAL "gl")
    add_definitions(-DOPENGL_CORE)
elseif (${backend_opengl_version} STREQUAL "gles")
    add_definitions(-DOPENGL_ES)
endif()

option(BACKEND_OPENGL_DEBUG "enable Opengl debug" OFF)
if (BACKEND_OPENGL_DEBUG)
        add_definitions(-DENABLE_OPENGL_DEBUG)
endif()

# Sources
file(GLOB_RECURSE sources Opengl*.h Opengl*.cpp)
if(backend_opengl_version STREQUAL "gl")
    set(sources "${sources};${CMAKE_CURRENT_SOURCE_DIR}/glad/glad.c;${CMAKE_CURRENT_SOURCE_DIR}/glad/glad/glad.h")
endif()

# Make lib
add_library(backend_opengl STATIC ${sources})
set_folder_structure("${sources}" ${CMAKE_CURRENT_SOURCE_DIR})

# Includes
target_include_directories(backend_opengl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" "${utils_path}" "${backend_interfaces_path}")
if(backend_opengl_version STREQUAL "gl")
    target_include_directories(backend_opengl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/glad")
endif()

# Libraries
target_link_libraries(backend_opengl PUBLIC utils glm::glm)
if(NOT EMSCRIPTEN)
    target_link_libraries(backend_opengl PUBLIC OpenGL::GL)
endif()

# Set output directories
set_target_properties(backend_opengl PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(backend_opengl PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(backend_opengl PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
set_target_properties(backend_opengl PROPERTIES FOLDER "backends")

project(opengl_backend)

# Opengl
find_package(OpenGL REQUIRED)
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
    set(sources "${sources};${backend_opengl_path}/glad/glad.c" "${sources};${backend_opengl_path}/glad/glad/glad.h")
endif()

# Make lib
add_library(opengl_backend STATIC ${sources})
set_folder_structure("${sources}" ${backend_opengl_path})

# Includes
target_include_directories(opengl_backend PUBLIC "${backend_opengl_path}" "${utils_path}" "${backend_interfaces_path}" "${glm_path}")

if(backend_opengl_version STREQUAL "gl")
    target_include_directories(opengl_backend PUBLIC "${backend_opengl_path}/glad")
endif()

# Libraries
target_link_libraries(opengl_backend PUBLIC utils)

if(NOT EMSCRIPTEN)
    target_link_libraries(opengl_backend PUBLIC OpenGL::GL)
endif()

# glm
target_include_directories(opengl_backend PUBLIC "${glm_path}")
add_dependencies(opengl_backend glm)

# Set output directories
set_target_properties(opengl_backend PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(opengl_backend PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(opengl_backend PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
set_target_properties(opengl_backend PROPERTIES FOLDER "backends")

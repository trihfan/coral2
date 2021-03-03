project(opengl_backend)

# Opengl version
set(BACKEND_OPENGL_VERSION "gl" CACHE STRING "Backend OpenGL version")
set_property(CACHE BACKEND_OPENGL_VERSION PROPERTY STRINGS gl gles)

# Sources
file(GLOB_RECURSE sources src/*.h src/*.cpp)
if(NOT EMSCRIPTEN)
    list(APPEND ${sources} "${BACKEND_OPENGL_VERSION}/src/glad.c")
endif()

# Make lib
add_library(opengl_backend STATIC ${sources})
target_include_directories(opengl_backend PUBLIC "${backend_opengl_path}/src")
target_include_directories(opengl_backend PUBLIC "${utils_path}")
target_link_libraries(opengl_backend PUBLIC utils)
set_folder_structure("${sources}" ${backend_opengl_path})

if(NOT EMSCRIPTEN)
    target_include_directories(opengl_backend PUBLIC "${backend_opengl_path}/${BACKEND_OPENGL_VERSION}/include")
endif()

# Interfaces
target_include_directories(opengl_backend PUBLIC "${backend_interfaces_path}")

# OpenGL
find_package(OpenGL REQUIRED)
if(NOT EMSCRIPTEN)
    target_link_libraries(opengl_backend PUBLIC OpenGL::GL)
endif()
option(BACKEND_OPENGL_DEBUG "enable Opengl debug" OFF)
if (BACKEND_OPENGL_DEBUG)
        add_definitions(-DENABLE_OPENGL_DEBUG)
endif()

# glm
target_include_directories(opengl_backend PUBLIC "${glm_path}")
add_dependencies(opengl_backend glm)

# Set output directories
set_target_properties(opengl_backend PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(opengl_backend PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(opengl_backend PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
set_target_properties(opengl_backend PROPERTIES FOLDER "backends")

project(opengl_backend)

set(OPENGL_VERSION "gl" CACHE STRING "Opengl version")
set_property(CACHE OPENGL_VERSION PROPERTY STRINGS gl gles)

# Sources
file(GLOB_RECURSE sources src/*.h src/*.cpp)

# Make lib
add_library(opengl_backend STATIC ${sources} "${OPENGL_VERSION}/src/glad.c")
target_include_directories(opengl_backend PUBLIC "${backend_opengl_path}/src")
target_include_directories(opengl_backend PUBLIC "${backend_opengl_path}/${OPENGL_VERSION}/include")
target_include_directories(opengl_backend PUBLIC "${utils_path}")
target_link_libraries(opengl_backend PUBLIC utils)
set_folder_structure("${sources}" ${backend_opengl_path})

# Interfaces
target_include_directories(opengl_backend PUBLIC "${backend_interfaces_path}")

# OpenGL
find_package(OpenGL REQUIRED)
if(EMSCRIPTEN)
        target_link_libraries(opengl_backend PUBLIC OPENGL_gl_LIBRARY)
else()
        target_link_libraries(opengl_backend PUBLIC OpenGL::GL)
endif()
option(ENABLE_OPENGL_DEBUG "enable Opengl debug" OFF)
if (ENABLE_OPENGL_DEBUG)
        add_definitions(-DENABLE_OPENGL_DEBUG)
endif()

# glm
target_include_directories(opengl_backend PUBLIC "${glm_path}")
add_dependencies(opengl_backend glm)

# glew
find_package(GLEW)
target_link_libraries(opengl_backend PUBLIC GLEW::glew_s)

# Set output directories
set_target_properties(opengl_backend PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(opengl_backend PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(opengl_backend PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
set_target_properties(opengl_backend PROPERTIES FOLDER "backends")

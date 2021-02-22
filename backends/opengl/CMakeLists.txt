project(opengl_backend)

# Sources
file(GLOB_RECURSE sources *.h *.cpp *.c)

# Make lib
add_library(opengl_backend STATIC ${sources})
target_include_directories(opengl_backend PUBLIC "${backend_opengl_path}")
target_include_directories(opengl_backend PUBLIC "${utils_path}")
target_link_libraries(opengl_backend PUBLIC utils)
set_folder_structure("${sources}" ${backend_opengl_path})

# Interfaces
target_include_directories(opengl_backend PUBLIC "${backend_interfaces_path}")

# OpenGL
find_package(OpenGL REQUIRED)
if(NOT EMSCRIPTEN)
        target_link_libraries(opengl_backend PUBLIC OpenGL::GL)
	find_package(GLEW REQUIRED)
        target_include_directories(opengl_backend PUBLIC ${GLEW_INCLUDE_DIRS})
        target_link_libraries(opengl_backend PUBLIC GLEW::GLEW)
endif()

# glm
target_include_directories(opengl_backend PUBLIC "${glm_path}")
add_dependencies(opengl_backend glm)

# Set output directories
set_target_properties(opengl_backend PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(opengl_backend PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(opengl_backend PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
set_target_properties(opengl_backend PROPERTIES FOLDER "backends")

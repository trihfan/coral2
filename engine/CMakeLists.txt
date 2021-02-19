project(engine)

# Sources
file(GLOB_RECURSE sources *.h *.cpp *.c)
set_folder_structure("${sources}" ${engine_path})

# Make lib
add_library(engine STATIC ${sources})
target_include_directories(engine PUBLIC "${engine_path}")
target_include_directories(engine PUBLIC "${utils_path}")
target_link_libraries(engine PUBLIC utils)

# Backends
target_include_directories(engine PUBLIC "${backend_interfaces_path}")
target_link_libraries(engine PUBLIC backend_interfaces)

# glm
target_include_directories(engine PUBLIC "${glm_path}")
add_dependencies(engine glm)

# Set output directories
set_target_properties(engine PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(engine PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(engine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
set_target_properties(engine PROPERTIES FOLDER "coral")

# tmp opengl
find_package(OpenGL REQUIRED)
target_link_libraries(engine PUBLIC OpenGL::GL)
find_package(GLEW REQUIRED)
target_include_directories(engine PUBLIC ${GLEW_INCLUDE_DIRS})
target_link_libraries(engine PUBLIC GLEW::GLEW)

target_include_directories(engine PUBLIC "${backend_opengl_path}")
target_link_libraries(engine PUBLIC opengl_backend)

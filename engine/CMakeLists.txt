project(engine)

# Sources
file(GLOB_RECURSE sources *.h *.cpp *.c)
set_folder_structure("${sources}" ${engine_path})

# Make lib
add_library(engine STATIC ${sources})
target_include_directories(engine PUBLIC "${engine_path}")

# OpenGL
if(ENABLE_OPENGL)
    find_package(OpenGL REQUIRED)
    if(NOT EMSCRIPTEN)
        target_link_libraries(engine PUBLIC OpenGL::GL)
        find_package(GLEW REQUIRED)
        target_include_directories(engine PUBLIC ${GLEW_INCLUDE_DIRS})
        target_link_libraries(engine PUBLIC GLEW::GLEW)
    endif()
endif()

# Vulkan
if(ENABLE_VULKAN AND NOT EMSCRIPTEN)
    find_package(Vulkan REQUIRED)

    find_package(glfw3 CONFIG REQUIRED)
    add_definitions(-DGLFW_INCLUDE_VULKAN)

    option(USE_VALIDATION_LAYERS "use vulkan validation layers" OFF)
    if (USE_VALIDATION_LAYERS)
        add_definitions(-DUSE_VALIDATION_LAYERS)
    endif()

    target_link_libraries(engine PUBLIC Vulkan::Vulkan)
    target_link_libraries(engine PUBLIC glfw)
endif()

# glm
target_include_directories(engine PUBLIC "${glm_path}")
add_dependencies(engine glm)

# Set output directories
set_target_properties(engine PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(engine PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(engine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
set_target_properties(engine PROPERTIES FOLDER "coral")

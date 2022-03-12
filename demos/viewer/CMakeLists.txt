project (viewer)

# Dependencies
get_property(engine_path GLOBAL PROPERTY engine_path)
get_property(backend_opengl_path GLOBAL PROPERTY backend_opengl_path)
get_property(backend_vulkan_path GLOBAL PROPERTY backend_vulkan_path)

if(NOT EMSCRIPTEN)
    find_package(glfw3 CONFIG REQUIRED)
endif()

# Definitions
if(BACKEND_VULKAN_ENABLED)
    add_definitions(-DGLFW_INCLUDE_VULKAN)
elseif(EMSCRIPTEN)
    add_definitions(-DGLFW_INCLUDE_ES3)
endif()

if(EMSCRIPTEN)
    set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -s ALLOW_MEMORY_GROWTH=1 -s DISABLE_EXCEPTION_CATCHING=0")
endif()

# Make lib
file(GLOB_RECURSE sources *.h *.cpp)
add_executable(viewer ${sources})
set_folder_structure("${sources}" ${CMAKE_CURRENT_SOURCE_DIR})

# Includes and libs
target_include_directories(viewer PRIVATE ${engine_path})
target_link_libraries(viewer PRIVATE engine glfw)

# Set output directories
set_target_properties(viewer PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(viewer PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(viewer PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
set_target_properties(viewer PROPERTIES FOLDER "coral/tests")

# Deployement
if(EMSCRIPTEN)
    option(VIEWER_DEPLOY_EMSCRIPTEN "deploy emscripten binaries to a directory" OFF)
    if (VIEWER_DEPLOY_EMSCRIPTEN)
        set(VIEWER_DEPLOY_EMSCRIPTEN_DIRECTORY "" CACHE STRING "emscripten deploy directory for viewer app")

        add_custom_target(viewer_deploy_emscripten ALL)
        add_dependencies(viewer_deploy_emscripten viewer)

        # Copy binaries
        add_custom_command(TARGET viewer_deploy_emscripten PRE_BUILD COMMAND scp -i ~/.ssh/id_rsa "${bin_output_directory}/viewer*" "${VIEWER_DEPLOY_EMSCRIPTEN_DIRECTORY}")

        # Copy loader
        add_custom_command(TARGET viewer_deploy_emscripten PRE_BUILD COMMAND scp -i ~/.ssh/id_rsa "${CMAKE_CURRENT_SOURCE_DIR}/index.html" "${VIEWER_DEPLOY_EMSCRIPTEN_DIRECTORY}")
    endif()
endif()

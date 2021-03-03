project (viewer)

# Link directories
link_directories(viewer ${lib_output_directory})

# Make lib
file(GLOB_RECURSE sources *.h *.cpp)
add_executable(viewer ${sources})
set_folder_structure("${sources}" ${viewer_path})
target_link_libraries(viewer engine)

# Backend
if(BACKEND_OPENGL_ENABLED)
    target_link_libraries(viewer opengl_backend)
endif()
if(BACKEND_VULKAN_ENABLED)
    target_link_libraries(viewer vulkan_backend)
endif()

# Glfw3
if(NOT EMSCRIPTEN)
    find_package(glfw3 CONFIG REQUIRED)
    target_link_libraries(viewer glfw)
endif()

if(BACKEND_VULKAN_ENABLED)
    add_definitions(-DGLFW_INCLUDE_VULKAN)
elseif(EMSCRIPTEN)
    add_definitions(-DGLFW_INCLUDE_ES3)
endif()

# Set output directories
set_target_properties(viewer PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(viewer PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(viewer PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
set_target_properties(viewer PROPERTIES FOLDER "coral/tests")

# Deployement
if(EMSCRIPTEN)
    option(EMSCRIPTEN_DEPLOY "deploy emscripten binaries to a directory" OFF)
    if (EMSCRIPTEN_DEPLOY)
        set(EMSCRIPTEN_DEPLOY_DIRECTORY "" CACHE STRING "emscripten deploy directory root")

        add_custom_target(emscripten_deploy ALL)
        add_dependencies(emscripten_deploy viewer)

        # Copy binaries
        add_custom_command(TARGET emscripten_deploy PRE_BUILD COMMAND scp -r -i ~/.ssh/id_rsa "${bin_output_directory}/*" "${EMSCRIPTEN_DEPLOY_DIRECTORY}")

        # Copy loader
        add_custom_command(TARGET emscripten_deploy PRE_BUILD COMMAND scp -i ~/.ssh/id_rsa "${viewer_path}/viewer.html" "${EMSCRIPTEN_DEPLOY_DIRECTORY}")
    endif()
endif()

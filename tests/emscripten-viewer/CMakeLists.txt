project (emscripten_viewer)

# Emscripten
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -s USE_GLFW=3 -s EXPORTED_FUNCTIONS=['_initialize','_update'] -s EXTRA_EXPORTED_RUNTIME_METHODS=['cwrap']")

# link directories
link_directories(emscripten_viewer ${lib_output_directory})

# make lib
file(GLOB_RECURSE sources *.h *.cpp)
add_executable(emscripten_viewer ${sources})

# includes
target_include_directories(emscripten_viewer PRIVATE "${emscripten_path}")

# dependencies
target_link_libraries(emscripten_viewer engine)

# set output directories
set_target_properties(emscripten_viewer PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(emscripten_viewer PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(emscripten_viewer PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})

# Resources

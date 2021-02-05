project (emscripten)

set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -s USE_WEBGL2=1 -s WASM=1")

# link directories
link_directories(emscripten ${lib_output_directory})

# make lib
file(GLOB_RECURSE sources *.h *.cpp)
set_folder_structure("${sources}" ${web_path})
add_executable(emscripten ${sources})

# includes
target_include_directories(emscripten PRIVATE "${emscripten_path}")

# dependencies
target_link_libraries(emscripten engine)

# set output directories
set_target_properties(emscripten PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(emscripten PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(emscripten PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})

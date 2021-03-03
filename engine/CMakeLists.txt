project(engine)

# Sources
file(GLOB_RECURSE sources *.h *.cpp *.c)

# Make lib
add_library(engine STATIC ${sources})
target_include_directories(engine PUBLIC "${engine_path}" "${utils_path}" "${backends_path}/interfaces" "${glm_path}" "${CMAKE_BINARY_DIR}")
target_link_libraries(engine PUBLIC utils backend_interfaces glm)
set_folder_structure("${sources}" ${engine_path})

# Set output directories
set_target_properties(engine PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(engine PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(engine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
set_target_properties(engine PROPERTIES FOLDER "coral")


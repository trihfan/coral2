set_property(GLOBAL PROPERTY assimp_path ${CMAKE_CURRENT_SOURCE_DIR}/assimp)
add_subdirectory(assimp)

# Set output directories
set_target_properties(assimp PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(assimp PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(assimp PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
set_target_properties(assimp PROPERTIES FOLDER "externals")

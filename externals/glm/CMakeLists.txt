set_property(GLOBAL PROPERTY glm_path ${CMAKE_CURRENT_SOURCE_DIR}/glm)
add_subdirectory(glm)

# Set output directories
set_target_properties(glm PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(glm PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(glm PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
set_target_properties(glm PROPERTIES FOLDER "externals")

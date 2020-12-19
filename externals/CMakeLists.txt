project (external)

# add submodules
add_subdirectory(assimp)
add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(rang)

# set output directories
set_target_properties(assimp PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(assimp PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(assimp PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
set_target_properties(glfw PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(glfw PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(glfw PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
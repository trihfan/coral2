project (external)

add_subdirectory(assimp)
add_subdirectory(glfw)
add_subdirectory(glm)

set_target_properties(glfw PROPERTIES FOLDER "externals/glfw")
set_target_properties(assimp PROPERTIES FOLDER "externals/assimp")
set_target_properties(assimp_cmd PROPERTIES FOLDER "externals/assimp")
set_target_properties(glm_shared PROPERTIES FOLDER "externals/glm")

set_target_properties(assimp PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(assimp PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(assimp PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
set_target_properties(glfw PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(glfw PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(glfw PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
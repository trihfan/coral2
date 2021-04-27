project(utils)
set_property(GLOBAL PROPERTY utils_path ${CMAKE_CURRENT_SOURCE_DIR})

# Dependencies
get_property(glm_path GLOBAL PROPERTY glm_path)

# Sources
file(GLOB_RECURSE sources *.h *.cpp *.c)

# Make lib
add_library(utils STATIC ${sources})
target_include_directories(utils PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
set_folder_structure("${sources}" ${CMAKE_CURRENT_SOURCE_DIR})

# glm
target_include_directories(utils PUBLIC "${glm_path}")

# Set output directories
set_target_properties(utils PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(utils PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(utils PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
set_target_properties(utils PROPERTIES FOLDER "utils")

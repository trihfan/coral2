project(utils)
set_property(GLOBAL PROPERTY utils_path ${CMAKE_CURRENT_SOURCE_DIR})

# Sources
file(GLOB_RECURSE sources *.h *.cpp *.c)

# Make lib
add_library(utils STATIC ${sources})
target_include_directories(utils PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
set_folder_structure("${sources}" ${CMAKE_CURRENT_SOURCE_DIR})

# Includes
target_include_directories(utils PUBLIC)

# Libraries
target_link_libraries(utils PUBLIC glm::glm)

# Set output directories
set_target_properties(utils PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(utils PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(utils PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
set_target_properties(utils PROPERTIES FOLDER "utils")

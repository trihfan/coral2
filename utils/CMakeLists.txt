project(utils)

# Sources
file(GLOB_RECURSE sources *.h *.cpp *.c)
set_folder_structure("${sources}" ${utils_path})

# Make lib
add_library(utils STATIC ${sources})
target_include_directories(utils PUBLIC "${utils_path}")

# Set output directories
set_target_properties(utils PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(utils PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(utils PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
set_target_properties(utils PROPERTIES FOLDER "utils")

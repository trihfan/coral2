project(backend_interfaces)

# Sources
file(GLOB_RECURSE sources *.h *.cpp *.c)

# Make lib
add_library(backend_interfaces STATIC ${sources})
target_include_directories(backend_interfaces PUBLIC "${backend_interfaces_path}")
target_include_directories(backend_interfaces PUBLIC "${utils_path}")
target_link_libraries(backend_interfaces PUBLIC utils)
set_folder_structure("${sources}" ${backend_interfaces_path})

# glm
target_include_directories(backend_interfaces PUBLIC "${glm_path}")
add_dependencies(backend_interfaces glm)

# Set output directories
set_target_properties(backend_interfaces PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(backend_interfaces PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(backend_interfaces PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
set_target_properties(backend_interfaces PROPERTIES FOLDER "backends")

project(backend_interfaces)
set_property(GLOBAL PROPERTY backend_interfaces_path ${CMAKE_CURRENT_SOURCE_DIR})

# Dependencies
get_property(glm_path GLOBAL PROPERTY glm_path)
get_property(utils_path GLOBAL PROPERTY utils_path)

# Sources
file(GLOB_RECURSE sources *.h *.cpp *.c)

# Make lib
add_library(backend_interfaces STATIC ${sources})
set_folder_structure("${sources}" ${CMAKE_CURRENT_SOURCE_DIR})

# Includes
target_include_directories(backend_interfaces PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" "${utils_path}" "${glm_path}")

# Libraries
target_link_libraries(backend_interfaces PUBLIC utils glm)

# Set output directories
set_target_properties(backend_interfaces PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(backend_interfaces PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(backend_interfaces PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
set_target_properties(backend_interfaces PROPERTIES FOLDER "backends")

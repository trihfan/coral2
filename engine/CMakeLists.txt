project (engine)

# include directories
include_directories("${external_path}/assimp/include")
include_directories("${assimp_config_path}")
include_directories("${external_path}/glm")
include_directories("${external_path}/stb")
include_directories("${engine_path}")

# link directories
link_directories(engine ${lib_output_directory})

# make lib
file(GLOB_RECURSE sources *.h *.cpp *.inl *.c)
set_folder_structure("${sources}" ${engine_path})
add_library(engine STATIC ${sources} "${external_path}/stb/stb_vorbis.c" "${external_path}/stb/deprecated/stb_image.c")
add_dependencies(engine assimp)
target_link_libraries(engine assimp)

# set output directories
set_target_properties(engine PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(engine PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(engine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})
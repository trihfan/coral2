project (engine)

# link directories
link_directories(engine ${lib_output_directory})

# make lib
file(GLOB_RECURSE sources *.h *.cpp *.inl *.c)
set_folder_structure("${sources}" ${engine_path})
add_library(engine STATIC ${sources})

# includes
target_include_directories(engine PUBLIC "${external_path}/assimp/include")
target_include_directories(engine PUBLIC "${assimp_config_path}")
target_include_directories(engine PUBLIC "${external_path}/glm")
target_include_directories(engine PUBLIC "${external_path}/stb")
target_include_directories(engine PUBLIC "${external_path}/rang/include")
target_include_directories(engine PUBLIC "${engine_path}")

# dependencies
target_link_libraries(engine assimp)

# set output directories
set_target_properties(engine PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(engine PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${lib_output_directory})
set_target_properties(engine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_output_directory})

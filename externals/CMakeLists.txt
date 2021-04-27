cmake_minimum_required(VERSION 3.16)

set(externals_src_dir "" CACHE STRING "coral externals_src_dir")
set(externals_lib_dir "" CACHE STRING "coral externals_lib_dir")

set_property(GLOBAL PROPERTY glm_path ${externals_src_dir}/externals/glm/glm)
set_property(GLOBAL PROPERTY assimp_path ${externals_src_dir}/externals/assimp/assimp/include)
set_property(GLOBAL PROPERTY assimp_lib_path ${externals_lib_dir})
set_property(GLOBAL PROPERTY assimp_config_path ${externals_lib_dir}/../externals/assimp/assimp/include)
project(assets_packager)

# List of assets directory
set(external_asset_directories "" CACHE STRING "Add assets directories to package with the engine")
set(asset_directories "${assets_path}" ${external_asset_directories})
set(asset_binary_dir "${bin_output_directory}/${CMAKE_CFG_INTDIR}/assets")

# Prepare index
#file(WRITE ${CMAKE_BINARY_DIR}/assets.h "")

# Package shaders
#file(APPEND ${CMAKE_BINARY_DIR}/assets.h "#define SHADER_ASSETS_LIST std::vector<ShaderAsset>{\\\n")

foreach(asset_directory ${asset_directories})
    #if (asset_directories STREQUAL "shaders")
    #endif()
    # List all assets recursively
    file(GLOB_RECURSE asset_list ${asset_directory}/*)

    # For each asset
    foreach(asset_file ${asset_list})

        # Copy to binary dir
        file(RELATIVE_PATH asset_file_relative ${asset_directory} ${asset_file})
        configure_file(${asset_file} "${asset_binary_dir}/${asset_file_relative}" COPYONLY)
    endforeach()
endforeach()

# Finalize index
#file(APPEND ${CMAKE_BINARY_DIR}/assets.h "}")

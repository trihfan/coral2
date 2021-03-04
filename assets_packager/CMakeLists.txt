project(assets)

# List of assets directory
set(external_asset_directories "" CACHE STRING "Add assets directories to package with the engine")
set(asset_directories "${assets_path}" ${external_asset_directories})
set(asset_binary_dir "${bin_output_directory}/${CMAKE_CFG_INTDIR}/assets")

# Prepare index
file(WRITE ${CMAKE_BINARY_DIR}/assets.h "")

# Package shaders
file(APPEND ${CMAKE_BINARY_DIR}/assets.h "#define SHADER_ASSETS_LIST std::vector<ShaderAsset>{\\\n")

foreach(asset_directory ${asset_directories})
    if (asset_directories STREQUAL "shaders")



    endif()
endforeach()

# Finalize index
file(APPEND ${CMAKE_BINARY_DIR}/assets.h "}")

# Asset package
set(asset_binary_dir "${bin_output_directory}/${CMAKE_CFG_INTDIR}/assets")
set(ASSET_DIRECTORIES "${assets_path}")

file(WRITE ${CMAKE_BINARY_DIR}/assets.h "")
file(APPEND ${CMAKE_BINARY_DIR}/assets.h "#define ASSETS_LIST std::vector<Asset>{\\\n")

file(MAKE_DIRECTORY ${bin_output_directory})
foreach(ASSET_DIRECTORY ${ASSET_DIRECTORIES})

    # List all assets recursively
    file(GLOB_RECURSE asset_list ${ASSET_DIRECTORY}/*)

    # For each asset
    foreach(asset_file ${asset_list})

        # Copy to binary dir
        file(RELATIVE_PATH asset_file_relative ${ASSET_DIRECTORY} ${asset_file})
        configure_file(${asset_file} "${asset_binary_dir}/${asset_file_relative}" COPYONLY)


        # Package in asset.h
        get_filename_component(asset_file_name ${asset_file} NAME)
        file(APPEND ${CMAKE_BINARY_DIR}/assets.h "Asset{\"${asset_file_name}\", \"assets/${asset_file_relative}\"},\\\n")

    endforeach()
endforeach()

file(APPEND ${CMAKE_BINARY_DIR}/assets.h "}")

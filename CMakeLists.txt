cmake_minimum_required(VERSION 3.10)
project(tenshi_engine)

set(EXTERNAL_DIR "external")

# find_package(SDL3       REQUIRED)
# find_package(SDL3_image REQUIRED)
# find_package(SDL3_ttf   REQUIRED)

option(TENSHI_INSTALL "enable install rules for tenshi" OFF)

####################
#     External     #
####################

add_library(miniaudio STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/${EXTERNAL_DIR}/miniaudio/miniaudio.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/${EXTERNAL_DIR}/miniaudio/miniaudio.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/${EXTERNAL_DIR}/stb_vorbis.c"
)
target_include_directories(miniaudio PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/${EXTERNAL_DIR}/miniaudio")

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
add_subdirectory(
    "${CMAKE_CURRENT_SOURCE_DIR}/${EXTERNAL_DIR}/lz4/build/cmake"
    lz4
)

#########################
#     Tenshi engine     #
#########################

set(TENSHI_INCLUDE_DIR "include")
set(TENSHI_SOURCE_DIR  "src")

add_library(tenshi STATIC)
add_library(tenshi::tenshi ALIAS tenshi)
target_sources(tenshi PRIVATE
    "${TENSHI_INCLUDE_DIR}/Tenshi/Tenshi.h"
    "${TENSHI_INCLUDE_DIR}/Tenshi/Engine.h"
    "${TENSHI_INCLUDE_DIR}/Tenshi/Game.h"
    "${TENSHI_INCLUDE_DIR}/Tenshi/Log.h"
    "${TENSHI_INCLUDE_DIR}/Tenshi/Input.h"
    "${TENSHI_INCLUDE_DIR}/Tenshi/AssetRegistry.h"
    "${TENSHI_INCLUDE_DIR}/Tenshi/Sprite.h"
    "${TENSHI_INCLUDE_DIR}/Tenshi/Camera.h"
    "${TENSHI_INCLUDE_DIR}/Tenshi/TextureAtlas.h" 
    "${TENSHI_INCLUDE_DIR}/Tenshi/Collider.h"
    "${TENSHI_INCLUDE_DIR}/Tenshi/Font.h"
    "${TENSHI_INCLUDE_DIR}/Tenshi/Text.h"
    "${TENSHI_INCLUDE_DIR}/Tenshi/Entity.h"
    "${TENSHI_INCLUDE_DIR}/Tenshi/Tilemap.h"
    "${TENSHI_INCLUDE_DIR}/Tenshi/Tileset.h"
    "${TENSHI_INCLUDE_DIR}/Tenshi/SpriteSheet.h"
    "${TENSHI_INCLUDE_DIR}/Tenshi/AssetManager.h"
    "${TENSHI_INCLUDE_DIR}/Tenshi/SpriteBatch.h"
    "${TENSHI_INCLUDE_DIR}/Tenshi/CharacterModel.h"
    "${TENSHI_INCLUDE_DIR}/Tenshi/Background.h"
    "${TENSHI_INCLUDE_DIR}/Tenshi/ResourceLoading.h"
    "${TENSHI_INCLUDE_DIR}/Tenshi/Audio.h"
    "${TENSHI_INCLUDE_DIR}/Tenshi/Sound.h"
    "${TENSHI_INCLUDE_DIR}/Tenshi/Scene.h"
    "${TENSHI_INCLUDE_DIR}/Tenshi/Actor.h"
    "${TENSHI_INCLUDE_DIR}/Tenshi/Subroutine.h"
    "${TENSHI_INCLUDE_DIR}/Tenshi/FileSystem.h"
    "${TENSHI_INCLUDE_DIR}/Tenshi/Player.h"
    "${TENSHI_INCLUDE_DIR}/Tenshi/Geometry/Rect.h"
    
    "${TENSHI_SOURCE_DIR}/Engine.cpp"
    "${TENSHI_SOURCE_DIR}/Game.cpp"
    "${TENSHI_SOURCE_DIR}/Log.cpp"
    "${TENSHI_SOURCE_DIR}/Input.cpp"
    "${TENSHI_SOURCE_DIR}/Sprite.cpp"
    "${TENSHI_SOURCE_DIR}/SpriteBatch.cpp"
    "${TENSHI_SOURCE_DIR}/Camera.cpp"
    "${TENSHI_SOURCE_DIR}/TextureAtlas.cpp"          
    "${TENSHI_SOURCE_DIR}/Font.cpp"
    "${TENSHI_SOURCE_DIR}/Text.cpp"
    "${TENSHI_SOURCE_DIR}/Entity.cpp"
    "${TENSHI_SOURCE_DIR}/Tilemap.cpp"
    "${TENSHI_SOURCE_DIR}/Tileset.cpp"
    "${TENSHI_SOURCE_DIR}/AssetManager.cpp"
    "${TENSHI_SOURCE_DIR}/CharacterModel.cpp"
    "${TENSHI_SOURCE_DIR}/SpriteSheet.cpp"
    "${TENSHI_SOURCE_DIR}/Background.cpp"
    "${TENSHI_SOURCE_DIR}/ResourceLoading.cpp"
    "${TENSHI_SOURCE_DIR}/Audio.cpp"
    "${TENSHI_SOURCE_DIR}/Sound.cpp"
    "${TENSHI_SOURCE_DIR}/Scene.cpp"
    "${TENSHI_SOURCE_DIR}/Actor.cpp"
    "${TENSHI_SOURCE_DIR}/Player.cpp"
    "${TENSHI_SOURCE_DIR}/FileSystem.cpp"
    "${TENSHI_SOURCE_DIR}/PakArchive.cpp"
    "${TENSHI_SOURCE_DIR}/PakArchive.h"
    "${TENSHI_SOURCE_DIR}/PakFile.cpp"
    "${TENSHI_SOURCE_DIR}/PakFile.h"
)

target_link_libraries(tenshi PUBLIC
    SDL3::SDL3 
    SDL3_image::SDL3_image 
    SDL3_ttf::SDL3_ttf
    miniaudio
    lz4
)

target_include_directories(tenshi
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

#####################
#     Resources     #
#####################

set(TENSHI_RESOURCE_DIR
    "${CMAKE_SOURCE_DIR}/resources"
)

set(TENSHI_PAK_FILE
    "${CMAKE_BINARY_DIR}/resource0.tpk"
)

if(NOT EMSCRIPTEN)

    # Native packer
    add_executable(packer_tool
        tools/pak/main.cpp
    )

    target_link_libraries(packer_tool
        PRIVATE
            lz4
    )

    add_custom_command(
        OUTPUT
            "${TENSHI_PAK_FILE}"

        COMMAND
            "$<TARGET_FILE:packer_tool>"
            "${TENSHI_RESOURCE_DIR}"
            "${TENSHI_PAK_FILE}"

        DEPENDS
            packer_tool

        COMMENT
            "Packing game resources"

        VERBATIM
    )
else()
    # Native host packer
    set(TENSHI_PACKER_BUILD_DIR
        "${CMAKE_BINARY_DIR}/tenshi_packer"
    )

    set(TENSHI_PACKER_INSTALL_DIR
        "${TENSHI_PACKER_BUILD_DIR}/install"
    )

    ExternalProject_Add(tenshi_packer
        SOURCE_DIR
            "${CMAKE_CURRENT_SOURCE_DIR}/tools/pak"

        BINARY_DIR
            "${CMAKE_BINARY_DIR}/tenshi_packer/build"

        INSTALL_DIR
            "${CMAKE_BINARY_DIR}/tenshi_packer/install"

        CMAKE_ARGS
            "-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>"
            "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
            "-DCMAKE_TOOLCHAIN_FILE="

        BUILD_BYPRODUCTS
            "${CMAKE_BINARY_DIR}/tenshi_packer/install/bin/packer_tool.exe"

        INSTALL_COMMAND
            "${CMAKE_COMMAND}"
            --build <BINARY_DIR>
            --target install

        BUILD_ALWAYS
            OFF
    )

    if(WIN32)
        set(TENSHI_PACKER
            "${TENSHI_PACKER_INSTALL_DIR}/bin/packer_tool.exe"
        )
    else()
        set(TENSHI_PACKER
            "${TENSHI_PACKER_INSTALL_DIR}/bin/packer_tool"
        )
    endif()

    # Pack resources
    add_custom_command(
        OUTPUT
            "${TENSHI_PAK_FILE}"
        COMMAND
            "${TENSHI_PACKER}"
            "${TENSHI_RESOURCE_DIR}"
            "${TENSHI_PAK_FILE}"
        DEPENDS tenshi_packer
        COMMENT "Packing game resources"
        VERBATIM
    )
endif()

add_custom_target(
    pack_resources
    DEPENDS
        "${TENSHI_PAK_FILE}"
)

# install

if(TENSHI_INSTALL)
    include(CMakePackageConfigHelpers)

    install(TARGETS tenshi
        EXPORT tenshiTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
    )

    install(DIRECTORY include/
        DESTINATION include
    )

    install(EXPORT tenshiTargets
        FILE tenshiTargets.cmake
        NAMESPACE tenshi::
        DESTINATION lib/cmake/tenshi
    )

    configure_package_config_file(
        cmake/tenshiConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/tenshiConfig.cmake
        INSTALL_DESTINATION lib/cmake/tenshi
    )

    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/tenshiConfig.cmake
        DESTINATION lib/cmake/tenshi
    )
endif()
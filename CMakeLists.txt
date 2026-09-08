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

#########################
#     tenshi engine     #
#########################

set(TENSHI_INCLUDE_DIR "include")
set(TENSHI_SOURCE_DIR  "src")

add_library(tenshi STATIC)
add_library(tenshi::tenshi ALIAS tenshi)
target_sources(tenshi PRIVATE
    "${TENSHI_INCLUDE_DIR}/Tenshi/Tenshi.h"
    "${TENSHI_SOURCE_DIR}/Engine.cpp"
    "${TENSHI_SOURCE_DIR}/Engine.h"
    "${TENSHI_SOURCE_DIR}/Game.h"
    "${TENSHI_SOURCE_DIR}/Game.cpp"
    "${TENSHI_SOURCE_DIR}/Log.cpp"
    "${TENSHI_SOURCE_DIR}/Log.h"
    "${TENSHI_SOURCE_DIR}/Input.cpp"
    "${TENSHI_SOURCE_DIR}/Input.h"
    "${TENSHI_SOURCE_DIR}/AssetRegistry.h"
    "${TENSHI_SOURCE_DIR}/Sprite.cpp"
    "${TENSHI_SOURCE_DIR}/Sprite.h"
    "${TENSHI_SOURCE_DIR}/SpriteBatch.h"
    "${TENSHI_SOURCE_DIR}/SpriteBatch.cpp"
    "${TENSHI_SOURCE_DIR}/Camera.cpp"
    "${TENSHI_SOURCE_DIR}/Camera.h" 
    "${TENSHI_SOURCE_DIR}/TextureAtlas.cpp"
    "${TENSHI_SOURCE_DIR}/TextureAtlas.h" 
    "${TENSHI_SOURCE_DIR}/Collider.h"
    "${TENSHI_SOURCE_DIR}/Geometry/Rect.h"             
    "${TENSHI_SOURCE_DIR}/Font.cpp"
    "${TENSHI_SOURCE_DIR}/Font.h"
    "${TENSHI_SOURCE_DIR}/Text.cpp"
    "${TENSHI_SOURCE_DIR}/Text.h"
    "${TENSHI_SOURCE_DIR}/Entity.cpp"
    "${TENSHI_SOURCE_DIR}/Entity.h"
    "${TENSHI_SOURCE_DIR}/Tilemap.cpp"
    "${TENSHI_SOURCE_DIR}/Tilemap.h" 
    "${TENSHI_SOURCE_DIR}/Tileset.cpp"
    "${TENSHI_SOURCE_DIR}/Tileset.h"
    "${TENSHI_SOURCE_DIR}/AssetManager.h"
    "${TENSHI_SOURCE_DIR}/AssetManager.cpp"
    "${TENSHI_SOURCE_DIR}/CharacterModel.h"
    "${TENSHI_SOURCE_DIR}/CharacterModel.cpp"
    "${TENSHI_SOURCE_DIR}/SpriteSheet.h"
    "${TENSHI_SOURCE_DIR}/SpriteSheet.cpp"
    "${TENSHI_SOURCE_DIR}/Background.h"
    "${TENSHI_SOURCE_DIR}/Background.cpp"
    "${TENSHI_SOURCE_DIR}/ResourceLoading.h"
    "${TENSHI_SOURCE_DIR}/ResourceLoading.cpp"
    "${TENSHI_SOURCE_DIR}/StaticLoader.h"
    "${TENSHI_SOURCE_DIR}/StaticLoader.cpp"
    "${TENSHI_SOURCE_DIR}/Audio.h"
    "${TENSHI_SOURCE_DIR}/Audio.cpp"
    "${TENSHI_SOURCE_DIR}/Sound.h"
    "${TENSHI_SOURCE_DIR}/Sound.cpp"
    "${TENSHI_SOURCE_DIR}/Scene.h"
    "${TENSHI_SOURCE_DIR}/Scene.cpp"
    "${TENSHI_SOURCE_DIR}/Actor.h"
    "${TENSHI_SOURCE_DIR}/Actor.cpp"
    "${TENSHI_SOURCE_DIR}/Player.h"
    "${TENSHI_SOURCE_DIR}/Player.cpp"
    "${TENSHI_SOURCE_DIR}/Subroutine.h"
    "${TENSHI_SOURCE_DIR}/FileSystem.h"
    "${TENSHI_SOURCE_DIR}/FileSystem.cpp"
)

target_link_libraries(tenshi PUBLIC
    SDL3::SDL3 
    SDL3_image::SDL3_image 
    SDL3_ttf::SDL3_ttf
    miniaudio
)

target_include_directories(tenshi
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
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
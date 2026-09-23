cmake_minimum_required(VERSION 3.10)
project(tenshi_engine)

set(EXTERNAL_DIR "external")

# ============================================================
# Options
# ============================================================

option(TENSHI_PACKED
    "Pack game resources into TPK"
    OFF
)

option(TENSHI_BUILD_PACKER_ONLY
    "Build only the native resource packer"
    OFF
)

option(TENSHI_INSTALL
    "enable install rules for tenshi"
    OFF
)

set(TENSHI_RESOURCE_DIR
    "${CMAKE_SOURCE_DIR}/resources"
    CACHE PATH
    "Directory containing game resources"
)

set(TENSHI_PAK_FILE
    "${CMAKE_BINARY_DIR}/packed/resource0.tpk"
    CACHE FILEPATH
    "Output resource archive"
)

# --------------------------------------------------------
# Packer-only build
# Used by the Emscripten build to create a native packer
# --------------------------------------------------------
if(TENSHI_BUILD_PACKER_ONLY)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    add_subdirectory(
        "${CMAKE_CURRENT_SOURCE_DIR}/${EXTERNAL_DIR}/lz4/build/cmake"
        lz4
    )
    add_executable(packer_tool
        "${CMAKE_CURRENT_SOURCE_DIR}/tools/pak/main.cpp"
    )
    target_link_libraries(packer_tool 
        PRIVATE 
            lz4
    )

    return()
endif()

# ============================================================
# External dependencies
# ============================================================

add_library(miniaudio STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/${EXTERNAL_DIR}/miniaudio/miniaudio.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/${EXTERNAL_DIR}/miniaudio/miniaudio.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/${EXTERNAL_DIR}/stb_vorbis.c"
)

target_include_directories(miniaudio
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/${EXTERNAL_DIR}/miniaudio"
)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
add_subdirectory(
    "${CMAKE_CURRENT_SOURCE_DIR}/${EXTERNAL_DIR}/lz4/build/cmake"
    lz4
)

add_library(fkyaml INTERFACE)
target_include_directories(fkyaml 
    INTERFACE 
        "${CMAKE_CURRENT_SOURCE_DIR}/${EXTERNAL_DIR}/fkyaml/include/"
)

add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE 
    "${CMAKE_CURRENT_SOURCE_DIR}/${EXTERNAL_DIR}/stb_image/"
)

# ============================================================
# Tenshi engine
# ============================================================

set(TENSHI_INCLUDE_DIR "include")
set(TENSHI_SOURCE_DIR  "src")

add_library(tenshi STATIC)
add_library(tenshi::tenshi ALIAS tenshi)

target_sources(tenshi
    PRIVATE
        # Public includes
        "${TENSHI_INCLUDE_DIR}/Tenshi/Tenshi.h"
        
        "${TENSHI_INCLUDE_DIR}/Tenshi/Assets/AssetManager.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/Assets/AssetRegistry.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/Assets/Font.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/Assets/ResourceLoading.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/Audio/Audio.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/Audio/Sound.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/Audio/SoundInstance.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/Core/Engine.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/Core/Game.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/Core/Log.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/Core/Subroutine.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/Files/FileSystem.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/Files/Yaml.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/Geometry/Rect.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/Geometry/Vector.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/Graphics/Sprite.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/Graphics/SpriteBatch.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/Graphics/Text.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/Graphics/TextureAtlas.h" 
        "${TENSHI_INCLUDE_DIR}/Tenshi/Graphics/Tilemap.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/Graphics/Tileset.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/Graphics/TileTypes.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/GUI/Frame.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/GUI/Manager.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/GUI/Object.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/Input/Input.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/Scene/Actor.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/Scene/Background.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/Scene/Camera.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/Scene/CharacterModel.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/Scene/Collider.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/Scene/Entity.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/Scene/Player.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/Scene/Scene.h"
        "${TENSHI_INCLUDE_DIR}/Tenshi/Utility/Colour.h"

        # Private
        "${TENSHI_SOURCE_DIR}/Assets/Font.cpp"
        "${TENSHI_SOURCE_DIR}/Assets/Image.cpp"
        "${TENSHI_SOURCE_DIR}/Assets/Image.h"
        "${TENSHI_SOURCE_DIR}/Assets/ResourceLoading.cpp"
        "${TENSHI_SOURCE_DIR}/Audio/Audio.cpp"
        "${TENSHI_SOURCE_DIR}/Audio/Sound.cpp"
        "${TENSHI_SOURCE_DIR}/Audio/SoundInstance.cpp"
        "${TENSHI_SOURCE_DIR}/Core/CrashHandler.h"
        "${TENSHI_SOURCE_DIR}/Core/Engine.cpp"
        "${TENSHI_SOURCE_DIR}/Core/Game.cpp"
        "${TENSHI_SOURCE_DIR}/Core/GameHelper.h"
        "${TENSHI_SOURCE_DIR}/Core/Log.cpp"
        "${TENSHI_SOURCE_DIR}/Files/FileSystem.cpp"
        "${TENSHI_SOURCE_DIR}/Files/PakArchive.cpp"
        "${TENSHI_SOURCE_DIR}/Files/PakArchive.h"
        "${TENSHI_SOURCE_DIR}/Files/PakFile.cpp"
        "${TENSHI_SOURCE_DIR}/Files/PakFile.h"
        "${TENSHI_SOURCE_DIR}/Files/Yaml.cpp"
        "${TENSHI_SOURCE_DIR}/Graphics/Sprite.cpp"
        "${TENSHI_SOURCE_DIR}/Graphics/SpriteBatch.cpp"
        "${TENSHI_SOURCE_DIR}/Graphics/Text.cpp"
        "${TENSHI_SOURCE_DIR}/Graphics/TextureAtlas.cpp"          
        "${TENSHI_SOURCE_DIR}/Graphics/Tilemap.cpp"
        "${TENSHI_SOURCE_DIR}/Graphics/TilemapFormat.h"
        "${TENSHI_SOURCE_DIR}/Graphics/Tileset.cpp"
        "${TENSHI_SOURCE_DIR}/GUI/Frame.cpp"
        "${TENSHI_SOURCE_DIR}/GUI/Manager.cpp"
        "${TENSHI_SOURCE_DIR}/Input/Input.cpp"
        "${TENSHI_SOURCE_DIR}/Platform/Window.cpp"
        "${TENSHI_SOURCE_DIR}/Platform/Window.h"
        "${TENSHI_SOURCE_DIR}/Scene/Actor.cpp"
        "${TENSHI_SOURCE_DIR}/Scene/Background.cpp"
        "${TENSHI_SOURCE_DIR}/Scene/Camera.cpp"
        "${TENSHI_SOURCE_DIR}/Scene/CharacterModel.cpp"
        "${TENSHI_SOURCE_DIR}/Scene/Entity.cpp"
        "${TENSHI_SOURCE_DIR}/Scene/Player.cpp"
        "${TENSHI_SOURCE_DIR}/Scene/Scene.cpp"
)

target_link_libraries(tenshi
    PUBLIC
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
        miniaudio
        lz4
        fkyaml
        stb_image
)

target_include_directories(tenshi
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_BINARY_DIR}/include"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# ============================================================
# Resource packer
# ============================================================

if(TENSHI_PACKED)
    # --------------------------------------------------------
    # Native build
    # --------------------------------------------------------
    if(NOT EMSCRIPTEN)
        add_executable(packer_tool
            "${CMAKE_CURRENT_SOURCE_DIR}/tools/pak/main.cpp"
        )

        target_link_libraries(packer_tool
            PRIVATE
                lz4
        )

        add_custom_command(
            OUTPUT
                "${TENSHI_PAK_FILE}"
            COMMAND
                "${CMAKE_COMMAND}"
                -E
                make_directory
                "${CMAKE_BINARY_DIR}/packed"
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

    # --------------------------------------------------------
    # Emscripten build
    # --------------------------------------------------------
    else()
        # --------------------------------------------------------
        # Emscripten host packer
        # --------------------------------------------------------

        include(ExternalProject)

        set(TENSHI_PACKER_BUILD_DIR
            "${CMAKE_BINARY_DIR}/tenshi-packer-build"
        )

        set(TENSHI_PACKER_INSTALL_DIR
            "${CMAKE_BINARY_DIR}/tenshi-packer-install"
        )

        if(CMAKE_HOST_WIN32)
            set(TENSHI_PACKER_SUFFIX ".exe")
        else()
            set(TENSHI_PACKER_SUFFIX "")
        endif()

        ExternalProject_Add(tenshi_packer
            SOURCE_DIR
                "${CMAKE_CURRENT_SOURCE_DIR}"
            BINARY_DIR
                "${TENSHI_PACKER_BUILD_DIR}"
            INSTALL_DIR
                "${TENSHI_PACKER_INSTALL_DIR}"
            CMAKE_ARGS
                "-DCMAKE_BUILD_TYPE=Release"
                "-DTENSHI_PACKED=ON"
                "-DTENSHI_BUILD_PACKER_ONLY=ON"
            BUILD_COMMAND
                "${CMAKE_COMMAND}"
                --build .
                --target packer_tool
                --config Release
            INSTALL_COMMAND
                ""
            BUILD_BYPRODUCTS
                "${TENSHI_PACKER_BUILD_DIR}/packer_tool${TENSHI_PACKER_SUFFIX}"
        )

        # this executable runs on the build host, not emscripten
        set(TENSHI_PACKER
            "${TENSHI_PACKER_BUILD_DIR}/packer_tool${TENSHI_PACKER_SUFFIX}"
        )

        add_custom_command(
            OUTPUT
                "${TENSHI_PAK_FILE}"
            COMMAND
                "${CMAKE_COMMAND}"
                -E
                make_directory
                "${CMAKE_BINARY_DIR}/packed"
            COMMAND
                "${TENSHI_PACKER}"
                "${TENSHI_RESOURCE_DIR}"
                "${TENSHI_PAK_FILE}"
            DEPENDS
                tenshi_packer
            COMMENT
                "Packing game resources"
            VERBATIM
        )
    endif()

    add_custom_target(pack_resources
        DEPENDS
            "${TENSHI_PAK_FILE}"
    )
endif()

# ============================================================
#   Game resource setup
# ============================================================

function(tenshi_setup_game TARGET)
    if(NOT TARGET ${TARGET})
        message(FATAL_ERROR
            "tenshi_setup_game(): target '${TARGET}' does not exist"
        )
    endif()

    if(TENSHI_PACKED)
        # Tell game code that resources are packed
        target_compile_definitions(
            ${TARGET}
            PRIVATE
                TENSHI_PACKED
        )

        # ensure the archive exists before game is built
        add_dependencies(${TARGET} pack_resources)

        if(NOT EMSCRIPTEN)
            # copy generated archive next to the game executable
            add_custom_command(
                TARGET 
                    ${TARGET}
                    POST_BUILD
                COMMAND
                    "${CMAKE_COMMAND}"
                    -E
                    copy_if_different
                    "${TENSHI_PAK_FILE}"
                    "$<TARGET_FILE_DIR:${TARGET}>/resource0.tpk"
                VERBATIM
            )
        else()
            # put archive into the Emscripten virtual filesystem
            target_link_options(${TARGET}
                PRIVATE
                    "--preload-file"
                    "${TENSHI_PAK_FILE}@/resource0.tpk"
            )
        endif()
    else()
        # --------------------------------------------------------
        # Resource directory
        # --------------------------------------------------------

        if(NOT EMSCRIPTEN)
            add_custom_command(
                TARGET 
                    ${TARGET}
                    POST_BUILD
                COMMAND
                    "${CMAKE_COMMAND}"
                    -E
                    copy_directory
                    "${TENSHI_RESOURCE_DIR}"
                    "$<TARGET_FILE_DIR:${TARGET}>/resources"
                VERBATIM
            )
        endif()
    endif()
endfunction()

# ============================================================
# Installation
# ============================================================

if(TENSHI_INSTALL)
    include(CMakePackageConfigHelpers)

    install(
        TARGETS tenshi
        EXPORT tenshiTargets
    )

    install(
        DIRECTORY
            "${CMAKE_CURRENT_SOURCE_DIR}/include/"
        DESTINATION
            include
    )

    install(
        EXPORT tenshiTargets
        FILE tenshiTargets.cmake
        NAMESPACE tenshi::
        DESTINATION lib/cmake/tenshi
    )

    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/tenshiConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/tenshiConfig.cmake"
        INSTALL_DESTINATION lib/cmake/tenshi
    )

    install(
        FILES
            "${CMAKE_CURRENT_BINARY_DIR}/tenshiConfig.cmake"
        DESTINATION
            lib/cmake/tenshi
    )
endif()
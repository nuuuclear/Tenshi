cmake_minimum_required(VERSION 3.10)

project(tenshi_packer LANGUAGES C CXX)

set(EXTERNAL_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/../../external"
)

###############
#     LZ4     #
###############

add_subdirectory(
    "${EXTERNAL_DIR}/lz4/build/cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/lz4"
)

##################
#     Packer     #
##################

add_executable(packer_tool
    main.cpp
)

target_link_libraries(packer_tool
    PRIVATE
        lz4
)

install(
    TARGETS packer_tool
    RUNTIME DESTINATION bin
)
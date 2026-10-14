if(NOT DEFINED SOURCE_DIR OR NOT DEFINED DESTINATION_DIR)
    message(FATAL_ERROR "SOURCE_DIR and DESTINATION_DIR are required")
endif()

file(GLOB existingChunks "${DESTINATION_DIR}/resource[0-9]*.tpk")
if(existingChunks)
    file(REMOVE ${existingChunks})
endif()

file(GLOB sourceChunks "${SOURCE_DIR}/resource[0-9]*.tpk")
if(NOT sourceChunks)
    message(FATAL_ERROR "No generated TPK chunks found in ${SOURCE_DIR}")
endif()

file(COPY ${sourceChunks} DESTINATION "${DESTINATION_DIR}")


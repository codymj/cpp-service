option(BIN_PATH "Assign binary output paths" OFF)
if (BIN_PATH)
    set(
        BIN_PATH
        get_filename_component(DIRECTORY ${BIN_PATH} ABSOLUTE)
    )
else (BIN_PATH)
    set(
        BIN_PATH
        "${CMAKE_SOURCE_DIR}/bin"
    )
endif (BIN_PATH)

message(STATUS "Binary output path has been set to ${BIN_PATH}")

set(
    CMAKE_LIBRARY_OUTPUT_DIRECTORY 
    ${BIN_PATH}/${TARGET_COMPILER_NAME}/${CMAKE_BUILD_TYPE}
)
set(
    CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${BIN_PATH}/${TARGET_COMPILER_NAME}/${CMAKE_BUILD_TYPE}
)
set(
    CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${BIN_PATH}/${TARGET_COMPILER_NAME}/${CMAKE_BUILD_TYPE}/lib
)

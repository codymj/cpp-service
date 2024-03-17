set(PROJECT_NAME cpp-service)
set(CMAKE_CXX_FLAGS_DEBUG "-O -ggdb")
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(
    Threads REQUIRED
    libpqxx REQUIRED
    libquill REQUIRED
    libsimdjson REQUIRED
)

add_executable(${PROJECT_NAME} ${SOURCE_DIR}/app.cpp)

target_compile_options(
    ${PROJECT_NAME} PRIVATE
    ${MY_COMPILE_OPTIONS}
)

target_include_directories(
    ${PROJECT_NAME} PRIVATE
    ${SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/thirdparty
    ${PROJECT_SOURCE_DIR}/include/thirdparty/cpp-httplib
    ${PROJECT_SOURCE_DIR}/include/thirdparty/simdjson/include/**
    ${PROJECT_SOURCE_DIR}/include/thirdparty/quill/quill/include/quill/**
    ${PROJECT_SOURCE_DIR}/include/thirdparty/libpqxx/include/pqxx/**
)

add_subdirectory(${PROJECT_SOURCE_DIR}/include/thirdparty/simdjson)
add_subdirectory(${PROJECT_SOURCE_DIR}/include/thirdparty/quill)
add_subdirectory(${PROJECT_SOURCE_DIR}/include/thirdparty/libpqxx)

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    ${CMAKE_THREAD_LIBS_INIT}
    pqxx
    quill
    simdjson
)

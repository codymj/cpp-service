set(PROJECT_NAME cpp-service)
set(CMAKE_CXX_FLAGS_DEBUG "-O -ggdb")
set(THREADS_PREFER_PTHREAD_FLAG ON)

add_executable(${PROJECT_NAME} ${SOURCE_DIR}/main.cpp)

target_compile_options(
    ${PROJECT_NAME} PRIVATE
    ${MY_COMPILE_OPTIONS}
)

target_include_directories(
    ${PROJECT_NAME} PRIVATE
    ${SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/include
)

target_sources(
    ${PROJECT_NAME} PRIVATE
    ${SOURCE_DIR}/app.cpp
    ${SOURCE_DIR}/request_handler_factory.cpp
    ${SOURCE_DIR}/handlers/hello.cpp
)

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    ${CMAKE_THREAD_LIBS_INIT}
    PocoFoundation
    PocoNet
    PocoUtil
)

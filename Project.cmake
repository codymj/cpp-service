set(PROJECT_NAME cpp-service)
set(CMAKE_CXX_FLAGS_DEBUG "-O -ggdb")
set(THREADS_PREFER_PTHREAD_FLAG ON)

add_executable(${PROJECT_NAME} ${SOURCE_DIR}/main.cpp)

target_compile_options(${PROJECT_NAME} PRIVATE
    ${MY_COMPILE_OPTIONS}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${SOURCE_DIR}
)

target_sources(${PROJECT_NAME} PRIVATE
    ${SOURCE_DIR}/core/app.cpp
    ${SOURCE_DIR}/core/handler_factory.cpp
    ${SOURCE_DIR}/core/router.cpp
    ${SOURCE_DIR}/handlers/misc/not_found.cpp
    ${SOURCE_DIR}/handlers/users/users_get.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    PocoFoundation
    PocoNet
    PocoUtil
)

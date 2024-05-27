set(PROJECT_NAME cpp-service)
set(CMAKE_CXX_FLAGS_DEBUG "-O -ggdb")
set(THREADS_PREFER_PTHREAD_FLAG ON)

add_executable(${PROJECT_NAME} ${SOURCE_DIR}/main.cpp)

target_compile_options(${PROJECT_NAME} PRIVATE
    ${MY_COMPILE_OPTIONS}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${INCLUDE_DIR}
    ${INCLUDE_DIR}/external
)

target_sources(${PROJECT_NAME} PRIVATE
    # Core level
    ${SOURCE_DIR}/core/app.cpp
    ${SOURCE_DIR}/core/handler_factory.cpp
    ${SOURCE_DIR}/core/router.cpp
    ${SOURCE_DIR}/core/service_registry.cpp
    ${SOURCE_DIR}/core/store_registry.cpp

    # Handlers
    ${SOURCE_DIR}/handler/misc/not_found.cpp
    ${SOURCE_DIR}/handler/user/users_get.cpp
    ${SOURCE_DIR}/handler/user/users_post.cpp

    # Services
    ${SOURCE_DIR}/service/user/user_service.cpp

    # Data stores
    ${SOURCE_DIR}/store/user/postgres_user_store.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    argon2
    pqxx
    PocoFoundation
    PocoNet
    PocoUtil
    simdjson
)

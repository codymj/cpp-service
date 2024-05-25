set(PROJECT_NAME cpp-service)
set(CMAKE_CXX_FLAGS_DEBUG "-O -ggdb")
set(THREADS_PREFER_PTHREAD_FLAG ON)

add_executable(${PROJECT_NAME} ${SOURCE_DIR}/main.cpp)

target_compile_options(${PROJECT_NAME} PRIVATE
    ${MY_COMPILE_OPTIONS}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${INCLUDE_DIR}
)

target_sources(${PROJECT_NAME} PRIVATE
    ${INCLUDE_DIR}/postgres_connection.cpp

    ${SOURCE_DIR}/core/app.cpp
    ${SOURCE_DIR}/core/handler_factory.cpp
    ${SOURCE_DIR}/core/router.cpp
    ${SOURCE_DIR}/core/service_registry.cpp
    ${SOURCE_DIR}/core/store_registry.cpp

    ${SOURCE_DIR}/handler/misc/not_found.cpp
    ${SOURCE_DIR}/handler/user/users_get.cpp

    ${SOURCE_DIR}/service/user/user_service.cpp

    ${SOURCE_DIR}/store/user/user_model.cpp
    ${SOURCE_DIR}/store/user/user_store.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    pqxx
    PocoFoundation
    PocoNet
    PocoUtil
)

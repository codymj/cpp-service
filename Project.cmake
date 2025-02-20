set(PROJECT_NAME cpp-service)
#set(CMAKE_CXX_FLAGS_DEBUG "-O -ggdb")
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Boost CONFIG)
find_library(ARGON2_LIBRARY argon2)
find_library(PQXX_LIBRARY libpqxx)
find_package(nlohmann_json REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(quill REQUIRED)
find_package(yaml-cpp REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCE_DIR}/main.cpp)

target_compile_options(${PROJECT_NAME} PRIVATE
    ${MY_COMPILE_OPTIONS}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${INCLUDE_DIR}
)

target_sources(${PROJECT_NAME} PRIVATE
    # Core
    ${SOURCE_DIR}/core/app.cpp
    ${SOURCE_DIR}/core/listener.cpp
    ${SOURCE_DIR}/core/router.cpp
    ${SOURCE_DIR}/core/service_registry.cpp
    ${SOURCE_DIR}/core/session.cpp
    ${SOURCE_DIR}/core/store_registry.cpp

    # Handlers
    ${SOURCE_DIR}/handler/middleware/logger.cpp
    ${SOURCE_DIR}/handler/misc/not_found.cpp
    ${SOURCE_DIR}/handler/user/users_get.cpp
    ${SOURCE_DIR}/handler/user/users_post.cpp

    # Services
    ${SOURCE_DIR}/service/user/user_service.cpp

    # Stores
    ${SOURCE_DIR}/store/user/postgres_user_store.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    Boost::boost
    argon2
    pqxx
    pq
    quill::quill
    yaml-cpp
)

# Define the Docker build command
add_custom_target(docker_build
    COMMAND docker buildx build -t cpp-service:0.0.1
        --build-arg BUILD_NAME=cpp-service
        --build-arg BUILD_REF=0.0.1
        --build-arg BUILD_DATE=`date -u +"%Y-%m-%dT%H:%M:%SZ"`
        .
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Building Docker image"
)

# Add the custom target to the default build
add_custom_target(docker DEPENDS docker_build)

set(PROJECT_NAME cpp-service)
set(CMAKE_CXX_FLAGS_DEBUG "-O -ggdb")
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(
    Threads REQUIRED
)

add_executable(
    ${PROJECT_NAME}
    ${SOURCE_DIR}/app.cpp
)

target_compile_options(
    ${PROJECT_NAME} PRIVATE
    "-D_GLIBCXX_USE_CXX11_ABI=0"
    "-DNO_ZMQ_SUPPORT"
)

target_include_directories(
    ${PROJECT_NAME} PRIVATE
    ${SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/thirdparty/simdjson
    ${PROJECT_SOURCE_DIR}/include/thirdparty/cpp-httplib
)

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    ${CMAKE_THREAD_LIBS_INIT}
    # dl
)

# add_subdirectory(test)

# add_custom_command(
#     TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/${AppConfig}" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
# )

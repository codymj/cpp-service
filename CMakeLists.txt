cmake_minimum_required(VERSION 3.25)
project(http-cpp-service)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CompilerFlags REQUIRED MODULE)
find_package(ConditionalCxxFlag REQUIRED MODULE)
find_package(PublishPaths REQUIRED MODULE)

include(Project.cmake)

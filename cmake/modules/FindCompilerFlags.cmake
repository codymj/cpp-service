# common compiler flags
if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang" OR ${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
    # warnings as errors - https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html#index-Werror
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")

    # https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html#index-Wall
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

    # https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html#index-Wextra
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")

    # Revert deprecated to warning
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=deprecated-declarations")

    # specific compiler flags
    if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    elseif(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
        # todo resolve clang warnings and move this up to common flags
        # https://gcc.gnu.org/onlinedocs/gcc/Standards.html#index-pedantic
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors")
    endif()

    # https://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html#index-g
    SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g")

    # SET(CMAKE_CXX_FLAGS_RELEASE "-O2")
    SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb")

    # EPX specific flags
    SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -g")

    # https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html#index-rdynamic
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -rdynamic")

    message(STATUS "CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}")
    message(STATUS "CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG}")
    message(STATUS "CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE}")
    message(STATUS "CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS}")

else()
    message(FATAL_ERROR "Unsupported compiler ${CMAKE_CXX_COMPILER_ID}.")
endif()

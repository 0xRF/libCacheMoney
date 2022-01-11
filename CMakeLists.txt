cmake_minimum_required(VERSION 3.20)

project(libCacheMoney)

set(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")

if (UNIX)
    set(PLATFORM_DIR linux)
else ()
    set(PLATFORM_DIR windows)
endif ()

add_library(libCacheMoney SHARED
        src/${PLATFORM_DIR}/utils.cpp
        src/${PLATFORM_DIR}/intrinsics.cpp

        src/common/utils.cpp
        src/common/flush_reload.cpp
        src/common/prime_probe.cpp
        src/common/cache.cpp
        src/common/l1.cpp
        src/common/eviction_set_builder.cpp
        )

target_include_directories(libCacheMoney PRIVATE include)

add_subdirectory(victims)
enable_testing()
add_subdirectory(tests)

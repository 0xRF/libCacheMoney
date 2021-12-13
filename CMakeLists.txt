cmake_minimum_required(VERSION 3.20)
project(libCacheMoney)

set(CMAKE_CXX_STANDARD 20)

if (UNIX)
    set(PLATFORM_DIR linux)
else ()
    set(PLATFORM_DIR windows)
endif()
add_library(libCacheMoney SHARED
        src/${PLATFORM_DIR}/utils.cpp
        src/${PLATFORM_DIR}/intrinsics.cpp

        src/common/utils.cpp
        src/common/flush_reload.cpp
        src/common/cache.cpp
)

target_include_directories(libCacheMoney PRIVATE include)

enable_testing()
add_subdirectory(tests)
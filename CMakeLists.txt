cmake_minimum_required(VERSION 3.21)
project(libCacheMoney)

set(CMAKE_CXX_STANDARD 20)

add_library(libCacheMoney SHARED
            src/linux/utils.cpp
            src/linux/intrinsics.cpp
            src/common/flush_reload.cpp
        )

target_include_directories(libCacheMoney PRIVATE include)

enable_testing()
add_subdirectory(tests)
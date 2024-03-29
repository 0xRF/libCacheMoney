cmake_minimum_required(VERSION 3.20)

project(libCacheMoney)

set(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")

message("Compiler: ${CMAKE_CXX_COMPILER_ID} - ${CMAKE_CXX_COMPILER_VERSION}")

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
        src/common/l3.cpp
        src/common/eviction_set_builder.cpp
        src/common/eviction_set_l3.cpp
        src/common/ps_evset.cpp
        include/linked_list.hpp
        )

target_include_directories(libCacheMoney PRIVATE include)

add_subdirectory(victims)
enable_testing()
add_subdirectory(tests)

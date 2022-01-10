cmake_minimum_required(VERSION 3.20)
project(victims)

set(CMAKE_C_STANDARD 20)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")

function(create_victim VICTIM_NAME)
    add_library(${VICTIM_NAME} ${VICTIM_NAME}.cpp)
    target_link_libraries(libCacheMoney ${VICTIM_NAME})
    target_include_directories(libCacheMoney PRIVATE ${CMAKE_SOURCE_DIR}/include)
    message(STATUS "Creating Victim ${VICTIM_NAME}")
endfunction()

file(GLOB VICTIM_FILES "*.cpp")
foreach(FILE_PATH ${VICTIM_FILES})
    cmake_path(GET FILE_PATH STEM LAST_ONLY VICTIM)
    create_victim(${VICTIM})
endforeach()

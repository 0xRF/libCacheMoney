cmake_minimum_required(VERSION 3.20)
project(victims)

set(CMAKE_C_STANDARD 23)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")

function(create_victim VICTIM_NAME)
    add_executable(${VICTIM_NAME} ${VICTIM_NAME}.cpp)
    target_link_libraries(${VICTIM_NAME} libCacheMoney)
    target_include_directories(${VICTIM_NAME} PRIVATE ${CMAKE_HOME_DIRECTORY}/include)
    message(STATUS "Creating Victim ${VICTIM_NAME}")
endfunction()

file(GLOB VICTIM_FILES "victim_*.cpp")
foreach(FILE_PATH ${VICTIM_FILES})
    cmake_path(GET FILE_PATH STEM LAST_ONLY VICTIM)
    create_victim(${VICTIM})
endforeach()

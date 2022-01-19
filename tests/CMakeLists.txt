cmake_minimum_required(VERSION 3.20)
project(tests)

include(${CMAKE_HOME_DIRECTORY}/cmake/CPM.cmake)
include(CTest)


set(CMAKE_CXX_STANDARD 23)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")

CPMAddPackage(
        NAME matplot
        GITHUB_REPOSITORY alandefreitas/matplotplusplus
        GIT_TAG  v1.1.0
        OPTIONS
)
target_compile_features(matplot PUBLIC cxx_std_17)

function(create_test TEST_NAME)
    add_executable(${TEST_NAME} ${TEST_NAME}.cpp)
    target_link_libraries(${TEST_NAME} PUBLIC libCacheMoney matplot)
    target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_HOME_DIRECTORY}/include ${CMAKE_HOME_DIRECTORY}/victims/include)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    message(STATUS "Creating Test ${TEST_NAME}")

endfunction()

file(GLOB TEST_FILES "test_*.cpp")
foreach(FILE_PATH ${TEST_FILES})
    cmake_path(GET FILE_PATH STEM LAST_ONLY TEST)
    create_test(${TEST})
endforeach()

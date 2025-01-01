project(test)

cmake_minimum_required(VERSION 3.11)

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fexceptions -fnon-call-exceptions")
endif()


set(TARGET test)

message(STATUS "GTest found")
include(GoogleTest)

add_executable(${TARGET} 
    main.cpp
    utils.cpp
)

target_link_libraries(${TARGET} segvcatch GTest::GTest GTest::Main)

gtest_discover_tests(tests)

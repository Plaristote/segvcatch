project(test)

cmake_minimum_required(VERSION 3.11)

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fexceptions -fnon-call-exceptions")
endif()

set(TARGET test)

add_executable(${TARGET} 
	main.cpp
    utils.cpp
)

target_link_libraries(${TARGET} segvcatch)
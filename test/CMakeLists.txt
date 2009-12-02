project(test)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fexceptions -fnon-call-exceptions")

set(TARGET test)

add_executable(${TARGET} 
	main.cpp
)

target_link_libraries(${TARGET} segvcatch)
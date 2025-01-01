project(segvcatch)
cmake_minimum_required(VERSION 3.10)

include_directories(lib)

add_subdirectory(lib)

include(CTest)
enable_testing()
find_package(GTest)

if (GTest_FOUND)
    add_subdirectory(test)
else()
    message(STATUS "GTest not found")
endif()



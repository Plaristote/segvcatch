project(segvcatch)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -fnon-call-exceptions")

set(TARGET segvcatch)

set(SOURCES
  segvcatch.cpp 
  segvcatch.h
  i386-signal.h
  x86_64-signal.h
)

add_library(${TARGET} STATIC ${SOURCES})

install(TARGETS ${TARGET} ARCHIVE DESTINATION lib)
install(FILES segvcatch.h DESTINATION include)
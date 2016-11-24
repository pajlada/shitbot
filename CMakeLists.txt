cmake_minimum_required(VERSION 3.7.0)
project(WNMAbot CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CMAKE_CXX_FLAGS "-pthread -std=c++14 ${CMAKE_CXX_FLAGS}")

set(SOURCE
    ${SOURCE} 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main2.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/connhandler.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utilities.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/channel.cpp
)

add_executable(WNMAbot ${SOURCE})
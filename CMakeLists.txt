cmake_minimum_required(VERSION 3.21)
project(emehcs)

set(CMAKE_CXX_STANDARD 20)

add_definitions(-D__EMEHCS_DEBUG=1)

add_executable(emehcs main.cpp value.cpp parser.cpp defs.cpp)
target_include_directories(emehcs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

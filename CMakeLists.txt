cmake_minimum_required(VERSION 3.21)
project(emechs)

set(CMAKE_CXX_STANDARD 20)

add_definitions(-D__EME_DEBUG=1)

add_executable(emechs main.cpp value.cpp parser.cpp defs.cpp)
target_include_directories(emechs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

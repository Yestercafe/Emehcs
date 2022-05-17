cmake_minimum_required(VERSION 3.10)
project(emehcs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_definitions(-D__EMEHCS_DEBUG=1)

add_executable(emehcs preload.cpp ui.cpp main.cpp value.cpp defs.cpp parser.cpp environment.cpp
        eval.cpp details/scm-string.cpp details/scm-io.cpp tests.cpp)
target_include_directories(emehcs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

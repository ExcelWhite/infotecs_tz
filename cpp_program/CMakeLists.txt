cmake_minimum_required(VERSION 3.10)
project(Program1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)
add_executable(program1
    src/main.cpp
    src/shared_buffer.cpp
    src/input_thread.cpp
    src/processing_thread.cpp
    src/socket_client.cpp
)
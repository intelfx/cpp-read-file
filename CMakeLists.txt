cmake_minimum_required(VERSION 3.27)
project(file_read)

set(CMAKE_CXX_STANDARD 20)

add_library(nonius INTERFACE IMPORTED)
target_include_directories(nonius INTERFACE nonius/include)

add_executable(file_read main.cpp)
target_link_libraries(file_read nonius)

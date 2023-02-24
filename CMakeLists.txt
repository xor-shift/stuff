cmake_minimum_required(VERSION 3.25)
project(Tracer)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(stuff/libs/blas)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} stuff_blas)

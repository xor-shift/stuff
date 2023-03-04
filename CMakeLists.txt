cmake_minimum_required(VERSION 3.25)

project(stuff LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

add_library(${PROJECT_NAME} INTERFACE)

add_subdirectory(blas)
